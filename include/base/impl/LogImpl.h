
#pragma once

#if _MSC_VER>=1400
#define _CRT_SECURE_NO_DEPRECATE
#endif

#if defined _WINDOWS_ || defined WIN32

#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif

#ifndef snprintf
#define snprintf _snprintf
#endif

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#endif


#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include "../MemoryPool.h"

namespace Communications
{
	inline
	std::string GetModulePath( void )
	{
#if defined _WINDOWS_ || defined WIN32

		char szFile[MAX_PATH];
		GetModuleFileNameA(GetModuleHandle(NULL), szFile, MAX_PATH);

		char szDrive[MAX_PATH], szPath[MAX_PATH];

		_splitpath(szFile, szDrive, szPath, NULL, NULL);
		strncat(szDrive, szPath, sizeof(szDrive)-strlen(szDrive)-1);

		std::string ret = szDrive;
		if ( ret.find_last_of('\\') != ret.length()-1 && ret.find_first_of('/') != ret.length()-1 )
		{
			ret.append("\\");
		}

		return ret;

#else

		char dir[1024] = {0};
		int count = readlink( "/proc/self/exe", dir, 1024 );
		if ( count < 0 || count >= 1024 )
			return "./";
		else
			dir[count] = 0;
		std::string ret = dir;
		std::string::size_type pos = ret.find_last_of('/');

		if( pos == std::string::npos ) return ret;

		return ret.substr(0, pos+1);

#endif
	};

	inline
	bool CreateDir(const char * path)
	{
#if defined _WINDOWS_ || defined WIN32

		if( PathFileExistsA( path ) ) return true;
		return (CreateDirectoryA( path, NULL )==TRUE);

#else

		mkdir(path, 0755);
		return true;

#endif
	}

	static const char* g_logDesc[LOG_NULL]={" [DEBUG] ", " [INFO ] ", " [WARN ] ", " [ERROR] "};

#define MAX_LOG_LEN		1024
#define MAX_LOG_COUNT   1024

#if defined _WINDOWS_ || defined WIN32
#define STDCOUT
#endif

	class CLogThread
	{
	public:
		class CLogPacket : public CMemoryPool_Public<CLogPacket, 5>
		{
		public:
			char	level;
			char	type;
			int		len;
			char	log[MAX_LOG_LEN];
		};

		CLogThread( void )
		{
			m_bruning			= false;
			m_bDirCreated	= false;
			m_nLogLevel		= LOG_NULL;
			m_nDate			= 0;
			m_nMonth		= 0;
		}

		void Init( const char* prefix, int level )
		{
			boost::mutex::scoped_lock lock( m_mutex );

			if ( m_bruning ) return;

			m_bruning = true;
			m_sLogRootDir = GetModulePath()+"/log/";
			m_nLogLevel = level;

			if ( m_nLogLevel<LOG_ALL )			m_nLogLevel=LOG_ALL;
			else if( m_nLogLevel>=LOG_NULL )	m_nLogLevel=LOG_NULL;

			if ( prefix )	m_sFilePrefix = prefix;

			CreateDir(  m_sLogRootDir.c_str() );

			CreateMonthDir();

			m_thread.reset( new boost::thread( boost::bind( &CLogThread::Run, this ) ) );
		}

		void Fini( void )
		{
			boost::mutex::scoped_lock lock( m_mutex );

			m_bruning = false;
			m_condition.wait( lock );

			if( m_fp.is_open() )
			{
				m_fp.close();
			}

			CLogPacket* packet = 0;
			while ( !m_packets.empty() )
			{
				packet = m_packets.front( );
				m_packets.pop_front();

				delete packet;
			}
		}

		void SetLogLevel( int level )
		{
			boost::mutex::scoped_lock lock( m_mutex );
			m_nLogLevel = level;

			if ( m_nLogLevel<LOG_ALL )			m_nLogLevel=LOG_ALL;
			else if( m_nLogLevel>=LOG_NULL )	m_nLogLevel=LOG_NULL;
		}

		void LogText( int level, const char* str )
		{
			if( !m_bruning ) return;
			if( level<m_nLogLevel )				return;
			if( m_nLogLevel==LOG_NULL )	return;

			CLogThread::CLogPacket* packet = new CLogThread::CLogPacket( );

			packet->level = level;
			packet->type  = 0;

			if ( strlen(str)>=MAX_LOG_LEN )
			{
				memcpy( packet->log, str, MAX_LOG_LEN );
				packet->len = MAX_LOG_LEN;
			}
			else
			{
				memcpy( packet->log, str, strlen(str)+1 );
				packet->len = int(strlen(str)+1);
			}

			PostLog( packet );
		}

		void LogHex( int level, const char * hex, size_t len )
		{
			if( !m_bruning ) return;
			if( level<m_nLogLevel )				return;
			if( m_nLogLevel==LOG_NULL )	return;

			CLogThread::CLogPacket* packet = new CLogThread::CLogPacket( );

			packet->level = level;
			packet->type  = 1;
			packet->len   = (int)len;
			if( packet->len>(int)sizeof(packet->log) ) packet->len=(int)sizeof(packet->log);
			memcpy( packet->log, hex, packet->len );

			PostLog( packet );
		}

		void PostLog( CLogPacket* logpacket )
		{
			if( 0==logpacket )	return;

			boost::mutex::scoped_lock lock( m_mutex );

			if ( m_packets.size()>=MAX_LOG_COUNT )
			{
				delete logpacket;
				return;
			}

			m_packets.push_back( logpacket );
		}

	protected:
		int Run( void )
		{
			CLogPacket* packet = 0;
			while ( 1 )
			{
				{
					boost::mutex::scoped_lock lock( m_mutex );

					if ( !m_packets.empty() )
					{
						packet = m_packets.front( );
						m_packets.pop_front();
					}
					else if( !m_bruning )
					{
						break;
					}
				}

				if ( packet==0 )
				{
					boost::xtime xt;
					boost::xtime_get(&xt, boost::TIME_UTC); 
					xt.nsec += 200000000;
					boost::thread::sleep(xt); 

				}
				else
				{
					if (packet->type==0)
					{
						Log_Text_2File( packet->level, packet->log );
					}
					else
					{
						Log_Hex_2File( packet->level, packet->log, packet->len );
					}

					delete packet;
					packet = 0;
				}
			}

			boost::mutex::scoped_lock lock( m_mutex );
			m_condition.notify_all( );

			return 0;
		}

		void CreateMonthDir()
		{
			boost::gregorian::date today = boost::gregorian::day_clock::local_day();
			char subDir[255];

			m_bDirCreated = false;
			int year = today.year();
			int month= today.month();
			//int day  = today.day();

			sprintf( subDir, "%s%d-%d/", m_sLogRootDir.c_str(), year, month );
			m_sLogDir = subDir;
			m_bDirCreated = true;
			m_nMonth = month;

			CreateDir(  m_sLogDir.c_str() );
		}

		void Log_Text_2File( int level, const char* str )
		{
			//if( level<m_nLogLevel )				return;
			//if( m_nLogLevel==LOG_NULL )	return;

			boost::gregorian::date today = boost::gregorian::day_clock::local_day();
			int year = today.year();
			int month= today.month();
			int day  = today.day();

			bool			bMonthChange = false;

			if( month!=m_nMonth )
			{
				CreateMonthDir();
				bMonthChange = true;
			}

			if( !m_bDirCreated ) return;

			if( !m_fp.is_open() || m_nDate!=day || bMonthChange )
			{
				if( m_fp.is_open() )
				{
					m_fp.close();
				}

				char file[255];
				if ( m_sFilePrefix.length()>0 )
				{
					sprintf( file, "%s%s_%02d-%02d-%02d.log", m_sLogDir.c_str(), m_sFilePrefix.c_str(), year, month, day );
				}
				else
				{
					sprintf( file, "%s%02d-%02d-%02d.log", m_sLogDir.c_str(), year, month, day );
				}
				
				m_fp.open( file, std::ios::out|std::ios::app);
				if ( !m_fp.is_open() )	return;
				m_nDate = day;
			}

			boost::posix_time::ptime pt = boost::posix_time::second_clock::local_time();
			m_fp<<pt.time_of_day()<<g_logDesc[level]<<str<<std::endl;

#ifdef STDCOUT
			std::cout<<pt.time_of_day()<<g_logDesc[level]<<str<<std::endl;
#endif
		}

		void Log_Hex_2File( int level, const char * hex, size_t len )
		{
			char buffer[4096];

			sprintf( buffer, "(0x)" );

			size_t head = strlen( buffer );
			size_t pos = 0;
			while ( pos < len )
			{
				sprintf( buffer + (pos<<2) + head, "  %02X", (unsigned char)hex[pos] );
				if ( ((++pos)<<2) + head >= 4096 )
					break;
			}

			Log_Text_2File(level, buffer);
		}

	public:
		volatile bool			m_bruning;
		boost::shared_ptr<boost::thread> m_thread;
		boost::mutex			m_mutex;
		boost::condition		m_condition;

		std::deque<CLogPacket*>	m_packets;

		std::string			m_sLogRootDir;
		std::string			m_sLogDir;
		std::string			m_sFilePrefix;
		bool				m_bDirCreated;
		int					m_nLogLevel;
		std::fstream		m_fp;
		int					m_nDate;
		int					m_nMonth;

		static CLogThread * getinstance()
		{
			static CLogThread logthread;
			return &logthread;
		}

	private:
		CLogThread(const CLogThread &l);
		CLogThread &operator=(const CLogThread &l);
	};



	inline
	void InitLogger( const char* prefix, int level )
	{
		CLogThread::getinstance()->Init( prefix, level );
	}

	inline
	void FiniLogger( void )
	{
		CLogThread::getinstance()->Fini( );
	}

	inline
	void SetLogLevel( int level )
	{
		CLogThread::getinstance()->SetLogLevel( level );
	}

	inline
	void Log_Text( int level, const char * str )
	{
		CLogThread::getinstance()->LogText( level, str ); 
	}

	inline
	void Log_Text_Format( int level, char* szstr, ... )
	{
		if( !CLogThread::getinstance()->m_bruning ) return;
		if( level<CLogThread::getinstance()->m_nLogLevel )				return;
		if( CLogThread::getinstance()->m_nLogLevel==LOG_NULL )	return;

		CLogThread::CLogPacket* packet = new CLogThread::CLogPacket( );

		packet->level = level;
		packet->type  = 0;

		va_list args;
		va_start(args, szstr);
		packet->len = vsnprintf(packet->log, sizeof(packet->log)-1, szstr, args);
		va_end(args);

		CLogThread::getinstance()->PostLog( packet );
	}

	inline
	void Log_Hex( int level, const char * hex, size_t len )
	{
		CLogThread::getinstance()->LogHex( level, hex, len );
	}

}
