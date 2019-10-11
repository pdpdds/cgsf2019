#pragma once
#include "asios.h"
#include <map>
#include <EngineInterface/ISession.h>

class INetworkEngine;

class MGServerReceiver : public INetworkReceiver, public ISession
{
	typedef std::map<long, ASSOCKDESCEX> SessionMap;
public:
	MGServerReceiver(INetworkEngine* pOwner);
	virtual ~MGServerReceiver(void);

	virtual void notifyRegisterSocket(ASSOCKDESCEX& sockdesc, SOCKADDR_IN& ip) override;
	virtual void notifyReleaseSocket(ASSOCKDESCEX& sockdesc) override;
	virtual void notifyMessage(ASSOCKDESCEX& sockdesc, size_t length, char* data) override;
	virtual void notifyConnectingResult(INT32 requestID, ASSOCKDESCEX& sockdesc, DWORD error);

	virtual bool SendRequest(BasePacket* pPacket) override;

	bool Disconnect(int serial);

protected:

private:
//��Ƽ������� �����ؼ�
	SessionMap m_SessionMap;
	CriticalSectionLock m_SessionLock;

	INetworkEngine* m_pOwner;
};

