
#pragma once

#include "imba_basexy.h"
#include "imba_structs.h"

namespace Imba_ConfigXY
{
	const int XY_BEGIN				= IMBA_CONFIGXY_BEGIN;
	const int XY_END				= IMBA_CONFIGXY_BND;

	const int TCP_HEARTBEAT			= XY_BEGIN+1;	// client -> server

	const int TCP_REQ_LOGIN			= XY_BEGIN+2;	// client -> server		暂不验证
	const int TCP_RESP_LOGIN		= XY_BEGIN+3;	// server -> client

	const int TCP_REQ_USERDB		= XY_BEGIN+4;	// client -> server
	const int TCP_RESP_USERDB		= XY_BEGIN+5;	// server -> client

	const int TCP_REQ_GAMEDB		= XY_BEGIN+6;	// client -> server
	const int TCP_RESP_GAMEDB		= XY_BEGIN+7;	// server -> client

	const int TCP_REQ_SVRCONFIG		= XY_BEGIN+8;	// client -> server
	const int TCP_RESP_SVRCONFIG	= XY_BEGIN+9;	// server -> client	

	const int TCP_REQ_CONFIGBYSQL	= XY_BEGIN+10;	// client -> server
	const int TCP_RESP_CONFIGBYSQL	= XY_BEGIN+11;	// server -> client

	const int UDP_GETXY_BEGIN			= XY_BEGIN+60;
	const Imba_UdpXY UDP_GET_USERDBS	= Imba_UdpXY(UDP_GETXY_BEGIN+1, "get userdbs"); // client -> server
	const Imba_UdpXY UDP_GET_GAMEDBS	= Imba_UdpXY(UDP_GETXY_BEGIN+2, "get gamedbs");
	const Imba_UdpXY UDP_GET_LOGINSVRS	= Imba_UdpXY(UDP_GETXY_BEGIN+3, "get loginsvrs");
	const Imba_UdpXY UDP_GET_GATESVRS	= Imba_UdpXY(UDP_GETXY_BEGIN+4, "get gatesvrs");
	const Imba_UdpXY UDP_GET_GHOSTSVRS	= Imba_UdpXY(UDP_GETXY_BEGIN+5, "get ghostsvrs");
	const Imba_UdpXY UDP_GET_DBSVRS		= Imba_UdpXY(UDP_GETXY_BEGIN+6, "get dbsvrs");
	const Imba_UdpXY UDP_GET_SVRDOTMAP	= Imba_UdpXY(UDP_GETXY_BEGIN+7, "get svrdotmap");
	const Imba_UdpXY UDP_GET_SVRSTATUS	= Imba_UdpXY(UDP_GETXY_BEGIN+8, "get svrstatus");	// 获取本服务的状态

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

	struct Req_Login : public Imba_TCPXY<TCP_REQ_LOGIN>
	{
		std::string		user;
		std::string		passwd;

		void reset() 
		{
			user.clear();
			passwd.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_Login& msg)
		{
			bos << msg.user;
			bos << msg.passwd;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_Login& msg)
		{
			msg.reset();

			bis >> msg.user;
			bis >> msg.passwd;

			return bis;
		}
	};

	struct Resp_Login : public Imba_TCPXY<TCP_RESP_LOGIN>
	{
		int				flag; // 1成功

		void reset() 
		{
			flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_Login& msg)
		{
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_Login& msg)
		{
			msg.reset();
			bis >> msg.flag;

			return bis;
		}
	};


	struct Req_UserDB : public Imba_TCPXY<TCP_REQ_USERDB>
	{
		int				askid;
		int				userdbid;

		void reset() 
		{
			askid = userdbid = 0;
		}

		friend bostream& operator<<(bostream& bos, const Req_UserDB& msg)
		{
			bos << msg.askid;
			bos << msg.userdbid;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_UserDB& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.userdbid;

			return bis;
		}
	};

	struct Resp_UserDB : public Imba_TCPXY<TCP_RESP_USERDB>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA	= 1,
			DBERR	= 2,
		};
		int				askid;
		int				flag;		// 0 succeed , 1 fail
		ImbaUserDB		info;

		void reset() 
		{
			askid = flag = 0;
			info.reset();
		}

		friend bostream& operator<<(bostream& bos, const Resp_UserDB& msg)
		{
			bos << msg.askid;
			bos << msg.flag;
			bos << msg.info;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_UserDB& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;
			bis >> msg.info;

			return bis;
		}
	};


	struct Req_GameDB : public Imba_TCPXY<TCP_REQ_GAMEDB>
	{
		int				askid;
		int				gamedbid;

		void reset() 
		{
			askid = gamedbid = 0;
		}

		friend bostream& operator<<(bostream& bos, const Req_GameDB& msg)
		{
			bos << msg.askid;
			bos << msg.gamedbid;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_GameDB& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.gamedbid;

			return bis;
		}
	};

	struct Resp_GameDB : public Imba_TCPXY<TCP_RESP_GAMEDB>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA	= 1,
			DBERR	= 2,
		};
		int				askid;
		int				flag;		// 0 succeed , 1 fail
		ImbaGameDB		info;

		void reset() 
		{
			askid = flag = 0;
			info.reset();
		}

		friend bostream& operator<<(bostream& bos, const Resp_GameDB& msg)
		{
			bos << msg.askid;
			bos << msg.flag;
			bos << msg.info;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_GameDB& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;
			bis >> msg.info;

			return bis;
		}
	};


	struct Req_SvrConfig : public Imba_TCPXY<TCP_REQ_SVRCONFIG>
	{
		enum { ALLSERVER = 0 };
		int				askid;
		int				svrtype;
		int				svrid;	// 当svrid==ALLSERVER，为获取所有的服务

		void reset() 
		{
			askid = svrtype = svrid = 0;
		}

		friend bostream& operator<<(bostream& bos, const Req_SvrConfig& msg)
		{
			bos << msg.askid;
			bos << msg.svrtype;
			bos << msg.svrid;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_SvrConfig& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.svrtype;
			bis >> msg.svrid;

			return bis;
		}
	};

	struct Resp_SvrConfig : public Imba_TCPXY<TCP_RESP_SVRCONFIG>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA	= 1,
			DBERR	= 2,
			UNKNOWNSVR=3,
		};
		int				askid;
		int				flag;		// 0 succeed , 1 fail
		int				svrtype;

		ImbaLoginSvrConfig	loginsvr_cfg;
		ImbaGateSvrConfig	gatesvr_cfg;
		ImbaGhostSvrConfig	ghostsvr_cfg;
		ImbaDBSvrConfig		dbsvr_cfg;

		void reset() 
		{
			askid = flag = svrtype = 0;
			loginsvr_cfg.reset();
			gatesvr_cfg.reset();
			ghostsvr_cfg.reset();
			dbsvr_cfg.reset();
		}

		friend bostream& operator<<(bostream& bos, const Resp_SvrConfig& msg)
		{
			bos << msg.askid;
			bos << msg.flag;
			bos << msg.svrtype;

			switch ( msg.svrtype )
			{
			case IMBASVRTYPE_LOGIN:
				bos << msg.loginsvr_cfg;
				break;
			case IMBASVRTYPE_GATE:
				bos << msg.gatesvr_cfg;
				break;
			case IMBASVRTYPE_GHOST:
				bos << msg.ghostsvr_cfg;
				break;
			case IMBASVRTYPE_DB:
				bos << msg.dbsvr_cfg;
				break;
			default:
				break;
			}

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_SvrConfig& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;
			bis >> msg.svrtype;

			switch ( msg.svrtype )
			{
			case IMBASVRTYPE_LOGIN:
				bis >> msg.loginsvr_cfg;
				break;
			case IMBASVRTYPE_GATE:
				bis >> msg.gatesvr_cfg;
				break;
			case IMBASVRTYPE_GHOST:
				bis >> msg.ghostsvr_cfg;
				break;
			case IMBASVRTYPE_DB:
				bis >> msg.dbsvr_cfg;
				break;
			default:
				break;
			}

			return bis;
		}
	};

	struct Req_ConfigBySql : public Imba_TCPXY<TCP_REQ_CONFIGBYSQL>
	{
		int				askid;
		std::string		sql;
		std::string		split;

		void reset() 
		{
			askid = 0;
			sql.clear();
			split.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_ConfigBySql& msg)
		{
			bos << msg.askid;
			bos << msg.sql;
			bos << msg.split;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ConfigBySql& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.sql;
			bis >> msg.split;

			return bis;
		}
	};

	struct Resp_ConfigBySql : public Imba_TCPXY<TCP_RESP_CONFIGBYSQL>
	{
		enum { MAX_LEN = 4000 };
		enum FLAG
		{
			SUCCEED = 0,
			NODATA	= 1,
			DBERR	= 2,
			UNKNOWNSVR=3,
		};

		int				askid;
		int				flag;
		std::string		result;

		void reset() 
		{
			askid = flag = 0;
			result.clear();
		}

		friend bostream& operator<<(bostream& bos, const Resp_ConfigBySql& msg)
		{
			bos << msg.askid;
			bos << msg.flag;
			bos << msg.result;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_ConfigBySql& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;
			bis >> msg.result;

			return bis;
		}
	};

};

