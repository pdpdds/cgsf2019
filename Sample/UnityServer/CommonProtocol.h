#pragma once
class CommonProtocol
{
	/* ���Ƿ� ������.
	* �ý���/�׽�Ʈ ��Ŷ�� 1001���Ͽ� ����@
	* ��û-req��Ŷ�� 3�����뿡 ����@
	* ����-response��Ŷ�� 6�����뿡 ����@
	*
	*/
public:
	const static USHORT RES_DISCONNECTED = 1; 
	const static USHORT REQ_ECHO = 1000;
	const static USHORT RET_ECHO = 1001;
	const static USHORT KEEPALIVE = 90000;///���� ����@
	const static USHORT REQ_HELLo = 30000;
	const static USHORT RES_HELLo = 60000;


	const static USHORT REQ_LOGIN = 30001;///�α��� ��û@
	const static USHORT REQ_IDISIDVALID = 30002;///���̵� �ߺ�Ȯ��@
	const static USHORT REQ_REGISTRATION = 30003;///�ű� ���� ��û@


	const static USHORT RES_LOGIN = 60001;///�α��� ����@
	const static USHORT RES_IDISIDVALID = 60002;///���̵� �ߺ�����@
	const static USHORT RES_REGISTRATION = 60003;//�ű� ���� ����@
	
};