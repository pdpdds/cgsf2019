#include "StdAfx.h"
#include "SFDatabaseSQLite.h"
//#include "CppSQLite3.h"

//////////////////////////////////////////////////////////////////////////////////
//20120521 : Thread Safe ���θ� Ȯ������ ���߱� ������ üũ�ؾ� ��
//////////////////////////////////////////////////////////////////////////////////
SFDatabaseSQLite::SFDatabaseSQLite(void)
//: m_pDatabase(NULL)
{
}

SFDatabaseSQLite::~SFDatabaseSQLite(void)
{
	/*if(m_pDatabase)
	{
		delete m_pDatabase;
		m_pDatabase = NULL;
	}*/
}

BOOL SFDatabaseSQLite::Initialize()
{
	/*m_pDatabase = new CppSQLite3DB();

	LOG(ERROR) << "MySQL Version : " <<  m_pDatabase->SQLiteVersion();

	m_pDatabase->open(pInfo->szDBName);*/

	return TRUE;
}

BOOL SFDatabaseSQLite::Execute(char* szQuery)
{
	/*try
	{
		int nRows = m_pDatabase->execDML(szQuery);

		//LOG_IF(FATAL, Count >= 90);
			
	}
	catch (CppSQLite3Exception& e)
	{
		LOG(ERROR) << e.errorCode() << ":" << e.errorMessage();
	}*/

	return TRUE;
}