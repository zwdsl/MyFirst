
#pragma once

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <deque>

namespace Communications
{

template<class T>
class CBlockingQueue
{
private:
	class PopQueueGuard
	{
	public:
		PopQueueGuard( std::deque<T>& queue ) : m_queue( queue )
		{
		}
		~PopQueueGuard( void )
		{
			m_queue.pop_front( );
		}

	private:
		std::deque<T>& m_queue;
	};

public:
	CBlockingQueue( void ) {}
	~CBlockingQueue( void ) { }

	void Push( const T& data )
	{
		boost::mutex::scoped_lock	lock( m_mutex );

		m_queue.push_back( data );
		m_condition.notify_one( );
	}

	T Pop( void )
	{
		boost::mutex::scoped_lock	lock( m_mutex );

		while( m_queue.empty() )
		{
			m_condition.wait( lock );
		}

		//T data( m_queue.front() );
		//m_queue.pop_front( );
		//return data;

		// 减少一次copy构造
		PopQueueGuard popqueue( m_queue );

		return T( m_queue.front() );
	}

	// 时间单位毫秒
	bool Pop( T& data, int usetime )
	{
		boost::mutex::scoped_lock	lock( m_mutex );

		boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(usetime);
		if( m_queue.empty() && !m_condition.timed_wait( lock, timeout ) )
		{
			return false;
		}

		if ( m_queue.empty() )
		{
			return false;
		}

		data = m_queue.front();
		m_queue.pop_front( );

		return true;
	}

	std::size_t Size( void ) const
	{
		boost::mutex::scoped_lock	lock( m_mutex );

		return m_queue.size( );
	}

	bool IsEmpty( void ) const
	{
		boost::mutex::scoped_lock	lock( m_mutex );

		return m_queue.empty();
	}

private:
	mutable boost::mutex m_mutex;
	boost::condition	m_condition;
	std::deque<T>		m_queue;

private:
	CBlockingQueue( const CBlockingQueue& );
	CBlockingQueue& operator=( const CBlockingQueue& );
};

}

