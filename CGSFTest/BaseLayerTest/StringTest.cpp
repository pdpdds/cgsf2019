#include "stdafx.h"
#include "StringTest.h"
#include "SFString.h"
#include "SFFString.h"

#define MAX_NICKNAME_LEN 8
DECLARE_STRING_TYPE(SSNickName, MAX_NICKNAME_LEN); //����ο� ������ �Ѵ�. 

StringTest::StringTest(void)
{
}


StringTest::~StringTest(void)
{
}


bool StringTest::Run()
{
	SSNickName szName;
	szName = _T("NickName");//MAX_NICKNAME_LEN���� ��ٸ� �Ҵ��� ������.

	wprintf(L"%s\n", (TCHAR*)szName.GetBuffer());

	SFFString String("NickName");//��Ƽ�ڵ�� ���ڿ� ���� 
	String.convert(SFFSTRType_Unicode16); //�����ڵ�� ����

	wprintf(L"%s\n", String.getUnicode16());

	return true;
}
