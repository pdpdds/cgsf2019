#pragma once
#include <map>
#include "SFGameMode.h"

class SFRoomPlay;

class SFGameModeFSM : public SFGameMode
{
	friend class SFRoomPlay;
	typedef std::map<int, SFGameMode*> GameModeMap;

public:
	SFGameModeFSM(int Mode);
	virtual ~SFGameModeFSM(void);

	virtual SFGameMode* Clone(){return FALSE;} //�޼ҵ带 �� Ŭ������ �и��ϴ� ���� ����

	BOOL Initialize(SFRoomPlay* pOwner);

	virtual BOOL OnEnter(int gameMode) override;
	virtual BOOL Onleave() override;

	virtual BOOL Update(DWORD timerId) override;

protected:
	BOOL AddGameMode(SFRoomPlay* pOwner);
	BOOL ProcessUserRequest(SFPlayer* pPlayer, BasePacket* pPacket) override;
	BOOL ProcessUserRequest(SFPlayer* pPlayer, int Msg) override;
	BOOL SetGameMode(int gameMode);

private:
	GameModeMap m_GameModeMap;
	SFGameMode* m_pCurrentGameMode;
};
