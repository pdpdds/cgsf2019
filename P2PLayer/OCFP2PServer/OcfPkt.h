////////////////////////////////////////////////////////////////////////////////
// ����ý���  : 
// ���α׷� ID : OcfPkt.h
// ��ɿ��    : 
// �ۼ���      : 2006.11.14
// �ۼ���      : �̿���
// ������      :
// ������      :
// ��������    :
////////////////////////////////////////////////////////////////////////////////

#ifndef __OCFPKT_H__
#define __OCFPKT_H__

////////////////////////////////////////////////////////////////////////////////
//
// Packet�� ���� ��� ����..
//
#define OCF_PACKET_MINHEADSIZE						(0x04)
#define OCF_PACKET_MAXBODYSIZE						(0x00FFFFFF)
#define OCF_PACKET_SIZEBITMASK						(0x00FFFFFF)
#define OCF_PACKET_HEAD_LEN_LENGTH					(3)
#define OCF_PACKET_HEAD_LEN_SERVICETYPE				(1)
#define OCF_PACKET_HEAD_IDX_SERVICETYPE				(3)

////////////////////////////////////////////////////////////////////////////////
//
// Packet Head
//
typedef union
{
	unsigned char m_achHead[OCF_PACKET_MINHEADSIZE];
	ULONG m_ulHead;
} OCF_PACKET_HEAD;

//
#define OCF_PACKET_HEAD_LEN							(sizeof(OCF_PACKET_HEAD))

#endif

