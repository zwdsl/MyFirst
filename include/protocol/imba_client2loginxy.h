
#pragma once

#include "imba_basexy.h"
#include "imba_structs.h"
#include "ImbaUUID.h"


namespace Imba_Client2LoginXY
{
	const int XY_BEGIN				= IMBA_CLIENT2LOGINXY_BEGIN;
	const int XY_END				= IMBA_CLIENT2LOGINXY_END;

	const int TCP_HEARTBEAT					= XY_BEGIN+1;	// client -> server; 

	const int TCP_REQ_AUTHENTICATION		= XY_BEGIN+2;	// client -> server; 帐号密码 请求验证
	const int TCP_RESP_AUTHENTICATION		= XY_BEGIN+3;

	const int TCP_REQ_SELECTGAMESERVER		= XY_BEGIN+4;	// client -> server; 选择游戏服务器
	const int TCP_RESP_SELECTGAMESERVER		= XY_BEGIN+5;	

	const int UDP_GETXY_BEGIN			= XY_BEGIN+60;
	const Imba_UdpXY UDP_GET_SVRSTATUS	= Imba_UdpXY(UDP_GETXY_BEGIN+1, "get svrstatus");	// 获取本服务的状态

	const int UDP_SETXY_BEGIN			= XY_BEGIN+80;
	const Imba_UdpXY UDP_SET_LOGLEVEL	= Imba_UdpXY(UDP_SETXY_BEGIN+1, "set loglevel", 1);

	const int UDP_REPLY_XY				= XY_BEGIN+99;


	struct HeartBeat : public Imba_TCPXY<TCP_HEARTBEAT>
	{
		friend bostream& operator<<(bostream& bos, const HeartBeat& msg)
		{
			return bos;
		}

		friend bistream& operator>>(bistream& bis, HeartBeat& msg)
		{
			return bis;
		}
	};

	struct Req_Authentication : public Imba_TCPXY<TCP_REQ_AUTHENTICATION>
	{
		int				askid;
		std::string		user;
		std::string		passwd;

		void reset() 
		{
			askid = 0;
			user.clear();
			passwd.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_Authentication& msg)
		{
			bos << msg.askid;
			bos << msg.user;
			bos << msg.passwd;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_Authentication& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.user;
			bis >> msg.passwd;

			return bis;
		}
	};

	struct Resp_Authentication : public Imba_TCPXY<TCP_RESP_AUTHENTICATION>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NOUSER,
			ERRDB,
			ERRPASSWD,
			HASBANNED,	
			TIMEOUT,
			ERR_USERID_LEN,
			ERR_PASSWD_LEN,
		};

		int		askid;
		int		flag;

		CImbaUUID		uuid;
		ImbaUserInfo	userinfo;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_Authentication& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			bos << msg.uuid;
			bos << msg.userinfo;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_Authentication& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			bis >> msg.uuid;
			bis >> msg.userinfo;

			return bis;
		}
	};

	struct Req_SelectServer : public Imba_TCPXY<TCP_REQ_SELECTGAMESERVER>
	{
		int		askid;
		int		gameid;			// 游戏ID
		int		nettype;		// 0电信 1网通 2教育网 3其他

		void reset() 
		{
			askid = gameid = nettype = 0;
		}

		friend bostream& operator<<(bostream& bos, const Req_SelectServer& msg)
		{
			bos << msg.askid;
			bos << msg.gameid;
			bos << msg.nettype;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_SelectServer& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.gameid;
			bis >> msg.nettype;

			return bis;
		}
	};

	struct Resp_SelectServer : public Imba_TCPXY<TCP_RESP_SELECTGAMESERVER>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NOGAME,			
		};
		int				askid;
		int				flag;
		int				gameid;
		std::string		ip;			// gate server ip
		int				port;		// gate server port

		void reset() 
		{
			askid = flag = gameid = 0;
			ip.clear();
			port = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_SelectServer& msg)
		{
			bos << msg.askid;
			bos << msg.flag;
			bos << msg.gameid;

			bos << msg.ip;
			bos << msg.port;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_SelectServer& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;
			bis >> msg.gameid;

			bis >> msg.ip;
			bis >> msg.port;

			return bis;
		}
	};

};

#pragma pack(push)
#pragma pack(1)

namespace Imba_Client2LoginXY_Old
{
	const int XY_BEGIN					= 0;
	const int XY_END					= 100;

	const int CMD_LOBBY_REQ_LOGIN_AUTH	= XY_BEGIN+1;	// client -> server; 帐号密码 请求验证
	const int CMD_LOGIN_RESP_LOBBY_AUTH	= XY_BEGIN+2;

	const int CMD_LOBBY_NOTE_LOGIN_QUIT	= XY_BEGIN+3;	// client -> server;

	const int TCP_REQ_SELECTGAMESERVER	= XY_BEGIN+4;	// client -> server; 选择游戏服务器
	const int TCP_RESP_SELECTGAMESERVER	= XY_BEGIN+5;	

	struct Lobby_Req_Login_Auth : public Imba_TCPXY<CMD_LOBBY_REQ_LOGIN_AUTH>
	{
		enum { MAX_DATA_LEN = 2000 };

		unsigned int	user_id;
		unsigned int	session_id;
		unsigned short	login_area_id;
		unsigned char	client_type;
		unsigned short	user_name_len;
		unsigned short	pwd_len;
		char			data[MAX_DATA_LEN];

		Lobby_Req_Login_Auth( void ) { memset(this, 0, sizeof(Lobby_Req_Login_Auth)); }
		const int get_msglen( void )
		{
			return sizeof(Lobby_Req_Login_Auth)-MAX_DATA_LEN+user_name_len+pwd_len;
		}
	};

	struct Login_Resp_Lobby_Auth : public Imba_TCPXY<CMD_LOGIN_RESP_LOBBY_AUTH>
	{
		enum AUTH_RES
		{
			AUTH_SUCCESS=0,
			AUTH_LOBBY_ERR,
			AUTH_LOGINSVR_ERR,
			AUTH_DBSVR_ERR,
			AUTH_HTTPSVR_ERR,
		};

		enum CLIENT_ERR
		{
			ERR_USER_NAME_LEN=1,
			ERR_PWD_LEN,
			NO_USER,
			ERR_PWD,
			ERR_USERNAME,
			AUTHING,
			ERR_UNKNOWN,
		};

		enum DBSERVER_ERR
		{
			NOUSER	= 1,
			ERRDB,
			ERRPASSWD,
			HASBANNED,
		};

		enum { MAX_DATA_LEN = 200 };

		unsigned char res;
		unsigned char why;
		unsigned int  session_id;
		unsigned int  user_id;
		unsigned char sex;
		unsigned int  lobby_admin;
		unsigned int  user_props;

		unsigned char user_name_len;
		unsigned char nick_name_len;
		unsigned char last_login_tm_len;
		char		  data[MAX_DATA_LEN];

		Login_Resp_Lobby_Auth( void ) { memset(this, 0, sizeof(Login_Resp_Lobby_Auth)); }
		const int get_msglen( void )
		{
			return sizeof(Login_Resp_Lobby_Auth)-MAX_DATA_LEN+user_name_len+nick_name_len+last_login_tm_len;
		}
	};

	struct Lobby_Note_Login_Quit : public Imba_TCPXY<CMD_LOBBY_NOTE_LOGIN_QUIT>
	{
		unsigned int user_id;
		unsigned int session_id;

		Lobby_Note_Login_Quit( void ) { memset(this, 0, sizeof(Lobby_Note_Login_Quit)); }
		const int get_msglen( void )
		{
			return sizeof(Lobby_Note_Login_Quit);
		}
	};

	struct Req_SelectServer : public Imba_TCPXY<TCP_REQ_SELECTGAMESERVER>
	{
		unsigned int	user_id;
		unsigned int	session_id;
		int				askid;
		int				gameid;			// 游戏ID
		int				nettype;		// 0电信 1网通 2教育网 3其他

		Req_SelectServer( void ) { memset(this, 0, sizeof(Req_SelectServer)); }
		const int get_msglen( void )
		{
			return sizeof(Req_SelectServer);
		}
	};

	struct Resp_SelectServer : public Imba_TCPXY<TCP_RESP_SELECTGAMESERVER>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NOGAME,			
		};
		int				askid;
		int				flag;
		int				gameid;
		char			ip[32];			// gate server ip
		int				port;			// gate server port

		Resp_SelectServer( void ) { memset(this, 0, sizeof(Resp_SelectServer)); }
		const int get_msglen( void )
		{
			return sizeof(Resp_SelectServer);
		}
	};
};

#pragma pack(pop)
