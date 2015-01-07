
#pragma once

#include <include/tinytools/stringutil.h>
#include <include/tinytools/splitstring.h>

// 从db获取配置的超时时间，单位毫秒
#define CONFIGCLIENT_GET_OVERTIME		10000
#define CONFIGCLIENT_SOCKETID			1

inline
CImbaConfig::CImbaConfig( void )
: m_status( 0 ), m_svrtype( IMBASVRTYPE_COUNT ), m_svrid( 0 ), m_genaskid( 0 )
{
	config_init( &m_local_config );
}

inline
CImbaConfig::~CImbaConfig( void )
{
	if ( m_status==1 )
	{
		if ( m_session )
		{
			m_session->Close( );
		}

		if ( m_iothread )
		{
			m_iothread->join( );
		}
	}
	config_destroy( &m_local_config );
}

inline
bool CImbaConfig::InitConfigFromDB( int svrtype, int svrid, const std::string& ip, int port )
{
	if ( svrtype<=0 || svrtype>=IMBASVRTYPE_COUNT )	return false;

	m_svrtype	= svrtype;
	m_svrid		= svrid;
	m_status	= 1;

	m_dbservername = ip + ":" + TypeToString(port);

	m_session = boost::shared_ptr<ConfigSession>( new ConfigSession( &m_messagepool, &m_io, CONFIGCLIENT_SOCKETID, 
		boost::bind( &CImbaConfig::OnSocketConnect, this, _1 ),
		boost::bind( &CImbaConfig::OnSocketClose, this, _1, _2 ),
		boost::bind( &CImbaConfig::OnSocketMessage, this, _1, _2 ) ) );
	m_session->SetTimeOutCallback( boost::bind( &CImbaConfig::OnSocketTimeOut, this, _1 ), 50 );

	m_iothread = boost::shared_ptr<boost::thread>( new boost::thread( boost::bind( &boost::asio::io_service::run, &m_io ) ) );

	{
		boost::mutex::scoped_lock	lock( m_mutex );
		m_session->Connect( ip, port );
		boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME);
		if ( !m_condition.timed_wait( lock, timeout ) )
		{
			// 服务器返回数据超时
			m_session->Close( );
			return false;
		}
	}

	m_localfilename = GetModulePath() + IMBA_SERVERNAME[svrtype] + "(" + TypeToString(svrid) + ")_Config.cfg";

	config_read_file( &m_local_config, m_localfilename.c_str() );

	return true;
}

inline
bool CImbaConfig::InitConfigFromLocal( int svrtype, int svrid )
{
	if ( svrtype<=0 || svrtype>=IMBASVRTYPE_COUNT )	return false;

	m_svrtype	= svrtype;
	m_svrid		= svrid;
	m_status	= 2;

	m_localfilename = GetModulePath() + IMBA_SERVERNAME[svrtype] + "(" + TypeToString(svrid) + ")_Config.cfg";

	config_read_file( &m_local_config, m_localfilename.c_str() );

	return true;
}

inline
int  CImbaConfig::GetSvrType( void ) const
{
	return m_svrtype;
}

inline
int CImbaConfig::GetSvrID( void ) const
{
	return m_svrid;
}

inline
const std::string& CImbaConfig::GetDBServer( void ) const
{
	return m_dbservername;
}

inline
const std::string& CImbaConfig::GetLocalFile( void ) const
{
	return m_localfilename;
}

inline
bool CImbaConfig::GetImbaUserDB( int id, ImbaUserDB& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaUserDB_FromDB(id, data) )
		{
			SetImbaUserDB_ToLocal( id, data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaUserDB_FromLocal( id, data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaGameDB( int id, ImbaGameDB& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaGameDB_FromDB(id, data) )
		{
			SetImbaGameDB_ToLocal( id, data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaGameDB_FromLocal( id, data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaLoginSvrConfig( int id, ImbaLoginSvrConfig& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaLoginSvrConfig_FromDB(id, data) )
		{
			SetImbaLoginSvrConfig_ToLocal( id, data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaLoginSvrConfig_FromLocal( id, data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaGateSvrConfig( int id, ImbaGateSvrConfig& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaGateSvrConfig_FromDB(id, data) )
		{
			SetImbaGateSvrConfig_ToLocal( id, data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaGateSvrConfig_FromLocal( id, data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaGhostSvrConfig( int id, ImbaGhostSvrConfig& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaGhostSvrConfig_FromDB(id, data) )
		{
			SetImbaGhostSvrConfig_ToLocal( id, data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaGhostSvrConfig_FromLocal( id, data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaDBSvrConfig( int id, ImbaDBSvrConfig& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaDBSvrConfig_FromDB(id, data) )
		{
			SetImbaDBSvrConfig_ToLocal( id, data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaDBSvrConfig_FromLocal( id, data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaLoginSvrConfigs( std::vector<ImbaLoginSvrConfig>& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaLoginSvrConfigs_FromDB( data ) )
		{
			SetImbaLoginSvrConfigs_ToLocal( data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaLoginSvrConfigs_FromLocal( data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaGateSvrConfigs( std::vector<ImbaGateSvrConfig>& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaGateSvrConfigs_FromDB( data ) )
		{
			SetImbaGateSvrConfigs_ToLocal( data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaGateSvrConfigs_FromLocal( data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaGhostSvrConfigs( std::vector<ImbaGhostSvrConfig>& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaGhostSvrConfigs_FromDB( data ) )
		{
			SetImbaGhostSvrConfigs_ToLocal( data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaGhostSvrConfigs_FromLocal( data );
	}

	return false;
}

inline
bool CImbaConfig::GetImbaDBSvrConfigs( std::vector<ImbaDBSvrConfig>& data )
{
	if ( m_status==1 )
	{
		if ( GetImbaDBSvrConfigs_FromDB( data ) )
		{
			SetImbaDBSvrConfigs_ToLocal( data );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetImbaDBSvrConfigs_FromLocal( data );
	}

	return false;
}

inline
bool CImbaConfig::GetConfigBySql( const std::string& key, const std::string& sql, std::vector<std::string>& result, 
					const std::string& split )
{
	if ( m_status==1 )
	{
		if ( GetConfigBySql_FromDB( sql, result, split ) )
		{
			SetConfigBySql_ToLocal( key, result, split );
			return true;
		}
	}
	else if ( m_status==2 )
	{
		return GetConfigBySql_FromLocal( key, result, split );
	}

	return false;
}





inline
void CImbaConfig::OnSocketConnect( int socketid )
{
	boost::mutex::scoped_lock	lock( m_mutex );
	m_condition.notify_one( );
}

inline
void CImbaConfig::OnSocketClose( int socket, bool bretain )
{
	boost::mutex::scoped_lock	lock( m_mutex );
	m_condition.notify_one( );
}

inline
void CImbaConfig::OnSocketMessage( int socket, boost::shared_ptr<CImbaBaseProtocol> message )
{
	boost::mutex::scoped_lock	lock( m_mutex );

	m_receivedmsg.push( message );
	m_condition.notify_one( );
}

inline
void CImbaConfig::OnSocketTimeOut( int socketid )
{
	Imba_ConfigXY::HeartBeat msg;
	SendProtocol( socketid, msg );
}


template<class T>
void CImbaConfig::SendProtocol( int socketid, const T& t )
{
	boost::shared_ptr<CImbaBaseProtocol> msgptr( m_messagepool.New_Object( ), boost::bind( &configclient_message_pool<CImbaBaseProtocol>::Delete_Object, &m_messagepool, _1 ) );

	bostream	bos;
	bos.attach(msgptr->body(), CImbaBaseProtocol::MAXBODYLEN);
	bos << t;

	msgptr->msg_id( t.get_msgid() );
	msgptr->body_length( (unsigned short)bos.length() );

	m_session->Send( msgptr );
}

inline
int CImbaConfig::GenAskID( void )
{
	return ++m_genaskid;
}

inline
bool CImbaConfig::GetImbaUserDB_FromDB( int id, ImbaUserDB& data )
{
	Imba_ConfigXY::Req_UserDB msg;
	msg.askid = GenAskID( );
	msg.userdbid = id;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME);
	if ( !m_condition.timed_wait( lock, timeout ) )
	{
		// 服务器返回数据超时
		return false;
	}

	if ( m_receivedmsg.size()==0 )
	{
		// 接收数据为空
		return false;
	}

	// 处理完 m_receivedmsg 后，清空数据
	boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
	m_receivedmsg.pop( );

	// 错误消息id
	if ( receivedmsg->msg_id( )!=Imba_ConfigXY::TCP_RESP_USERDB )
	{
		return false;
	}

	bistream bis;
	bis.attach( receivedmsg->body(), receivedmsg->body_length() );

	Imba_ConfigXY::Resp_UserDB respmsg;
	try
	{
		bis >> respmsg;
	}
	catch( ... )
	{
		return false;
	}

	// 错误askid 或者 错误的返回
	if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED )
	{
		return false;
	}

	data = respmsg.info;

	return true;
}

inline
bool CImbaConfig::GetImbaGameDB_FromDB( int id, ImbaGameDB& data )
{
	Imba_ConfigXY::Req_GameDB msg;
	msg.askid = GenAskID( );
	msg.gamedbid = id;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME);
	if ( !m_condition.timed_wait( lock, timeout ) )
	{
		// 服务器返回数据超时
		return false;
	}

	if ( m_receivedmsg.size()==0 )
	{
		// 接收数据为空
		return false;
	}

	// 处理完 m_receivedmsg 后，清空数据
	boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
	m_receivedmsg.pop( );

	// 错误消息id
	if ( receivedmsg->msg_id( )!=Imba_ConfigXY::TCP_RESP_GAMEDB )
	{
		return false;
	}

	bistream bis;
	bis.attach( receivedmsg->body(), receivedmsg->body_length() );

	Imba_ConfigXY::Resp_GameDB respmsg;
	try
	{
		bis >> respmsg;
	}
	catch( ... )
	{
		return false;
	}

	// 错误askid 或者 错误的返回
	if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED )
	{
		return false;
	}

	data = respmsg.info;

	return true;
}

inline
bool CImbaConfig::GetImbaLoginSvrConfig_FromDB( int id, ImbaLoginSvrConfig& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_LOGIN;
	msg.svrid = id;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME);
	if ( !m_condition.timed_wait( lock, timeout ) )
	{
		// 服务器返回数据超时
		return false;
	}

	if ( m_receivedmsg.size()==0 )
	{
		// 接收数据为空
		return false;
	}

	// 处理完 m_receivedmsg 后，清空数据
	boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
	m_receivedmsg.pop( );

	// 错误消息id
	if ( receivedmsg->msg_id( )!=Imba_ConfigXY::TCP_RESP_SVRCONFIG )
	{
		return false;
	}

	bistream bis;
	bis.attach( receivedmsg->body(), receivedmsg->body_length() );

	Imba_ConfigXY::Resp_SvrConfig respmsg;
	try
	{
		bis >> respmsg;
	}
	catch( ... )
	{
		return false;
	}

	// 错误askid 或者 错误的返回
	if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
	{
		return false;
	}

	data = respmsg.loginsvr_cfg;

	return true;
}

inline
bool CImbaConfig::GetImbaGateSvrConfig_FromDB( int id, ImbaGateSvrConfig& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_GATE;
	msg.svrid = id;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME);
	if ( !m_condition.timed_wait( lock, timeout ) )
	{
		// 服务器返回数据超时
		return false;
	}

	if ( m_receivedmsg.size()==0 )
	{
		// 接收数据为空
		return false;
	}

	// 处理完 m_receivedmsg 后，清空数据
	boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
	m_receivedmsg.pop( );

	// 错误消息id
	if ( receivedmsg->msg_id( )!=Imba_ConfigXY::TCP_RESP_SVRCONFIG )
	{
		return false;
	}

	bistream bis;
	bis.attach( receivedmsg->body(), receivedmsg->body_length() );

	Imba_ConfigXY::Resp_SvrConfig respmsg;
	try
	{
		bis >> respmsg;
	}
	catch( ... )
	{
		return false;
	}

	// 错误askid 或者 错误的返回
	if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
	{
		return false;
	}

	data = respmsg.gatesvr_cfg;

	return true;
}

inline
bool CImbaConfig::GetImbaGhostSvrConfig_FromDB( int id, ImbaGhostSvrConfig& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_GHOST;
	msg.svrid = id;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME);
	if ( !m_condition.timed_wait( lock, timeout ) )
	{
		// 服务器返回数据超时
		return false;
	}

	if ( m_receivedmsg.size()==0 )
	{
		// 接收数据为空
		return false;
	}

	// 处理完 m_receivedmsg 后，清空数据
	boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
	m_receivedmsg.pop( );

	// 错误消息id
	if ( receivedmsg->msg_id( )!=Imba_ConfigXY::TCP_RESP_SVRCONFIG )
	{
		return false;
	}

	bistream bis;
	bis.attach( receivedmsg->body(), receivedmsg->body_length() );

	Imba_ConfigXY::Resp_SvrConfig respmsg;
	try
	{
		bis >> respmsg;
	}
	catch( ... )
	{
		return false;
	}

	// 错误askid 或者 错误的返回
	if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
	{
		return false;
	}

	data = respmsg.ghostsvr_cfg;

	return true;
}

inline
bool CImbaConfig::GetImbaDBSvrConfig_FromDB( int id, ImbaDBSvrConfig& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_DB;
	msg.svrid = id;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME);
	if ( !m_condition.timed_wait( lock, timeout ) )
	{
		// 服务器返回数据超时
		return false;
	}

	if ( m_receivedmsg.size()==0 )
	{
		// 接收数据为空
		return false;
	}

	// 处理完 m_receivedmsg 后，清空数据
	boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
	m_receivedmsg.pop( );

	// 错误消息id
	if ( receivedmsg->msg_id( )!=Imba_ConfigXY::TCP_RESP_SVRCONFIG )
	{
		return false;
	}

	bistream bis;
	bis.attach( receivedmsg->body(), receivedmsg->body_length() );

	Imba_ConfigXY::Resp_SvrConfig respmsg;
	try
	{
		bis >> respmsg;
	}
	catch( ... )
	{
		return false;
	}

	// 错误askid 或者 错误的返回
	if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
	{
		return false;
	}

	data = respmsg.dbsvr_cfg;

	return true;
}

inline
bool CImbaConfig::GetImbaLoginSvrConfigs_FromDB( std::vector<ImbaLoginSvrConfig>& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_LOGIN;
	msg.svrid = 0;

	bool bend = false;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
	{
		LOGE( "CImbaConfig::GetImbaLoginSvrConfigs_FromDB recv msg time out 1" );
		// 服务器返回数据超时
		return false;
	}

	do 
	{
		//// 处理完 m_receivedmsg 后，清空数据
		boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
		m_receivedmsg.pop( );

		if ( receivedmsg->msg_id( )==COMMONXY_BATCH_BEGIN )
		{
			data.clear();
		}
		else if ( receivedmsg->msg_id( )==COMMONXY_BATCH_END )
		{
			bend = true;
		}
		else if ( receivedmsg->msg_id( )==Imba_ConfigXY::TCP_RESP_SVRCONFIG )
		{
			bistream bis;
			bis.attach( receivedmsg->body(), receivedmsg->body_length() );

			Imba_ConfigXY::Resp_SvrConfig respmsg;
			try
			{
				bis >> respmsg;
			}
			catch( ... )
			{
				LOGE( "CImbaConfig::GetImbaLoginSvrConfigs_FromDB explain catch err" );
				return false;
			}

			// 错误askid 或者 错误的返回
			if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
			{
				LOGE( "CImbaConfig::GetImbaLoginSvrConfigs_FromDB recv msg err askid or flag or svrtype" );
				return false;
			}

			data.push_back( respmsg.loginsvr_cfg );
		}
		else 
		{
			LOGE( "CImbaConfig::GetImbaLoginSvrConfigs_FromDB recv err msg:"<<receivedmsg->msg_id( ) );
			return false;
		}

		if ( !bend && m_receivedmsg.size()==0 )
		{
			if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
			{
				// 服务器返回数据超时
				LOGE( "CImbaConfig::GetImbaLoginSvrConfigs_FromDB recv msg time out 2" );
				return false;
			}
		}
	} while ( !bend );

	return true;
}

inline
bool CImbaConfig::GetImbaGateSvrConfigs_FromDB( std::vector<ImbaGateSvrConfig>& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_GATE;
	msg.svrid = 0;

	bool bend = false;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
	{
		// 服务器返回数据超时
		LOGE( "CImbaConfig::GetImbaGateSvrConfigs_FromDB recv msg time out 1" );
		return false;
	}

	do 
	{
		//// 处理完 m_receivedmsg 后，清空数据
		boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
		m_receivedmsg.pop( );


		// 错误消息id
		if ( receivedmsg->msg_id( )==COMMONXY_BATCH_BEGIN )
		{
			data.clear();
		}
		else if ( receivedmsg->msg_id( )==COMMONXY_BATCH_END )
		{
			bend = true;
		}
		else if ( receivedmsg->msg_id( )==Imba_ConfigXY::TCP_RESP_SVRCONFIG )
		{
			bistream bis;
			bis.attach( receivedmsg->body(), receivedmsg->body_length() );

			Imba_ConfigXY::Resp_SvrConfig respmsg;
			try
			{
				bis >> respmsg;
			}
			catch( ... )
			{
				LOGE( "CImbaConfig::GetImbaGateSvrConfigs_FromDB explain catch err" );
				return false;
			}

			// 错误askid 或者 错误的返回
			if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
			{
				LOGE( "CImbaConfig::GetImbaGateSvrConfigs_FromDB recv msg err askid or flag or svrtype" );
				return false;
			}

			data.push_back( respmsg.gatesvr_cfg );
		}
		else 
		{
			LOGE( "CImbaConfig::GetImbaGateSvrConfigs_FromDB recv err msg:"<<receivedmsg->msg_id( ) );
			return false;
		}

		if ( !bend && m_receivedmsg.size()==0 )
		{
			if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
			{
				// 服务器返回数据超时
				LOGE( "CImbaConfig::GetImbaGateSvrConfigs_FromDB recv msg time out 2" );
				return false;
			}
		}
	} while ( !bend );

	return true;
}

inline
bool CImbaConfig::GetImbaGhostSvrConfigs_FromDB( std::vector<ImbaGhostSvrConfig>& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_GHOST;
	msg.svrid = 0;

	bool bend = false;

	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
	{
		// 服务器返回数据超时
		return false;
	}

	do 
	{
		//// 处理完 m_receivedmsg 后，清空数据
		boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
		m_receivedmsg.pop( );

		if ( receivedmsg->msg_id( )==COMMONXY_BATCH_BEGIN )
		{
			data.clear();
		}
		else if ( receivedmsg->msg_id( )==COMMONXY_BATCH_END )
		{
			bend = true;
		}
		else if ( receivedmsg->msg_id( )==Imba_ConfigXY::TCP_RESP_SVRCONFIG )
		{
			bistream bis;
			bis.attach( receivedmsg->body(), receivedmsg->body_length() );

			Imba_ConfigXY::Resp_SvrConfig respmsg;
			try
			{
				bis >> respmsg;
			}
			catch( ... )
			{
				return false;
			}

			// 错误askid 或者 错误的返回
			if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
			{
				return false;
			}

			data.push_back( respmsg.ghostsvr_cfg );
		}
		else 
		{
			return false;
		}

		if ( !bend && m_receivedmsg.size()==0 )
		{
			if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
			{
				// 服务器返回数据超时
				return false;
			}
		}
	} while ( !bend );

	return true;
}

inline
bool CImbaConfig::GetImbaDBSvrConfigs_FromDB( std::vector<ImbaDBSvrConfig>& data )
{
	Imba_ConfigXY::Req_SvrConfig msg;
	msg.askid = GenAskID( );
	msg.svrtype = IMBASVRTYPE_DB;
	msg.svrid = 0;

	bool bend = false;
	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
	{
		// 服务器返回数据超时
		return false;
	}

	do 
	{
		//// 处理完 m_receivedmsg 后，清空数据
		boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
		m_receivedmsg.pop( );

		if ( receivedmsg->msg_id( )==COMMONXY_BATCH_BEGIN )
		{
			data.clear();
		}
		else if ( receivedmsg->msg_id( )==COMMONXY_BATCH_END )
		{
			bend = true;
		}
		else if ( receivedmsg->msg_id( )==Imba_ConfigXY::TCP_RESP_SVRCONFIG )
		{
			bistream bis;
			bis.attach( receivedmsg->body(), receivedmsg->body_length() );

			Imba_ConfigXY::Resp_SvrConfig respmsg;
			try
			{
				bis >> respmsg;
			}
			catch( ... )
			{
				return false;
			}

			// 错误askid 或者 错误的返回
			if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED || respmsg.svrtype!=msg.svrtype )
			{
				return false;
			}

			data.push_back( respmsg.dbsvr_cfg );
		}
		else 
		{
			return false;
		}

		if ( !bend && m_receivedmsg.size()==0 )
		{
			if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
			{
				// 服务器返回数据超时
				return false;
			}
		}
	} while ( !bend );

	return true;
}

inline
bool CImbaConfig::GetConfigBySql_FromDB( const std::string& sql, std::vector<std::string>& result, const std::string& split )
{
	Imba_ConfigXY::Req_ConfigBySql msg;
	msg.askid	= GenAskID( );
	msg.sql		= sql;
	msg.split	= split;

	bool bend = false;
	std::string data;
	boost::mutex::scoped_lock	lock( m_mutex );
	SendProtocol( CONFIGCLIENT_SOCKETID, msg );

	if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
	{
		// 服务器返回数据超时
		return false;
	}

	do 
	{
		//// 处理完 m_receivedmsg 后，清空数据
		boost::shared_ptr<CImbaBaseProtocol> receivedmsg = m_receivedmsg.front();
		m_receivedmsg.pop( );

		if ( receivedmsg->msg_id( )==COMMONXY_BATCH_BEGIN )
		{
			;
		}
		else if ( receivedmsg->msg_id( )==COMMONXY_BATCH_END )
		{
			bend = true;
		}
		else if ( receivedmsg->msg_id( )==Imba_ConfigXY::TCP_RESP_CONFIGBYSQL )
		{
			bistream bis;
			bis.attach( receivedmsg->body(), receivedmsg->body_length() );

			Imba_ConfigXY::Resp_ConfigBySql respmsg;
			try
			{
				bis >> respmsg;
			}
			catch( ... )
			{
				return false;
			}

			// 错误askid 或者 错误的返回
			if ( respmsg.askid!=msg.askid || respmsg.flag!=respmsg.SUCCEED )
			{
				return false;
			}

			data += respmsg.result;
		}
		else 
		{
			return false;
		}

		if ( !bend && m_receivedmsg.size()==0 )
		{
			if ( !m_condition.timed_wait( lock, boost::get_system_time() + boost::posix_time::milliseconds(CONFIGCLIENT_GET_OVERTIME) ) )
			{
				// 服务器返回数据超时
				return false;
			}
		}
	} while ( !bend );

	CSplitString s( data.c_str(), split.c_str() );
	int count = s.getCount( );
	result.clear();

	for ( int i=0; i<count; ++i )
	{
		result.push_back( s.get(i) );
	}

	return true;
}


static std::string get_config_key( const char* name, int id )
{
	return std::string( name )+"_"+TypeToString( id );
}

static std::string get_userdb_key( int id )
{
	return get_config_key("userdb", id);
}

static std::string get_gamedb_key( int id )
{
	return get_config_key("gamedb", id);
}

static std::string get_loginsvr_key( int id )
{
	return get_config_key(IMBA_SERVERNAME[IMBASVRTYPE_LOGIN], id);
}

static std::string get_gatesvr_key( int id )
{
	return get_config_key(IMBA_SERVERNAME[IMBASVRTYPE_GATE], id);
}

static std::string get_ghostsvr_key( int id )
{
	return get_config_key(IMBA_SERVERNAME[IMBASVRTYPE_GHOST], id);
}

static std::string get_dbsvr_key( int id )
{
	return get_config_key(IMBA_SERVERNAME[IMBASVRTYPE_DB], id);
}

#define GET_CONFIG_GROUP( key )	\
	do	\
	{	\
		rootset = config_lookup( &m_local_config, key.c_str() );	\
		if( !rootset )	return false;	\
	} while ( 0 ) 

#define GET_CONFIG_INT( key, variable ) \
	do	\
	{	\
		if ( !config_setting_lookup_int( rootset, key, &inttemp ) ) return false;	\
		variable = inttemp;	\
	} while ( 0 )

#define GET_CONFIG_STRING( key, variable ) \
	do	\
	{	\
		if ( !config_setting_lookup_string( rootset, key, &strtemp ) ) return false;	\
		variable = strtemp;	\
	} while ( 0 )

#define GET_CONFIG_ARRAY( key ) \
	do	\
	{	\
		rootset = config_lookup( &m_local_config, key );	\
		if ( rootset )	\
		{	\
			int ipcount = config_setting_length(rootset);	\
			for (int i=0; i<ipcount; i++)	\
			{	\
				config_setting_t * p = config_setting_get_elem(rootset, i);\
				ids.push_back( config_setting_get_int(p) );\
			}\
		}\
	} while ( 0 )


inline
bool CImbaConfig::GetImbaUserDB_FromLocal( int id, ImbaUserDB& data )
{
	config_setting_t* rootset = 0;
	const char* strtemp = 0;
	int			inttemp = 0;

	GET_CONFIG_GROUP( get_userdb_key( id ) );

	GET_CONFIG_INT( "id", data.id );

	GET_CONFIG_STRING( "odb_config", data.odb_config );
	GET_CONFIG_STRING( "login_user", data.login_user );
	GET_CONFIG_STRING( "login_passwd", data.login_passwd );

	GET_CONFIG_STRING( "db_ip", data.db_ip );
	GET_CONFIG_INT( "db_port", data.db_port );

	GET_CONFIG_STRING( "db_name", data.db_name );
	GET_CONFIG_STRING( "table_name", data.table_name );

	return true;
}

inline
bool CImbaConfig::GetImbaGameDB_FromLocal( int id, ImbaGameDB& data )
{
	config_setting_t* rootset = 0;
	const char* strtemp = 0;
	int			inttemp = 0;

	GET_CONFIG_GROUP( get_gamedb_key( id ) );

	GET_CONFIG_INT( "id", data.id );
	GET_CONFIG_STRING( "name", data.name );

	GET_CONFIG_INT( "supportmode", data.supportmode );
	GET_CONFIG_STRING( "config", data.config );

	GET_CONFIG_STRING( "mapversion", data.mapversion );
	GET_CONFIG_STRING( "mapdownloadurl", data.mapdownloadurl );

	GET_CONFIG_STRING( "odb_config", data.odb_config );
	GET_CONFIG_STRING( "login_user", data.login_user );
	GET_CONFIG_STRING( "login_passwd", data.login_passwd );
	GET_CONFIG_STRING( "db_ip", data.db_ip );
	GET_CONFIG_INT( "db_port", data.db_port );
	GET_CONFIG_STRING( "db_name", data.db_name );
	GET_CONFIG_STRING( "gametable_name", data.gametable_name );
	GET_CONFIG_STRING( "proptable_name", data.proptable_name );
	GET_CONFIG_STRING( "mapname", data.mapname );
	return true;
}

inline
bool CImbaConfig::GetImbaLoginSvrConfig_FromLocal( int id, ImbaLoginSvrConfig& data )
{
	config_setting_t* rootset = 0;
	const char* strtemp = 0;
	int			inttemp = 0;

	GET_CONFIG_GROUP( get_loginsvr_key( id ) );

	GET_CONFIG_INT( "id", data.id );

	GET_CONFIG_STRING( "toclient_ip", data.toclient_ip );
	GET_CONFIG_INT( "toclient_port", data.toclient_port );
	GET_CONFIG_STRING( "publish_ips", data.publish_ips );

	GET_CONFIG_INT( "togate_dns", data.togate_dns );

	GET_CONFIG_STRING( "cmdudp_ip", data.cmdudp_ip );
	GET_CONFIG_INT( "cmdudp_port", data.cmdudp_port );

	GET_CONFIG_INT( "thread_count", data.thread_count );
	GET_CONFIG_INT( "dbsvr_id", data.dbsvr_id );

	GET_CONFIG_STRING( "config", data.config );
	GET_CONFIG_STRING( "version", data.version );
	GET_CONFIG_STRING( "dir", data.dir );

	return true;
}

inline
bool CImbaConfig::GetImbaGateSvrConfig_FromLocal( int id, ImbaGateSvrConfig& data )
{
	config_setting_t* rootset = 0;
	const char* strtemp = 0;
	int			inttemp = 0;

	GET_CONFIG_GROUP( get_gatesvr_key( id ) );

	GET_CONFIG_INT( "id", data.id );

	GET_CONFIG_STRING( "toclient_ip", data.toclient_ip );
	GET_CONFIG_INT( "toclient_port", data.toclient_port );
	GET_CONFIG_STRING( "publish_ips", data.publish_ips );

	GET_CONFIG_STRING( "toghost_ip", data.toghost_ip );
	GET_CONFIG_INT( "toghost_port", data.toghost_port );
	GET_CONFIG_INT( "toghost_dns", data.toghost_dns );

	GET_CONFIG_INT( "thread_count", data.thread_count );
	GET_CONFIG_INT( "dbsvr_id", data.dbsvr_id );

	GET_CONFIG_INT( "gameid", data.gameid );
	GET_CONFIG_INT( "gamesupportmode", data.gamesupportmode );
	GET_CONFIG_STRING( "gameconfig", data.gameconfig );

	GET_CONFIG_STRING( "cmdudp_ip", data.cmdudp_ip );
	GET_CONFIG_INT( "cmdudp_port", data.cmdudp_port );

	GET_CONFIG_STRING( "version", data.version );
	GET_CONFIG_STRING( "dir", data.dir );

	return true;
}

inline
bool CImbaConfig::GetImbaGhostSvrConfig_FromLocal( int id, ImbaGhostSvrConfig& data )
{
	config_setting_t* rootset = 0;
	const char* strtemp = 0;
	int			inttemp = 0;

	GET_CONFIG_GROUP( get_ghostsvr_key( id ) );

	GET_CONFIG_INT( "id", data.id );

	GET_CONFIG_STRING( "toclient_ip", data.toclient_ip );
	GET_CONFIG_INT( "toclient_port", data.toclient_port );
	GET_CONFIG_STRING( "publish_ips", data.publish_ips );

	GET_CONFIG_INT( "thread_count", data.thread_count );
	GET_CONFIG_INT( "gatesvr_id", data.gatesvr_id );

	GET_CONFIG_STRING( "version", data.version );
	GET_CONFIG_STRING( "dir", data.dir );

	return true;
}

inline
bool CImbaConfig::GetImbaDBSvrConfig_FromLocal( int id, ImbaDBSvrConfig& data )
{
	config_setting_t* rootset = 0;
	const char* strtemp = 0;
	int			inttemp = 0;

	GET_CONFIG_GROUP( get_dbsvr_key( id ) );

	GET_CONFIG_INT( "id", data.id );

	GET_CONFIG_STRING( "listen_ip", data.listen_ip );
	GET_CONFIG_INT( "listen_port", data.listen_port );

	GET_CONFIG_INT( "thread_count", data.thread_count );
	GET_CONFIG_INT( "userdb_id", data.userdb_id );
	GET_CONFIG_STRING( "game_ids", data.game_ids );

	GET_CONFIG_STRING( "version", data.version );
	GET_CONFIG_STRING( "dir", data.dir );

	return true;
}

inline
bool CImbaConfig::GetImbaLoginSvrConfigs_FromLocal( std::vector<ImbaLoginSvrConfig>& data )
{
	//rootset = config_lookup( &m_local_config, IMBA_SERVERNAME[IMBASVRTYPE_LOGIN] );
	//if ( rootset )
	//{
	//	int ipcount = config_setting_length(rootset);
	//	for (int i=0; i<ipcount; i++)
	//	{
	//		config_setting_t * p = config_setting_get_elem(rootset, i);
	//		ids.push_back( config_setting_get_int(p) );
	//	}
	//}

	std::vector<int> ids;
	config_setting_t* rootset = 0;

	GET_CONFIG_ARRAY( IMBA_SERVERNAME[IMBASVRTYPE_LOGIN] );

	for ( std::vector<int>::iterator it=ids.begin(); it!=ids.end(); ++it )
	{
		ImbaLoginSvrConfig cfg;
		if( GetImbaLoginSvrConfig_FromLocal( *it, cfg ) )
			data.push_back( cfg );
	}

	return true;
}

inline
bool CImbaConfig::GetImbaGateSvrConfigs_FromLocal( std::vector<ImbaGateSvrConfig>& data )
{
	std::vector<int> ids;
	config_setting_t* rootset = 0;

	GET_CONFIG_ARRAY( IMBA_SERVERNAME[IMBASVRTYPE_GATE] );

	for ( std::vector<int>::iterator it=ids.begin(); it!=ids.end(); ++it )
	{
		ImbaGateSvrConfig cfg;
		if( GetImbaGateSvrConfig_FromLocal( *it, cfg ) )
			data.push_back( cfg );
	}

	return true;
}

inline
bool CImbaConfig::GetImbaGhostSvrConfigs_FromLocal( std::vector<ImbaGhostSvrConfig>& data )
{
	std::vector<int> ids;
	config_setting_t* rootset = 0;

	GET_CONFIG_ARRAY( IMBA_SERVERNAME[IMBASVRTYPE_GHOST] );

	for ( std::vector<int>::iterator it=ids.begin(); it!=ids.end(); ++it )
	{
		ImbaGhostSvrConfig cfg;
		if( GetImbaGhostSvrConfig_FromLocal( *it, cfg ) )
			data.push_back( cfg );
	}

	return true;
}

inline
bool CImbaConfig::GetImbaDBSvrConfigs_FromLocal( std::vector<ImbaDBSvrConfig>& data )
{
	std::vector<int> ids;
	config_setting_t* rootset = 0;

	GET_CONFIG_ARRAY( IMBA_SERVERNAME[IMBASVRTYPE_DB] );

	for ( std::vector<int>::iterator it=ids.begin(); it!=ids.end(); ++it )
	{
		ImbaDBSvrConfig cfg;
		if( GetImbaDBSvrConfig_FromLocal( *it, cfg ) )
			data.push_back( cfg );
	}

	return true;
}

inline
bool CImbaConfig::GetConfigBySql_FromLocal( const std::string& key, std::vector<std::string>& result, const std::string& split )
{
	const char* temp = 0;
	if ( !config_lookup_string( &m_local_config, key.c_str(), &temp ) )
	{
		return false;
	}

	CSplitString s( temp, split.c_str() );
	int count = s.getCount( );
	result.clear();

	for ( int i=0; i<count; ++i )
	{
		result.push_back( s.get(i) );
	}

	return true;
}

#define UPDATE_CONFIG_GROUP( key ) \
	do	\
	{	\
		rootset = config_lookup( &m_local_config, key.c_str() );	\
		if( !rootset ) rootset = config_setting_add( m_local_config.root, key.c_str(), CONFIG_TYPE_GROUP ); \
	} while ( 0 )

#define UPDATE_CONFIG_INT( key, value )	\
	do \
	{	\
		settemp = config_setting_get_member( rootset, key );	\
		if ( !settemp )	settemp = config_setting_add( rootset, key, CONFIG_TYPE_INT );	\
		config_setting_set_int( settemp, value );	\
	} while ( 0 )	

#define UPDATE_CONFIG_STRING( key, value ) \
	do	\
	{	\
		settemp = config_setting_get_member( rootset, key );	\
		if ( !settemp )	settemp = config_setting_add( rootset, key, CONFIG_TYPE_STRING );	\
		config_setting_set_string( settemp, value.c_str() );	\
	} while ( 0 )

#define UPDATE_2_FILE( ) \
	do	\
	{	\
		FILE* file = fopen( GetLocalFile().c_str(), "w+" );	\
		config_write(&m_local_config, file);	\
		fclose( file );	\
	} while ( 0 )

inline
bool CImbaConfig::SetImbaUserDB_ToLocal( int id, const ImbaUserDB& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	//rootset = config_lookup( &m_local_config, get_userdb_key( id ).c_str() );	
	//if( !rootset ) rootset = config_setting_add( m_local_config.root, get_userdb_key( id ).c_str(), CONFIG_TYPE_GROUP ); 

	UPDATE_CONFIG_GROUP( get_userdb_key( id ) );

	UPDATE_CONFIG_INT( "id", data.id );
	UPDATE_CONFIG_STRING( "odb_config", data.odb_config );
	UPDATE_CONFIG_STRING( "login_user", data.login_user );
	UPDATE_CONFIG_STRING( "login_passwd", data.login_passwd );
	UPDATE_CONFIG_STRING( "db_ip", data.db_ip );
	UPDATE_CONFIG_INT( "db_port", data.db_port );
	UPDATE_CONFIG_STRING( "db_name", data.db_name );
	UPDATE_CONFIG_STRING( "table_name", data.table_name );

	if( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetImbaGameDB_ToLocal( int id, const ImbaGameDB& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	UPDATE_CONFIG_GROUP( get_gamedb_key( id ) );

	UPDATE_CONFIG_INT( "id", data.id );
	UPDATE_CONFIG_STRING( "name", data.name );

	UPDATE_CONFIG_INT( "supportmode", data.supportmode );
	UPDATE_CONFIG_STRING( "config", data.config );

	UPDATE_CONFIG_STRING( "mapversion", data.mapversion );
	UPDATE_CONFIG_STRING( "mapdownloadurl", data.mapdownloadurl );

	UPDATE_CONFIG_STRING( "odb_config", data.odb_config );
	UPDATE_CONFIG_STRING( "login_user", data.login_user );
	UPDATE_CONFIG_STRING( "login_passwd", data.login_passwd );
	UPDATE_CONFIG_STRING( "db_ip", data.db_ip );
	UPDATE_CONFIG_INT( "db_port", data.db_port );
	UPDATE_CONFIG_STRING( "db_name", data.db_name );
	UPDATE_CONFIG_STRING( "gametable_name", data.gametable_name );
	UPDATE_CONFIG_STRING( "proptable_name", data.proptable_name );
	UPDATE_CONFIG_STRING( "mapname", data.mapname );

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetImbaLoginSvrConfig_ToLocal( int id, const ImbaLoginSvrConfig& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	UPDATE_CONFIG_GROUP( get_loginsvr_key( id ) );

	UPDATE_CONFIG_INT( "id", data.id );

	UPDATE_CONFIG_STRING( "toclient_ip", data.toclient_ip );
	UPDATE_CONFIG_INT( "toclient_port", data.toclient_port );
	UPDATE_CONFIG_STRING( "publish_ips", data.publish_ips );

	UPDATE_CONFIG_INT( "togate_dns", data.togate_dns );

	UPDATE_CONFIG_STRING( "cmdudp_ip", data.cmdudp_ip );
	UPDATE_CONFIG_INT( "cmdudp_port", data.cmdudp_port );

	UPDATE_CONFIG_INT( "thread_count", data.thread_count );
	UPDATE_CONFIG_INT( "dbsvr_id", data.dbsvr_id );

	UPDATE_CONFIG_STRING( "config", data.config );
	UPDATE_CONFIG_STRING( "version", data.version );
	UPDATE_CONFIG_STRING( "dir", data.dir );

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetImbaGateSvrConfig_ToLocal( int id, const ImbaGateSvrConfig& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	UPDATE_CONFIG_GROUP( get_gatesvr_key( id ) );

	UPDATE_CONFIG_INT( "id", data.id );

	UPDATE_CONFIG_STRING( "toclient_ip", data.toclient_ip );
	UPDATE_CONFIG_INT( "toclient_port", data.toclient_port );
	UPDATE_CONFIG_STRING( "publish_ips", data.publish_ips );

	UPDATE_CONFIG_STRING( "toghost_ip", data.toghost_ip );
	UPDATE_CONFIG_INT( "toghost_port", data.toghost_port );
	UPDATE_CONFIG_INT( "toghost_dns", data.toghost_dns );

	UPDATE_CONFIG_INT( "thread_count", data.thread_count );
	UPDATE_CONFIG_INT( "dbsvr_id", data.dbsvr_id );

	UPDATE_CONFIG_INT( "gameid", data.gameid );
	UPDATE_CONFIG_INT( "gamesupportmode", data.gamesupportmode );
	UPDATE_CONFIG_STRING( "gameconfig", data.gameconfig );
	
	UPDATE_CONFIG_STRING( "cmdudp_ip", data.cmdudp_ip );
	UPDATE_CONFIG_INT( "cmdudp_port", data.cmdudp_port );

	UPDATE_CONFIG_STRING( "version", data.version );
	UPDATE_CONFIG_STRING( "dir", data.dir );

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}
	
	return true;
}

inline
bool CImbaConfig::SetImbaGhostSvrConfig_ToLocal( int id, const ImbaGhostSvrConfig& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	UPDATE_CONFIG_GROUP( get_ghostsvr_key( id ) );

	UPDATE_CONFIG_INT( "id", data.id );

	UPDATE_CONFIG_STRING( "toclient_ip", data.toclient_ip );
	UPDATE_CONFIG_INT( "toclient_port", data.toclient_port );
	UPDATE_CONFIG_STRING( "publish_ips", data.publish_ips );

	UPDATE_CONFIG_INT( "thread_count", data.thread_count );
	UPDATE_CONFIG_INT( "gatesvr_id", data.gatesvr_id );

	UPDATE_CONFIG_STRING( "version", data.version );
	UPDATE_CONFIG_STRING( "dir", data.dir );

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetImbaDBSvrConfig_ToLocal( int id, const ImbaDBSvrConfig& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	UPDATE_CONFIG_GROUP( get_dbsvr_key( id ) );

	UPDATE_CONFIG_INT( "id", data.id );

	UPDATE_CONFIG_STRING( "listen_ip", data.listen_ip );
	UPDATE_CONFIG_INT( "listen_port", data.listen_port );

	UPDATE_CONFIG_INT( "thread_count", data.thread_count );
	UPDATE_CONFIG_INT( "userdb_id", data.userdb_id );
	UPDATE_CONFIG_STRING( "game_ids", data.game_ids );

	UPDATE_CONFIG_STRING( "version", data.version );
	UPDATE_CONFIG_STRING( "dir", data.dir );

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetImbaLoginSvrConfigs_ToLocal( const std::vector<ImbaLoginSvrConfig>& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	rootset = config_lookup( &m_local_config, IMBA_SERVERNAME[IMBASVRTYPE_LOGIN] );
	if ( rootset )
	{
		config_setting_remove( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_LOGIN] );
	}
	rootset = config_setting_add( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_LOGIN], CONFIG_TYPE_ARRAY );
	for ( std::size_t i=0; i<data.size(); ++i )
	{
		const ImbaLoginSvrConfig& cfg = data[i];
		settemp = config_setting_add( rootset, 0, CONFIG_TYPE_INT );
		config_setting_set_int( settemp, cfg.id );
		SetImbaLoginSvrConfig_ToLocal( cfg.id, cfg, false );
	}

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}
	
	return true;
}

inline
bool CImbaConfig::SetImbaGateSvrConfigs_ToLocal( const std::vector<ImbaGateSvrConfig>& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	rootset = config_lookup( &m_local_config, IMBA_SERVERNAME[IMBASVRTYPE_GATE] );
	if ( rootset )
	{
		config_setting_remove( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_GATE] );
	}
	rootset = config_setting_add( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_GATE], CONFIG_TYPE_ARRAY );
	for ( std::size_t i=0; i<data.size(); ++i )
	{
		const ImbaGateSvrConfig& cfg = data[i];
		settemp = config_setting_add( rootset, 0, CONFIG_TYPE_INT );
		config_setting_set_int( settemp, cfg.id );
		SetImbaGateSvrConfig_ToLocal( cfg.id, cfg, false );
	}

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetImbaGhostSvrConfigs_ToLocal( const std::vector<ImbaGhostSvrConfig>& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	rootset = config_lookup( &m_local_config, IMBA_SERVERNAME[IMBASVRTYPE_GHOST] );
	if ( rootset )
	{
		config_setting_remove( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_GHOST] );
	}
	rootset = config_setting_add( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_GHOST], CONFIG_TYPE_ARRAY );
	for ( std::size_t i=0; i<data.size(); ++i )
	{
		const ImbaGhostSvrConfig& cfg = data[i];
		settemp = config_setting_add( rootset, 0, CONFIG_TYPE_INT );
		config_setting_set_int( settemp, cfg.id );
		SetImbaGhostSvrConfig_ToLocal( cfg.id, cfg, false );
	}

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetImbaDBSvrConfigs_ToLocal( const std::vector<ImbaDBSvrConfig>& data, bool updatetofile )
{
	config_setting_t* rootset = 0;
	config_setting_t* settemp = 0;

	rootset = config_lookup( &m_local_config, IMBA_SERVERNAME[IMBASVRTYPE_DB] );
	if ( rootset )
	{
		config_setting_remove( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_DB] );
	}
	rootset = config_setting_add( m_local_config.root, IMBA_SERVERNAME[IMBASVRTYPE_DB], CONFIG_TYPE_ARRAY );
	for ( std::size_t i=0; i<data.size(); ++i )
	{
		const ImbaDBSvrConfig& cfg = data[i];
		settemp = config_setting_add( rootset, 0, CONFIG_TYPE_INT );
		config_setting_set_int( settemp, cfg.id );
		SetImbaDBSvrConfig_ToLocal( cfg.id, cfg, false );
	}

	if ( updatetofile )
	{
		UPDATE_2_FILE( );
	}

	return true;
}

inline
bool CImbaConfig::SetConfigBySql_ToLocal( const std::string& key, std::vector<std::string>& result, const std::string& split )
{
	std::string data;
	int len = result.size();

	if ( len<=0 )	return true;

	int pos = 0;
	do 
	{
		data += result[pos];
		if ( ++pos<len )
		{
			data += split;
		}
		else
		{
			break;
		}
	} while ( 1 );

	config_setting_t* settemp = 0;

	settemp = config_lookup( &m_local_config, key.c_str() );
	if ( !settemp )
	{
		settemp = config_setting_add( m_local_config.root, key.c_str(), CONFIG_TYPE_STRING );
	}
	config_setting_set_string( settemp, data.c_str() );
	UPDATE_2_FILE( );

	return true;
}


