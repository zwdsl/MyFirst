
#pragma once

#include "Socket.h"

namespace Communications
{
	// 对象必须用boost::shared_ptr封装成指针
	template< class MessageProtocol = CMessageProtocol_2_4, 
		template <class MessageProtocol>class MessageProtocolPool=udp_socket_message_pool >
	class CUdpSocket : public boost::enable_shared_from_this< CUdpSocket<MessageProtocol, MessageProtocolPool> >, public boost::noncopyable
	{
	public:
		typedef CUdpSocket<MessageProtocol, MessageProtocolPool>					ImplSocket;
		typedef MessageProtocol								Message;
		typedef typename boost::shared_ptr<Message>			MessagePtr;
		typedef MessageProtocolPool<Message>				MessagePool;
		typedef typename std::deque<MessagePtr>				MessageQueue;

		// socket接收消息的通知
		typedef boost::function< void ( int, MessagePtr ) >	udp_message_callback;

		// 一般不会对 CUdpSocket 重用，所以将一些参数放置在构造函数
		CUdpSocket( MessagePool* message_pool, boost::asio::io_service* io, 
			int socketid,
			udp_message_callback onmessage, 
			const std::string& ip="0.0.0.0", unsigned short port=0, bool buse_tcpv6=false )
			: m_io( io )
			, m_resolver( *m_io )
			, m_buse_tcpv6( buse_tcpv6 )
			, m_closed( true )
			, m_message_pool( message_pool )
			, m_socket( *m_io, udp::endpoint( 
			(buse_tcpv6?
			(boost::asio::ip::address)(boost::asio::ip::address_v6::from_string(ip))
			:(boost::asio::ip::address)(boost::asio::ip::address_v4::from_string(ip))
			), port ) )
			, m_socketid(socketid) 
			, m_onmessage( onmessage )
		{

		}

		~CUdpSocket( void )
		{
			m_onmessage = 0;
		}

		bool Open( void )
		{
			if ( !m_closed )	return false;
			m_io->post( boost::bind( &ImplSocket::do_open, this->shared_from_this() ) );

			return true;
		}

		// 在外部设置好 hostname、servicename
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
			m_io->post( boost::bind( &ImplSocket::do_close, this->shared_from_this() ) );

			return true;
		}

	private:
		void do_open( void )
		{
			if ( m_closed )
			{
				m_closed = false;
				m_writemsgs.clear();

				m_socket.async_receive_from( boost::asio::buffer(m_readmsg.body(), m_readmsg.body_size()),
					m_endpoint, boost::bind( &ImplSocket::handle_recv, this->shared_from_this(),
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );
			}
		}

		void do_send( MessagePtr msg )
		{
			msg->encode_body( );
			//if( !msg->encode_header( ) )
			//{
			//	LOGE( "udp client socket do_write err" );
			//	return ;
			//}

			bool write_in_progress = !m_writemsgs.empty();
			m_writemsgs.push_back(msg);
			if (!write_in_progress)
			{
				boost::system::error_code resolve_err;
				udp::resolver::query	query( m_buse_tcpv6?udp::v6():udp::v4(), m_writemsgs.front()->hostname(), m_writemsgs.front()->servicename() );
				udp::resolver::iterator iterator = m_resolver.resolve( query, resolve_err );

				if ( !resolve_err )
				{
					udp::endpoint endpoint = *iterator;

					m_socket.async_send_to( boost::asio::buffer(m_writemsgs.front()->body(),
						m_writemsgs.front()->body_length() ), endpoint, boost::bind(&ImplSocket::handle_send, this->shared_from_this(),
						boost::asio::placeholders::error) );
				}
				else
				{
					m_writemsgs.pop_front();
				}
			}
		}

		void do_close( void )
		{
			if ( !m_closed )
			{
				m_closed = true;
				m_socket.close( );
			}
		}

		void handle_send( const boost::system::error_code& error )
		{
			// 发送不过去不再重发
			if ( error )
			{
				LOGE( "udp client socket handle_send err" );
			}

			do 
			{
				m_writemsgs.pop_front();
				if ( !m_writemsgs.empty() )
				{
					boost::system::error_code resolve_err;
					udp::resolver::query	query( m_buse_tcpv6?udp::v6():udp::v4(), m_writemsgs.front()->hostname(), m_writemsgs.front()->servicename() );
					udp::resolver::iterator iterator = m_resolver.resolve( query, resolve_err );

					if ( !resolve_err )
					{
						udp::endpoint endpoint = *iterator;

						m_socket.async_send_to( boost::asio::buffer(m_writemsgs.front()->body(),
							m_writemsgs.front()->body_length() ), endpoint, boost::bind(&ImplSocket::handle_send, this->shared_from_this(),
							boost::asio::placeholders::error) );

						break;
					}
				}
				else
				{
					break;
				}
			} while ( 1 );
		}

		void handle_recv( const boost::system::error_code& error, size_t bytes_transferred )
		{
			if ( !error /*&& m_readmsg.decode_header()*/ )
			{
				bool messageok = true;
				try
				{
					char servicename[16];
					sprintf( servicename, "%d", m_endpoint.port() );

					m_readmsg.socketid( m_socketid );
					m_readmsg.hostname( m_endpoint.address().to_string().c_str() );
					m_readmsg.servicename( servicename );
					m_readmsg.body_length( (unsigned short)bytes_transferred );
					m_readmsg.decode_body( );
				}
				catch( ... )
				{
					messageok = false;
				}

				if ( messageok )
				{
					MessagePtr newmsg( m_message_pool->New_Object( m_readmsg ), boost::bind( &MessagePool::Delete_Object, m_message_pool, _1 ) );
					m_onmessage( m_socketid, newmsg );
				}
			}
			else
			{
				;//LOGE( "udp client socket handle_recv err " );
			}

			if ( !m_closed )
			{
				m_socket.async_receive_from( boost::asio::buffer(m_readmsg.body(), m_readmsg.body_size()),
					m_endpoint, boost::bind( &ImplSocket::handle_recv, this->shared_from_this(),
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );
			}
		}

	private:
		boost::asio::io_service*	m_io;
		udp::resolver				m_resolver;
		bool						m_buse_tcpv6;
		volatile bool				m_closed;
		MessagePool*				m_message_pool;

		udp::socket					m_socket;
		int							m_socketid;
		udp::endpoint				m_endpoint;
		Message						m_readmsg;
		MessageQueue				m_writemsgs;

		udp_message_callback		m_onmessage;
	};
}
