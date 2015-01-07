
#pragma once

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#if defined _WINDOWS_ || defined _WINDOWS_
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
#else
	typedef char			CHAR;
	typedef unsigned char	BYTE;
	typedef short			SHORT;
	typedef unsigned short	WORD;
	typedef	int				INT;
	typedef unsigned int	UINT;
	typedef long			LONG;
	typedef unsigned long	DWORD;
	typedef long long		LONGLONG;
	typedef unsigned long long ULONGLONG;
#endif


#define MAX_CHAR			(127)
#define MIN_CHAR			(-128)

#define MAX_BYTE			(255U)
#define MIN_BYTE			(0)

#define MAX_SHORT			(32767U)
#define MIN_SHORT			(-32768)

#define MAX_WORD			(65535U)
#define MIN_WORD			(0)

#define MAX_INT				(2147483647)
#define MIN_INT				(-2147483648)

#define MAX_UINT			(4294967295U)
#define MIN_UINT			(0)

#define MAX_LONG			(2147483647)
#define MIN_LONG			(-2147483648)

#define MAX_DWORD			(4294967295U)
#define MIN_DWORD			(0)

#define MAX_LONGLONG		(9223372036854775807LL)
#define MIN_LONGLONG		(-9223372036854775808LL)

#define MAX_ULONGLONG		(18446744073709551615ULL)
#define MIN_ULONGLONG		(0)
