#pragma once
#include "../Common/P2PCommon.h"
#include "../Interface/P2PClient.h"
#include "../Interface/IP2PManager.h"
#include <SDL2/SDL_net.h>
#include <map>

class IUDPNetworkCallback;

class P2PManager : public IP2PManager
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
	BOOL DataSend(const char* pszIP, USHORT uiPort, USHORT uiLen, const BYTE* pbyBuff); //Ư�� �����ǿ� ������ ������	

	void HandleRecv();
	bool UpdateConnectionState();
	bool SendUDPPingPongPacket(int channel);
	bool EncodePacket(void* pData, UDPpacket* pPacket, int len);

	P2PPeerInfo* FindPeer(int channel);

protected:
	void ProcessPublicIP(UDPpacket* pPacket);
	void ProcessPingPongReq(UDPpacket* pPacket);
	void ProcessPingPongRes(UDPpacket* pPacket);

private:
	struct lfds611_queue_state* m_pQueue;
	int m_peerIndex;
	std::map<int, P2PPeerInfo> m_mapPeer;

	///////////////////////////////////////////////////////////////////////////////////////
	BOOL PushPacket(BYTE* pData, int Length);
};

