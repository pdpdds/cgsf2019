#include "stdafx.h"
#include "FilePackingTest.h"
#include "SFFilePack.h"

FilePackingTest::FilePackingTest(void)
{
}


FilePackingTest::~FilePackingTest(void)
{
}

//////////////////////////////////////////////////////////////////////////
//���� ��ŷ, ���� �� ����(�޸𸮻�) �׽�Ʈ
//////////////////////////////////////////////////////////////////////////
bool FilePackingTest::Run()
{
	SFFilePack FilePack;
	if(FilePack.Initialize() == TRUE)	
		FilePack.UnPackFile(_T("test.zip"));

	return true;
}
