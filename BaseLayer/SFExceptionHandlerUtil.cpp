#include "StdAfx.h"
#include "tchar.h"
#include "Process.h"
#include "SFExceptionHandlerUtil.h"
#include "SFStackWalker.h"
#include <fstream>

using namespace  std;

char g_pSetUnhandledFilterEntryValue[5] = {0,};

////////////////////////////////////////////////////////////////////////////////////////
//Exception Unhandled Filter�� Overriding�Ǵ� ���� ���� ���� ��ƾ
//Kernel32.dll�� SetUnhandledExceptionFilter �Լ��� �������� 5����Ʈ �����ڵ带 �Ἥ
//MyDummySetUnhandledExceptionFilter�� ȣ��ǰ� �ϰ� �� �Լ������� ��� ó���� �������� ����
//��������� CRT �Լ� ���� set_invalid_parameter�� pure_function_call�� ���� ������ �� �ʿ䰡 ���� �ȴ�.
////////////////////////////////////////////////////////////////////////////////////////
#ifndef _WIN64
#ifndef _M_IX86
#error "The following code only works for x86!"
#endif
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(
	LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return NULL;
}
#endif

BOOL PreventSetUnhandledExceptionFilter()
{
#ifndef _WIN64
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));

	if (hKernel32  == NULL) return FALSE;

	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if(NULL == pOrgEntry) 
		return FALSE;

	memcpy(g_pSetUnhandledFilterEntryValue, pOrgEntry, 5 * sizeof(char));

	unsigned char newJump[ 100 ];
	DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;

	dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far
	void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
	DWORD dwNewEntryAddr = (DWORD) pNewFunc;

	DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

	newJump[ 0 ] = 0xE9;  // JMP absolute
	memcpy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));
	SIZE_T bytesWritten;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
		pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);	

	return bRet;
#else
	return FALSE;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
//Exception Unhandled Filter �Լ��� �����ϵ��� �����ϴ� ��ƾ
//������ ����� ����ϰ� �Ͱ�, ������ ��⿡�� ���� Ȯ��� ����� �߰��ϰ� ���� ���
//SetUnhandledFilter �Լ��� ����� �����ϵ��� ������� ���� �ʿ䰡 ������ �׶� ����ϴ� �Լ�
//FS �������͸� ���� �����Լ� ü�ο� �߰��ϴ� ������� ������ ���� ������ �ߵ��� �ʾƼ�....
////////////////////////////////////////////////////////////////////////////////////////
BOOL RecoverSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));

	if (hKernel32  == NULL) return FALSE;

	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if(NULL == pOrgEntry) 
		return FALSE;

	if(g_pSetUnhandledFilterEntryValue[0] == 0)
		return FALSE;

	//memcpy(pOrgEntry, g_pSetUnhandledFilterEntryValue, 5 * sizeof(char));
	
	SIZE_T bytesWritten;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
		pOrgEntry, g_pSetUnhandledFilterEntryValue, 5 * sizeof(char), &bytesWritten);

	return bRet;
}

void New_OutOfMemory_Handler()
{
	DWORD dwCurrentThreadId = GetCurrentThreadId();
	
	AddOutofMemoryLogHeader(dwCurrentThreadId);

	HANDLE hCurrentThread = OpenThread(THREAD_GET_CONTEXT, FALSE, dwCurrentThreadId);

	HANDLE hThread = (HANDLE)_beginthreadex(0, NULL, CreateOutofMemoryLog, &hCurrentThread, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	
	exit(0);
	//RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL);
}


void GetOutofMemoryLogPath(TCHAR* pPathName)
{
	TCHAR szCurDirectory[MAX_PATH] = {0,};
	TCHAR* pEnd = NULL;
	GetModuleFileName(NULL, szCurDirectory, MAX_PATH);
	pEnd = _tcsrchr(szCurDirectory, _T('\\'));

	if(NULL == pEnd)
	{
		return;
	}

	*pEnd = _T('\0');
	pEnd = pEnd + 1;


	_tcscpy_s( pPathName, MAX_PATH-1, szCurDirectory);
	_tcscat_s( pPathName,  MAX_PATH-1, TEXT("\\OutofMemoryLog.txt"));
}

void GetUserDefinedLogPath(TCHAR* pPathName)
{
	TCHAR szCurDirectory[MAX_PATH] = {0,};
	TCHAR* pEnd = NULL;
	GetModuleFileName(NULL, szCurDirectory, MAX_PATH);
	pEnd = _tcsrchr(szCurDirectory, _T('\\'));

	if(NULL == pEnd)
	{
		return;
	}

	*pEnd = _T('\0');
	pEnd = pEnd + 1;


	_tcscpy_s( pPathName, MAX_PATH-1, szCurDirectory);
	_tcscat_s( pPathName,  MAX_PATH-1, TEXT("\\UserDefinedLog.txt"));
}

void AddOutofMemoryLogHeader(DWORD dwCurrentThreadId)
{
	wprintf(L"Out of Memory %d\n", dwCurrentThreadId);

	TCHAR szCurrentPath[MAX_PATH] = {0,};
	GetOutofMemoryLogPath(szCurrentPath);

	wofstream outFile(szCurrentPath,ios::app);
	if(!outFile.good())
	{
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	outFile << "OccurTime " << sysTime.wYear << '/'<< sysTime.wMonth << '/'
		<< sysTime.wDay << ' ' << sysTime.wHour << ':' << sysTime.wMinute 
		<< ':' << sysTime.wSecond << endl;

	outFile << "Out Of Memory. Thread ID : " << dwCurrentThreadId << endl;
}

//////////////////////////////////////////////////////////////
//��ƿ��Ƽ �Լ�
//////////////////////////////////////////////////////////////
unsigned __stdcall CreateOutofMemoryLog( void* pArguments )
{
	HANDLE hThread = (*(HANDLE*)pArguments);

	SFStackWalker StackWalker;
	StackWalker.SetLogFilePath();

	StackWalker.ShowCallstack(hThread);
		
	_endthreadex(0);
	return 0;
}