#pragma once
#include "SFLogicDispatcher.h"

class SFCasualGameDispatcher : public SFLogicDispatcher
{
public:
	SFCasualGameDispatcher(void);
	virtual ~SFCasualGameDispatcher(void);

	virtual void Dispatch(BasePacket* pPacket) override;
	virtual bool CreateLogicSystem(ILogicEntry* pLogicEntry) override;
	virtual bool ShutDownLogicSystem() override;

	virtual bool AddRPCService(IRPCService* pService) override;

private:
	static bool m_bLogicEnd;
	int m_nLogicThreadCnt;
	int m_logicThreadGroupId;
	int m_rpcThreadGroupId;

	IRPCService* m_pRPCService;

	static void LogicThreadProc(void* Args);
	static void RPCThreadProc(void* Args);
};

