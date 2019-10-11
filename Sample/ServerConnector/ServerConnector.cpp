// ServerConnector.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ServerConnectorLogicEntry.h"
#include "SFJsonProtocol.h"
#include "SFJsonPacket.h"
#include "SFServerConnectionManager.h"
#include "AuthServerCallback.h"
#include "DatabaseServerCallback.h"

#pragma comment(lib, "EngineLayer.lib")

#define GAME_DATABASE_SERVER_1 10000
#define AUTH_SERVER_1 9000

#define PACKET_PROTOCOL_JSON_1		1
#define PACKET_PROTOCOL_JSON_2		2
#define PACKET_PROTOCOL_PROTO_BUF	3

int _tmain(int argc, _TCHAR* argv[])
{
	ServerConnectorLogicEntry* pLogicEntry = new ServerConnectorLogicEntry();

//������ Ŀ���Ϳ� ���� �ݹ� ��ü�� �߰��Ѵ�.
	AuthServerCallback* pAuthServerCallback = new AuthServerCallback();
	DatabaseServerCallback* pDatabaseServerCallback = new DatabaseServerCallback();

	SFEngine::GetInstance()->Intialize(pLogicEntry);
	SFEngine::GetInstance()->AddPacketProtocol(PACKET_PROTOCOL_JSON_1, new SFPacketProtocol<SFJsonProtocol>);

	if (false == SFEngine::GetInstance()->LoadConnectorList(L"ServerConnection.xml"))
		return 0;

	pLogicEntry->AddConnectorCallback(AUTH_SERVER_1, pAuthServerCallback, PACKET_PROTOCOL_JSON_1);
	pLogicEntry->AddConnectorCallback(GAME_DATABASE_SERVER_1, pDatabaseServerCallback, PACKET_PROTOCOL_JSON_1);

	if (false == SFEngine::GetInstance()->SetupServerReconnectSys())
		return 0;

	getchar();

	SFEngine::GetInstance()->ShutDown();

	return 0;
}