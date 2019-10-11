#pragma once
#include <EngineInterface\/INetworkEngine.h>
#include "ProactorAcceptor.h"
#include "ACETimeOutHandler.h"
#include <map>

typedef std::map<int, ProactorAcceptor*> mapAcceptor;

class ACEEngine : public INetworkEngine
{
public:
	ACEEngine(IEngine* pEngine);
	virtual ~ACEEngine(void);

    virtual bool Shutdown() override;

	virtual bool SendRequest(BasePacket* pPacket) override;

	virtual bool Disconnect(int serial) override;

	virtual bool CheckTimerImpl() override; //default timer ���
	virtual long AddTimer(unsigned int timerID, unsigned int startTime, unsigned int period) override;
	virtual bool CancelTimer(int timerID) override;

	virtual int AddConnector(int connectorIndex, char* szIP, unsigned short port) override;
	virtual int AddListener(char* szIP, unsigned short port, bool bDefaultListener = false) override;

protected:
	bool NetworkOpen();

	int m_acceptorIndex;
	int m_connectorIndex;
	int m_workThreadGroupID;
	mapAcceptor m_mapAcceptor;

private:
	ACETimeOutHandler m_TimeOutHandler;
};

class ACEServerEngine : public ACEEngine
{
public:
	ACEServerEngine(IEngine* pEngine);

	virtual bool Init(int ioThreadCnt) override;
	virtual bool Start(char* szIP, unsigned short port) override;
};

class ACEClientEngine : public ACEEngine
{
public:
	ACEClientEngine(IEngine* pEngine);

	virtual bool Init(int ioThreadCnt) override;
	virtual bool Start(char* szIP, unsigned short port) override;
};

