#pragma once
#include <ace/task_t.h>
#include <ace/Activation_Queue.h>
#include <ace/Method_Request.h>

class IDBManager;
class SFDatabase;

class SFDBWorker : public ACE_Task<ACE_SYNCH>
{
public:
	SFDBWorker(IDBManager* pManager);
	virtual ~SFDBWorker(void);	

	int perform(ACE_Method_Request* pReq)
	{
		return this->m_queue.enqueue(pReq);
	}

	virtual int svc(void);

	void SetDatabase(SFDatabase* pDatabase)
	{
		m_pDatabase = pDatabase;
	}

	SFDatabase* GetDatabase(){return m_pDatabase;}
		 
protected:

private:
	ACE_Activation_Queue m_queue;
	ACE_thread_t m_threadID;
	IDBManager* m_pManager;
	SFDatabase* m_pDatabase;
};


//#pragma comment(linker, "?DataBase@@3V?$ACE_TSS@VSFDatabaseMySQL@@@@A");
