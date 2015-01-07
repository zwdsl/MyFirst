#pragma once

#include "Socket.h"

namespace Communications
{

	// 对象必须用boost::shared_ptr封装成指针
	template< class MessageProtocol = CMessageProtocol_2_4, 
		template <class MessageProtocol>class MessageProtocolPool=tcp_socket_message_pool>
	class CTcpSocketServer : public boost::enable_shared_from_this< CTcpSocketServer<MessageProtocol, MessageProtocolPool> >, public boost::noncopyable
	{
	public:
		typedef CTcpSocketServer<MessageProtocol, MessageProtocolPool>			ImplSocket;
		typedef typename boost::shared_ptr<ImplSocket>		ImplSocketPtr;
		typedef MessageProtocol								Message;
		typedef typename boost::shared_ptr<Message>			MessagePtr;
		typedef MessageProtocolPool<Message>				MessagePool;
		typedef typename std::deque<MessagePtr>				MessageQueue;

		// socket连接上来的通知
		typedef boost::function< void ( int ) >				tcp_connect_callback;
		// socket关闭通知，网络io不再使用本socket对象
		typedef boost::function< void ( int ) >				tcp_close_callback;
		// socket接收消息的通知
		typedef boost::function< void ( int, MessagePtr ) >	tcp_message_callback;
		// socket 定时操作，比如心跳等
		typedef boost::function< void ( int ) >				tcp_timeout_callback;


		CTcpSocketServer( MessagePool* message_pool,
			tcp_socket_ptr socket,
			int	socketid,
			tcp_connect_callback onconnect,
			tcp_close_callback	 onclose,
			tcp_message_callback onmessage )
			: m_io( &socket->get_io_service() )
			, m_closed( true )
			, m_message_pool( message_pool )
			, m_socket( socket )
			, m_socketid( socketid )
			, m_disouttime( 0 )
			, m_distimer( *m_io, boost::posix_time::seconds(60) )
			, m_onconnect( onconnect )
			, m_onclose( onclose )
			, m_onmessage( onmessage )
		{
			boost::system::error_code error;
			boost::asio::ip::tcp::no_delay option(true);
			m_socket->set_option(option, error);
		}

		~CTcpSocketServer( void )
		{
			m_disouttime = 0;
			m_onoutime = 0;
			m_onconnect = 0;
			m_onclose = 0;
			m_onmessage = 0;
			m_socket.reset( );
		}

		// MessagePool 放在这里可以让每个线程都可以指定 消息池，且不影响CTcpSocketServer重用
		bool BeConnected( void )
		{
			if ( m_closed )
			{
				m_io->post( boost::bind( &ImplSocket::do_beconnected, this->shared_from_this() ) );

				return true;
			}
			
			return false;
		}
		bool BeConnectedDirect( void )
		{
			do_beconnected();
			return true;
		}
		void SetTimeOutCallback( tcp_timeout_callback timeout_callback, long distime )
		{
			m_io->post( boost::bind( &ImplSocket::do_settimeout, this->shared_from_this(), timeout_callback, distime) );
		}

		bool Send( const MessagePtr& msg )
		{
			if ( m_closed )		return false;

			m_io->post( boost::bind( &ImplSocket::do_send, this->shared_from_this(), msg ) );
			return true;
		}

		bool Send( const Message& msg )
		{
			if ( m_closed )		return false;

			MessagePtr msgptr( m_message_pool->New_Object( msg ), boost::bind( &MessagePool::Delete_Object, m_message_pool, _1 ) );
			m_io->post( boost::bind( &ImplSocket::do_send, this->shared_from_this(), msgptr ) );
			return true;
		}

		bool SendDirect( const MessagePtr& msg )
		{
			if ( m_closed )		return false;

			do_send(msg);
			return true;
		}

		bool SendDirect( const Message& msg )
		{
			if ( m_closed )		return false;

			MessagePtr msgptr( m_message_pool->New_Object( msg ), boost::bind( &MessagePool::Delete_Object, m_message_pool, _1 ) );
			do_send(msgptr);
			return true;
		}

		bool Close( void )
		{
			if ( m_closed )		return false;
			m_io->post( boost::bind( &ImplSocket::do_close, this->shared_from_this(), 0 ) );
			return true;
		}

		std::string GetPeerStringIP( void )
		{
			try
			{
				if ( m_closed )		return "";
				return m_socket->remote_endpoint( ).address().to_string( );
			}
			catch( ... )
			{
				return "";
			}
		}

		unsigned long GetPeerLongIP( void )
		{
			try
			{
				if ( m_closed )		return 0;

				if ( m_socket->remote_endpoint( ).address().is_v4() )
					return m_socket->remote_endpoint( ).address().to_v4().to_ulong( );

				if ( m_socket->remote_endpoint( ).address().is_v6() )
					return 0; 
			}
			catch (...)
			{
				return 0;
			}
		}

		std::string GetLocalStringIP( void )
		{
			try
			{
				if ( m_closed )		return "";
				return m_socket->local_endpoint().address().to_string( );
			}
			catch( ... )
			{
				return "";
			}
		}

		unsigned long GetLocalLongIP( void )
		{
			try
			{
				if ( m_closed )		return 0;

				if ( m_socket->local_endpoint( ).address().is_v4() )
					return m_socket->local_endpoint( ).address().to_v4().to_ulong( );

				if ( m_socket->local_endpoint( ).address().is_v6() )
					return 0; 
			}
			catch (...)
			{
				return 0;
			}
		}

		unsigned short GetPort( void )
		{
			try 
			{
				if ( m_closed )		return 0;
				return m_socket->remote_endpoint( ).port();
			}
			catch( ... )
			{
				return 0;
			}
		}

	private:
		void do_beconnected( void )
		{
			if ( m_closed )
			{
				m_closed = false;
				m_writemsgs.clear();
				m_onconnect( m_socketid );

				boost::asio::async_read( *m_socket,
					boost::asio::buffer(m_readmsg.data(), m_readmsg.head_length()),
					boost::bind( &ImplSocket::handle_recv_head, this->shared_from_this(),
					boost::asio::placeholders::error) );
			}
		}

		void do_settimeout( tcp_timeout_callback timeout_callback, long distime )
		{
			m_onoutime	= timeout_callback; // close old timer
			m_disouttime= distime;
			m_distimer.cancel();

			if ( m_onoutime && m_disouttime>0 )
			{
				m_distimer.expires_from_now( boost::posix_time::seconds(m_disouttime) );
				m_distimer.async_wait( boost::bind( &ImplSocket::handle_timeout, this->shared_from_this() ,_1) );
			}
		}

		void do_send( MessagePtr msg )
		{
			msg->encode_body( );
			if( !msg->encode_header( ) )
			{
				LOGW( "server socket encode_header err" );
				return ;
			}

			bool write_in_progress = !m_writemsgs.empty();
			m_writemsgs.push_back(msg);
			if (!write_in_progress)
			{
				boost::asio::async_write(*m_socket,
					boost::asio::buffer(m_writemsgs.front()->data(),
					m_writemsgs.front()->length() ),
					boost::bind(&ImplSocket::handle_send, this->shared_from_this(),
					boost::asio::placeholders::error));
			}
		}

		void do_close( int errid )
		{
			if ( !m_closed )
			{
				m_disouttime = 0;
				m_distimer.cancel();

				m_closed = true;
				m_socket->close( );

				m_onclose( m_socketid );

				m_onconnect = 0;
				m_onmessage = 0;
				m_onoutime  = 0;
				m_onclose   = 0;
			}
		}

		void handle_send( const boost::system::error_code& error )
		{
			if (!error)
			{
				m_writemsgs.pop_front();
				if ( !m_writemsgs.empty() )
				{
					boost::asio::async_write(*m_socket,
						boost::asio::buffer(m_writemsgs.front()->data(),
						m_writemsgs.front()->length()),
						boost::bind( &ImplSocket::handle_send, this->shared_from_this(),
						boost::asio::placeholders::error));
				}
			}
			else
			{
				do_close( error.value() );
			}
		}

		void handle_recv_head( const boost::system::error_code& error )
		{
			if (!error && m_readmsg.decode_header())
			{
				m_readmsg.socketid( m_socketid );
				if ( m_readmsg.body_length()==0 )
				{
					if ( !m_closed )
					{
						MessagePtr newmsg( m_message_pool->New_Object( m_readmsg ), boost::bind( &MessagePool::Delete_Object, m_message_pool, _1 ) );

						m_onmessage( m_socketid, newmsg );
					}
					else
					{
						LOGW( "server socket closed, but recv" );
					}

					boost::asio::async_read(*m_socket,
						boost::asio::buffer(m_readmsg.data(), m_readmsg.head_length()),
						boost::bind( &ImplSocket::handle_recv_head, this->shared_from_this(),
						boost::asio::placeholders::error));
				}
				else
				{
					boost::asio::async_read(*m_socket,
						boost::asio::buffer(m_readmsg.body(), m_readmsg.body_length()),
						boost::bind( &ImplSocket::handle_recv_body, this->shared_from_this(),
						boost::asio::placeholders::error));
				}
			}
			else
			{
				do_close( error.value() );
			}
		}

		void handle_recv_body( const boost::system::error_code& error )
		{
			if (!error)
			{
				m_readmsg.decode_body( );

				if ( !m_closed )
				{
					MessagePtr newmsg( m_message_pool->New_Object( m_readmsg ), boost::bind( &MessagePool::Delete_Object, m_message_pool, _1 ) );

					m_onmessage( m_socketid, newmsg );
				}
				else
				{
					LOGW( "server socket closed, but recv 2" );
				}

				boost::asio::async_read(*m_socket,
					boost::asio::buffer(m_readmsg.data(), m_readmsg.head_length()),
					boost::bind( &ImplSocket::handle_recv_head, this->shared_from_this(),
					boost::asio::placeholders::error));
			}
			else
			{
				do_close( error.value() );
			}
		}

		void handle_timeout( const boost::system::error_code& e )
		{
			if ( e ) return;

			if ( m_onoutime )
			{
				m_onoutime( m_socketid );

				if ( m_disouttime>0 )
				{
					m_distimer.expires_from_now( boost::posix_time::seconds(m_disouttime) );
					m_distimer.async_wait( boost::bind( &ImplSocket::handle_timeout, this->shared_from_this(), _1) );
				}
			}
		}

	private:
		boost::asio::io_service*	m_io;
		volatile bool				m_closed;
		MessagePool*				m_message_pool;

		tcp_socket_ptr				m_socket;
		int							m_socketid;
		Message						m_readmsg;
		MessageQueue				m_writemsgs;

		// for timeout
		long						m_disouttime;
		tcp_timeout_callback		m_onoutime;
		boost::asio::deadline_timer m_distimer;

		tcp_connect_callback		m_onconnect;
		tcp_close_callback			m_onclose;
		tcp_message_callback		m_onmessage;
	};

}

