#include "stdafx.h"
#include "TomCryptTest.h"
//#include <tomcrypt.h>

//#pragma comment(lib, "tomcrypt.lib")

TomCryptTest::TomCryptTest(void)
{
}


TomCryptTest::~TomCryptTest(void)
{
}

bool TomCryptTest::Run()
{
	HMACTest();
	R4Test();

	return true;
}

void TomCryptTest::HMACTest()
{
	/*int idx, err;
	hmac_state hmac;
	unsigned char key[16] = "CGSF"; 
	unsigned char dst[MAXBLOCKSIZE];
	unsigned long dstlen;
	//�ؽ� �˰������� SHA-1 ��� 
	if (register_hash(&sha1_desc) == -1)
	{
		printf("Error registering SHA1\n");
		return -1;

		//Descriptor ���̺��� SHA-1 �ؽ� �˰����� �ε��� ���� 
		dx = find_hash("sha1");
		dstlen = sizeof(dst);
	//SHA-1 �˰���� Ű "CGSF"�� �̿��Ͽ� "hello"��� �������� �ؽ� ������� dst�� ���� 
		err = hmac_memory(idx, key, 4, (const unsigned char*)"hello", 5, dst, &dstlen);*/
}

bool TomCryptTest::R4Test()
{
	/*prng_state prng; //�ǻ� ���� �ѹ� ���ʷ����� 
	unsigned char buf[32];

	int err;
	if ((err = rc4_start(&prng)) != CRYPT_OK) //RC4 �ʱ�ȭ 
		return FALSE;
	//Ű "CGSF"�� ���ؼ� ��Ʈ���Ǹ� ������Ŵ 
	if ((err = rc4_add_entropy((const unsigned char*)"CGSF", 4, &prng)) != CRYPT_OK)
		return FALSE;

	//RC4 �غ� ����.. PRNG Init 
	if ((err = rc4_ready(&prng)) != CRYPT_OK)
		return FALSE;

	// 11����Ʈ "hello world" ���� ��ȣȭ 
	strcpy((char*)buf, "hello world");
	if (rc4_read(buf, 11, &prng) != 11)
		return FALSE;

	//��ȣȭ ��� 11����Ʈ�� buf ���ۿ� ������ �� 

	rc4_done(&prng);*/

	return true;
}
