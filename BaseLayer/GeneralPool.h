#pragma once
#include <windows.h>


	class iMemPool
	{
	public:
		virtual				~iMemPool() {}
		virtual void*		alloc(unsigned int n) = 0;
		virtual void		free(void*& p) = 0;
	};

	/**
		@class Runtime Memory Pool v0.1
		@brief �޸� Page�� ������������ �����Ͽ� ������ �������� �޸𸮸� �Ҵ���
	 */

	class RtMemPool : public iMemPool
	{
	public:
		virtual void*		alloc(unsigned int n);
		virtual void		free(void*& p);
	};

	enum
	{
		// �⺻ ������ ������
		default_page_size	=	1024 * 16,
		// ���м�
		max_miss_count		=	8,
		// �ּ� �Ҵ� ������
		min_alloc_size		=	32
	};

	class GeneralPool_Impl;

	/**
		@class ���� �Ҵ��� v0.1
		@brief �޸� Page�� ������������ �����Ͽ� ������ �������� �޸𸮸� �Ҵ���
		
		@remark 
			�޸𸮰� �Ҵ�ɶ� 32byte �̻��� �޸� ������ �Ҵ�ȴ�. �׷��Ƿ� �ʹ� ���� �޸𸮸� �Ҵ��Ҷ� ����ϸ� ���� ����
			�޸� �Ҵ�� page size�� �ּ� 4k�� �����Ǹ� Page size(32k) �� �Ѿ�� �޸� ������ ���ؼ��� �⺻ �Ҵ��ڰ� ���Ǿ� ����. 
			�������� �޸� ���� ���� �����Ǿ� ���� �Ҵ��� ���� ������ �������� ã�� ���� ���� �˰����� ���Ǿ�����.

		@date 2008/01
		@author kkkkkkman
		@code
		GeneralPool m;

		std::vector< void* > v;

		for( int i=0;i<1000;++i)
			v.push_back( m.alloc( 20 ) );

		for( int i=0;i<1000;++i)
			m.free( v[i]) ;
		@endcode

	 */
	class GeneralPool : public iMemPool
	{
		GeneralPool_Impl*	m_impl;
	public:
		
	public:
							GeneralPool();
							~GeneralPool();

		// �޸𸮸� �Ҵ�޴´�
		virtual void*		alloc(unsigned int n);
		// �޸𸮸� �����Ѵ�
		virtual void		free(void*& p);

		// 
		static unsigned int	size(const void* p);
	};

