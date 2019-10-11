#include "StdAfx.h"
#include "SFFreeForAll.h"
#include "SFRoom.h"
#include "SFRoomPlay.h"
#include "GamePacketStructure.h"
#include "SFPlayer.h"
#include "FPSSendPacket.h"
#include "BasePacket.h"
#include "SFProtobufPacket.h"

SFFreeForAll::SFFreeForAll(int Mode)
: SFGameMode(Mode)
{

}

SFFreeForAll::~SFFreeForAll(void)
{
}

BOOL SFFreeForAll::OnEnter( int GameMode )
{
	return TRUE;
}

BOOL SFFreeForAll::Onleave()
{
	return TRUE;
}

BOOL SFFreeForAll::Update(DWORD timerId)
{
	return TRUE;
}

BOOL SFFreeForAll::ProcessUserRequest( SFPlayer* pPlayer, int Msg )
{
	return TRUE;
}

BOOL SFFreeForAll::ProcessUserRequest( SFPlayer* pPlayer, BasePacket* pPacket )
{
	SFRoom* pRoom = GetOwner()->GetOwner();

	switch(pPacket->GetPacketID())
	{
	case FPS::MSG_PLAYER_SCORE:
	case FPS::MSG_PLAYER_WEAPON_CHANGE:
	case FPS::MSG_PLAYER_WEAPON_CHANGING:
		{
			pRoom->BroadCast(pPacket);

		}
		break;
	case FPS::MSG_SPAWN_PLAYER:
		{
			OnSpawnPlayer(pPlayer, pPacket);
		}
		break;
	case FPS::MSG_PLAYER_HEALTH:
		{
			OnPlayerHealth(pPlayer, pPacket);
		}
	}

	return TRUE;
}

BOOL SFFreeForAll::OnPlayerHealth(SFPlayer* pPlayer, BasePacket* pPacket)
{
	SFRoom* pRoom = GetOwner()->GetOwner();

	SFProtobufPacket<FPSPacket::MSG_PLAYER_HEALTH>* pHealth = (SFProtobufPacket<FPSPacket::MSG_PLAYER_HEALTH>*)pPacket;
	
	PlayerHealthMsg msg;

	SF_GETPACKET_ARG(&msg, pHealth->GetData().playerhealth(), PlayerHealthMsg);

	SFPlayer* pHurtPlayer = NULL;
	SFRoom::RoomMemberMap mapMember = pRoom->GetRoomMemberMap();

	for (auto& iter : mapMember)
	{
		if(iter.second->GetSerial() == msg.PlayerID)
		{
			pHurtPlayer = iter.second;
			break;
		}
	}

	if(pHurtPlayer == NULL)
		return FALSE;

	_CharacterInfo* pInfo = pHurtPlayer->GetCharacterInfo();
	
	pInfo->health = msg.health;

	if(pInfo->health == 0)
	{
		pInfo->IsAlive = false;
		pInfo->DeathTime = GetTickCount();
	}

	for (auto& iter : mapMember)
	{
		SFPlayer* pTarget = iter.second;
		SendPlayerHealth(pTarget, pHurtPlayer);
	}

	return TRUE;
}

BOOL SFFreeForAll::OnSpawnPlayer(SFPlayer* pPlayer, BasePacket* pPacket)
{
	SFRoom* pRoom = GetOwner()->GetOwner();

	SFProtobufPacket<FPSPacket::MSG_SPAWN_PLAYER>* pSpawnPlayer = (SFProtobufPacket<FPSPacket::MSG_SPAWN_PLAYER>*)pPacket;

	SpawnPlayerMsg msg;

	SF_GETPACKET_ARG(&msg, pSpawnPlayer->GetData().spawnplayer(), SpawnPlayerMsg);

	_CharacterInfo* pInfo = pPlayer->GetCharacterInfo();
	pInfo->translation = msg.translation;
	pInfo->IsAlive = true;

	SFRoom::RoomMemberMap mapMember = pRoom->GetRoomMemberMap();

	for (auto& iter : mapMember)
	{
		SFPlayer* pTarget = iter.second;
		SendSpawnPlayer(pTarget, pPlayer);
	}

	return TRUE;
}
