// ChatServer.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "ChatLogicEntry.h"
#include "SFJsonProtocol.h"
#include "SFCasualGameDispatcher.h"

#pragma comment(lib, "EngineLayer.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	ChatLogicEntry* pLogicEntry = new ChatLogicEntry();

	SFEngine::GetInstance()->Intialize(pLogicEntry);
	SFEngine::GetInstance()->AddPacketProtocol(0, new SFPacketProtocol<SFJsonProtocol>);
//Ÿ�̸� ��� �׽�Ʈ �ڵ�
	SFEngine::GetInstance()->AddTimer(100, 500, 1000);
	SFEngine::GetInstance()->Start(0);

	getchar();

	SFEngine::GetInstance()->ShutDown();

	return 0;
}