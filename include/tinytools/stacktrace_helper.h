
#pragma once

#include <string>
#include <include/base/Base.h>
//_MSC_VER
#if defined _WINDOWS || defined WIN32

	#ifndef snprintf
	#define snprintf _snprintf
	#endif

	#define LOCAL_THREAD_VAR	__declspec(thread)
	typedef DWORD				__THREADID;
	#define GET_THREADID()		GetCurrentThreadId()

	#define __STACK_TRACE_IN__(level)	CStackTrace __##trace_##__LINE__(__FILE__, __LINE__, __FUNCTION__, level)
#else
	#define LOCAL_THREAD_VAR	__thread
	typedef pthread_t			__THREADID;
	#define GET_THREADID()		pthread_self()

	#define __STACK_TRACE_IN__(level)	CStackTrace __##trace_##__LINE__(__FILE__, __LINE__, __PRETTY_FUNCTION__, level)
#endif

static LOCAL_THREAD_VAR int			_ltv_indent		= 0;
static LOCAL_THREAD_VAR __THREADID	_ltv_threadid	= 0;

class CStackTrace
{
	const char* m_file;
	size_t		m_line;
	const char* m_func;
	int			m_level;
public:
	CStackTrace( const char* file, unsigned int line, const char* func, int level )
		: m_file( file ), m_line( line ), m_func( func ), m_level( level )
	{
		printstack( _ltv_indent, "->", m_file, m_line, m_func );
		_ltv_indent++;
	}
	~CStackTrace( void )
	{
		_ltv_indent--;
		printstack( _ltv_indent, "<-", m_file, m_line, m_func );
	}

private:
	void printstack( int indent, const char* action, const char* file, size_t line, const char* func )
	{
		char	buffer[1024];
		char*	p = buffer;
		size_t	remain = sizeof(buffer);

		while ( indent-- > 0 )
		{
			int n	= snprintf(p, remain, "  ");
			p		+= n;
			remain	-= n;
		}

//#if defined _WINDOWS || defined WIN32
//		snprintf(p, remain, "[%#x] %s %s(%s:%#u)", (unsigned int)gettheadid(), action, func, file, line);
//#else
//		snprintf(p, remain, "[%zx] %s %s(%s:%zu)", (unsigned int)gettheadid(), action, func, file, line);
//#endif
#if defined _WINDOWS || defined WIN32
		snprintf(p, remain, "[%#x] %s %s", (unsigned int)gettheadid(), action, func);
#else
		snprintf(p, remain, "[%zx] %s %s", (unsigned int)gettheadid(), action, func);
#endif
		
		Communications::Log_Text( m_level, buffer );
	}

	__THREADID gettheadid( void )
	{
		if ( _ltv_threadid==0 )
		{
			_ltv_threadid = GET_THREADID();
		}
		return _ltv_threadid;
	}
};



