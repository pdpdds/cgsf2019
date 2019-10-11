////////////////////////////////////////////////////////////////////////////////
//
// �ۼ��� : �̿���
// �ۼ��� : 2003.03.07
// ��  �� :
//

#ifndef __THREADCTRLCLIENT_H__
#define __THREADCTRLCLIENT_H__

////////////////////////////////////////////////////////////////////////////////
// 
// ������ ���°�
//
typedef enum 
{
	THREADEXITSTATUS_NORMAL = 0,
	THREADEXITSTATUS_EXCEPTION,
} THREADEXITSTATUS;

////////////////////////////////////////////////////////////////////////////////
// 
// ������ ���� ���°�
//
typedef enum
{
	THREADSTATUS_STARTING = 0,
	THREADSTATUS_ENDING,
	THREADSTATUS_RUNNING,
	THREADSTATUS_ZOMBIE
} THREADSTATUS;

////////////////////////////////////////////////////////////////////////////////
// 
// CThreadBaseContainer�� ���� �����Ǿ����� �ִ� Ŭ������ �⺻��...
//
class CThreadClient
{
friend unsigned int __stdcall g_fThreadFunc(LPVOID pcComponent);

public:
	CThreadClient();
	virtual ~CThreadClient();

public:
	virtual BOOL Init();
	virtual void Uninit() {};
	virtual BOOL Start();
	virtual BOOL DoEnd();
	virtual BOOL End(DWORD dwWaitTime=INFINITE);
	virtual BOOL Do() = 0;

public:
	void WaitSync();
	void SetSync();
	BOOL WaitWithMessageLoop(HANDLE hEvent, DWORD dwMilliseconds=INFINITE);

public:
	inline BOOL IsEnd() { return m_bEndFlag; }
	inline void SetEndFlag(BOOL bFlag) { m_bEndFlag = bFlag; }
	inline HANDLE GetThreadHandle() { return m_hThread; }
	inline void SetThreadExitStatus(THREADEXITSTATUS enumThreadExitStatus) { m_enumThreadExitStatus = enumThreadExitStatus; }
	inline THREADEXITSTATUS GetThreadExitStatus() { return m_enumThreadExitStatus; }
	inline THREADSTATUS GetThreadStatus() { return m_enumThreadStatus; }

private:
	inline void SetThreadStatus(THREADSTATUS enumThreadStatus) { m_enumThreadStatus = enumThreadStatus; }

private:
	THREADSTATUS m_enumThreadStatus;
	BOOL m_bEndFlag;
	THREADEXITSTATUS m_enumThreadExitStatus;
	HANDLE m_hThread;

private:
	HANDLE m_hSync;
};

#endif
