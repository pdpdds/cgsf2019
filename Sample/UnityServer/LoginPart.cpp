#include "stdafx.h"
#include "LoginPart.h"
#include "SFJsonPacket.h"
#include "CommonProtocol.h"
void LoginPart::REQD_LOGIN(BasePacket* pPacket)
{
	SFJsonPacket* pJsonPacket = (SFJsonPacket*)pPacket;//1. JsonPacket���� ����@
	JsonObjectNode& req_Node = pJsonPacket->GetData();//2. iteration�� ���� node;
	
	CommonData::REQ_LoginInfo reqd_login;//3. ��û ��Ŷ@
	reqd_login.sId = req_Node.GetValue<std::string>("sId");//3-1. json��Ŷ���� sID@
	reqd_login.sPwd = req_Node.GetValue<std::string>("sPwd");//3-2. json��Ŷ���� sPwd@	

	//CommonData::RES_LoginInfo res_login;//4. ���� ��Ŷ@
	SFJsonPacket JsonPacket(CommonProtocol::RES_LOGIN);//4-1. json���� ��Ŷ�� ��������@
	JsonObjectNode& res_Node = JsonPacket.GetData();//4-2. json �����͸� �ֱ����� node@
	
	int iPwdChk = PersonalInfoManager::Ins()->tryLogin(reqd_login.sId, reqd_login.sPwd);
	//��û ���� ���̵� ������ �����ϴٸ�@
	//��������. ������ Ž�� ����� �޾� ó��@
	if(iPwdChk == 0)
	{
		res_Node.Add("iResult", 0);//�н�@
	}
	else if (iPwdChk == 1)
	{
		res_Node.Add("iResult", iPwdChk);//���¾��̵�@
	}
	else if (iPwdChk == 3)
	{
		res_Node.Add("iResult", iPwdChk);//����@
	}
	else if (iPwdChk == 4)
	{
		res_Node.Add("iResult", iPwdChk);//�� �� ���� ����@
	}

	///���� �׽�Ʈ �ڵ�@
	if (reqd_login.sId.compare("foranie") ==0)
	{
		/*
		//���� ���� �� ����@
		res_login.iResult = 0;
		res_login.iWorldCount = 2;
		res_login.sIp = new std::string[2];
		res_login.sIp[0] = "127.0.0.1";
		res_login.sIp[1] = "127.0.0.1";
		*/
		res_Node.Add("iResult", 0);//�α��� ���@
		res_Node.Add("iWorldCount", 2);
		std::cout << "req_login result : 0" << endl;
	}
	else if (reqd_login.sId.compare("noID") == 0)
	{
		res_Node.Add("iResult", 1);//���̵� ���� ��� ��ȯ@
		std::cout << "req_login result : 1" << endl;
	}
	else if (reqd_login.sId.compare("serverFULL") == 0)
	{
		res_Node.Add("iResult", 3);//�������� ��� ��ȯ@
		std::cout << "req_login result : 3" << endl;
	}
	else if (reqd_login.sId.compare("etcError") == 0)
	{
		res_Node.Add("iResult", 4);//��Ÿ ����@
		std::cout << "req_login result : 4" << endl;
	}
	///������� �׽�Ʈ �ڵ�@
	JsonPacket.SetSerial(pPacket->GetSerial());
	SFEngine::GetInstance()->SendRequest(&JsonPacket);
}
void LoginPart::REQD_IDISIDVALID(BasePacket* pPacket)
{
	SFJsonPacket* pJsonPacket = (SFJsonPacket*)pPacket;//1. JsonPacket���� ����@
	JsonObjectNode& req_Node = pJsonPacket->GetData();//2. iteration�� ���� node;

	std::string sId = req_Node.GetValue<std::string>("sId");
	int iNew = PersonalInfoManager::Ins()->isNewUser(sId);

	SFJsonPacket JsonPacket(CommonProtocol::RES_IDISIDVALID);//4-1. json���� ��Ŷ�� ��������@
	JsonObjectNode& res_Node = JsonPacket.GetData();//4-2. json �����͸� �ֱ����� node@
	
	if (iNew == 0)//�� ���̵�@
	{
		res_Node.Add("iResult", 0);
	}
	else if (iNew == 1)//�ߺ�@
	{
		res_Node.Add("iResult", 1);
	}

	JsonPacket.SetSerial(pPacket->GetSerial());
	SFEngine::GetInstance()->SendRequest(&JsonPacket);
}
void LoginPart::REQD_REGISTRATION(BasePacket* pPacket)
{
	SFJsonPacket* pJsonPacket = (SFJsonPacket*)pPacket;//1. JsonPacket���� ����@
	JsonObjectNode& req_Node = pJsonPacket->GetData();//2. iteration�� ���� node;
	
	CommonData::REQ_Registration reqd_reg;//3. ��û ��Ŷ@
	reqd_reg.sId = req_Node.GetValue<std::string>("sId");//3-1. json��Ŷ���� sID@
	reqd_reg.sPwd = req_Node.GetValue<std::string>("sPwd");//3-2. json��Ŷ���� sPwd@
	reqd_reg.sMail = req_Node.GetValue<std::string>("sMail");//3-3. json��Ŷ���� sMail@
	reqd_reg.sExtra = req_Node.GetValue<std::string>("sExtra");//3-4. json��Ŷ���� sExtra@
	reqd_reg.sUDID = req_Node.GetValue<std::string>("sUDID");//3-5. json��Ŷ���� sUDID@

	SFJsonPacket JsonPacket(CommonProtocol::RES_REGISTRATION);//4-1. json���� ��Ŷ�� ��������@
	JsonObjectNode& res_Node = JsonPacket.GetData();//4-2. json �����͸� �ֱ����� node@
	
	bool bAddUser = PersonalInfoManager::Ins()->addUser(reqd_reg);
	if (bAddUser == true)
	{
		res_Node.Add("iResult", 0);
	}
	else
	{
		res_Node.Add("iResult", 2);
	}

	JsonPacket.SetSerial(pPacket->GetSerial());
	SFEngine::GetInstance()->SendRequest(&JsonPacket);
}
