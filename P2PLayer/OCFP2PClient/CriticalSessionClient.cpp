////////////////////////////////////////////////////////////////////////////////
//
// �ۼ��� : �̿���
// �ۼ��� : 2002.03.27
// ��  �� :
//

/////////////////////////////////////////////////////////////////////////////////////////
//
#include "AllOcfClientSys.h"
#include "AllOcfClient.h"

/////////////////////////////////////////////////////////////////////////////////////////
// 
CCriticalClient::CCriticalClient()
{
	try
	{
		InitializeCriticalSection(&m_stCriticalSection);
		m_bInitFlag = TRUE;
	}
	catch (...)
	{
		m_bInitFlag = FALSE;
	}
}

CCriticalClient::~CCriticalClient()
{
	if (TRUE == m_bInitFlag)
	{
		DeleteCriticalSection(&m_stCriticalSection);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// 
CSmartLock::CSmartLock(CCriticalClient* pcLock):
	m_pcLock(pcLock)
{
	if (m_pcLock)
	{
		m_pcLock->Lock();
	}
}

CSmartLock::CSmartLock(CCriticalClient& cLock):
	m_pcLock(&cLock)
{
	m_pcLock->Lock();
}

CSmartLock::~CSmartLock()
{
	if (m_pcLock)
	{
		m_pcLock->Unlock();
	}
}
