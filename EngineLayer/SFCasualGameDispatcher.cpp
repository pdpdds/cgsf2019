#include "stdafx.h"
#include "SFCasualGameDispatcher.h"
#include "SFEngine.h"
#include "IRPCService.h"
#include "SFPacket.h"
#include "SFSinglton.h"
bool SFCasualGameDispatcher::m_bLogicEnd = false;


SFCasualGameDispatcher::SFCasualGameDispatcher(void)
{
//ĳ��� ���� ������ ��ũ�� ���� ������ ���� �ϳ���
	m_nLogicThreadCnt = 1;
	m_rpcThreadGroupId = -1;
	m_logicThreadGroupId = -1;
}

SFCasualGameDispatcher::~SFCasualGameDispatcher(void)
{
}

void SFCasualGameDispatcher::Dispatch(BasePacket* pPacket)
{
	if (pPacket->GetPacketType() == SFPACKET_RPC && SFEngine::GetInstance()->IsServer())
	{
		RPCGatewaySingleton::instance()->PushPacket(pPacket);
	}
	else
	{
		LogicGatewaySingleton::instance()->PushPacket(pPacket);
	}
}

void SFCasualGameDispatcher::LogicThreadProc(void* Args)
{
	UNREFERENCED_PARAMETER(Args);
	LogicEntry* pEntry = LogicEntrySingleton::instance();

	while (m_bLogicEnd == false)
	{
//��������Ʈ���� ť���� ��Ŷ�� ������.
//������Ʈ�� ��ü�� ProcessPacket �޼ҵ带 ȣ���ؼ� ��Ŷ ó���� �����Ѵ�.
		BasePacket* pPacket = LogicGatewaySingleton::instance()->PopPacket();
		pEntry->ProcessPacket(pPacket);

//20150113
//DB ó���� ������Ʈ�� ��ü�� �����ؼ� �������̾ �����ͺ��̽� ���̾ ���ӵ��� �ʵ��� �Ѵ�.
//DB ��Ŷ�� ���Ŵ� �������̾�� ó���ϵ��� �Ѵ�.
//���� ������ ������ ���̴�.
		if (pPacket->GetPacketType() != SFPACKET_DB)
		{
			ReleasePacket(pPacket);			
		}
	}
}

void SFCasualGameDispatcher::RPCThreadProc(void* Args)
{
	SFCasualGameDispatcher* pDisPatcher = (SFCasualGameDispatcher*)Args;
	while (m_bLogicEnd == false)
	{
		//��������Ʈ���� ť���� ��Ŷ�� ������.
		//BasePacket* pPacket = RPCGatewaySingleton::instance()->PopPacket();

//20140608 �ӽ�..
		SFPacket* pPacket = (SFPacket*)RPCGatewaySingleton::instance()->PopPacket();
	
		if (pPacket)
		{
			pDisPatcher->m_pRPCService->ProcessRPCService(pPacket);
			delete pPacket;
		}
	}
}

bool SFCasualGameDispatcher::CreateLogicSystem(ILogicEntry* pLogicEntry)
{	
	m_logicThreadGroupId = ACE_Thread_Manager::instance()->spawn_n(m_nLogicThreadCnt, (ACE_THR_FUNC)LogicThreadProc, this, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);

	LogicEntrySingleton::instance()->SetLogic(pLogicEntry);

	m_rpcThreadGroupId = ACE_Thread_Manager::instance()->spawn_n(1, (ACE_THR_FUNC)RPCThreadProc, this, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY);

	return true;
}

bool SFCasualGameDispatcher::ShutDownLogicSystem()
{
	m_bLogicEnd = true;
	
	for (int i = 0; i < m_nLogicThreadCnt; i++)
	{
		BasePacket* pCommand = PacketPoolSingleton::instance()->Alloc();
		pCommand->SetSerial(-1);
		pCommand->SetPacketType(SFPACKET_SERVERSHUTDOWN);
		LogicGatewaySingleton::instance()->PushPacket(pCommand);
	}

	LogicEntrySingleton::instance()->DestroyLogic();
	
	RPCGatewaySingleton ::instance()->PushPacket(NULL);
	ACE_Thread_Manager::instance()->wait_grp(m_rpcThreadGroupId);		

	return true;
}

bool SFCasualGameDispatcher::AddRPCService(IRPCService* pService)
{
	m_pRPCService = pService;
	return true;
}

