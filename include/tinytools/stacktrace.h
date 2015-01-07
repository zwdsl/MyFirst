
#pragma once


#ifdef __USE_TRACE__
	#define __STACK_TRACE__()				__STACK_TRACE_IN__(0)
	#define __STACK_TRACE_LEVEL__(level)	__STACK_TRACE_IN__(level)
	#include "stacktrace_helper.h"
#else
	#define __STACK_TRACE__()				( (void)0 )
	#define __STACK_TRACE_LEVEL__(level)	( (void)0 )
#endif

