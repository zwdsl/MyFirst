
#pragma once

// 避免strcopy 等等的一些警告
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <string>
#include <vector>
#include <queue>
#include <time.h>

#include <include/base/Base.h>
using namespace Communications;

#define LIBCONFIG_STATIC
#include <tools/libconfig-1.4.8/lib/libconfig.h>

#if defined _WINDOWS_ || defined WIN32
#pragma comment(lib, "../../libs/libconfig.lib")
#endif

#include <include/protocol/ImbaBaseProtocol.h>
#include <include/protocol/imba_configxy.h>

template< class T >
class configclient_message_pool : public __OBJECT__POOL< T >
{

};

typedef CTcpSocketClient<CImbaBaseProtocol, configclient_message_pool>	ConfigSession;


class CImbaConfig : public boost::noncopyable
{
public:
	CImbaConfig( void );
	~CImbaConfig( void );

	// ---------提供给外部接口 begin
	bool InitConfigFromDB( int svrtype, int svrid, const std::string& ip, int port );
	bool InitConfigFromLocal( int svrtype, int svrid );

	int  GetSvrType( void ) const;
	int  GetSvrID( void ) const;
	const std::string& GetDBServer( void ) const;
	const std::string& GetLocalFile( void ) const;

	// 固定数据
	bool GetImbaUserDB( int id, ImbaUserDB& data );
	bool GetImbaGameDB( int id, ImbaGameDB& data );

	bool GetImbaLoginSvrConfig( int id, ImbaLoginSvrConfig& data );
	bool GetImbaGateSvrConfig( int id, ImbaGateSvrConfig& data );
	bool GetImbaGhostSvrConfig( int id, ImbaGhostSvrConfig& data );
	bool GetImbaDBSvrConfig( int id, ImbaDBSvrConfig& data );

	bool GetImbaLoginSvrConfigs( std::vector<ImbaLoginSvrConfig>& data );
	bool GetImbaGateSvrConfigs( std::vector<ImbaGateSvrConfig>& data );
	bool GetImbaGhostSvrConfigs( std::vector<ImbaGhostSvrConfig>& data );
	bool GetImbaDBSvrConfigs( std::vector<ImbaDBSvrConfig>& data );

	// 可变数据, key必须是 变量方式 的定义
	bool GetConfigBySql( const std::string& key, const std::string& sql, std::vector<std::string>& result, 
			const std::string& split="|@|" );
	// ---------提供给外部接口 end

public:
	void OnSocketConnect( int socketid );
	void OnSocketClose( int socket, bool bretain );
	void OnSocketMessage( int socket, boost::shared_ptr<CImbaBaseProtocol> message );
	void OnSocketTimeOut( int socketid );

private:
	bool GetImbaUserDB_FromDB( int id, ImbaUserDB& data );
	bool GetImbaGameDB_FromDB( int id, ImbaGameDB& data );
	bool GetImbaLoginSvrConfig_FromDB( int id, ImbaLoginSvrConfig& data );
	bool GetImbaGateSvrConfig_FromDB( int id, ImbaGateSvrConfig& data );
	bool GetImbaGhostSvrConfig_FromDB( int id, ImbaGhostSvrConfig& data );
	bool GetImbaDBSvrConfig_FromDB( int id, ImbaDBSvrConfig& data );
	bool GetImbaLoginSvrConfigs_FromDB( std::vector<ImbaLoginSvrConfig>& data );
	bool GetImbaGateSvrConfigs_FromDB( std::vector<ImbaGateSvrConfig>& data );
	bool GetImbaGhostSvrConfigs_FromDB( std::vector<ImbaGhostSvrConfig>& data );
	bool GetImbaDBSvrConfigs_FromDB( std::vector<ImbaDBSvrConfig>& data );

	bool GetConfigBySql_FromDB( const std::string& sql, std::vector<std::string>& result, const std::string& split );


	bool GetImbaUserDB_FromLocal( int id, ImbaUserDB& data );
	bool GetImbaGameDB_FromLocal( int id, ImbaGameDB& data );
	bool GetImbaLoginSvrConfig_FromLocal( int id, ImbaLoginSvrConfig& data );
	bool GetImbaGateSvrConfig_FromLocal( int id, ImbaGateSvrConfig& data );
	bool GetImbaGhostSvrConfig_FromLocal( int id, ImbaGhostSvrConfig& data );
	bool GetImbaDBSvrConfig_FromLocal( int id, ImbaDBSvrConfig& data );
	bool GetImbaLoginSvrConfigs_FromLocal( std::vector<ImbaLoginSvrConfig>& data );
	bool GetImbaGateSvrConfigs_FromLocal( std::vector<ImbaGateSvrConfig>& data );
	bool GetImbaGhostSvrConfigs_FromLocal( std::vector<ImbaGhostSvrConfig>& data );
	bool GetImbaDBSvrConfigs_FromLocal( std::vector<ImbaDBSvrConfig>& data );

	bool GetConfigBySql_FromLocal( const std::string& key, std::vector<std::string>& result, const std::string& split );


	bool SetImbaUserDB_ToLocal( int id, const ImbaUserDB& data, bool updatetofile=true );
	bool SetImbaGameDB_ToLocal( int id, const ImbaGameDB& data, bool updatetofile=true );
	bool SetImbaLoginSvrConfig_ToLocal( int id, const ImbaLoginSvrConfig& data, bool updatetofile=true );
	bool SetImbaGateSvrConfig_ToLocal( int id, const ImbaGateSvrConfig& data, bool updatetofile=true );
	bool SetImbaGhostSvrConfig_ToLocal( int id, const ImbaGhostSvrConfig& data, bool updatetofile=true );
	bool SetImbaDBSvrConfig_ToLocal( int id, const ImbaDBSvrConfig& data, bool updatetofile=true );
	bool SetImbaLoginSvrConfigs_ToLocal( const std::vector<ImbaLoginSvrConfig>& data, bool updatetofile=true );
	bool SetImbaGateSvrConfigs_ToLocal( const std::vector<ImbaGateSvrConfig>& data, bool updatetofile=true );
	bool SetImbaGhostSvrConfigs_ToLocal( const std::vector<ImbaGhostSvrConfig>& data, bool updatetofile=true );
	bool SetImbaDBSvrConfigs_ToLocal( const std::vector<ImbaDBSvrConfig>& data, bool updatetofile=true );

	bool SetConfigBySql_ToLocal( const std::string& key, std::vector<std::string>& result, const std::string& split );

	template<class T>
	void SendProtocol( int socketid, const T& t );
	int  GenAskID( void );
private:
	// 0 为初始化，1 从db读取数据， 2 从本地文件读取数据
	int											m_status;
	int											m_svrtype;
	int											m_svrid;

	// 从db读取数据相关
	std::string									m_dbservername;
	boost::asio::io_service						m_io;
	configclient_message_pool<CImbaBaseProtocol> m_messagepool;
	boost::shared_ptr<ConfigSession>			m_session;
	boost::shared_ptr<boost::thread>			m_iothread;	
	boost::mutex								m_mutex;
	boost::condition							m_condition;
	std::queue<boost::shared_ptr<CImbaBaseProtocol> > m_receivedmsg;
	int											m_genaskid;

	// 从本地读取数据相关
	std::string									m_localfilename;
	config_t									m_local_config;
};

#include "configclientimpl.hpp"