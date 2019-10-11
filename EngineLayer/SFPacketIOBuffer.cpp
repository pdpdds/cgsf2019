#include "StdAfx.h"
#include "SFPacketIOBuffer.h"

SFPacketIOBuffer::SFPacketIOBuffer(void)
{
}

SFPacketIOBuffer::~SFPacketIOBuffer(void)
{
}

bool SFPacketIOBuffer::GetPacket(SFPacketHeader& header, char* pBuffer, unsigned short packetSize, int& errorCode)
{
	errorCode = PACKETIO_ERROR_NONE;
	int oldHead = GetHead();

	USHORT headerSize = sizeof(SFPacketHeader);

	if (GetUsedBufferSize() < headerSize)
	{
		return false;
	}

	if (headerSize != GetData((char*)&header, headerSize))
	{
		SetHead(oldHead);
		errorCode = PACKETIO_ERROR_HEADER;
	}

	unsigned int dataSize = header.dataSize;

	if (dataSize > packetSize - sizeof(SFPacketHeader))
	{
		SetHead(oldHead);
		errorCode = PACKETIO_ERROR_DATA;
		return false;
	}

	if (GetUsedBufferSize() < (int)dataSize)
	{
		SetHead(oldHead);
		return false;
	}

	int getDataSize = GetData((char*)pBuffer, dataSize);

	if (dataSize != (unsigned int)getDataSize)
	{
		SetHead(oldHead);
		errorCode = PACKETIO_ERROR_DATA;
		return false;
	}

	return true;
}

bool SFPacketIOBuffer::GetHeader(SFPacketHeader& header, int& errorCode)
{
	errorCode = PACKETIO_ERROR_NONE;
	int oldHead = GetHead();

	USHORT headerSize = sizeof(SFPacketHeader);

	if (GetUsedBufferSize() < headerSize)
	{
		return false;
	}

	if (headerSize != GetData((char*)&header, headerSize))
	{
		SetHead(oldHead);
		errorCode = PACKETIO_ERROR_HEADER;
		return false;
	}

	SetHead(oldHead);

	return true;
}