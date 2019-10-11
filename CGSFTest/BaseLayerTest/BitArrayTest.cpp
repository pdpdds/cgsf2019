#include "stdafx.h"
#include "BitArrayTest.h"
#include "SFBitArray.h"

BitArrayTest::BitArrayTest(void)
{
}


BitArrayTest::~BitArrayTest(void)
{
}

bool BitArrayTest::Initialize()
{
	return true;
}

bool BitArrayTest::Run()
{
	SFBitArray a;
	
	//4578��° ��Ʈ�� ������ ��. 4578/8��° ����Ʈ�� 4578%8 ��Ʈ�� ��Ʈ���� 1�� ���� 
	a.SetAt(4578);

	//328��° ��Ʈ�� ������ ��. 328/8��° ����Ʈ�� 328%8 ��Ʈ�� ��Ʈ���� 1�� ���� 
	a.SetAt(323);

	// 1�� ���õ� ��Ʈ�� �� ���� 2���� ���õǾ� �����Ƿ� 2�� �����Ѵ�. 
	SFASSERT( 2 == a.GetCount());
	
	// 323��° ��Ʈ�� XOR�Ѵ�. 323��° ��Ʈ�� 1�� ���õǾ� �־����Ƿ� ������ 323��° ��Ʈ�� 0�� ���� ���� �ȴ�. 
	a.XOrAt(323);
	
	SFASSERT( 1 == a.GetCount());

	SFBitArray b;
	
	char buffer[4] = {1,1,1,1};
	b.Attach((BYTE*)buffer, 4);

	//�̸� ��Ʈ���õ� ���۸� BitArray�� �ʱⰪ���� ����Ѵ�. 
	//4����Ʈ �� ��Ʈ Array 00000001000000010000000100000001 �̹Ƿ� ��Ʈ���õ� ������ �� 4�� 
	SFASSERT( 4 == b.GetCount());

	b.Detach();

	return true;
}

bool BitArrayTest::Finally()
{
	return true;
}