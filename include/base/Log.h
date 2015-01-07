
#pragma once

#include <string>
#include <sstream>

namespace Communications
{
	std::string	GetModulePath( void );
	bool		CreateDir(const char * path);

	enum LogLevel
	{
		LOGLEVEL_DEBUG = 0,
		LOGLEVEL_INFO,
		LOGLEVEL_WARN,
		LOGLEVEL_ERROR,
	};

	const int LOG_ALL	= LOGLEVEL_DEBUG;
	const int LOG_NULL	= LOGLEVEL_ERROR+1;

	void InitLogger( const char* prefix, int level );
	void FiniLogger( void );
	void SetLogLevel( int level );
	void Log_Text( int level, const char* str );
	void Log_Text_Format( int level, char* szstr, ... );
	void Log_Hex( int level, const char* hex, size_t len );

	//	ss << boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()); 

	// 用流写日志
	/*ss << " [" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << os;\*/
#define LOG_STREAM( level, os ) \
	{\
	std::ostringstream ss;\
	ss << os; \
	std::string str( ss.str() ); \
	Communications::Log_Text( level, (const char *)str.c_str() );\
	}

#define LOGI( os ) LOG_STREAM( Communications::LOGLEVEL_INFO, os )
#define LOGD( os ) LOG_STREAM( Communications::LOGLEVEL_DEBUG, os )
#define LOGW( os ) LOG_STREAM( Communications::LOGLEVEL_WARN, os )
#define LOGE( os ) LOG_STREAM( Communications::LOGLEVEL_ERROR, os )

}

#include "impl/LogImpl.h"

