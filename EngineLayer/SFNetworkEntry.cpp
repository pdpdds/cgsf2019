#include "stdafx.h"
#include "SFNetworkEntry.h"
#include "SFCasualGameDispatcher.h"
#include "SFTCPNetwork.h"
#include "SFUDPNetwork.h"
#include "SFIni.h"
#include <XML/StringConversion.h>

SFNetworkEntry* SFNetworkEntry::m_pNetworkEntry = NULL;

SFNetworkEntry::SFNetworkEntry(void)
	: m_pTCPNetwork(NULL)
	, m_pUDPNetwork(NULL)
{
}

SFNetworkEntry::~SFNetworkEntry(void)
{
}

SFNetworkEntry* SFNetworkEntry::GetInstance()
{
	if (m_pNetworkEntry == NULL)
		m_pNetworkEntry = new SFNetworkEntry();

	return m_pNetworkEntry;
}

bool SFNetworkEntry::TCPSend(BasePacket* pPacket )
{
	if(m_pTCPNetwork)
		return (bool)m_pTCPNetwork->SendRequest(pPacket);

	return false;
}

bool SFNetworkEntry::UDPSend(unsigned char* pMessage, int BufSize )
{
	if(m_pUDPNetwork)
	return m_pUDPNetwork->Send((unsigned char*)pMessage, BufSize);

	return false;
}

bool SFNetworkEntry::Initialize(INetworkCallback* pTCPCallBack, IPacketProtocol* pProtocol, ILogicDispatcher* pDispatcher, IUDPNetworkCallback* pUDPCallback)
{
	if(pTCPCallBack == NULL)
		return FALSE;

	SFIni ini;
	WCHAR szEngine[MAX_PATH];
	TCHAR szP2PModuleName[MAX_PATH];
	ini.SetPathName(_T("./Connection.ini"));
	ini.GetString(L"Engine",L"NAME",szEngine, MAX_PATH);
	ini.GetString(L"P2PModule", L"NAME", szP2PModuleName, MAX_PATH);

	std::string str = StringConversion::ToASCII(szEngine);

	m_pTCPNetwork = new SFTCPNetwork();
	m_pTCPNetwork->Initialize((char*)str.c_str(), pTCPCallBack);

	if(pUDPCallback)
	{
		m_pUDPNetwork = new SFUDPNetwork();
		m_pUDPNetwork->Initialize(pUDPCallback, szP2PModuleName);
	}

	m_pTCPNetwork->SetPacketProtocol(pProtocol);

	if (pDispatcher == NULL)
		pDispatcher = new SFCasualGameDispatcher();

	m_pTCPNetwork->SetLogicDispatcher(pDispatcher);	
	
	return true;
}

bool SFNetworkEntry::ShutDown()
{
	if(m_pTCPNetwork)
		delete m_pTCPNetwork;

	return true;
}

bool SFNetworkEntry::Run()
{
	SFIni ini;
	
	WCHAR szIP[20];
	USHORT Port;

	ini.SetPathName(_T("./Connection.ini"));
	ini.GetString(L"ServerInfo",L"IP",szIP, 20);
	Port = ini.GetInt(L"ServerInfo",L"PORT",0);

	std::string str = StringConversion::ToASCII(szIP);
	if(m_pTCPNetwork->Start((char*)str.c_str(), Port) == FALSE)
	{
		return false;
	}

	if(m_pUDPNetwork)
	{
		m_pUDPNetwork->Start();
	}

	return Update();;
}

bool SFNetworkEntry::Update()
{
	m_pTCPNetwork->Update();

	if(m_pUDPNetwork)
		m_pUDPNetwork->Update();

	return true;
}

bool SFNetworkEntry::IsConnected()
{
	return m_pTCPNetwork->IsConnected();
}

bool SFNetworkEntry::AddPeer(int serial, int externalIP, short externalPort, int localIP, short localPort)
{
	return m_pUDPNetwork->AddPeer(serial, externalIP, externalPort, localIP, localPort);
}

bool SFNetworkEntry::DeletePeer(int serial)
{
	return m_pUDPNetwork->DeletePeer(serial);
}

