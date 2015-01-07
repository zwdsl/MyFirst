
#pragma once


#include "Chunk.h"

namespace Communications
{

typedef long (*exchange_and_add)( long volatile*, long );


class CChunkManager
{
public:
	CChunkManager( void ) 
		: m_chunksize( 0 ), m_datasize( 0 )
		, m_usedchunk_pos( 0 ), m_freechunk_count( 0 ), m_eaa_fun( 0 )
	{
		
	}

	~CChunkManager( void )
	{

	}

	bool init( unsigned int chunksize, size_t datasize, exchange_and_add fun )
	{
		m_chunksize		= chunksize;
		m_datasize		= datasize;

		m_used_chunk.allocate( m_chunksize, m_datasize+sizeof(CChunkManager*) );

		for ( unsigned int i=0; i<m_chunksize; ++i )
		{
			char* p	= m_used_chunk.m_datas[i];
			set_chunk_manager( p, datasize, this );
		}
		
		m_usedchunk_pos		= 0;
		m_freechunk_count	= 0;
		m_eaa_fun			= fun;

		return true;
	}

	void fini( void )
	{
		m_used_chunk.free( );
	}

	// 返回是否已经用完
	bool allocate( void*& data_ptr )
	{
		long now_usedchunk_pos = (*m_eaa_fun)( &m_usedchunk_pos, 1 );

		//if ( now_usedchunk_pos>=m_chunksize )	return true;

		data_ptr = m_used_chunk.m_datas[now_usedchunk_pos];

		return (unsigned long)now_usedchunk_pos>=(m_chunksize-1);
	}

	// 返回是否可以reset
	bool free( void* data_ptr )
	{
		long now_freechunk_count = (*m_eaa_fun)( &m_freechunk_count, 1 );

		return (unsigned long)now_freechunk_count>=(m_chunksize-1);
	}

	void reset( void )
	{
		m_usedchunk_pos		= 0;
		m_freechunk_count	= 0;
	}
	
	static void set_chunk_manager( char* p, size_t datasize, CChunkManager* pcm )
	{
		p			+= datasize;
		CChunkManager** ppcm = (CChunkManager**)( p );
		*ppcm		= pcm;
	}

	static CChunkManager* get_chunk_manager( char* p, size_t datasize )
	{
		p			+= datasize;
		CChunkManager** ppcm = (CChunkManager**)( p );

		return *ppcm;
	}

private:
	unsigned long	m_chunksize;
	size_t			m_datasize;

	CChunk			m_used_chunk;
	volatile long	m_usedchunk_pos;

	volatile long	m_freechunk_count;

	exchange_and_add	m_eaa_fun;
};

}

