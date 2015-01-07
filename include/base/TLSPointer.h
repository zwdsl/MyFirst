
#pragma once


#if defined _WINDOWS_ || defined WIN32
#include <process.h>
#define THREAD_LOCAL_STORAGE __declspec(thread)
#else
#include <pthread.h>
#define THREAD_LOCAL_STORAGE __thread
#endif

namespace Communications
{
	template<class T>
	class CTlsPointer
	{
	public:
#if defined _WINDOWS_ || defined WIN32
		CTlsPointer( void )
		{
			m_key = ::TlsAlloc();
		}

		~CTlsPointer( void )
		{
			::TlsFree( m_key );
		}

		void Set( const T* plcass )
		{
			::TlsSetValue( m_key, (LPVOID)plcass );
		}


		T* Get( void )
		{
			return (T*)::TlsGetValue( m_key );
		}
	private:
		DWORD			m_key;
#else
		CTlsPointer( void )
		{
			pthread_key_create( &m_key, CTlsPointer<T>::deltls );
		}

		~CTlsPointer( void )
		{

		}

		void Set( const T* plcass )
		{
			pthread_setspecific( m_key, (const void*)plcass );
		}

		T* Get( void )
		{
			return (T*)pthread_getspecific( m_key );
		}

		static void deltls( void* p )
		{

		}
	private:
		pthread_key_t	m_key;
#endif

	public:
		CTlsPointer& operator=( const T* pclass )
		{
			Set( pclass );
			return *this;
		}

		bool operator == ( const T* pclass )
		{
			return Get()==pclass;
		}

		bool operator !( void )
		{
			return Get()==0;
		}

		T* operator ->( void )
		{
			return Get();
		}

		T& operator *( void )
		{
			return *Get();
		}

		operator T*( void )
		{
			return Get();
		}
	};
}


