
#pragma once

#include <assert.h>
#include <vector>

namespace Communications
{

class CChunk
{
public:
	CChunk( void ) : m_trunk( 0 ) 
	{

	}
	~CChunk( void )
	{

	}

	void allocate( unsigned int chunksize, size_t datasize )
	{
		m_trunk = ::malloc( datasize*chunksize );
		assert( m_trunk );
		char* p = (char*)m_trunk;
		for ( unsigned int i=0; i<chunksize; ++i )
		{
			m_datas.push_back( p );
			p += datasize;
		}
	}

	void free( void )
	{
		if ( m_trunk )
		{
			::free( m_trunk );
			m_trunk = 0;
		}
	}

public:
	void*				m_trunk;
	std::vector<char*>	m_datas;
};

}

