// ProtocolServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ProtocolLogicEntry.h"
#include "SFCGSFPacketProtocol.h"
#include "ProtocolCGSFHandler.h"
#include "ProtocolProtobufHandler.h"
#include "ProtocolServerProtocol.h"
#include "SFMsgPackProtocol.h"
#include "ProtocolMsgPackHandler.h"
#include "ProtocolAvroProtocol.h"
#include "ProtocolAvroHandler.h"

#pragma comment(lib, "EngineLayer.lib")

void testAvro();
void testProtobuf();
void testCGSF();
void testMsgPack();
void testJson();

int _tmain(int argc, _TCHAR* argv[])
{
	testProtobuf();
	
	SFEngine::GetInstance()->Start(0);

	google::FlushLogFiles(google::GLOG_INFO);

	getchar();

	SFEngine::GetInstance()->ShutDown();

	return 0;
}

void testAvro()
{
	ProtocolLogicEntry<ProtocolAvroHandler>* pLogicEntry = new ProtocolLogicEntry<ProtocolAvroHandler>();
	SFEngine::GetInstance()->Intialize(pLogicEntry);
	SFEngine::GetInstance()->AddPacketProtocol(0, new SFPacketProtocol<ProtocolAvroProtocol>(MAX_IO_SIZE, MAX_PACKET_SIZE));
}

void testProtobuf()
{
	ProtocolLogicEntry<ProtocolProtobufHandler>* pLogicEntry = new ProtocolLogicEntry<ProtocolProtobufHandler>();
	SFEngine::GetInstance()->Intialize(pLogicEntry);
	SFEngine::GetInstance()->AddPacketProtocol(0, new SFPacketProtocol<ProtocolServerProtocol>);

}
void testCGSF()
{
	ProtocolLogicEntry<ProtocolCGSFHandler>* pLogicEntry = new ProtocolLogicEntry<ProtocolCGSFHandler>();	
	SFEngine::GetInstance()->Intialize(pLogicEntry);
	SFEngine::GetInstance()->AddPacketProtocol(0, new SFPacketProtocol<SFCGSFPacketProtocol>(MAX_IO_SIZE, MAX_PACKET_SIZE, CGSF_PACKET_OPTION));
}
void testMsgPack()
{
	ProtocolLogicEntry<ProtocolMsgPackHandler>* pLogicEntry = new ProtocolLogicEntry<ProtocolMsgPackHandler>();
	SFEngine::GetInstance()->Intialize(pLogicEntry);
	SFEngine::GetInstance()->AddPacketProtocol(0, new SFPacketProtocol<SFMsgPackProtocol>(MAX_IO_SIZE, MAX_PACKET_SIZE));
}

void testJson()
{

}