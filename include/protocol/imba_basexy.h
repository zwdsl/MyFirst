
#pragma once

#include <string>
#include <vector>
#include "biostream.h"

//        begin  <  xy  < end
const int IMBA_XY_BEGIN	= 0;
const int IMBA_XY_END	= 10000;

// 通用协议
const int IMBA_COMMONXY_BEGIN	= 0;
const int IMBA_COMMONXY_END		= 100;

// config 协议号范围
const int IMBA_CONFIGXY_BEGIN	= 100;
const int IMBA_CONFIGXY_BND		= 200;

// db 协议范围
const int IMBA_DBXY_BEGIN		= 200;
const int IMBA_DBXY_END			= 300;

// gate<->ghost 协议范围
const int IMBA_GHOST2GATEXY_BEGIN	= 300;
const int IMBA_GHOST2GATEXY_END		= 400;

// client 与 login 的协议范围
const int IMBA_CLIENT2LOGINXY_BEGIN	= 10000;
const int IMBA_CLIENT2LOGINXY_END	= 10100;

// client 与 gate 的协议范围
const int IMBA_CLIENT2GATEXY_BEGIN	= 10100;
const int IMBA_CLIENT2GATEXY_END	= 10200;



template <const int msgid>
struct Imba_TCPXY
{
public:
	enum { MSGID=msgid };
	const int get_msgid( void ) const { return msgid; }
};


struct Imba_UdpXY
{
	Imba_UdpXY( const int id, const std::string& key, const size_t count=0 )
		: msgid( id ), paramcount( count ), msgkey( key )
	{
	}

	const int get_msgid( void ) const			{ return msgid; }
	const std::string& get_msgkey( void ) const	{ return msgkey; }
	const size_t get_paramcount( void ) const	{ return paramcount; }

	int				msgid;
	size_t			paramcount;
	std::string		msgkey;
};


inline
int Explain_UdpXY( const std::string& msg, const std::vector<Imba_UdpXY>& msgcontains, std::vector<std::string>& retparams )
{
	for ( std::vector<Imba_UdpXY>::const_iterator it=msgcontains.begin(); it!=msgcontains.end(); ++it )
	{
		const Imba_UdpXY& xy = *it;
		std::size_t	keylen = xy.get_msgkey().length( );
		if ( xy.get_msgkey().compare( 0, keylen, msg, 0, keylen )==0 )
		{
			std::string paramstr = msg.substr( keylen );
			std::vector<std::string> params;

			std::string sp = " ";
			std::size_t spl = sp.length();
			std::size_t pos = paramstr.find(sp);

			while ( pos != std::string::npos )
			{
				if ( pos!=0 )
				{
					params.push_back( paramstr.substr(0, pos) );
				}

				paramstr.erase( 0, pos+spl );
				pos = paramstr.find( sp );
			}

			if ( paramstr.length() >= 0 ) params.push_back( paramstr );

			size_t i = 0;
			while( i<xy.get_paramcount() && i<params.size() )
			{
				retparams.push_back( params[i] );
				++i;
			}
		
			return xy.get_msgid();
		}
	}

	return 0;
}



const int COMMONXY_BATCH_BEGIN	= IMBA_COMMONXY_BEGIN+1;
const int COMMONXY_BATCH_END	= IMBA_COMMONXY_BEGIN+2;

struct Imba_Batch_Begin : public Imba_TCPXY<COMMONXY_BATCH_BEGIN>
{
	friend bostream& operator<<(bostream& bos, const Imba_Batch_Begin& msg)
	{
		return bos;
	}

	friend bistream& operator>>(bistream& bis, Imba_Batch_Begin& msg)
	{
		return bis;
	}
};

struct Imba_Batch_End : public Imba_TCPXY<COMMONXY_BATCH_END>
{
	friend bostream& operator<<(bostream& bos, const Imba_Batch_End& msg)
	{
		return bos;
	}

	friend bistream& operator>>(bistream& bis, Imba_Batch_End& msg)
	{
		return bis;
	}
};

