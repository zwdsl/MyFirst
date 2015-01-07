
#pragma once

#include "imba_basexy.h"
#include "imba_structs.h"
#include "ImbaUUID.h"

#include <map>

namespace Imba_DBXY
{
	const int XY_BEGIN				= IMBA_DBXY_BEGIN;
	const int XY_END				= IMBA_DBXY_END;

	const int TCP_HEARTBEAT			= XY_BEGIN+1;	// client -> server; server -> client

	const int TCP_REQ_REGISTER		= XY_BEGIN+2;	// client -> server 请求注册帐号
	const int TCP_RESP_REGISTER		= XY_BEGIN+3;	// server -> client 

	const int TCP_REQ_AUTHENTICATION	= XY_BEGIN+4;	// client -> server; 帐号密码 请求验证
	const int TCP_RESP_AUTHENTICATION	= XY_BEGIN+5;

	const int TCP_REQ_AUTHENTICATION2	= XY_BEGIN+6;	// client -> server; 帐号ID session 请求验证
	const int TCP_RESP_AUTHENTICATION2	= XY_BEGIN+7;

	const int TCP_REQ_MODIFYUSERINFO	= XY_BEGIN+8;	// client -> server; 请求修改用户帐号数据
	const int TCP_RESP_MODIFYUSERINFO	= XY_BEGIN+9;

	const int TCP_REQ_GETGAMEINFO		= XY_BEGIN+10;	// client -> server; 请求用户某个游戏的游戏数据
	const int TCP_RESP_GETGAMEINFO		= XY_BEGIN+11;

	const int TCP_REQ_REPORTGAMERESULT	= XY_BEGIN+12;	// client -> server; 报告游戏结果
	const int TCP_RESP_REPORTGAMERESULT	= XY_BEGIN+13;	

	const int TCP_REQ_USECOIN			= XY_BEGIN+14;	// client -> server; 使用金币
	const int TCP_RESP_USECOIN			= XY_BEGIN+15;	

	const int TCP_REQ_AWARDPROP			= XY_BEGIN+16;	// client -> server; 地图奖励玩家道具
	const int TCP_RESP_AWARDPROP		= XY_BEGIN+17;

	const int TCP_REQ_REPORTONLINE		= XY_BEGIN+18;	// client -> server; gate报告在线人数
	const int TCP_RESP_REPORTONLINE		= XY_BEGIN+19;

	const int TCP_REQ_LOGFATALERR		= XY_BEGIN+20;	// client -> server; 写错误日志
	const int TCP_RESP_LOGFATALERR		= XY_BEGIN+21;	

	const int TCP_REQ_REPORTGATEPOS		= XY_BEGIN+22;	// client -> server; 报告位置
	const int TCP_RESP_REPORTGATEPOS	= XY_BEGIN+23;

	const int TCP_REQ_WEBUPDATECOIN		= XY_BEGIN+24;	// web -> server; +-金币
	const int TCP_RESP_WEBUPDATECOIN	= XY_BEGIN+25;

	const int TCP_REQ_REPORTBATTLESTART	= XY_BEGIN+26;

	const int TCP_REQ_REPORTBATTLEERR	= XY_BEGIN+27;
	const int TCP_REQ_REPORTBATTLEESCAPE= XY_BEGIN+28;

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


	struct Req_Register : public Imba_TCPXY<TCP_REQ_REGISTER>
	{
		int				askid;
		std::string		user;
		std::string		passwd;
		std::string		nickname;
		int				headid;
		char			sex;				// 0 女 1男 其他为未知

		void reset() 
		{
			askid = 0;
			user.clear();
			passwd.clear();
			nickname.clear();
			headid = 0;
			sex = 2;
		}

		friend bostream& operator<<(bostream& bos, const Req_Register& msg)
		{
			bos << msg.askid;
			bos << msg.user;
			bos << msg.passwd;
			bos << msg.nickname;
			bos << msg.headid;
			bos << msg.sex;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_Register& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.user;
			bis >> msg.passwd;
			bis >> msg.nickname;
			bis >> msg.headid;
			bis >> msg.sex;

			return bis;
		}
	};

	struct Resp_Register : public Imba_TCPXY<TCP_RESP_REGISTER>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA	= 1,
			ERRDB	= 2,
		};

		int		askid;
		int		flag;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_Register& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_Register& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			return bis;
		}
	};

	
	// auth for rencun
	struct Req_Authentication : public Imba_TCPXY<TCP_REQ_AUTHENTICATION>
	{
		int				askid;
		ImbaUserInfo	userinfo;
		ImbaGameInfo	gameinfo;

		void reset() 
		{
			askid = 0;
			userinfo.reset();
			gameinfo.reset();
		}

		friend bostream& operator<<(bostream& bos, const Req_Authentication& msg)
		{
			bos << msg.askid;
			bos << msg.userinfo;
			bos << msg.gameinfo;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_Authentication& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.userinfo;
			bis >> msg.gameinfo;

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

	struct Req_Authentication2 : public Imba_TCPXY<TCP_REQ_AUTHENTICATION2>
	{
		int			askid;
		int			userid;
		int			gameid;
		int			appid;
		std::string		ip;
		CImbaUUID		uuid; // 如果 uuid.IsValid() == false
		unsigned int	uuid2;// 则使用uuid2 作为匹配密码
		
		void reset( ) 
		{
			askid = 0;
			userid= 0;
			uuid2 = 0;
			gameid = appid = 0;
			ip.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_Authentication2& msg)
		{
			bos << msg.askid;
			bos << msg.userid;
			bos << msg.gameid;
			bos << msg.appid;
			bos << msg.ip;
			bos << msg.uuid;
			bos << msg.uuid2;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_Authentication2& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.userid;
			bis >> msg.gameid;
			bis >> msg.appid;
			bis >> msg.ip;
			bis >> msg.uuid;
			bis >> msg.uuid2;

			return bis;
		}
	};

	struct Resp_Authentication2 : public Imba_TCPXY<TCP_RESP_AUTHENTICATION2>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NOUSER,
			ERRDB,
			ERRPASSWD,
		};

		int		askid;
		int		flag;

		ImbaUserInfo	userinfo;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_Authentication2& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			bos << msg.userinfo;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_Authentication2& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			bis >> msg.userinfo;

			return bis;
		}
	};


	struct Req_ModifyUserInfo : public Imba_TCPXY<TCP_REQ_MODIFYUSERINFO>
	{
		enum KEYWORD
		{
			KW_BANON = 0,		// 封禁帐号, value为userid
			KW_BANOFF,			// 解封帐号, value为userid
		};
		int		askid;
		int		userid;
		std::map<int, std::string>	contents;

		void reset() 
		{
			askid = userid = 0;
			contents.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_ModifyUserInfo& msg)
		{
			bos << msg.askid;
			bos << msg.userid;

			int mapsize = msg.contents.size();
			bos << mapsize;
			for ( std::map<int, std::string>::const_iterator it=msg.contents.begin(); it!=msg.contents.end(); ++it )
			{
				bos << it->first;
				bos << it->second;
			}

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ModifyUserInfo& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.userid;

			int mapsize = 0;
			bis >> mapsize;
			for ( int i=0; i<mapsize; ++i )
			{
				int key;
				std::string value;
				bis >> key >> value;
				msg.contents.insert( std::make_pair( key, value ) );
			}

			return bis;
		}
	};

	struct Resp_ModifyUserInfo : public Imba_TCPXY<TCP_RESP_MODIFYUSERINFO>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA	= 1,
			ERRDB	= 2,
		};

		int		askid;
		int		flag;
		int		userid;
		// key->value 目前修改 passwd 和 nickname, 即key只能是"passwd" 和 "nickname"
		std::map<int, std::string>	contents;

		void reset() 
		{
			askid = flag = userid = 0;
			contents.clear();
		}

		friend bostream& operator<<(bostream& bos, const Resp_ModifyUserInfo& msg)
		{
			bos << msg.askid;
			bos << msg.flag;
			bos << msg.userid;

			int mapsize = msg.contents.size();
			bos << mapsize;
			for ( std::map<int, std::string>::const_iterator it=msg.contents.begin(); it!=msg.contents.end(); ++it )
			{
				bos << it->first;
				bos << it->second;
			}

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_ModifyUserInfo& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;
			bis >> msg.userid;

			int mapsize = 0;
			bis >> mapsize;
			for ( int i=0; i<mapsize; ++i )
			{
				int key;
				std::string value;
				bis >> key >> value;
				msg.contents.insert( std::make_pair( key, value ) );
			}

			return bis;
		}
	};


	struct Req_GetGameInfo : public Imba_TCPXY<TCP_REQ_GETGAMEINFO>
	{
		int askid;
		int userid;
		int gameid;

		void reset( ) 
		{
			askid = userid = gameid = 0;
		}

		friend bostream& operator<<(bostream& bos, const Req_GetGameInfo& msg)
		{
			bos << msg.askid;
			bos << msg.userid;
			bos << msg.gameid;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_GetGameInfo& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.userid;
			bis >> msg.gameid;

			return bis;
		}
	};

	struct Resp_GetGameInfo : public Imba_TCPXY<TCP_RESP_GETGAMEINFO>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
		};

		int askid;
		int flag;

		ImbaGameInfo info;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_GetGameInfo& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			bos << msg.info;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_GetGameInfo& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			bis >> msg.info;

			return bis;
		}
	};


	struct Req_ReportGameResult : public Imba_TCPXY<TCP_REQ_REPORTGAMERESULT>
	{
		int		askid;
		int		gameid;			// 什么游戏
		int		appid;			// 什么进程
		std::string uuid;		// 局号
		long long begintm;
		long long endtm;
		int		mode;
		
		std::vector<ImbaGameResult>	results; // 结果

		std::vector<int>	teammembertype;

		void reset( ) 
		{
			askid = gameid = appid = 0;
			uuid.clear( );
			begintm = endtm = 0;
			teammembertype.clear();
			results.clear();
			mode = 0;
		}

		friend bostream& operator<<(bostream& bos, const Req_ReportGameResult& msg)
		{
			bos << msg.askid;
			bos << msg.gameid;
			bos << msg.appid;
			bos << msg.uuid;
			bos << msg.begintm;
			bos << msg.endtm;

			bos << msg.results;
			bos << msg.teammembertype;
			bos << msg.mode;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ReportGameResult& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.gameid;
			bis >> msg.appid;
			bis >> msg.uuid;
			bis >> msg.begintm;
			bis >> msg.endtm;

			bis >> msg.results;
			bis >> msg.teammembertype;
			bis >> msg.mode;

			return bis;
		}
	};

	struct Resp_ReportGameResult : public Imba_TCPXY<TCP_RESP_REPORTGAMERESULT>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
		};

		int		askid;
		int		flag;
		std::vector<ImbaGameResult>	results; // 结果

		void reset() 
		{
			askid = flag = 0;
			results.clear();
		}

		friend bostream& operator<<(bostream& bos, const Resp_ReportGameResult& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			bos << msg.results;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_ReportGameResult& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			bis >> msg.results;

			return bis;
		}
	};


	struct Req_UseCoin : public Imba_TCPXY<TCP_REQ_USECOIN>
	{
		int			askid;
		int			gameid;
		int			appid;
		std::string uuid;		// 局号
		int			userid;
		int			usedcoin;
		int			mode;
		int			propid;

		void reset() 
		{
			askid = gameid = appid = userid = usedcoin = 0;
			mode = 0;
			propid = 0;
			uuid.clear( );
		}

		friend bostream& operator<<(bostream& bos, const Req_UseCoin& msg)
		{
			bos << msg.askid;
			bos << msg.gameid;
			bos << msg.appid;
			bos << msg.uuid;
			bos << msg.userid;
			bos << msg.usedcoin;
			bos << msg.mode;
			bos << msg.propid;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_UseCoin& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.gameid;
			bis >> msg.appid;
			bis >> msg.uuid;
			bis >> msg.userid;
			bis >> msg.usedcoin;
			bis >> msg.mode;
			bis >> msg.propid;

			return bis;
		}
	};

	struct Resp_UseCoin : public Imba_TCPXY<TCP_RESP_USECOIN>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
		};

		int		askid;
		int		flag;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_UseCoin& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_UseCoin& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			return bis;
		}
	};


	struct Req_AwardProp : public Imba_TCPXY<TCP_REQ_AWARDPROP>
	{
		int				askid;
		int				gameid;
		int				appid;
		std::string		uuid;		// 局号
		int				userid;
		unsigned char	awardprop;

		void reset() 
		{
			askid = gameid = appid = userid = awardprop = 0;
			uuid.clear( );
		}

		friend bostream& operator<<(bostream& bos, const Req_AwardProp& msg)
		{
			bos << msg.askid;
			bos << msg.gameid;
			bos << msg.appid;
			bos << msg.uuid;
			bos << msg.userid;
			bos << msg.awardprop;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_AwardProp& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.gameid;
			bis >> msg.appid;
			bis >> msg.uuid;
			bis >> msg.userid;
			bis >> msg.awardprop;

			return bis;
		}
	};

	struct Resp_AwardProp : public Imba_TCPXY<TCP_RESP_AWARDPROP>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
		};

		int		askid;
		int		flag;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_AwardProp& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_AwardProp& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			return bis;
		}
	};

	struct Req_ReportOnline : public Imba_TCPXY<TCP_REQ_REPORTONLINE>
	{
		int		askid;
		int		gameid;
		int		appid;
		map<int,int>		online;	// 在线人数
		map<int,int>		gaming;	// 游戏人数

		void reset() 
		{
			askid = gameid = appid =0;
			online.clear();
			gaming.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_ReportOnline& msg)
		{
			bos << msg.askid;
			bos << msg.gameid;
			bos << msg.appid;
			int onlinesize = msg.online.size();
			bos << onlinesize;
			for ( std::map<int, int>::const_iterator it=msg.online.begin(); it!=msg.online.end(); ++it )
			{
				bos << it->first;
				bos << it->second;
			}
			int gamingsize = msg.gaming.size();
			bos << gamingsize;
			for ( std::map<int, int>::const_iterator it=msg.gaming.begin(); it!=msg.gaming.end(); ++it )
			{
				bos << it->first;
				bos << it->second;
			}

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ReportOnline& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.gameid;
			bis >> msg.appid;
			int onlinesize;
			bis >> onlinesize;
			for(int i=0;i<onlinesize;i++)
			{
				int mode=0,online=0;
				bis >> mode;
				bis >> online;
				msg.online.insert(std::make_pair(mode,online));
			}
			int gamingsize;
			bis >> gamingsize;
			for(int i=0;i<gamingsize;i++)
			{
				int mode=0,gaming=0;
				bis >> mode;
				bis >> gaming;
				msg.gaming.insert(std::make_pair(mode,gaming));
			}

			return bis;
		}
	};

	struct Resp_ReportOnline : public Imba_TCPXY<TCP_RESP_REPORTONLINE>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
		};

		int		askid;
		int		flag;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_ReportOnline& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_ReportOnline& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			return bis;
		}
	};

	struct Req_LogFatalErr : public Imba_TCPXY<TCP_REQ_LOGFATALERR>
	{
		int				askid;
		int				svrtype;
		int				logmode;	
		int				appid;
		int				user_id;
		int				game_id;
		std::string		uuid;		// 局号
		std::string		user_ip;
		std::string		logmsg;

		void reset( ) 
		{
			askid = svrtype = logmode = appid = user_id = game_id = 0;
			uuid.clear(); user_ip.clear();logmsg.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_LogFatalErr& msg)
		{
			bos << msg.askid;
			bos << msg.svrtype;
			bos << msg.logmode;
			bos << msg.appid;
			bos << msg.user_id;
			bos << msg.game_id;
			bos << msg.uuid;
			bos << msg.user_ip;
			bos << msg.logmsg;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_LogFatalErr& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.svrtype;
			bis >> msg.logmode;
			bis >> msg.appid;
			bis >> msg.user_id;
			bis >> msg.game_id;
			bis >> msg.uuid;
			bis >> msg.user_ip;
			bis >> msg.logmsg;

			return bis;
		}
	};

	struct Resp_LogFatalErr : public Imba_TCPXY<TCP_RESP_LOGFATALERR>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
		};

		int		askid;
		int		flag;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_LogFatalErr& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_LogFatalErr& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			return bis;
		}
	};

	struct Req_ReportGatePos : public Imba_TCPXY<TCP_REQ_REPORTGATEPOS>
	{
		enum TYPE 
		{
			T_LOGIN = 0,
			T_LOGOFF,
		};

		int			askid;
		int			type;
		UserGatePos	pos;

		void reset() 
		{
			askid = type = 0;
			pos.reset();
		}

		friend bostream& operator<<(bostream& bos, const Req_ReportGatePos& msg)
		{
			bos << msg.askid;
			bos << msg.type;
			bos << msg.pos;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ReportGatePos& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.type;
			bis >> msg.pos;

			return bis;
		}
	};

	struct Resp_ReportGatePos : public Imba_TCPXY<TCP_RESP_REPORTGATEPOS>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
		};

		int		askid;
		int		flag;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_ReportGatePos& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_ReportGatePos& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			return bis;
		}
	};

	struct Req_WebUpdateCoin : public Imba_TCPXY<TCP_REQ_WEBUPDATECOIN>
	{
		int			askid;
		int			gameid;		// 填3
		int			appid;		// 哪个web服务, 填10000以上的值
								//         服务类型  -游戏-协议种类-协议序列号-协议
		std::string uuid;		// web使用{WEBSERVICE-IMBA-COIN-0000-WEBUPDATECOIN}
		int			userid;		
		int			usedcoin;

		void reset( ) 
		{
			askid = gameid = appid = userid = usedcoin = 0;
			uuid.clear( );
		}

		friend bostream& operator<<(bostream& bos, const Req_WebUpdateCoin& msg)
		{
			bos << msg.askid;
			bos << msg.gameid;
			bos << msg.appid;
			bos << msg.uuid;
			bos << msg.userid;
			bos << msg.usedcoin;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_WebUpdateCoin& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.gameid;
			bis >> msg.appid;
			bis >> msg.uuid;
			bis >> msg.userid;
			bis >> msg.usedcoin;

			return bis;
		}
	};

	struct Resp_WebUpdateCoin : public Imba_TCPXY<TCP_RESP_WEBUPDATECOIN>
	{
		enum FLAG
		{
			SUCCEED = 0,
			NODATA,
			ERRDB,
			ERRGAME,
			GAMEING,
		};

		int		askid;
		int		flag;

		void reset() 
		{
			askid = flag = 0;
		}

		friend bostream& operator<<(bostream& bos, const Resp_WebUpdateCoin& msg)
		{
			bos << msg.askid;
			bos << msg.flag;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Resp_WebUpdateCoin& msg)
		{
			msg.reset();

			bis >> msg.askid;
			bis >> msg.flag;

			return bis;
		}
	};
	struct Req_ReportBattleStart : public Imba_TCPXY<TCP_REQ_REPORTBATTLESTART>
	{
		int		gameid;			// 什么游戏
		std::string uuid;		// 局号
		std::vector<User_Note_Calculate> users;

		void reset() 
		{
			gameid = 0;
			uuid.clear();
			users.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_ReportBattleStart& msg)
		{
			bos << msg.gameid;
			bos << msg.uuid;
			bos << msg.users;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ReportBattleStart& msg)
		{
			msg.reset();
			bis >> msg.gameid;
			bis >> msg.uuid;
			bis >> msg.users;

			return bis;
		}
	};
	struct Req_ReportBattleEscape : public Imba_TCPXY<TCP_REQ_REPORTBATTLEESCAPE>
	{
		int		gameid;			// 什么游戏
		std::string uuid;		// 局号
		std::vector<User_Note_Calculate> users;
		unsigned char bsecond;

		void reset() 
		{
			gameid = 0;
			uuid.clear();
			users.clear();
			bsecond = 0;
		}

		friend bostream& operator<<(bostream& bos, const Req_ReportBattleEscape& msg)
		{
			bos << msg.gameid;
			bos << msg.uuid;
			bos << msg.users;
			bos << msg.bsecond;
			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ReportBattleEscape& msg)
		{
			msg.reset();
			bis >> msg.gameid;
			bis >> msg.uuid;
			bis >> msg.users;
			bis >> msg.bsecond;
			return bis;
		}
	};
	struct Req_ReportBattleErr : public Imba_TCPXY<TCP_REQ_REPORTBATTLEERR>
	{
		int		gameid;			// 什么游戏
		std::string uuid;		// 局号
		std::vector<User_Note_Calculate> users;

		void reset() 
		{
			gameid = 0;
			uuid.clear();
			users.clear();
		}

		friend bostream& operator<<(bostream& bos, const Req_ReportBattleErr& msg)
		{
			bos << msg.gameid;
			bos << msg.uuid;
			bos << msg.users;

			return bos;
		}

		friend bistream& operator>>(bistream& bis, Req_ReportBattleErr& msg)
		{
			msg.reset();
			bis >> msg.gameid;
			bis >> msg.uuid;
			bis >> msg.users;

			return bis;
		}
	};
};
