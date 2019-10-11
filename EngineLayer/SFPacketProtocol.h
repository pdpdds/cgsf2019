#pragma once
// ============================================================================
// SFPacketProtocol Ŭ����
// author : pdpdds
// desc : ������ ������ ��Ʈ��ũ �����͸� �����ϰ� ��Ŷ�� ���� �� ó��, �׸��� �������� ��Ŷ�� �����ϴ� ������ ����ϴ� ���ø� Ŭ�����Դϴ�.
// ���� �������� ó���� ���ø����� ������ Ŭ������ ����մϴ�.
// ============================================================================

#include <EngineInterface/IPacketProtocol.h>
#include "SFConstant.h"
#include "SFChecksum.h"
#include "SFPacketIOBuffer.h"
#include "SFEncryption.h"
#include "SFEncryptionXOR.h"

#include "Macro.h"

class BasePacket;

void SendLogicLayer(BasePacket* pPacket);

template <typename T>
class SFPacketProtocol : public IPacketProtocol
{
public:
	SFPacketProtocol();
	SFPacketProtocol(int bufferIOSize, unsigned short packetDataSize, int packetOption = 0);
	virtual ~SFPacketProtocol(void) {}

	// ----------------------------------------------------------------
	//  Name:           OnReceive
	//  Description:    ������ ������ �����͸� ó���Ѵ�. 	
	// ----------------------------------------------------------------
	bool OnReceive(INT serial, char* pBuffer, UINT dwTransferred, _SessionDesc& desc) override;
	
	// ----------------------------------------------------------------
	//  Name:           SendRequest
	//  Description:    Ÿ�ٿ��� ��Ŷ�� �����Ѵ�.
	// ----------------------------------------------------------------
	virtual bool Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize) override;

	// ----------------------------------------------------------------
	//  Name:           DisposePacket
	//  Description:    ���� ������ ��Ŷ�� �����Ѵ�.
	// ----------------------------------------------------------------
	virtual bool DisposePacket(BasePacket* pPacket) override;

	// ----------------------------------------------------------------
	//  Name:           Clone
	//  Description:    ��Ŷ�������� Ŭ���� ��ü�� �����Ѵ�.
	// ----------------------------------------------------------------
	virtual IPacketProtocol* Clone() 
	{ 
		SFPacketProtocol* pProtocol = new SFPacketProtocol<T>();
		pProtocol->GetPacketProtocol().CopyBaseProtocol(m_Analyzer);
		return pProtocol;
	}
	T& GetPacketProtocol(){ return m_Analyzer; }
	
	//virtual BasePacket* CreatePacket() override;

private:

	// ----------------------------------------------------------------
	//  Name:           ���ø� Ŭ����
	//  Description:    ���� ��Ŷ ���������� ó���ϴ� Ŭ����
	//					AddTransferredData, DisposePacket, SendRequest GetPacket �޼ҵ带 �ݵ�� �����ؾ� �ȴ�.
	//					GetPacketData �޼ҵ�� �� �޼ҵ�� ����� �ε��� �Ѵ�.
	// ----------------------------------------------------------------
	T m_Analyzer;	
};

template <typename T>
SFPacketProtocol<T>::SFPacketProtocol()
{
	m_Analyzer.Initialize(PACKET_DEFAULT_IO_SIZE, PACKET_DEFAULT_PACKET_SIZE, 0);
}

template <typename T>
SFPacketProtocol<T>::SFPacketProtocol(int bufferIOSize, unsigned short packetDataSize, int packetOption)
{
	if (bufferIOSize > MAX_IO_SIZE)
		bufferIOSize = MAX_IO_SIZE;

	if (packetDataSize > MAX_PACKET_SIZE)
		packetDataSize = MAX_PACKET_SIZE;

	if (packetDataSize > bufferIOSize)
		packetDataSize = (unsigned short)bufferIOSize;

	m_Analyzer.Initialize(bufferIOSize, packetDataSize, packetOption);
}

template <typename T>
bool SFPacketProtocol<T>::DisposePacket(BasePacket* pPacket)
{
	return m_Analyzer.DisposePacket(pPacket);
}

template <typename T>
bool SFPacketProtocol<T>::Encode(BasePacket* pPacket, char** ppBuffer, int& bufferSize)
{
	return m_Analyzer.Encode(pPacket, ppBuffer, bufferSize);
}

template <typename T>
bool SFPacketProtocol<T>::OnReceive(INT serial, char* pBuffer, UINT dwTransferred, _SessionDesc& desc)
{
	auto bRet = m_Analyzer.AddTransferredData(pBuffer, dwTransferred);

	if(false == bRet)
	{
		SFASSERT(0);
		return false;
	}

	auto iErrorCode = PACKETIO_ERROR_NONE;
	
	do
	{
		BasePacket* pPacket = m_Analyzer.GetPacket(iErrorCode);
		
		if(NULL == pPacket)
			break;

		if (SFPACKET_RPC_PACKET == pPacket->GetPacketID())
			pPacket->SetPacketType(SFPACKET_RPC);
		else
			pPacket->SetPacketType(SFPACKET_DATA);

		pPacket->SetSerial(serial);	
		pPacket->SetSessionDesc(desc);
		
		SendLogicLayer(pPacket);
	}
	while(true);
	
	if(iErrorCode != PACKETIO_ERROR_NONE)
	{
		printf("PACKETIO_ERROR %d\n", iErrorCode);

		return false;
	}

	return true;
}

/*
template <typename T>
BasePacket* SFPacketProtocol<T>::CreatePacket()
{
return m_Analyzer.CreatePacket();
}*/