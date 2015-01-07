

#pragma once

#include <time.h>

// 
// 2�ֽڳ���+4���ֽ�xyid+msg
//
// ������Ϣ encode_body->encode_header
// ������Ϣ decode_header->decode_body

class CImbaBaseProtocol
{
public:
	struct Head
	{
		unsigned int	msg_len;		// ��Ϣ���ȣ�������Ϣͷ
		unsigned int	msg_id;			// ��Ϣid
		unsigned int	msg_index;		// ��Ϣ��������ʱ����
		unsigned int	send_tm;		// ����ʱ��
	};
	enum 
	{
		HEADLEN			= sizeof(Head),
		MAXBODYLEN		= 8192-HEADLEN
	};
public:
	CImbaBaseProtocol( void )
	{
		m_socketid	  = 0;
		m_isudppacket = false;
		memset( m_hostname, 0, sizeof(m_hostname) );
		memset( m_servicename, 0, sizeof(m_servicename) );

		memset( m_data, 0, sizeof(m_data) );
		m_head = (Head*)m_data;
	}

	CImbaBaseProtocol( const CImbaBaseProtocol& message )
	{
		m_socketid	  = message.m_socketid;
		m_isudppacket = message.m_isudppacket;
		strncpy( m_hostname, message.m_hostname, sizeof(m_hostname)-1 );
		strncpy( m_servicename, message.m_servicename, sizeof(m_servicename)-1 );

		memcpy( m_data, message.data(), message.length() );
		m_head = (Head*)m_data;
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
		return m_head->msg_len;
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
		return m_head->msg_len-HEADLEN;
	}

	unsigned short body_size( void ) const
	{
		return MAXBODYLEN;
	}

	unsigned int msg_id( void ) const
	{
		return m_head->msg_id;
	}

	void body_length( unsigned short length )
	{
		m_head->msg_len = length+HEADLEN;
	}

	void msg_id( unsigned int msg_id )
	{
		m_head->msg_id = msg_id;
	}

	bool decode_header( void )
	{
		if ( m_head->msg_len>MAXBODYLEN+HEADLEN )
		{
			m_head->msg_len = MAXBODYLEN;
			return false;
		}

		return true;
	}

	bool encode_header( void )
	{
		if ( m_head->msg_len>MAXBODYLEN+HEADLEN ) return false;
		if ( m_head->msg_id == 0 ) return false;

		m_head->send_tm = (unsigned int)time(NULL);

		return true;
	}

	void decode_body( void )
	{
		// ���������ܡ���ѹ��������
	}

	void encode_body( void )
	{
		// ���������ܡ�ѹ��body���ֵ�����
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

	bool isudppacket( void )
	{
		return m_isudppacket;
	}

	void hostname( const char* hostname )
	{
		m_isudppacket = true;
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
	int		m_socketid;
	bool	m_isudppacket;

	// for udp
	char	m_hostname[16];		// 255.255.255.255
	char	m_servicename[16];	// 

	// udp ��ʹ��ͷ
	char			m_data[HEADLEN+MAXBODYLEN];
	Head*			m_head;

	CImbaBaseProtocol& operator=( const CImbaBaseProtocol& );
};

