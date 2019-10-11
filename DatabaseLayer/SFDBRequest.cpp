#include "StdAfx.h"
#include "SFDBRequest.h"
#include "SFDBWorker.h"
#include "SFDatabase.h"

int SFDBRequest::call()
{
	SFASSERT(m_pMessage != NULL);

/////////////////////////////////////////////////
//Call method���� m_pMessage ��ü�� ȸ���Ѵ�.
/////////////////////////////////////////////////
	m_pOwner->GetDatabase()->Call(m_pMessage);

	m_pMessage = NULL;
	
	return 0;
}

