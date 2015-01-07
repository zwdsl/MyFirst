
#pragma once

//#ifndef __AGBASE_LOCK_H__
//#define __AGBASE_LOCK_H__
#if defined _WINDOWS_ || defined WIN32

	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#include <windows.h>
	#include <assert.h>
#else
	#include <cassert>
	#include <cerrno>
	#include <pthread.h>
#endif

namespace Communications
{
	class ILock
	{
	public:
		virtual ~ILock( void )
		{

		}

		virtual void Lock( void )  = 0;
		virtual void Unlock( void )= 0;
	};

	class NULL_LOCK : public ILock
	{
	public:
		static ILock* Instance( void )
		{
			static NULL_LOCK nulllock;
			return &nulllock;
		}
	
		virtual void Lock( void )
		{

		}

		virtual void Unlock( void )
		{
		}
	};

	// 锁的包装类，
	// 注意：
	//		windows使用CRITICAL_SECTION，是一个可重入锁
	//		linux使用pthread_mutex_t，默认参数是一个不可重入锁
	class CLock : public ILock
	{
	public:
		explicit CLock( void )
		{
#if defined _WINDOWS_ || defined _WINDOWS_
			::InitializeCriticalSection(&m_crit);
#else
			int ret = ::pthread_mutex_init(&m_crit, 0);
			assert(ret==0);
#endif	
		}
		~CLock( void )
		{
#if defined _WINDOWS_ || defined _WINDOWS_
			::DeleteCriticalSection(&m_crit);
#else
			int ret = ::pthread_mutex_destroy(&m_crit);

			assert( ret != EBUSY );
			assert( ret == 0 );
#endif
		}

		// 加锁
		virtual void Lock( void )
		{
#if defined _WINDOWS_ || defined _WINDOWS_
			::EnterCriticalSection(&m_crit);
#else
			int rc = ::pthread_mutex_lock(&m_crit);

			assert( rc != EINVAL );
			assert( rc != EDEADLK );
			assert( rc == 0 );
#endif
		}
		// 解锁
		virtual void Unlock( void )
		{
#if defined _WINDOWS_ || defined _WINDOWS_
			::LeaveCriticalSection(&m_crit);
#else
			int rc = ::pthread_mutex_unlock(&m_crit);

			assert( rc != EINVAL );
			assert( rc != EPERM );
			assert( rc == 0 );
#endif
		}

	private:
	#if defined _WINDOWS_ || defined WIN32
		CRITICAL_SECTION	m_crit;
	#else
		pthread_mutex_t		m_crit;
	#endif

		// No copies do not implement
		CLock(const CLock &l);
		CLock &operator=(const CLock &l);
	};

	// 简化的CLock包装类，使用时直接定义即可
	class CLockGuard
	{
	public:
		explicit CLockGuard(ILock* lock) : m_lock(lock)
		{
			m_lock->Lock();
		}
		~CLockGuard( void )
		{
			m_lock->Unlock();
		}

	private :
		ILock*	m_lock;

		// No copies do not implement
		CLockGuard(const CLockGuard &l);
		CLockGuard &operator=(const CLockGuard &l);
	};
}

#define CLockGuard(x) error "Missing lock guard var name"

//#endif // __AGBASE_LOCK_H__

