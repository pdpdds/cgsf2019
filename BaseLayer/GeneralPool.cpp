#include "stdafx.h"
#include "generalpool.h"
#include <assert.h>
#include <stdio.h>

//------------------------------------------------------------------- my codeing style

#define null NULL

// debug ��忡���� ��ȿ�� üũ�� �ǽ���
#ifdef _DEBUG 
#define validate_page( p )	valid( p );
#else
#define validate_page( p )	__noop
#endif

#ifdef _DEBUG
#define assert_msg( msg, exp )		assert( (exp) && msg )
#else
#define assert_msg( msg, exp)		__noop
#endif

	struct CriSecT
	{
		CRITICAL_SECTION m_cs;
		CriSecT() { InitializeCriticalSection( &m_cs ); }
		~CriSecT() { DeleteCriticalSection( &m_cs ); }

		__forceinline void lock() { EnterCriticalSection( &m_cs ); }
		__forceinline void unlock() { LeaveCriticalSection( &m_cs ); }
	};

	struct NullMutex
	{
		__forceinline void lock() {}
		__forceinline void unlock(){}
	};

	//------------------------------------------------------------------- Runtime Memory

	void*RtMemPool::alloc(unsigned int n)
	{
		return ::malloc( n );
	}

	void RtMemPool::free(void*& p)
	{
		if ( p == null )
			return;
		::free( p );
		p = null;
	}

	RtMemPool _default_allocator;

    
	//------------------------------------------------------------------- GeneralPool

	class GeneralPool_Page;
	class GeneralPool_Impl;

	class GeneralPool_Node
	{
	public:
		GeneralPool_Page*	_owner;
		unsigned int		_size;
		unsigned int		_realSize;

		GeneralPool_Node*	_availablePrev;
		GeneralPool_Node*	_availableNext;

		GeneralPool_Node*	_prev;
		GeneralPool_Node*	_next;

		bool				_allocated;

		__forceinline void		clear(GeneralPool_Page* owner)
		{
			_owner			= owner;
			_realSize		= 0;
			_size			= 0;

			_availablePrev	= null;
			_availableNext	= null;

			_prev			= null;
			_next			= null;

			_allocated		= false;
		}

		__forceinline BYTE*		getMemory()
		{
			return ((BYTE*) this) + sizeof(GeneralPool_Node);
		}
	};

	__forceinline unsigned int		nodeSize()
	{
		return sizeof(GeneralPool_Node);
	}
	__forceinline GeneralPool_Node* Node(void* _pMemoryBlock)
	{
		return (GeneralPool_Node*) (((unsigned int) _pMemoryBlock) - nodeSize());
	}

	__forceinline const GeneralPool_Node*	Node(const void* _pMemoryBlock)
	{
		return (const GeneralPool_Node*) (((unsigned int) _pMemoryBlock) - nodeSize());
	}

	class GeneralPool_Page
	{
	public:
		GeneralPool_Page*	_prev;
		GeneralPool_Page*	_next;

		GeneralPool_Node*	_available;			// 
		unsigned int		_availableSize;

		BYTE				_mem[default_page_size];

		unsigned int		_miss;

		
		GeneralPool_Node*	_firstNode;
		
							GeneralPool_Page() : _next(null), _available(null), _firstNode((GeneralPool_Node*)_mem), _availableSize(0), _miss(0){}

		void				removeAvailableList( GeneralPool_Node* node)
		{
			// ��ũ�� �����Ѵ�
			if (node->_availablePrev == null)
			{
				assert_msg("�߸��� Root ����", _available == node);
				_available = node->_availableNext;
			}
			else
			{
				node->_availablePrev->_availableNext = node->_availableNext;
			}

			if (node->_availableNext != null)
			{
				node->_availableNext->_availablePrev = node->_availablePrev;
			}

			node->_availablePrev = null;
			node->_availableNext = null;

			// ������̼� �÷��׸� �����Ѵ�
			node->_allocated = true;
		}
	};

	class GeneralPool_Impl
	{
	public:
		GeneralPool_Page*		_missingPage;
		GeneralPool_Page*		_availablePage;
		CriSecT					_mutex;


			 					GeneralPool_Impl();
								~GeneralPool_Impl();

		/** 
			�޸� �Ҵ�
		 */
		void*					alloc(unsigned int size);

		/**
			�޸� �����Ѵ�.
		 */
		void					free(void* _pBuffer);
		
		/** 
			�� ������ �Ҵ�
		 */
		void					allocPage();

		/** 
			�Ҵ����� ��带 �и��Ѵ�
		 */	
		void					splitAllocNode( GeneralPool_Page* page, GeneralPool_Node* node, unsigned int size);

		/**	
			��,���� ���� ��ģ�� (���� ��常 ���´�)
		 */
		void					freeMergePrevAndNext(GeneralPool_Node* node);
		/**
			���� ���� ��ģ�� (���� ��常 ���´�)
		 */
		void					freeMergePrev(GeneralPool_Node* node);
		/** 
			���� ���� ��ģ�� (���� ��常 ���´�)
		 */
		void					freeMergeNext(GeneralPool_Node* node);		
		/**
			��� ���� - �ƹ� ���͵� �������� �ʴ´�
		 */
		void					freeMergeNone(GeneralPool_Node* node);

		/** 
			����� �˻�
		 */
		void					valid(GeneralPool_Page* page);

		void					lock();
		void					unlock();

	};


GeneralPool::GeneralPool()
{	
	m_impl = new GeneralPool_Impl( );
}

GeneralPool::~GeneralPool()
{
	delete m_impl;
}


void* GeneralPool::alloc(unsigned int size)
{
	return m_impl->alloc(size);
}


void GeneralPool::free(void*& p)
{
	if (p == null)	{	return;	}
	m_impl->free(p);
	p = null;
}

unsigned int GeneralPool::size(const void*  p)
{
	const GeneralPool_Node*	node;

	if (p == null)	{	return 0;	}

	node = Node(p);

	return node->_realSize;
}

GeneralPool_Impl::GeneralPool_Impl(): _availablePage(null),_missingPage(null)
{
	allocPage();
}

GeneralPool_Impl::~GeneralPool_Impl()
{
	lock();

	GeneralPool_Page*	page;
	GeneralPool_Page*	pPageNext;

	page = _availablePage;

	// ��� �������� �����
	while (page != null)
	{
		pPageNext = page->_next;
		delete page;
		page = pPageNext;
	}
	page = _missingPage;

	// ��� �������� �����
	while (page != null)
	{
		pPageNext = page->_next;
		delete page;
		page = pPageNext;
	}

	unlock();

}

inline
void GeneralPool_Impl::lock()
{
	_mutex.lock();
}

inline
void GeneralPool_Impl::unlock()
{
	_mutex.unlock();
}

void GeneralPool_Impl::allocPage()
{
	GeneralPool_Page*	page;

	lock();

	page					= new GeneralPool_Page;

	page->_prev				= null;
	page->_next				= _availablePage;
	page->_availableSize	= default_page_size;

	if( _availablePage )
		_availablePage->_prev = page;

	_availablePage			= page;

	// ù��° ��带 �غ��Ѵ�
	page->_firstNode->clear(page);

	// ù��° ���� �Ҵ� ������ ���
	page->_available = page->_firstNode;

	// �ִ� �Ҵ� ������ ũ��� ����� �� ������ ũ��(maximum space) = (page size) - (header size)
	page->_firstNode->_size = page->_availableSize - nodeSize();

	unlock();
}




void* GeneralPool_Impl::alloc(unsigned int size)				
{
	GeneralPool_Page*	page;
	GeneralPool_Node*	node;

	unsigned int realSize = size;

	// �ּ� �Ҵ� ������
	// ( �ʹ� ����� ���� �͵�� Node �� �Ҵ�Ǹ� Alloc �� Search�� Free�� ���ɿ� ���� �����ϱ�~ )
	if (size < min_alloc_size)
	{
		size = min_alloc_size;
	}
	
	// �ּ� �Ҵ� ������� �е�
	size = ((size / min_alloc_size) + (size % min_alloc_size == 0 ? 0 : 1)) * min_alloc_size;

	lock();

	if (size + nodeSize() > default_page_size)
	{
		GeneralPool_Node*	node;

		//===============================================================================================
		// ������ ũ�⺸�� ũ�Ƿ� �Ҵ��� �Ұ����ϴ� ����( �⺻ ) ��������Ϳ��� ���� �Ҵ�޴´�
		// ���� ���� �۾�
		//===============================================================================================
		node = (GeneralPool_Node*) _default_allocator.alloc(size + nodeSize());

		node->clear(null);

		node->_size			= size;
		node->_realSize		= realSize;
		node->_allocated	= true;

		unlock();

		return node->getMemory();
	}

	page = _availablePage;

	while (page != null)
	{
		if (page->_availableSize >= size + nodeSize())
		{
			// ���ӵ� �������� �����ϴ��� Ȯ���Ѵ�
			node = page->_available;

			while (node != null)
			{
				if (node->_size >= size)
				{
					// �������� ���� ������ �����Ѵ�
					page->_availableSize -= size + nodeSize();

					// ����� ū ����, �� ��带 �Ҵ��Ѵ�
					if (node->_size >= size + nodeSize() + min_alloc_size)
					{
						// �� ��带 �߶� �� ��带 �����
						splitAllocNode(page, node, size);

						unlock();

						// ������̼� ũ�⸦ ����Ѵ�
						node->_realSize = realSize;

						// ������ ��ȿ���� �˻�;
						validate_page( node->_owner );

						return node->getMemory();
					}
					else
					{
						// �� ��带 ��°�� �Ҵ��Ѵ�

						// �� ��� ����Ʈ���� �����Ѵ�
						page->removeAvailableList(node);

						unlock();

						// ������̼� ũ�⸦ ����Ѵ�
						node->_realSize = realSize;
						
						validate_page( node->_owner );

						return node->getMemory();
					}
				}

				node = node->_availableNext;
			}
		}
		else
		{

			page->_miss++;

			
			// ĳ�� �̽����� �ʹ� ������ ����Ʈ���� ���� Missing List�� ��� ��Ŵ
			if( page->_miss > max_miss_count )
			{
				GeneralPool_Page* missingPage = page;
				// ���� ����Ʈ���� ���
				if( page->_prev )
					page->_prev->_next = page->_next;
				if( page->_next )
					page->_next->_prev = page->_prev;

				if( page == _availablePage )
					_availablePage = page->_next;

				page = page->_next;

				if( page ) {
					if( page->_next == page ) {
						assert( page->_next == page);
					}
				}

				// missing page �� ����
				missingPage->_prev				= null;
				missingPage->_next				= _missingPage;
				if( _missingPage )
					_missingPage->_prev = missingPage;

				_missingPage = missingPage;
				continue;
			}
		}

		// ���� �������� �˻��Ѵ�
		page = page->_next;
	}

	// �Ҵ��� ������ �������� �����Ƿ� �� �������� ����	
	allocPage();

	void* pReturn = null;
	pReturn = alloc(size);
	assert_msg( "�� �������� �Ҵ��� �� ����", pReturn != null );

	unlock();

	return pReturn;
	
}

void GeneralPool_Impl::splitAllocNode( GeneralPool_Page* page, GeneralPool_Node* node, unsigned int size)
{
	GeneralPool_Node*	pNewBlankNode;
	BYTE*				pReturnAddress;

	// ��忡 ��� ������ �޸� ���� ���´�
	pReturnAddress = node->getMemory();

	// �� ��带 �Ҵ��Ѵ�
	pNewBlankNode = (GeneralPool_Node*) (pReturnAddress + size);
	pNewBlankNode->clear(node->_owner);

	assert_msg("��� �Ҵ��� ���� ������� ���� ����", node->_size >= size + nodeSize() + min_alloc_size);

	// �� ��忡 ��� ������ ������ ����Ѵ�
	pNewBlankNode->_size = node->_size - size - nodeSize();

	// ���� ��忡�� �� ��忡 �Ҵ�� ������ �����Ѵ�
	node->_size = size;

	// ���� ��带 �Ҵ�� ���·� �����Ѵ�
	node->_allocated = true;

	// 1. ��� ���� ��� ����Ʈ�� ��ũ�� �����Ѵ�
	if (node->_availablePrev == null)
	{
		assert_msg("��ȿ���� ���� ���", page->_available == node);

		// ���ŵ� ��尡 ��� ���� ����� ��������Ƿ� ������Ʈ
		page->_available = pNewBlankNode;
		pNewBlankNode->_availablePrev = null;
	}
	else
	{
		node->_availablePrev->_availableNext = pNewBlankNode;
		pNewBlankNode->_availablePrev = node->_availablePrev;
	}

	// ���ŵǴ� ����� ���� ��� ���� ��带 �� �� ����� ���� ��� ���� ���� �����Ѵ�
	pNewBlankNode->_availableNext = node->_availableNext;

	if (node->_availableNext != null)
	{
		node->_availableNext->_availablePrev = pNewBlankNode;
	}

	// ���ŵ� ����� ��ũ�� ���´�
	node->_availablePrev = null;
	node->_availableNext = null;

	// 2. ��� ����Ʈ�� ��ũ�� �����Ѵ�
	pNewBlankNode->_prev = node;
	pNewBlankNode->_next = node->_next;

	if (node->_next != null)
	{
		node->_next->_prev = pNewBlankNode;
	}

	node->_next = pNewBlankNode;
}

void GeneralPool_Impl::free(void* p)
{
	GeneralPool_Node*	node;

	lock();

	node = Node(p);
	assert_msg("�߸��� ���", node->_allocated);
	assert_msg("�߸��� ���", node->_availablePrev == null);
	assert_msg("�߸��� ���", node->_availableNext == null);

	if (node->_size + nodeSize() > default_page_size)
	{
		// ���� ��������Ϳ��� ���� �Ҵ���� ���̴�
		_default_allocator.free( (void*&)node);

		unlock();

		return;
	}
	// ������ ��ȿ�� üũ
	validate_page( node->_owner );

	// �Ҵ�Ǿ��� �÷��׸� �����Ѵ�
	node->_allocated = false;

	// �������� ���� ������ �����Ѵ�
	node->_owner->_availableSize += node->_size + nodeSize();

	// �� ���� ���� ��ĥ �ʿ䰡 �ִ��� ���캻��
	if (node->_prev != null)
	{
		if (node->_next != null)
		{
			// �� �� ��� ��� �����Ѵ�
			if (!node->_prev->_allocated)
			{
				// ���� ���� ���ľ� �Ѵ�
				if (!node->_next->_allocated)
				{
					freeMergePrevAndNext(node);
				}
				else
				{
					freeMergePrev(node);
				}
			}
			else if (!node->_next->_allocated)
			{
				freeMergeNext(node);
			}
			else
			{
				freeMergeNone(node);
			}
		}
		else
		{
			//  �� ��常 �����Ѵ�
			if (!node->_prev->_allocated)
			{
				freeMergePrev(node);
			}
			else
			{
				freeMergeNone(node);
			}
		}
	}
	else
	{
		if (node->_next != null)
		{
			// �� ��常 �����Ѵ�
			if (!node->_next->_allocated)
			{
				freeMergeNext(node);
			}
			else
			{
				freeMergeNone(node);
			}
		}
		else
		{
			// �� �� ��� ��� ����
			freeMergeNone(node);
		}
	}

	assert_msg("availabe�� Null�̸� �ȵ�", node->_owner->_available != null);
	validate_page( node->_owner );

	// ĳ�� Missing Rate�� ���Ƽ� missingList�� �����ϰ� ���� ��� ���� �ٽ� _availablePrev�� ���� ��������
	if( node->_owner->_miss > max_miss_count )
	{
		GeneralPool_Page* page = node->_owner;
		assert_msg( "AvaialbePage�� ���� �ؾ� ��", _availablePage != null );
		assert_msg( "Missing Count �� 64��", page->_miss == max_miss_count + 1 );
		page->_miss = 0;

		if( page->_prev )
			page->_prev->_next = page->_next;
		if( page->_next )
			page->_next->_prev = page->_prev;
        
		if( _missingPage == page )
		{
			_missingPage = page->_next;
		}

		page->_prev				= null;
		page->_next				= _availablePage;
		if( _availablePage )
			_availablePage->_prev = page;

		_availablePage = page;
	}

	unlock();
}

void GeneralPool_Impl::freeMergePrevAndNext(GeneralPool_Node* node)
{
	GeneralPool_Node*	prevNode;
	GeneralPool_Node*	nextNode;

	prevNode	= node->_prev;
	nextNode	= node->_next;
#ifdef _DEBUG
	assert_msg("�߸��� ����Ʈ", prevNode->_next == node);
	assert_msg("�߸��� ����Ʈ", nextNode->_prev == node);

	if (prevNode->_availablePrev == null)
	{
		assert_msg("�߸��� ����Ʈ", prevNode->_owner->_available == prevNode);
	}

	// ��� ���� ��� ����Ʈ���� ���� ��带 �����Ѵ�
	assert_msg("�߸��� ����Ʈ", prevNode->_availableNext == nextNode);
	assert_msg("�߸��� ����Ʈ", nextNode->_availablePrev == prevNode);
#endif
	prevNode->_availableNext = nextNode->_availableNext;
	if (nextNode->_availableNext != null)
	{
		nextNode->_availableNext->_availablePrev = prevNode;
	}

	// ��� ����Ʈ���� ������ ���� ���� ��带 ���ش�
	prevNode->_next = nextNode->_next;
	if (nextNode->_next != null)
	{
		nextNode->_next->_prev = prevNode;
	}

	// ��� ũ�⸦ �����Ѵ�
	prevNode->_size += node->_size + nodeSize() +	nextNode->_size + nodeSize();

}

void GeneralPool_Impl::freeMergePrev( GeneralPool_Node* node)

{
	GeneralPool_Node*	prevNode;

	prevNode	= node->_prev;

	assert_msg("�߸��� ����Ʈ", prevNode->_next == node);

#ifdef _DEBUG
	if (prevNode->_availablePrev == null)
	{
		assert_msg("�߸��� ����Ʈ", prevNode->_owner->_available == prevNode);
	}
#endif

	// ��� ����Ʈ���� ������ ��带 ���ش�
	prevNode->_next = node->_next;

	if (node->_next != null)
	{
		node->_next->_prev = prevNode;
	}

	// ��� ũ�⸦ �����Ѵ�
	prevNode->_size += node->_size + nodeSize();

}

void GeneralPool_Impl::freeMergeNext(GeneralPool_Node* node)
{
	GeneralPool_Node*	nextNode;

	nextNode	= node->_next;

	assert_msg("�߸��� ����Ʈ", nextNode->_prev == node);

#ifdef _DEBUG
	if (nextNode->_availablePrev == null)
	{
		assert_msg("�߸��� ����Ʈ", nextNode->_owner->_available == nextNode);
	}
#endif

	// ��� ���� ��� ����Ʈ���� ���� ��带 ���� ������ ���� ��ü�Ѵ�
	if (nextNode->_availableNext != null)
	{
		nextNode->_availableNext->_availablePrev = node;
		node->_availableNext = nextNode->_availableNext;
	}

	if (nextNode->_availablePrev != null)
	{
		nextNode->_availablePrev->_availableNext = node;
		node->_availablePrev = nextNode->_availablePrev;
	}
	else
	{
		GeneralPool_Page*	page;
		page = node->_owner;

		// ���� ��尡 ���� ���� ��� ���� ����̹Ƿ� ����Ʈ ��带 ��ü
		assert_msg("�߸��� ����Ʈ ���", page->_available == nextNode);
		page->_available = node;
	}

	// ��� ����Ʈ���� ���� ��带 ���ش�
	node->_next = nextNode->_next;
	if (nextNode->_next != null)
	{
		nextNode->_next->_prev = node;
	}

	// ��� ũ�⸦ �����Ѵ�
	node->_size += nextNode->_size + nodeSize();

}

void GeneralPool_Impl::freeMergeNone(GeneralPool_Node* node)
{
	GeneralPool_Page*	page;
	GeneralPool_Node*	prevNode;
	bool					bLinked = false;

	page = node->_owner;

	// ���� ��� ���� ��� ����Ʈ���� ������Ʈ�Ѵ�
	if (page->_available != null)
	{
		// ���ʿ��� ���� ���� ��� ������ ��带 ã�´�
		prevNode = node->_prev;

		while (prevNode != null)
		{
			if (!prevNode->_allocated)
			{
				// ��� ������ ��带 ã�Ҵ�, ����Ʈ�� �����ִ´�
				if (prevNode->_availableNext != null)
				{
					prevNode->_availableNext->_availablePrev = node;
					node->_availableNext = prevNode->_availableNext;
				}

				prevNode->_availableNext = node;
				node->_availablePrev = prevNode;

				bLinked = true;
				break;
			}

			prevNode = prevNode->_prev;
		}

		if (!bLinked)
		{
			// �������� �� ��尡 ����, �� �� ��尡 ���� ���� �� ��尡 �ȴ�
			page->_available->_availablePrev = node;
			node->_availableNext = page->_available;

			page->_available = node;
		}
	}
	else
	{
		// �� ��尡 ��� ������ ������ ��尡 �ȴ�
		page->_available = node;
		
	}
}


void GeneralPool_Impl::valid(GeneralPool_Page* page)
{
	GeneralPool_Node*	node;
	GeneralPool_Node*	pLastAvailableNode = null;

	// ù ������ Ž���Ѵ�
	node = page->_firstNode;

	while (node != null)
	{
		if (!node->_allocated)
		{
			assert_msg("�߸��� �Ҵ� ����Ʈ", node->_availablePrev == pLastAvailableNode);
			assert_msg("�߸��� �Ҵ� ����Ʈ", node->_availablePrev != node);
			assert_msg("�߸��� �Ҵ� ����Ʈ", node->_availableNext != node);

			if (node->_availablePrev == null)
			{
				assert_msg("�߸��� Root", node->_owner->_available == node);
			}

			pLastAvailableNode = node;
		}
		node = node->_next;
	}
}