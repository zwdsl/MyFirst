
#pragma once


#if defined _WINDOWS_ || defined WIN32
	#include <windows.h>
	typedef HINSTANCE	h_agdll;

#else
	#include <dlfcn.h>
	typedef void*		h_agdll;

#endif


class CAGDll
{
public:
	CAGDll( void ) : m_dll( 0 ) { }
	~CAGDll( void )
	{
		if ( m_dll  )
		{
#if defined _WINDOWS_ || defined WIN32
			::FreeLibrary( m_dll );
#else
			::dlclose( m_dll );
#endif
		}
	}

	bool Open( const char* dllfile )
	{
		if( 0!=m_dll )	return false;
		if( 0==dllfile)	return false;

#if defined _WINDOWS_ || defined WIN32
		m_dll = ::LoadLibraryA( dllfile );
#else
		m_dll = ::dlopen(dllfile, RTLD_NOW);
#endif
		return (m_dll!=0);
	}

	template<class Fun>
	bool Function( const char* infunname, Fun& outfun )
	{
		if( 0==m_dll )		return false;
		if( 0==infunname)	return false;

#if defined _WINDOWS_ || defined WIN32
		outfun = (Fun)::GetProcAddress( m_dll, infunname );
#else
		outfun = (Fun)::dlsym( m_dll, infunname );
#endif
		return (outfun!=0);
	}

	const char* GetError( void )
	{
#if defined _WINDOWS_ || defined WIN32
		static char temperror[32];
		sprintf(temperror, "%d", GetLastError());
		return temperror;
#else
		return ::dlerror();
#endif
	}

private:
	h_agdll		m_dll;

private:
	CAGDll(const CAGDll& L);
	CAGDll& operator=(const CAGDll& L);
};

