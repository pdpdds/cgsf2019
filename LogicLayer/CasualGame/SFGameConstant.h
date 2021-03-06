#pragma once

/////////////////////////////////////////////////////////////////////////
enum ePlayerState
{
	PLAYER_STATE_NONE = 0,
	PLAYER_STATE_INIT,
	PLAYER_STATE_LOGIN,
	PLAYER_STATE_LOBBY,
	PLAYER_STATE_ROOM,
	PLAYER_STATE_LOADING,
	PLAYER_STATE_PLAYREADY,
	PLAYER_STATE_PLAY,
	PLAYER_STATE_PLAYEND,
};

enum eRoomState
{
	ROOM_STATE_NONE = 0,
	ROOM_STATE_WAIT,
	ROOM_STATE_LOADING,
	ROOM_STATE_PLAYREADY,
	ROOM_STATE_PLAY,
	ROOM_STATE_PLAYEND,
};

enum eTeamType
{
	TEAM_RED = 0,
	TEAM_BLUE,
	TEAM_TYPE_MAX,
};