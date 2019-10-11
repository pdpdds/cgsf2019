#pragma once
#include <string>
namespace CommonData
{
	//��û ��Ŷ 3������@
	///@brief : �α��� ��û@
	class REQ_LoginInfo
	{
	public:
		std::string sId;
		std::string sPwd;
	};
	///@brief : ���̵� �ߺ� Ȯ��@
	class REQ_IdIsValid
	{
	public:
		std::string sId;
	};
	///@brief : ���� ��û@
	class REQ_Registration
	{
	public:
		std::string sId;
		std::string sPwd;
		std::string sMail;
		std::string sExtra;
		std::string sUDID;
	};
	#define UserInfo CommonData::REQ_Registration

	//���� ��Ŷ 6������@
	class RES
	{///��� ���� ��Ŷ�� ������� ������@
	public:
		int iResult = -1;
	};
	////////////
	class RES_LoginInfo : public RES
	{/// = 0 : ���� �����ǵ� ���� ����, = 1 :����@
	public:
		int iWorldCount;
		std::string* sIp;
	};
	class RES_IdIsValid : public RES
	{///RES = 0 : ����, = 1 : ��� �Ұ�, = 2 : �ߺ����̵�@
	};
	class RES_Regstration : public RES
	{
	};
}