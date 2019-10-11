#pragma once
#include "SFLogicContents.h"
#include "SFObjectPool.h"
#include "ILogicEntry.h"
#include "SFDataBaseProxy.h"
#include "SFDirectoryWatcherTask.h"
#include <SFPacketStore/SFPacketStore.pb.h>
#include "SFRoomManager.h"
#include "SFLobby.h"
#include "SFRoomFSM.h"
#include "SFDatabaseProxyLocal.h"
#include "SFDatabaseProxyImpl.h"
#include "SFEngine.h"
#include "SFMySQL.h"

///////////////////////////////////////////////////////////////////////////
//Logic Entry
//��� ������ ó���� �����Դϴ�.
//���� : �� Ŭ������ �̱������� ���Դϴ�.
///////////////////////////////////////////////////////////////////////////
class SFPlayer;
class SFMessage;
class SFRoomManager;
class SFLobby;
class SFCommand;
class SFGameMode;
class SFPlayerManager;

#include <map>

class SFLogicEntry : public ILogicEntry
{
public:
	typedef std::map<int, SFPlayer*> PlayerMap;
	typedef std::map<int, SFGameMode*> GameModeMap;

	SFLogicEntry(void);
	virtual ~SFLogicEntry(void);

	virtual bool Initialize() override;
	static SFLogicEntry* GetLogicEntry(){return m_pLogicEntry;}

	virtual bool ProcessPacket(BasePacket* pBase) override;
	
	BOOL CreateDirectoryWathcer();

	SFRoomManager* GetRoomManager(){return m_pRoomManager;}
	SFLobby* GetLobby(){return m_pLobby;}

	SFPlayerManager* GetPlayerManager(){return m_pPlayerManager;}

	BOOL Send(SFPlayer* pPlayer, BasePacket* pPacket);
	BOOL SendRequest(BasePacket* pPacket);

	SFDatabaseProxy* GetDataBaseProxy(){return m_pDatabaseProxy;}

	BOOL AddGameMode(int Mode, SFGameMode* pMode);
	GameModeMap* GetGameModeMap(){return &m_GameModeMap;}

	void SetP2PService(bool bP2PService){ m_bP2PService = bP2PService; }
	bool GetP2PService(){ return m_bP2PService; }

protected:
	BOOL OnConnectPlayer(int serial);
	BOOL OnPlayerData(BasePacket* pPacket);
	BOOL OnTimer(BasePacket* pPacket);
	BOOL OnShouter(BasePacket* pPacket);

	BOOL OnDBResult(SFMessage* pMessage);
	BOOL OnDisconnectPlayer(int serial);

private:
	static SFLogicEntry* m_pLogicEntry;
	bool m_bP2PService;

	SFObjectPool<SFPlayer>* m_pPlayerPool;
	PlayerMap m_PlayerMap;

	GameModeMap m_GameModeMap;
	
	SFRoomManager* m_pRoomManager;
	SFPlayerManager* m_pPlayerManager;
	SFLobby* m_pLobby;

	SFDatabaseProxy* m_pDatabaseProxy;
	SFDirectoryWatcherTask m_DirectoryWatcherTask;

/////////////////////////////////////////////////////////////////////////////////////////////
//20120619 ���� ó�� ���� �ʼ� �����͸� ��� Ŭ����
//���� ���۽� �ѹ� �ε�ǰ� �ٽô� ������� ���� ���̱⿡ ������ ������
//SFLogicEntry�� �̱������� ���̰� ������ ������ �����ȴٸ� SFLogicContents�� ���� ����� �̱������� ���� �ʿ� ����
/////////////////////////////////////////////////////////////////////////////////////////////
	SFLogicContents m_LogicContents;
};
