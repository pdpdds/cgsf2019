#pragma once
#include "SFPlayerState.h"

class SFPlayerLoading : public SFPlayerState
{
public:
	SFPlayerLoading(SFPlayer* pOwner, ePlayerState State);
	virtual ~SFPlayerLoading(void);

	virtual BOOL OnEnter() override;
	virtual BOOL OnLeave() override;
	virtual BOOL ProcessPacket(BasePacket* pPacket) override;
	virtual BOOL ProcessDBResult(SFMessage* pMessage) override;

protected:
	BOOL OnLoadingComplete(BasePacket* pPacket);

private:
};
