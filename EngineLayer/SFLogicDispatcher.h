#pragma once
#include "ILogicDispatcher.h"
#include "SFEngine.h"
#include "BasePacket.h"

class SFLogicDispatcher : public ILogicDispatcher
{
public:
	SFLogicDispatcher()
	{
	}
	
	virtual ~SFLogicDispatcher()
	{
	}

	static bool ReleasePacket(BasePacket* pPacket)
	{
		//����� ��Ŷ�� �����Ѵ�. ��Ŷ�� Ÿ�Կ� ���� ������ ���°� �ٸ�
		switch (pPacket->GetPacketType())
		{
		case SFPACKET_DATA:
			SFEngine::GetInstance()->ReleasePacket(pPacket);
			break;
		case SFPACKET_CONNECT:
		case SFPACKET_DISCONNECT:
		case SFPACKET_TIMER:
		case SFPACKET_SHOUTER:
		case SFPACKET_SERVERSHUTDOWN:
			delete pPacket;
			break;

		default:
			SFASSERT(0);
			return false;
		}

		return true;
	}

};

