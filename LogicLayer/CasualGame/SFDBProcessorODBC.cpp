#include "stdafx.h"
/*#include "SFDBProcessorODBC.h"
#include "DBMsg.h"
#include "SFSendDBRequest.h"

SFDBProcessorODBC::SFDBProcessorODBC(void)
{
}


SFDBProcessorODBC::~SFDBProcessorODBC(void)
{
}

BOOL SFDBProcessorODBC::Initialize()
{
	_DBConnectionInfo Info = {"cgsf", "test", "root", "root", "127.0.0.1", 3306};
	SetInfo(Info);

	if(FALSE == SFDatabaseODBC::Initialize())
		return FALSE;

	return RegisterDBService();
}


BOOL SFDBProcessorODBC::RegisterDBService()
{
	m_Dispatch.RegisterMessage(DBMSG_LOGIN, std::tr1::bind(&SFDBProcessorODBC::OnLogin, this, std::tr1::placeholders::_1));

	return TRUE;
}

BasePacket* SFDBProcessorODBC::Alloc()
{
	return NULL;
}

BOOL SFDBProcessorODBC::Release(BasePacket* pMessage)
{
	LogicEntrySingleton::instance()->ReleaseDBMessage((SFMessage*)pMessage);

	return TRUE;
}

BOOL SFDBProcessorODBC::Call(BasePacket* pMessage)
{
	return m_Dispatch.HandleMessage(pMessage->GetPacketID(), pMessage);

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SFDBProcessorODBC::OnLogin( BasePacket* pPacket )
{
	SFMessage* pMessage = (SFMessage*)pPacket;
	
////////////////////////////////////////////////////////////////////////////////
//����� ���� ������� ������ �� ���
////////////////////////////////////////////////////////////////////////////////
	SFMessage* pMsg = SFSendDBRequest::GetInitMessage(pMessage->GetCommand(), pMessage->GetSerial());
//	*pMsg << Result;
	SFSendDBRequest::SendToLogic(pMsg);
	
	return TRUE;
}*/