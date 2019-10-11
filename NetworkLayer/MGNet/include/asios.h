#ifndef ASIOS_H
#define ASIOS_H

#pragma once

#include "variver.h"
#include "mytypedef.h"
#include "maths.h"
#include "Synchronized.h"

struct OverlappedOperation
{
	enum eOpType
	{
		eOpType_Write,
		eOpType_Read,
		eOpType_Connect,
	};

	OVERLAPPED		overlapped;	
	size_t			length; // Length of the actual data to send,	
	int			optype; //
	int				opcode;
	INT32			requesterID; // [��û�ڰ� �ǳ״� ���а�] ���� connectSocket�� ���� ���� ��. �׿� �پ��� �������� ����� �� ����
	size_t			maxLength;
	char*			datas; // Used to hold data to send, direct new/delete or refer from pool

	OverlappedOperation() : overlapped(), length(0), optype(eOpType_Write), opcode(0), requesterID(0), maxLength(0), datas(NULL)
	{
	}

	OverlappedOperation(size_t maxLength) : overlapped(), length(0), optype(eOpType_Write), opcode(0), requesterID(0), maxLength(maxLength), datas(NULL)
	{
		if(0 < maxLength)
			datas = new char[maxLength];
	}

	~OverlappedOperation()
	{
		if(NULL != datas)
			delete[] datas;
	}

	void initHeader()
	{
		ZeroMemory(&overlapped, sizeof(overlapped));	
		length = 0;
		optype = eOpType_Write;
		opcode = 0;
		requesterID = 0; 
	}
};

struct ASSOCKDESC
{
	ASSOCKUID		assockUid;
	LARGE_INTEGER	tick;

	ASSOCKDESC() : assockUid(), tick()
	{
	}

	ASSOCKDESC(ASSOCKUID assockUid, LARGE_INTEGER& tick) : assockUid(assockUid), tick(tick)
	{
	}

	ASSOCKDESC(ASSOCKUID assockUid, LARGE_INTEGER* atick) : assockUid(assockUid), tick()
	{
		if(atick)
		{
			tick = *atick;
		}
	}
};

class INetworkSender;
class INetworkReceiver;
struct ASSOCKDESCEX : public ASSOCKDESC
{
	INetworkSender*	psender;

	ASSOCKDESCEX() : psender()
	{
	}

	ASSOCKDESCEX(INetworkSender* psender) : psender(psender)
	{
	}
};


class AsynchSocket
{
private:
	static AsynchSocket*		sAsynchSocketPrototype;

protected:
	static ULONG_PTRGenerator	assockuidGen;
	static CriticalSectionLock	assockuidGenLock;

protected:
	SOCKET			sockID;
	ASSOCKDESCEX	assockDesc;
	LONG			referCount;
	int				closed;

public:

	static AsynchSocket* GetASPrototype()
	{
		Synchronized sync(&assockuidGenLock);
		if(NULL == sAsynchSocketPrototype)
		{
			sAsynchSocketPrototype = new AsynchSocket();
		}
		return sAsynchSocketPrototype;
	}
	
	static ASSOCKUID retrieveASSOCKUID()
	{
		Synchronized sync(&assockuidGenLock);
		ASSOCKUID id = assockuidGen.retrieve();
		return id;
	}

	static void releaseASSOCKUID(ASSOCKUID id)
	{
		Synchronized sync(&assockuidGenLock);
		assockuidGen.release(id);
	}

public:
	AsynchSocket() : sockID(INVALID_SOCKET), assockDesc(), referCount(0), closed(0)
	{
		//assockuid�� ���⼭ �߾Ӱ����Ѵ�.
		assockDesc.assockUid = retrieveASSOCKUID();
		QueryPerformanceCounter(&assockDesc.tick);
	}

	virtual ~AsynchSocket()
	{
		//assockuid��ȯ�� asio�� ���ؼ� �Ѵ�.
	}

	virtual AsynchSocket* clone()
	{
		// �׳� ���� Ÿ���� �ν��Ͻ��� �ϳ� ������Ų��.
		return new AsynchSocket();
	}

	virtual void handleConnected() {}
	virtual size_t makePacket(char* dest, size_t destmaxsize, char* src, size_t srcsize);
	virtual int handleCompletionOfReceive(INetworkReceiver* receiver, INetworkSender* sender, DWORD bytesTransfer, char* datas);

	int getClosed() 
	{ 
		return closed; 
	}

	ASSOCKUID getASSOCKUID()
	{
		return assockDesc.assockUid;
	}

	void setSender(INetworkSender* sender)
	{
		assockDesc.psender = sender;
	}

	void setSOCKET(SOCKET id)
	{
		sockID = id;
	}

	SOCKET getSOCKET()
	{
		return sockID;
	}

	void getASSOCKDESCEX(ASSOCKDESCEX& output)
	{
		output = assockDesc;
	}

	LONG enterIO()
	{			
		// refercount�� Ŭ��������ϱ� ���� 4����Ʈ ������ ���ص� �ɵ�...
		return InterlockedIncrement(&referCount);
	}

	LONG exitIO()
	{		
		return InterlockedDecrement(&referCount);
	}

	bool disconnect();

	

};

class ASIOException
{	
	DWORD		errorcode;
	char		msg[128];
public:

	ASIOException( DWORD error, char* amsg ) : errorcode(error)
	{
		ZeroMemory(msg,sizeof(msg));
		if(amsg)
		{
			size_t len = strlen(amsg);
			if(sizeof(msg) > len)
			{
				strncpy_s(msg, amsg, len);
			}
		}

		// �߻��ڵ��α�
		//	�߻��� Ư�����Ͽ� �α׸� �����,
		//	�ݽ����� �������ر��� ����� �ش�.
	}

	virtual ~ASIOException()
	{
	}

	const char* toMsg()
	{
		return msg;
	}

	DWORD toError()
	{
		return errorcode;
	}
};

class INetworkSender
{
public:
	// �Ʒ��� �Լ����� �ʿ信 ���� INetworkReceiver ��ü���� �޼ҵ带 ������.
	// ���� INetworkReceiver ������ ��ü�� �� �Լ����� ȣ���ϸ� live lock�� �ɸ���.
	//�۽� ��û
	//socketUniqueId: ���ϰ�����ȣ, �����뺸�� �Ѱ��� ������ȣ [SOCKET���� �־����� ��ȣ�ʹ� �ٸ���!], �ڷᱸ��Ű����
	//length: ����
	//data: ���� ����
	//virtual DWORD postingSend(ASSOCKUID socketUniqueId, LARGE_INTEGER* tick, size_t length, char* data) = 0;
	virtual DWORD postingSend(ASSOCKDESC& sockdesc, size_t length, char* data) = 0;

	//�������� ��û
	//uniqueId: ���ϰ�����ȣ, �����뺸�� �Ѱ��� ������ȣ
	virtual DWORD disconnectSocket(ASSOCKUID uniqueId, LARGE_INTEGER* tick) = 0;
	virtual DWORD disconnectSocket(ASSOCKDESC& sockdesc) = 0;

	//���� ��û
	// out_socketUniqueId : �����û�� ���� ���� ���� ��ü�� ���� ��1�� ��ȯ�� �μ�
	// out_tick : �����û�� ���� ���� ���� ��ü�� ���� ��2�� ��ȯ�� �̼�
	// prototype : �ش� ���� ���� ��ü�� AsynchSocket�� ��� ���� Ÿ���� ���� ��ü, �ش� ��ü�� �����Ͽ� ����ȭ �� AsynchSocket ��ü�� ����Ѵ�.
	//				AsynchSocket Ŭ������ ��ӹ޾� ������ �ϴ� ������ AsynchSocket Ŭ���� ���� ����
	// ip : ������ ȣ��Ʈ[����]�� ip�ּ�[dns�� �������� ����]
	// port : ������ ȣ��Ʈ[����]�� ��Ʈ��ȣ
	// return true: ���� ��û�� ���������� ����
	// return false: ���� ��û�� ���� �Ͽ���
	// ���� ��������: �� �������̽��� ���� ������ ��ü�� ������ �Ѱ��� INetworkReceiver �� ������ ��ü�� notifyConnectingResult �Լ��� ���� �뺸 ����
	// ����ȭ �̽�: �� �Լ��� ȣ���ϴ� ����/��ü �� INetworkReceiver::notifyConnectingResult �� ȣ���ϴ� ���ư� �ش� �Լ��� ������ ��ü�� ���� �ٸ����̾�� �Ѵ�.
	// �� �Լ��� windows xp, windows 2003 server �ü�������� �۵��Ѵ�.
	// 20081215: ConnectEx()�� �������� �ʴ� ��쿣 ConnectThread�� ���ư��� ó������.
	//virtual bool connectSocket( int& out_socketUniqueId, LARGE_INTEGER& out_tick, AsynchSocket* prototype, char* ip, int port ) = 0;
	virtual DWORD connectSocket(INT32 reqeusterID, AsynchSocket* prototype, char* ip, u_short port ) = 0;

	// 20070411
	//	socketUniqueId ��ȯ
	//	����������� �ش� ���� ������ε�, ��Ʈ��ũ �������� �� ���� �����ع����� �������� ���ֱ� ���ؼ��̴�.
	virtual DWORD releaseSocketUniqueId( ASSOCKUID socketUniqueId ) = 0;

	virtual DWORD registerSocket(SOCKET sockid, AsynchSocket* prototype, SOCKADDR_IN& addrin) = 0;

	virtual size_t getIOMaxSize() = 0;

};



class INetworkReceiver
{
public:
	//�Ʒ��� �Լ����� multi-thread�鿡 ���� ȣ��Ǳ� ������ ������ synchronization�� �����ؾ��Ѵ�.
	//�Ʒ��� �Լ����� AsynchIOService�� ���� ȣ��ȴ�. �Ʒ��� �Լ� �������� �� �ٽ� AsynchIOService�� �޼ҵ带
	//ȣ���ϰ� �Ǹ� live lock[cross dead lock]�� �ɸ��� �ȴ�.
	//���� �Ʒ��� �Լ��� ���� AsynchIOService ��ü�� �޼ҵ带 ȣ���� �ʿ䰡 �ִٸ�, ���� ���α׷� ��������
	//���� �̺�Ʈ ť�� ����־ ����ȭ ������ �ٸ������� ó���ϰ� �Ѵ�.

	//���� ���� �뺸
	//socketUniqueId: ���ϰ�����ȣ, ���ϰ������� �����ϴ� ���Ϲ�ȣ�� �ƴ�
	//sender: ������ ���ۿ�û/���� ���� �������̽� ������
	virtual void notifyRegisterSocket(ASSOCKDESCEX& sockdesc, SOCKADDR_IN& ip) = 0;

	//���� �������� �뺸
	//socketUniqueId: ���������� ������ ������ȣ, �����뺸�� �Ѱ��� ������ȣ
	virtual void notifyReleaseSocket(ASSOCKDESCEX& sockdesc) = 0;

	//���ŵ� �޽������� �뺸
	//socketUniqueId: ���ŵ� ������ ������ȣ, �����뺸�� �Ѱ��� ������ȣ
	//length: ����
	//data: ���� ����
	virtual void notifyMessage(ASSOCKDESCEX& sockdesc, size_t length, char* data) = 0;

	//�����û ��� �뺸
	// INetworkSender�� ���� connectSocket�� ����� �̹� sender�� �˰� �ֱ� ������, ���������� sender�� �˷��� �ʿ䰡 ����.
	//requestID:  INetworkSender.connectSocket �� ���� �ǳ��־��� �ĺ���
	//socketUniqueId: ���Ἲ���� ���Ͻĺ���
	//tick : ���Ἲ���� ƽ, �ش� �������� ������ �����ؼ� ����ؾ� �Ѵ�.
	//isSuccess : ���Ἲ������
	//error : ������н� ���� GetLastError()����
	//virtual void notifyConnectingResult( int socketUniqueId, LARGE_INTEGER* tick, bool isSuccess, DWORD error ) = 0;
	virtual void notifyConnectingResult(INT32 requestID, ASSOCKDESCEX& sockdesc, DWORD error) = 0;
};

#endif