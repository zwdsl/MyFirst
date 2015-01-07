#pragma once

#include "Socket.h"

namespace Communications
{
	// 对象必须用boost::shared_ptr封装成指针
	class CTcpSocketListener : public boost::enable_shared_from_this<CTcpSocketListener>, public boost::noncopyable
	{
	public:								
		// 给将要accept的socket分配一个io，一般一个io绑定到一个线程上
		typedef boost::function< tcp_socket_ptr ( void*& ) >		tcp_prepare_callback;
		// accept到一个socket的通知
		typedef boost::function< void ( tcp_socket_ptr, void* ) >	tcp_accept_callback;
		// listen的socket关闭的通知，网络io不再使用本socket对象
		typedef boost::function< void ( void ) >					tcp_close_callback;

		CTcpSocketListener( boost::asio::io_service* listen_io, 
			const std::string& ip, unsigned short port, bool buse_tcpv6=false )
			: m_io( listen_io )
			, m_closed( true )
			, m_acceptor( *m_io, tcp::endpoint(
			(buse_tcpv6?
			(boost::asio::ip::address)(boost::asio::ip::address_v6::from_string(ip))
			:(boost::asio::ip::address)(boost::asio::ip::address_v4::from_string(ip))
			), port), true )
		{
			//m_acceptor.set_option( tcp::acceptor::reuse_address(true) );
			assert(m_io!=0);
		}

		~CTcpSocketListener( void )
		{
			m_onprepare = 0;
			m_onaccept = 0;
			m_onclose = 0;
		}

		bool Start( tcp_prepare_callback onprepare, tcp_accept_callback onaccept, tcp_close_callback onclose )
		{
			m_io->post( boost::bind( &CTcpSocketListener::do_accept, this->shared_from_this(), onprepare, onaccept, onclose ) );

			return true;
		}

		void Stop( void )
		{
			m_io->post( boost::bind( &CTcpSocketListener::do_close, this->shared_from_this() ) );
		}

		int GetPort( void )
		{
			try
			{
				return m_acceptor.local_endpoint( ).port( );
			}
			catch( ... )
			{
				return 0;
			}
		}

	private:
		void do_accept( tcp_prepare_callback onprepare, tcp_accept_callback onaccept, tcp_close_callback onclose  )
		{
			m_onprepare = onprepare;
			m_onaccept	= onaccept;
			m_onclose	= onclose;
			m_closed	= false;

			void*			param = 0;
			tcp_socket_ptr	new_socket( m_onprepare(param) );

			m_acceptor.async_accept( *new_socket,
				boost::bind( &CTcpSocketListener::handle_accept, this->shared_from_this( ), new_socket, param,
				boost::asio::placeholders::error) );
		}

		void do_close( void )
		{
			if ( !m_closed )
			{
				m_closed = true;
				m_acceptor.close();
				m_onclose( );

				m_onprepare = 0;
				m_onaccept = 0;
				m_onclose = 0;
			}
		}

		void handle_accept( tcp_socket_ptr socket, void* param, const boost::system::error_code& error )
		{
			if ( !error )
			{
				m_onaccept( socket, param );
			}
			else
			{
				socket->close();
				socket.reset();
				LOGW( "server listen accept err socket, error:"<<error.message() );
			}

			if ( !m_closed )
			{
				param = 0;
				tcp_socket_ptr	new_socket( m_onprepare(param) );

				m_acceptor.async_accept( *new_socket,
					boost::bind(&CTcpSocketListener::handle_accept, this->shared_from_this( ), new_socket, param,
					boost::asio::placeholders::error));
			}
		}

	private:
		boost::asio::io_service*	m_io;
		volatile bool				m_closed;
		tcp::acceptor				m_acceptor;

		tcp_prepare_callback		m_onprepare;
		tcp_accept_callback			m_onaccept;
		tcp_close_callback			m_onclose;
	};
}

