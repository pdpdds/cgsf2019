// ServerListener.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ServerListenerLogicEntry.h"
#include "SFJsonProtocol.h"
#include "SFCGSFPacketProtocol.h"

#pragma comment(lib, "EngineLayer.lib")

#define RPC_LISTEN_PORT 10000
#define GAMESERVER_LISTEN_PORT 10001
#define AUTHSERVER_LISTEN_PORT 10002

#define PACKET_PROTOCOL_JSON_1		1
#define PACKET_PROTOCOL_JSON_2		2
#define PACKET_PROTOCOL_PROTO_BUF	3

int _tmain(int argc, _TCHAR* argv[])
{
	ServerListenerLogicEntry* pLogicEntry = new ServerListenerLogicEntry();
	SFEngine::GetInstance()->Intialize(pLogicEntry);

	SFEngine::GetInstance()->AddPacketProtocol(PACKET_PROTOCOL_JSON_1, new SFPacketProtocol<SFJsonProtocol>);
	SFEngine::GetInstance()->AddPacketProtocol(PACKET_PROTOCOL_JSON_2, new SFPacketProtocol<SFJsonProtocol>(16384, 8192, 0));
	SFEngine::GetInstance()->AddPacketProtocol(PACKET_PROTOCOL_PROTO_BUF, new SFPacketProtocol<SFCGSFPacketProtocol>);

	int rpcListener = SFEngine::GetInstance()->AddListener(nullptr, RPC_LISTEN_PORT, PACKET_PROTOCOL_JSON_1);
	int gameListener = SFEngine::GetInstance()->AddListener(nullptr, GAMESERVER_LISTEN_PORT, PACKET_PROTOCOL_JSON_2);
	int authListener = SFEngine::GetInstance()->AddListener(nullptr, AUTHSERVER_LISTEN_PORT, PACKET_PROTOCOL_PROTO_BUF);
	
	if (false == SFEngine::GetInstance()->Start(PACKET_PROTOCOL_JSON_1))
	{
		LOG(ERROR) << "Server Start Fail";
		SFEngine::GetInstance()->ShutDown();
		return 0;
	}	

	getchar();

	SFEngine::GetInstance()->ShutDown();

	return 0;
}