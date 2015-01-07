
#pragma once

//#ifndef __AGBASE_MEMORYPOOL_H__
//#define __AGBASE_MEMORYPOOL_H__

// 注意问题：
// 1、保证线程安全
// 2、不支持new[] 和 delete[]，只支持new delete 操作
// 3、new之后CMemoryPool_Public不再保存new出来的内存，与delete必须成对出现，且不能对同一内存多次delete
// 4、所有内存都是单次::malloc出的，所以第一的申请内存的速度不快
// 5、T必须具有没有参数的构造函数

#include <memory>
#include "StackQueue.h"

namespace Communications
{

	template<class T, unsigned int ALLOC_BLOCK_SIZE=5>
	class CMemoryPool_Private
	{
	public:
		typedef Base_Node<T> node;

		CMemoryPool_Private( void )
		{	
		}

		~CMemoryPool_Private( void )
		{	
		}

		void* Alloc_From_Pool( void )
		{
			node *pret = 0;
			while ( !( pret=m_pool.Pop() ) )
			{
				AllocBlock( );
			}
			return pret;
		}

		T* New_Object( void )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T() );

			return 0;
		}

		template<class P0>
		T* New_Object( const P0& p0 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0 ) );

			return 0;
		}

		template<class P0>
		T* New_Object( P0& p0 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0 ) );

			return 0;
		}

		template<class P0, class P1>
		T* New_Object( const P0& p0, const P1& p1 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1 ) );

			return 0;
		}

		template<class P0, class P1>
		T* New_Object( P0& p0, P1& p1 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1 ) );

			return 0;
		}

		template<class P0, class P1, class P2>
		T* New_Object( const P0& p0, const P1& p1, const P2& p2 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2 ) );

			return 0;
		}

		template<class P0, class P1, class P2>
		T* New_Object( P0& p0, P1& p1, P2& p2 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2 ) );

			return 0;
		}

		template<class P0, class P1, class P2, class P3>
		T* New_Object( const P0& p0, const P1& p1, const P2& p2, const P3& p3 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2, p3 ) );

			return 0;
		}

		template<class P0, class P1, class P2, class P3>
		T* New_Object( P0& p0, P1& p1, P2& p2, P3& p3 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2, p3 ) );

			return 0;
		}

		template<class P0, class P1, class P2, class P3, class P4>
		T* New_Object( const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2, p3, p4 ) );

			return 0;
		}

		template<class P0, class P1, class P2, class P3, class P4>
		T* New_Object( P0& p0, P1& p1, P2& p2, P3& p3, P4& p4 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2, p3, p4 ) );

			return 0;
		}

		template<class P0, class P1, class P2, class P3, class P4, class P5>
		T* New_Object( const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2, p3, p4, p5 ) );

			return 0;
		}

		template<class P0, class P1, class P2, class P3, class P4, class P5>
		T* New_Object( P0& p0, P1& p1, P2& p2, P3& p3, P4& p4, P5& p5 )
		{
			void* object = Alloc_From_Pool( );

			if ( object )	return ( T* )( ::new(object)T( p0, p1, p2, p3, p4, p5 ) );

			return 0;
		}

		void Free_To_Pool(void* deletepointer)
		{
			assert( deletepointer );
			m_pool.Push( reinterpret_cast<node*>(deletepointer) );
		}

		void Delete_Object( T* object )
		{
			if ( object )
			{
				(object)->~T( );
				Free_To_Pool( object );
			}
		}

		void Clear_Pool( void )
		{
			node *pret = 0;
			while ( (pret=m_pool.Pop()) )
			{
				::free(pret);
			}
		}

	private:
		void AllocBlock( void )
		{
			assert( ALLOC_BLOCK_SIZE>0 );

			for (unsigned int i=0; i<ALLOC_BLOCK_SIZE; ++i)
			{
				// 可以使用new node，malloc可以减少node的构造调用
				node* ptemp = (node*)(::malloc( sizeof(node) ));
				if ( ptemp )
				{
					ptemp->pnext = 0;
					m_pool.Push( ptemp );
				}
			}
		}

	private:
		CMemoryPool_Private( const CMemoryPool_Private& );
		CMemoryPool_Private& operator=( const CMemoryPool_Private& );

		CBase_Stack<T>	m_pool;
	};


	template<class T, unsigned int ALLOC_BLOCK_SIZE=5>
	class CMemoryPool_Public
	{
	public:
		virtual ~CMemoryPool_Public( void )
		{

		}

		static void* operator new(size_t alloclength)
		{
			assert( sizeof(T)==alloclength );
			return m_memorypool.Alloc_From_Pool( );
		}

		static void operator delete(void* deletepointer)
		{
			m_memorypool.Free_To_Pool( deletepointer );
		}

		static void Clear_Pool( void )
		{
			m_memorypool.Clear_Pool( );
		}

	private:
		static void* operator new[](size_t alloclength);
		static void operator delete[](void* deletepointer);

		static CMemoryPool_Private<T, ALLOC_BLOCK_SIZE>	m_memorypool;
	};

	template<class T, unsigned int ALLOC_BLOCK_SIZE>
	CMemoryPool_Private<T, ALLOC_BLOCK_SIZE> CMemoryPool_Public<T, ALLOC_BLOCK_SIZE>::m_memorypool;
}

//#endif // __AGBASE_MEMORYPOOL_H__
