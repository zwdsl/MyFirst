#pragma once

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <queue>
#include <vector>

#include "MsgProtocol.h"

#include "Log.h"

//#define __USE_BASE_OBJECTPOOL

#ifdef __USE_BASE_OBJECTPOOL

// 如果是线程安全，需要调用SetThreadSefeParams
#include "ObjectPool.h"
#define __OBJECT__POOL	CObjectPool

#else

#include "MemoryPool.h"
#define __OBJECT__POOL	CMemoryPool_Private

#endif

namespace Communications
{
	using boost::asio::ip::tcp;

	typedef	boost::shared_ptr<tcp::socket>		tcp_socket_ptr;
	typedef __OBJECT__POOL<tcp::socket>			tcp_socket_pool;
	
	template< class T >
	class tcp_socket_message_pool : public __OBJECT__POOL< T >
	{

	};

	using boost::asio::ip::udp;

	template< class T >
	class udp_socket_message_pool : public __OBJECT__POOL< T >
	{

	};

	const boost::system::error_code				socket_no_error;
	// 以下代码一个 asio::io_service 只允许在一个线程中 运行
}
