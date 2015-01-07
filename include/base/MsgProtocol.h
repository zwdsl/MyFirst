

#pragma once


namespace Communications
{
	// 
	// 2字节长度+4个字节xyid+msg
	//
	// 发送消息 encode_body->encode_header
	// 接受消息 decode_header->decode_body

	class CMessageProtocol_2_4
	{
	public:
		enum 
		{
			HEADLEN			= 6,
			MAXBODYLEN		= 512
		};
	public:
		CMessageProtocol_2_4( void )
			: m_socketid( 0 ), m_body_length( 0 ), m_msg_id( 0 )
		{
			memset( m_data, 0, sizeof(m_data) );
			memset( m_hostname, 0, sizeof(m_hostname) );
			memset( m_servicename, 0, sizeof(m_servicename) );
		}

		CMessageProtocol_2_4( const CMessageProtocol_2_4& message )
		{
			m_socketid	  = message.m_socketid;
			m_body_length = message.m_body_length;
			m_msg_id	  = message.m_msg_id;
			memcpy( m_data, message.data(), message.length() );

			strncpy( m_hostname, message.m_hostname, sizeof(m_hostname)-1 );
			strncpy( m_servicename, message.m_servicename, sizeof(m_servicename)-1 );
		}

		const char* data( void ) const
		{
			return m_data;
		}
		char* data( void )
		{
			return m_data;
		}

		unsigned int size( void ) const
		{
			return HEADLEN+MAXBODYLEN;
		}

		unsigned int length( void ) const
		{
			return HEADLEN+m_body_length;
		}

		unsigned short head_length( void ) const
		{
			return HEADLEN;
		}

		const char* body( void ) const
		{
			return m_data+HEADLEN;
		}
		char* body( void )
		{
			return m_data+HEADLEN;
		}

		unsigned short body_length( void ) const
		{
			return m_body_length;
		}

		unsigned int msg_id( void ) const
		{
			return m_msg_id;
		}

		void body_length( unsigned short length )
		{
			m_body_length = length;
		}

		void msg_id( unsigned int msg_id )
		{
			m_msg_id = msg_id;
		}

		bool decode_header( void )
		{
			char temp[HEADLEN+1];

			memset( temp, 0, sizeof(temp) );
			memcpy( temp, m_data, sizeof(unsigned short) );
			m_body_length = atoi( temp );

			memset( temp, 0, sizeof(temp) );
			memcpy( temp, m_data+sizeof(unsigned short), sizeof(unsigned int) );
			m_msg_id = atoi( temp );

			if ( m_body_length>MAXBODYLEN )
			{
				m_body_length = 0;
				return false;
			}

			return true;
		}

		bool encode_header( void )
		{
			char temp[HEADLEN+1] = { 0 };

			if ( m_body_length>MAXBODYLEN ) return false;

			sprintf(temp, "%2u%4u", m_body_length, m_msg_id);
			memcpy(m_data, temp, HEADLEN);

			return true;
		}

		void decode_body( void )
		{
			// 可以做解密、解压缩等事情
		}

		void encode_body( void )
		{
			// 可以做加密、压缩body部分等事情
		}

		// for udp
		const char* hostname( void )
		{
			return m_hostname;
		}

		const char* servicename( void )
		{
			return m_servicename;
		}

		void hostname( const char* hostname )
		{
			strncpy( m_hostname, hostname, sizeof(m_hostname)-1 );
		}

		void servicename( const char* servicename )
		{
			strncpy( m_servicename, servicename, sizeof(m_servicename)-1 );
		}

		void socketid( int socketid )
		{
			m_socketid = socketid;
		}

		int socketid( void ) const 
		{
			return m_socketid;
		}

	private:
		char	m_hostname[16];		// 255.255.255.255
		char	m_servicename[16];	// 

	private:
		int				m_socketid;
		char			m_data[HEADLEN+MAXBODYLEN];
		unsigned short	m_body_length;
		unsigned int	m_msg_id;

		CMessageProtocol_2_4& operator=( const CMessageProtocol_2_4& );
	};
}


