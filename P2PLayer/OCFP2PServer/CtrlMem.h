////////////////////////////////////////////////////////////////////////////////
//
// 작성자 : 이용휴
// 작성일 : 
// 설  명 :
//

#ifndef __CTRLMEM_H__
#define __CTRLMEM_H__

////////////////////////////////////////////////////////////////////////////////
//
#define CTRLMEM_DEFAULT_SIZE			(2048)
#define CTRLMEM_DEFAULT_EXTSIZE			(1024)

////////////////////////////////////////////////////////////////////////////////
//
class CCtrlMem
{
public:
	CCtrlMem();
	virtual ~CCtrlMem();

	BOOL Init(LONG lSize = CTRLMEM_DEFAULT_SIZE, LONG lExtSize = CTRLMEM_DEFAULT_EXTSIZE);
	void Uninit();
	BOOL In(LONG lSize, const void* pMem);
	BOOL Out(LONG lSize, void* pMem);
	BOOL Collection(LONG lSize);
	BOOL Ext(LONG lSize);
	void ChangeNoCharge();
	void Dig();

public:
	inline void* Reference() { return m_pMem; }
	inline LONG MemSize() { return m_lMemSize; }
	inline LONG MemCharge() { return m_lCurrCharge; }
	inline LONG MemExtSize() { return m_lExtSize; }

private:
	LONG m_lMemSize;
	LONG m_lExtSize;
	LONG m_lCurrCharge;
	void* m_pMem;
};

#endif

