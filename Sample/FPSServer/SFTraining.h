#pragma once
#include "SFGameMode.h"

class SFTraining : public SFGameMode
{
public:
	SFTraining(int Mode);
	virtual ~SFTraining(void);

	virtual SFGameMode* Clone()
	{
		return new SFTraining(GetGameMode());
	}

	virtual BOOL OnEnter(int GameMode) override;
	virtual BOOL Onleave() override;

	virtual BOOL Update(DWORD timerId) override;

	BOOL ProcessUserRequest(SFPlayer* pPlayer, BasePacket* pPacket) override;
	BOOL ProcessUserRequest(SFPlayer* pPlayer, int Msg) override;

};
