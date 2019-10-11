#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <string>
#include <vector>

#include "ServerSession.h"
#include "Protocol.h"
#include "BasePacket.h"


class ASIOServer
{
public:
	ASIOServer(boost::asio::io_service& io_service, INetworkEngine* pEngine)
		: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
		, m_pOwner(pEngine)
	{
		m_bIsAccepting = false;

		InitializeCriticalSectionAndSpinCount(&m_lock, 4000);
	}

	~ASIOServer()
	{
		DeleteCriticalSection(&m_lock);

		for(auto& iter : m_SessionMap)
		{
			Session* pSession = iter.second;
			if (pSession->Socket().is_open())
			{
				pSession->Socket().close();
			}

			delete pSession;
		}
	}

	void Init( const int nMaxSessionCount )
	{
		for( int i = 0; i < nMaxSessionCount; ++i )
		{
			Session* pSession = new Session( i, m_acceptor.get_io_service(), this );
			pSession->SetOwner(m_pOwner);

			m_SessionMap.insert(std::make_pair(i, pSession));
			m_SessionQueue.push_back( i );
		}
	}

	void Start()
	{
		std::cout << "���� ����....." << std::endl;

		PostAccept();
	}

	void CloseSession( const int nSessionID )
	{
		std::cout << "Ŭ���̾�Ʈ ���� ����. ���� ID: " << nSessionID << std::endl;

		m_SessionQueue.push_back( nSessionID );

		if( m_bIsAccepting == false )
		{
			PostAccept();
		}
	}

	bool SendRequest(BasePacket* pPacket)
	{
		bool bResult = false;
		EnterCriticalSection(&m_lock);

		Session* pSession = FindSession(pPacket->GetSerial());
		if (pSession)
		{
			if (pSession->Socket().is_open())
			{
				pSession->SendRequest(pPacket);

				bResult = true;
			}
		}

		LeaveCriticalSection(&m_lock);

		return false;
	}

	Session* FindSession(unsigned int sessionId)
	{
		auto iter = m_SessionMap.find(sessionId);

		if (iter != m_SessionMap.end())
			return iter->second;

		return 0;
	}

private:
	bool PostAccept()
	{
		if( m_SessionQueue.empty() )
		{
			m_bIsAccepting = false;
			return false;
		}
				
		m_bIsAccepting = true;
		int nSessionID = m_SessionQueue.front();

		m_SessionQueue.pop_front();
		
		auto iter = m_SessionMap.find(nSessionID);
		Session* pSession = iter->second;
		m_acceptor.async_accept(pSession->Socket(),
			boost::bind(&ASIOServer::handle_accept,
												this, 
												pSession,
												boost::asio::placeholders::error)
								);

		return true;
	}

	void handle_accept(Session* pSession, const boost::system::error_code& error)
	{
		if (!error)
		{	
			std::cout << "Ŭ���̾�Ʈ ���� ����. SessionID: " << pSession->SessionID() << std::endl;
			
			pSession->Init();

			_SessionDesc desc;
			desc.sessionType = 0;
			desc.identifier = 1;
			pSession->OnConnect(pSession->SessionID(), desc);

			pSession->PostReceive();
			
			PostAccept();
		}
		else 
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
	}

	bool m_bIsAccepting;

	boost::asio::ip::tcp::acceptor m_acceptor;

	std::map<int, Session* > m_SessionMap;
	std::deque< int > m_SessionQueue;
	CRITICAL_SECTION m_lock;

	INetworkEngine* m_pOwner;
	
};