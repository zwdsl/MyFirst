
#pragma once

#include <list>
#include "Lock.h"
#include "impl/ChunkManager.h"
#include "TLSPointer.h"

namespace Communications
{

inline
long Default_EAA( long volatile* value, long addvalue )
{
	long nowvalue = *value;
	*value += addvalue;
	return nowvalue;
}

inline 
long Atomic_EAA( long volatile*_ptr, long dv )
{
#ifdef __GNUC__

	int r;

	__asm__ __volatile__
		(
		"lock\n\t"
		"xadd %1, %0":
	"=m"( *_ptr ), "=r"( r ): // outputs (%0, %1)
	"m"( *_ptr ), "1"( dv ): // inputs (%2, %3 == %1)
	"memory", "cc" // clobbers
		);

	return r;
	// /usr/src/kernels/2.6.18-164.el5-x86_64/include/asm-x86_64/atomic.h 
	//return __sync_fetch_and_add( _ptr, dv );

#else
	return InterlockedExchangeAdd(_ptr, dv);

#endif

}

// 如果不调用Delete_Object，用Clear_Pool清理内存，会导致析构函数不掉用
// 
template<class T, unsigned int CHUNKSIZE=32>
class CObjectPool
{
public:
	CObjectPool( void )
	{
		m_lock		= NULL_LOCK::Instance();
		m_eaa_fun	= Default_EAA;
	}

	~CObjectPool( void )
	{
	}

	void SetThreadSefeParams( ILock* lock, exchange_and_add eaa_fun )
	{
		m_lock		= lock;
		m_eaa_fun	= eaa_fun;
	}

	void Clear_Pool( )
	{
		CLockGuard l( m_lock );

		for ( std::list<CChunkManager*>::iterator it=m_alls.begin(); it!=m_alls.end(); ++it )
		{
			(*it)->fini();
			delete *it;
		}

		m_frees.clear();
		m_alls.clear();
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

	void Delete_Object( T* object )
	{
		if ( object )
		{
			(object)->~T( );
			Free_To_Pool( object );
		}
	}

	void* Alloc_From_Pool( void )
	{
		CChunkManager* cm = GetCurrentChunkManger( );

		void* p = 0;
		if ( cm->allocate( p ) )
		{
			ResetCurrentChunkManager( );
		}

		return p;
	}

	void Free_To_Pool( void* deletepointer )
	{
		if ( deletepointer )
		{
			CChunkManager* pcm = CChunkManager::get_chunk_manager( (char*)deletepointer, sizeof(T) );
			if ( pcm && pcm->free( deletepointer ) )
			{
				pcm->reset( );

				Push_ChunkManger( pcm );
			}
		}
	}

private:
	CChunkManager* GetCurrentChunkManger( void )
	{
		if ( !m_current )
		{
			m_current = Pop_ChunkManger( );
		}

		return m_current.Get();
	}

	void ResetCurrentChunkManager( void )
	{
		m_current = Pop_ChunkManger( );
	}

	CChunkManager* Pop_ChunkManger( void )
	{
		CChunkManager* cm = 0;

		{
			CLockGuard l( m_lock );

			if ( !m_frees.empty() )
			{
				cm = m_frees.front();
				m_frees.pop_front();
			}
		}

		if ( 0==cm )
		{
			cm = new CChunkManager( );
			cm->init( CHUNKSIZE, sizeof(T), m_eaa_fun );

			{
				CLockGuard l( m_lock );
				m_alls.push_back( cm );
			}
		}

		return cm;
	}

	void Push_ChunkManger( CChunkManager* cm )
	{
		CLockGuard l( m_lock );
		m_frees.push_back( cm );
	}

private:
	CTlsPointer<CChunkManager>			m_current;
	ILock*								m_lock;
	exchange_and_add					m_eaa_fun;
	std::list<CChunkManager*>			m_frees;
	std::list<CChunkManager*>			m_alls;
};

}

