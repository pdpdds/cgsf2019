#pragma once

#define MAX_IO_SIZE 16384 //�ý��ۻ󿡼� ������ �δ� IO �ִ�ġ
#define MAX_PACKET_SIZE 8192 //�ý��ۻ󿡼� ������ �δ� ��Ŷ ������ �ִ�ġ

#define ENCRYPTION_KEY 0xAFB7E3D9

#define PACKET_DEFAULT_IO_SIZE					8192 //����Ʈ IO ������
#define PACKET_DEFAULT_PACKET_SIZE				4096 //����Ʈ ��Ŷ ������

#define PACKETIO_ERROR_NONE				0
#define PACKETIO_ERROR_HEADER			-1
#define PACKETIO_ERROR_DATA				-2

#define PACKETIO_ERROR_DATA_CRC			-3
#define PACKETIO_ERROR_DATA_COMPRESS	-4
#define PACKETIO_ERROR_DATA_ENCRYPTION	-5

#define PACKETIO_ERROR_MEMORY_ALLOC		-6

#define PACKET_COMPRESS_LIMIT 1024

/////////////////////////////////////////////////////////////////////////
#define PACKET_OPTION_DATACRC			0x00000001		// �������: C++
#define PACKET_OPTION_COMPRESS			0x00000002		// �������: C++, C#
#define PACKET_OPTION_ENCRYPTION		0x00000004		// �������: C++
#define PACKET_OPTION_CRC_DEFAULT		0x00000008		// �������: C++
#define PACKET_OPTION_CRC_MD5			0x00000010		// �������: C++
#define PACKET_OPTION_ENCRYPTION_DES	0x00000020		// �������: C#
#define PACKET_OPTION_ENCRYPTION_3DES	0x00000040		// �������: C#
#define PACKET_OPTION_ENCRYPTION_AES	0x00000080		// �������: C#

#define PACKET_PROTOCOL_JSON		1
#define PACKET_PROTOCOL_CGSF		2
#define PACKET_PROTOCOL_PROTO_BUF	3
#define PACKET_PROTOCOL_MSG_PACK	4
#define PACKET_PROTOCOL_AVRO		5

/////////////////////////////////////////////////////////////////////////
#define TIMER_100_MSEC	 1
#define TIMER_200_MSEC	 2
#define TIMER_500_MSEC	 3
#define TIMER_1_SEC		 4
#define TIMER_2_SEC		 5
#define TIMER_5_SEC		 6
#define TIMER_10_SEC	 7

/////////////////////////////////////////////////////////////////////////
enum eDBResult
{
	DBRESULT_ERROR_UNKNOWN = -999,
	DBRESULT_STATE_SUCCESS = 0,
};

////////////////////////////////////////////////////////////////////////
//MACRO
////////////////////////////////////////////////////////////////////////
#define SF_GETPACKET_ARG(a,b,c) memcpy(a,b.c_str(), sizeof(c));
#define SF_SAFE_RELEASE(a) if(a) delete a;

#define SFPACKET_RPC_PACKET 60000
