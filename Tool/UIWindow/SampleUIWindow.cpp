#include "StdAfx.h"
#include "SampleUIWindow.h"
#include <Mmsystem.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "baselayer.lib")
#pragma comment(lib, "Netapi32.lib")

extern SampleUIWindow* g_SampleWindow;


// ����� Ŀ�ǵ��� ����.
class CCmdHelp : public UICommand
{
public:
	virtual BOOL DoProcess(WCHAR* lpParam)
	{
		if (g_SampleWindow) g_SampleWindow->ShowCommand();
		return TRUE;
	}
};

class CCmdClear : public UICommand
{
public:
	virtual BOOL DoProcess(WCHAR* lpParam)
	{
		UICmdMsgView::ClearMsgView();
		return TRUE;
	}
};

class CCmdClose : public UICommand
{
public:
	virtual BOOL DoProcess(WCHAR* lpParam)
	{
		if (g_SampleWindow) g_SampleWindow->EndCommand();
		return TRUE;
	}
};

class CCmdMsg : public UICommand
{
public:
	virtual BOOL DoProcess(WCHAR* lpParam)
	{
		if (g_SampleWindow) g_SampleWindow->SendMsg(lpParam);
		return TRUE;
	}
};

SampleUIWindow::SampleUIWindow(void)
{
	m_uTimerID		= 0;
	m_uTimerPeriod	= 1;
}

SampleUIWindow::~SampleUIWindow(void)
{
}

VOID SampleUIWindow::InitializeCommand()
{
	ADD_COMMAND(L"help", CCmdHelp, L"Show Command List.");
	ADD_COMMAND(L"clear", CCmdClear, L"Clear Display.");	
	ADD_COMMAND(L"close", CCmdClose, L"Close OrangeWindow.");
	ADD_COMMAND(L"msg", CCmdMsg, L"Send IRC Message");

	ShowCommand();
}

VOID SampleUIWindow::SendMsg(WCHAR* strParam)
{
	/*NaveNet::NFPacket packet;

	packet.SetCommand(IRC);
	packet.SetSize(sizeof(PKIRC));

	LPPKIRC lpIRC = (LPPKIRC)packet.m_Packet;

	strcpy(lpIRC->Key, "/SVR");
	strcpy(lpIRC->Message, Nave::ToASCII(strParam).c_str());

	g_UManager.SendPostAll(packet);*/
}

VOID SampleUIWindow::UpdateInfo()
{
	WCHAR	szTime[32];
	WCHAR	szDate[32];

	_tzset();
	_tstrdate_s( szDate );
	_tstrtime_s( szTime );

	WCHAR strInfo[512];

	_stprintf_s(strInfo, L"UpdateTime : %s-%s", szDate, szTime); 

	UICmdMsgView::UpdateInfo(strInfo);
}

VOID SampleUIWindow::InitObject()
{
	/*m_iMaxPacket = 0;
	m_iPrevTick = timeGetTime();

	INT Port = g_Config.GetPort();
	INT MaxConn = g_Config.GetMaxConn();
	Start(Port, MaxConn);*/

	timeBeginPeriod(m_uTimerPeriod);
	m_uTimerID = timeSetEvent(1000,0, (LPTIMECALLBACK)TimerProc,(DWORD)0,TIME_PERIODIC);
}

// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
VOID SampleUIWindow::ReleaseObject()
{
	//Stop();
	timeKillEvent(m_uTimerID); 
	timeEndPeriod(m_uTimerPeriod);
}

// Process Update...
VOID SampleUIWindow::Update()
{

}

//////////////////////////////////////////////////////////////////
// [1]DESCRIPTION : Ÿ�̸� �Լ�						 			//
// [2]PARAMETER : dwUser  - ���� ��ü�� �ѱ�� ���� 			//
// [3]RETURN : void							 					//
// [4]DATE : 2000�� 11�� 21��									//
//////////////////////////////////////////////////////////////////
void SampleUIWindow::TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if (g_SampleWindow)
		g_SampleWindow->UpdateInfo();
}