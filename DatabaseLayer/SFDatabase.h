#pragma once
#include "DBStruct.h"
#include "SFDBAdaptor.h"
#include "SFObjectPool.h"
#include "SFDBPacketSystem.h"
#include "BasePacket.h"
#include "SFIni.h"

class SFDatabase
{
public:
	SFDatabase(SFDBAdaptor* pAdaptor) : m_pAdaptor(pAdaptor)
	{					
	}

	virtual ~SFDatabase(void)
	{
	}

	BOOL Initialize()
	{
		SFIni ini;
		_DBConnectionInfo Info;

		ini.SetPathName(_T("./DataSource.ini"));
		ini.GetString(L"DataSourceInfo",L"DataSource",Info.szDataSource, 100);
		ini.GetString(L"DataSourceInfo",L"Database",Info.szDBName, 100);
		ini.GetString(L"DataSourceInfo",L"User",Info.szUser, 100);
		ini.GetString(L"DataSourceInfo",L"Password",Info.szPassword, 100);
		ini.GetString(L"DataSourceInfo",L"IP",Info.IP, 20);
		Info.Port = ini.GetInt(L"DataSourceInfo",L"PORT",0);

		SetInfo(Info);

		return m_pAdaptor->Initialize(&m_Info);
	}

	BOOL Call(BasePacket* pMessage)
	{
		ASSERT(pMessage->GetPacketType() == SFPACKET_DB);
		m_pAdaptor->Call(pMessage);

		return TRUE;
	}
	
	static _DBConnectionInfo* GetInfo(){return &m_Info;}
	static void SetInfo(_DBConnectionInfo& Info){m_Info = Info;}


protected:

private:
	SFDBAdaptor* m_pAdaptor;
	static _DBConnectionInfo m_Info;
};

