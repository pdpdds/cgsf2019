#include "stdafx.h"
#include "SFServerConnectionManager.h"
#include "SFPacketProtocolManager.h"
#include "SFEngine.h"
#include "XML/StringConversion.h"
#include "XML/Markup.h"
#include <process.h>

SFServerConnectionManager::SFServerConnectionManager()
: m_hThread(NULL)
, m_hTimerEvent(NULL)
, m_dwThreadID(0)
, m_bThreadEnd(false)
{
}


SFServerConnectionManager::~SFServerConnectionManager()
{
	if (m_hThread)
	{
		m_bThreadEnd = true;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

/*
bool SFServerConnectionManager::Save()
{
	CMarkup xml;

	xml.AddElem(L"SERVERLIST");
	xml.AddChildElem(L"SERVER");
	xml.IntoElem();
	xml.AddChildElem(L"IP", L"127.0.0.1");
	xml.AddChildElem(L"PORT", 10000);
	xml.AddChildElem(L"IDENTIFER", 10000);
	xml.AddChildElem(L"DESC", L"GAME DATABASE SERVER");
	xml.OutOfElem();

	xml.AddChildElem(L"SERVER");
	xml.IntoElem();
	xml.AddChildElem(L"IP", L"127.0.0.1");
	xml.AddChildElem(L"PORT", 10001);
	xml.AddChildElem(L"IDENTIFER", 9000);
	xml.AddChildElem(L"DESC", L"AUTH SERVER");
	xml.OutOfElem();

	std::wstring csXML = xml.GetDoc();
	xml.Save(L"ServerConnection.xml");

	wprintf(L"%s", csXML.c_str());

	return true;
}*/

bool SFServerConnectionManager::LoadConnectorList(WCHAR* szFileName)
{
	CMarkup xml;
	bool result = xml.Load(szFileName);

	if (result == false)
		return false;

	while (xml.FindChildElem(L"SERVER"))
	{
		_ConnectorInfo connectorInfo;
		xml.IntoElem();

		xml.FindChildElem(L"IP");
		connectorInfo.szIP = xml.GetChildData();

		xml.FindChildElem(L"PORT");
		connectorInfo.port = _ttoi(xml.GetChildData().c_str());

		xml.FindChildElem(L"IDENTIFER");
		connectorInfo.connectorId = _ttoi(xml.GetChildData().c_str());

		xml.FindChildElem(L"DESC");
		connectorInfo.szDesc = xml.GetChildData();

		xml.OutOfElem();

		
		AddConnectorInfo(connectorInfo);
	}

	return true;
}

void SFServerConnectionManager::AddConnectorInfo(_ConnectorInfo& connectorInfo)
{
	m_listConnectorInfo.push_back(connectorInfo);
	SFEngine::GetInstance()->GetPacketProtocolManager()->AddConnectorInfo(&connectorInfo);

	LOG(INFO) << "AddConnectorInfo. " << "connectID: " << connectorInfo.connectorId << ", ProtocolID: " << (int)connectorInfo.packetProtocolId << ", IP: " << connectorInfo.szIP.data() << ", Port: " << connectorInfo.port;
	google::FlushLogFiles(google::GLOG_INFO);
}

bool SFServerConnectionManager::SetupServerReconnectSys()
{
	m_hTimerEvent = CreateEvent(NULL, FALSE, FALSE, L"ServerReconnectEvent");

	LOG(INFO) << "SetupServerReconnectSys. " << "First Connect";
	google::FlushLogFiles(google::GLOG_INFO);

	for (auto& iter : m_listConnectorInfo)
	{
		_ConnectorInfo& info = iter;		

		LOG(INFO) << "connectID: " << info.connectorId << ", ProtocolID: " << (int)info.packetProtocolId << ", IP: " << info.szIP.data() << ", Port: " << info.port;
		google::FlushLogFiles(google::GLOG_INFO);

		int serial = -1;
		serial = SFEngine::GetInstance()->AddConnector(info.connectorId, (char*)StringConversion::ToASCII(info.szIP.c_str()).c_str(), info.port);

		if (serial >= 0)
		{
			info.connected = true;
		}
	}

	m_hThread = (HANDLE)_beginthreadex(0, NULL, ServerReconnectProc, this, 0, (unsigned*)&m_dwThreadID);

	if (!m_hThread)
	{
		int errorNum = (GetLastError() == ERROR_SUCCESS) ? ERROR_MAX_THRDS_REACHED : GetLastError();
		LOG(FATAL) << "ServerReconnectProc Thread Creation Fail. Error : " << errorNum;
		return false;
	}
	
	LOG(INFO) << "ServerReconnectProc success";

	return true;
}

UINT SFServerConnectionManager::ServerReconnectProc(LPVOID arg)
{
	SFServerConnectionManager * pConnectionManager = reinterpret_cast<SFServerConnectionManager*>(arg);
	
	while (WaitForSingleObject(pConnectionManager->m_hTimerEvent, 1000) != WAIT_OBJECT_0 && pConnectionManager->m_bThreadEnd == false)
	{
		for (auto& iter : pConnectionManager->m_listConnectorInfo)
		{
			_ConnectorInfo& info = iter;

			if (info.connected == false)
			{
				int serial = -1;
				serial = SFEngine::GetInstance()->AddConnector(info.connectorId, (char*)StringConversion::ToASCII(info.szIP.c_str()).c_str(), info.port);

				if (serial >= 0)
				{
					info.connected = true;
				}
			}			
		}
	}

	return 0;
}

bool SFServerConnectionManager::SetConnectorState(int connectorId, bool connected)
{
	for (auto& iter : m_listConnectorInfo)
	{
		_ConnectorInfo& info = iter;

		if (info.connectorId == connectorId)
		{
			info.connected = connected;
			return true;
		}
	}

	return false;
}