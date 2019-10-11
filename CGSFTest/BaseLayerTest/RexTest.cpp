#include "stdafx.h"
#include "RexTest.h"
#include "SFRegexChecker.h"

RexTest::RexTest(void)
{
}


RexTest::~RexTest(void)
{
}

bool RexTest::Run()
{
	SFRegexChecker Checker;

	//�ֹε�Ϲ�ȣ
	VERIFY(FALSE == Checker.IsValidResidentRegistrationNumber(L"801234-798999"));
	VERIFY(TRUE == Checker.IsValidResidentRegistrationNumber(L"801234-7989990"));
	VERIFY(FALSE == Checker.IsValidResidentRegistrationNumber(L"801234-79899909"));

	//URL
	VERIFY(TRUE == Checker.IsValidURL(L"http://onlinegameserver.org/444/"));
	VERIFY(TRUE == Checker.IsValidURL(L"http://onlinegameserver.org/444"));
	VERIFY(TRUE == Checker.IsValidURL(L"http://onlinegameserver.org"));
	VERIFY(TRUE == Checker.IsValidURL(L"http://onlinegameserver"));
	VERIFY(FALSE == Checker.IsValidURL(L"http://"));
	VERIFY(FALSE == Checker.IsValidURL(L"http://["));

	//�̸���
	VERIFY(FALSE == Checker.IsValidEMail(L"email"));
	VERIFY(FALSE == Checker.IsValidEMail(L"email@"));
	VERIFY(FALSE == Checker.IsValidEMail(L"email@daum"));
	VERIFY(FALSE == Checker.IsValidEMail(L"email@daum."));
	VERIFY(TRUE == Checker.IsValidEMail(L"a@daum.net"));
	VERIFY(TRUE == Checker.IsValidEMail(L"email-A@DAUM.Net"));
	VERIFY(FALSE == Checker.IsValidEMail(L"email-A@DAUM.Neta"));
	VERIFY(TRUE == Checker.IsValidEMail(L"email-A@DAUM.Ne"));
	VERIFY(FALSE == Checker.IsValidEMail(L"email-A@DAUM.N"));
	VERIFY(FALSE == Checker.IsValidEMail(L"email--A@DAUM.NET"));

	//IP Address
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333"));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333."));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333.333"));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333.333."));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333.333.333"));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333.333.333."));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333.333.333.333"));
	VERIFY(TRUE == Checker.IsValidIPAddress(L"133.133.133.133"));
	VERIFY(TRUE == Checker.IsValidIPAddress(L"255.255.255.255"));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"255.255.255.256"));
	VERIFY(TRUE == Checker.IsValidIPAddress(L"56.255.255.255"));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333.333.333.333."));
	VERIFY(FALSE == Checker.IsValidIPAddress(L"333.333.333.333.333"));

	//ĳ�����̸�
	VERIFY(FALSE == Checker.IsValidCharName(L"�ڰ�����abc-][(){}_AB194"));
	VERIFY(TRUE == Checker.IsValidCharName(L"������abc-][(){}_AB194"));
	VERIFY(FALSE == Checker.IsValidCharName(L"��������abc-][(){}_AB194"));

	return true;
}
