#pragma once

#include <unordered_map>

/////////////////////////////////////////////////////////////////////////////
//20120426 ���ø� �Ӽ�
// TMsgType : �޼����� Ÿ��(BYTE, USHORT, int)
// TMsgHandler �޼����� ó���ϴ� �ڵ鷯 �ּ�
// TMsg �ڵ鷯�� �Ѿ���� ����
/////////////////////////////////////////////////////////////////////////////
template <typename TMsgType, typename TMsgHandler, typename TMsg>
class SFDispatch
{
	typedef std::unordered_map<TMsgType, TMsgHandler> MsgMap;
public:
	SFDispatch(void){}
	virtual ~SFDispatch(void){m_MsgMap.clear();}

	void RegisterMessage(TMsgType Msg, TMsgHandler Method);
	bool HandleMessage(TMsgType Msg, TMsg& Message);

protected:

private:	
	MsgMap m_MsgMap; 
};

template <typename TMsgType, typename TMsgHandler, typename TMsg>
void SFDispatch<typename TMsgType, typename TMsgHandler, typename TMsg>::RegisterMessage(TMsgType MsgType, TMsgHandler Method)
{
	m_MsgMap.insert(std::make_pair(MsgType, Method));
}

template <typename TMsgType, typename TMsgHandler, typename TMsg>
bool SFDispatch<typename TMsgType, typename TMsgHandler, typename TMsg>::HandleMessage(TMsgType MsgType, TMsg& Message)
{
	MsgMap::iterator iter = m_MsgMap.find(MsgType);

	if(iter != m_MsgMap.end())
	{
		(*iter).second(Message);
	}
	else
	{
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
//20120823 ���ø� �Ӽ�
// TMsgType : �޼����� Ÿ��(BYTE, USHORT, int)
// TMsgHandler �޼����� ó���ϴ� �ڵ鷯 �ּ�
// TMsg �ڵ鷯�� �Ѿ���� ù��° ����
// TMsg2 �ڵ鷯�� �Ѿ���� �ι�° ����
/////////////////////////////////////////////////////////////////////////////
//Ÿ�Ը���Ʈ�� ����ϸ� �ϳ��� ���� �� ���� �� ���⵵ ������....

template <typename TMsgType, typename TMsgHandler, typename TMsg, typename TMsg2>
class SFDispatch_2
{
	typedef std::unordered_map<TMsgType, TMsgHandler> MsgMap;
public:
	SFDispatch_2(void){}
	virtual ~SFDispatch_2(void){m_MsgMap.clear();}

	void RegisterMessage(TMsgType Msg, TMsgHandler Method);
	bool HandleMessage(TMsgType Msg, TMsg& Message, TMsg2& Message2);

protected:

private:	
	MsgMap m_MsgMap; 
};

template <typename TMsgType, typename TMsgHandler, typename TMsg, typename TMsg2>
void SFDispatch_2<typename TMsgType, typename TMsgHandler, typename TMsg, typename TMsg2>::RegisterMessage(TMsgType MsgType, TMsgHandler Method)
{
	m_MsgMap.insert(std::make_pair(MsgType, Method));
}

template <typename TMsgType, typename TMsgHandler, typename TMsg, typename TMsg2>
bool SFDispatch_2<typename TMsgType, typename TMsgHandler, typename TMsg, typename TMsg2>::HandleMessage(TMsgType MsgType, TMsg& Message, TMsg2& Message2)
{
	MsgMap::iterator iter = m_MsgMap.find(MsgType);

	if(iter != m_MsgMap.end())
	{
		(*iter).second(Message, Message2);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}