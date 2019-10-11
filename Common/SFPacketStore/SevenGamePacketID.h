#pragma once

namespace SevenGame
{
	enum ENUM_PACKET_ID
	{
		InitCardCount = 10000,
		MyCardInfo,
		TableUpdate,
		CurrentTurn,
		TurnPass,
		CardSubmit,
		Winner,
		UserDie,
		PlayerID,
	};
};
