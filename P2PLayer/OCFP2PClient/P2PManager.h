#ifndef __P2PMANAGER_H__
#define __P2PMANAGER_H__
#include "../Interface/P2PClient.h"
#include "../Interface/IP2PManager.h"
#include "liblfds611.h"

class IUDPNetworkCallback;
class CPuPeers;

class P2PManager : public IP2PManager, public CPuPeers
{
public:
	P2PManager(void);
	virtual ~P2PManager(void);

	BOOL Initialize(IUDPNetworkCallback* pCallback) override;
	BOOL Finally() override;

	BOOL Update() override;

	BOOL RunP2P(char* szIP, unsigned short Port) override;
	BOOL EndP2P() override;

	BOOL AddPeer(PeerAddressInfo* pAddressInfo, BYTE& byIndex);//�Ǿ� �߰� �߰��ϸ鼭 ����� ������ �������� ��� üũ�Ѵ�.
	BOOL BroadCast(BYTE* pData, USHORT Size, BOOL ExceptMe = FALSE); //�Ǿ�� ������ ����. �Ǿ�� ������ ���� ������ �Ұ����ϸ� ������ ������ �Ǿ�� ������ ����. �װ͵� �ȵǸ� ������ ���� �̿�
	BOOL BroadCastWithRelayServer(BYTE* pData, USHORT Size); //������ ������ �̿��ؼ� ������ ��ε� ĳ��Ʈ
	BOOL Disconnect(); //�Ǿ����� ������ ���´�.
	BOOL BroadcastEcho(); //�Ǿ�鿡�� ���� ��ȣ�� ������.
	BOOL RemovePeer(BYTE byIndex); //Ư���Ǿ� ����..
	BOOL DataSend( const char* pszIP, USHORT uiPort, USHORT uiLen, const BYTE* pbyBuff); //Ư�� �����ǿ� ������ ������	

protected:
	void ProcessDataPeer(const SOCKADDR_IN& stRemote, PU_PEERADDRTYPE enumAddrType, ULONG ulLen, const BYTE* pbyBuff) override;
	void OnResCheckExternalIp( const SOCKADDR_IN& stRemote, PU_PEERADDRTYPE enumAddrType, ULONG ulRemoteE, USHORT uiRemotePortE ) override;

private:
	SOCKADDR_IN m_sinLocal;
	struct lfds611_queue_state* m_pQueue;

///////////////////////////////////////////////////////////////////////////////////////
	BOOL PushPacket(BYTE* pData, int Length);
};

#endif