#pragma once

//#include <boost/pool/pool.hpp>
#include <list>
#include "Synchronized.h"

// �� Ǯ�� 32, 64, 128, 256, 512, 1024, 1536, 2048, 3072, 4096 ũ���� �޸𸮸� �����Ѵ�.
//	��û�� �޸𸮺��� ũ�Ⱑ 33�̶�� 64ũ��¥�� �޸𸮸� �����ϰ� �ȴٴ� �ǹ��̴�.
class TraditionalMemoryPool
{
public:
	TraditionalMemoryPool(void);
	virtual ~TraditionalMemoryPool(void);

	void* Retreive(int size);

	void Release(void* p, int size);


private:

//	boost::pool<>*	p_0032;
//	boost::pool<>*	p_0064;
//	boost::pool<>*	p_0128;
//	boost::pool<>*	p_0256;
//	boost::pool<>*	p_0512;
//	boost::pool<>*	p_1024;
//	boost::pool<>*	p_1536;
//	boost::pool<>*	p_2048;
//	boost::pool<>*	p_3072;
//	boost::pool<>*	p_4096;

	std::list<void*>	p_0032;
	std::list<void*>	p_0064;
	std::list<void*>	p_0128;
	std::list<void*>	p_0256;
	std::list<void*>	p_0512;
	std::list<void*>	p_1024;
	std::list<void*>	p_1536;
	std::list<void*>	p_2048;
	std::list<void*>	p_3072;
	std::list<void*>	p_4096;
	std::list<void*>	p_8192;

	CriticalSectionLock		entireSynch;

};
