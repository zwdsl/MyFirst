
#pragma once

#include "imba_basexy.h"
#include "imba_structs.h"
#include "ImbaUUID.h"

namespace Imba_Client2GateXY
{
	const int XY_BEGIN				= IMBA_CLIENT2GATEXY_BEGIN;
	const int XY_END				= IMBA_CLIENT2GATEXY_END;

	const int UDP_GETXY_BEGIN			= XY_BEGIN+60;
	const Imba_UdpXY UDP_GET_SVRSTATUS	= Imba_UdpXY(UDP_GETXY_BEGIN+1, "get svrstatus");	// 获取本服务的状态
	const Imba_UdpXY UDP_GET_ONLINES	= Imba_UdpXY(UDP_GETXY_BEGIN+2, "get onlines");		// 获取服务在线人数
	const Imba_UdpXY UDP_GET_GAMESTATUS	= Imba_UdpXY(UDP_GETXY_BEGIN+3, "get gamestatus");	// 获取游戏状态
	const Imba_UdpXY UDP_GET_GAMECONFIG	= Imba_UdpXY(UDP_GETXY_BEGIN+4, "get gameconfig");	// 获取游戏配置状态
	const Imba_UdpXY UDP_GET_GHOSTDNS	= Imba_UdpXY(UDP_GETXY_BEGIN+5, "get ghostdns");	// 获得ghost路由方式
	const Imba_UdpXY UDP_GET_GHOSTONLINE= Imba_UdpXY(UDP_GETXY_BEGIN+6, "get ghostonline");	// 获得ghost服务在线
	const Imba_UdpXY UDP_GET_USERID		= Imba_UdpXY(UDP_GETXY_BEGIN+7, "get userid", 1);	// 根据用户的昵称获得用户的数字id
	const Imba_UdpXY UDP_GET_USERCOUNT	= Imba_UdpXY(UDP_GETXY_BEGIN+8, "get usercount", 2);// get usercount >(<) 200 获取服务器medal>200的在线人数
	const Imba_UdpXY UDP_GET_EIGENCODES = Imba_UdpXY(UDP_GETXY_BEGIN+9, "get eigencodes");	// 获取eigencodes条数
	const Imba_UdpXY UDP_GET_MATCHLIST	= Imba_UdpXY(UDP_GETXY_BEGIN+10,"get matchlist", 1);	// 重新从db读取matchlist配置


	const int UDP_SETXY_BEGIN			= XY_BEGIN+80;
	const Imba_UdpXY UDP_SET_LOGLEVEL	= Imba_UdpXY(UDP_SETXY_BEGIN+1, "set loglevel", 1);		// 设置日志等级
	const Imba_UdpXY UDP_SET_GAMECONFIG	= Imba_UdpXY(UDP_SETXY_BEGIN+2, "set gameconfig", 1);	// 
	const Imba_UdpXY UDP_SET_GHOSTDNS	= Imba_UdpXY(UDP_SETXY_BEGIN+3, "set ghostdns", 1);		// 设置ghost路由方式
	const Imba_UdpXY UDP_SET_MAINGHOST	= Imba_UdpXY(UDP_SETXY_BEGIN+4, "set mainghost", 1);	// 设置主从路由方式的 主机
	const Imba_UdpXY UDP_SET_GHOSTMAXONLINE=Imba_UdpXY(UDP_SETXY_BEGIN+5, "set ghostmaxonline", 2);// 设置ghost最大在线
	const Imba_UdpXY UDP_KICK_USER		= Imba_UdpXY(UDP_SETXY_BEGIN+6, "kick user", 1);		// 踢人
	const Imba_UdpXY UDP_SEND_SYSMSG	= Imba_UdpXY(UDP_SETXY_BEGIN+7, "send sysmsg", 2);		// 发送系统消息
	const Imba_UdpXY UDP_BANON_USER		= Imba_UdpXY(UDP_SETXY_BEGIN+8, "ban on", 1);			// 封帐号
	const Imba_UdpXY UDP_BANOFF_USER	= Imba_UdpXY(UDP_SETXY_BEGIN+9, "ban off", 1);			// 解封帐号
	const Imba_UdpXY UDP_UPDATE_GHOSTCFG= Imba_UdpXY(UDP_SETXY_BEGIN+10,"update ghostcfg");		// 重新从db读取ghost配置
	const Imba_UdpXY UDP_UPDATE_EIGENCODES=Imba_UdpXY(UDP_SETXY_BEGIN+11,"update eigencodes" );	// 重新从db读取eigencodes配置
	const Imba_UdpXY UDP_UPDATE_MATCHLIST=Imba_UdpXY(UDP_SETXY_BEGIN+12,"update matchlist", 1);	// 重新从db读取matchlist配置
	const Imba_UdpXY UDP_END_MATCHLIST	 =Imba_UdpXY(UDP_SETXY_BEGIN+13,"end matchlist", 1);			// 终止matchlist配置
	const Imba_UdpXY UDP_ENABLE_CHRISTMAS	 =Imba_UdpXY(UDP_SETXY_BEGIN+14,"enable christmas", 1);			// christmas配置
	

	const int UDP_REPLY_XY				= XY_BEGIN+99;
};

#pragma pack(push)
#pragma pack(1)

namespace Imba_Client2GateXY_Old
{
	struct User_Report
	{
		unsigned int  user_id;
		unsigned char b_matching;
		char		  user_name[50];
	};

	enum LIST_TYPE
	{
		LIST_QUIT=0,
		LIST_AUTHED,
		LIST_SLIENT,
		LIST_MATCHING,
		LIST_MATCHED,
		LIST_CNT,
	};

	enum TEAM_OPERATE
	{
		TOP_CREATE = 0,	// 创建小组
		TOP_DESTORY,	// 销毁小组	,, 这个暂时不实现
		TOP_START,		// 小组开始
		TOP_CANCELSTART	// 小组取消开始
	};

	enum TEAMMEMBER_OPERATE
	{ 
		TMOP_JOIN = 0,		// 加入
		TMOP_LEAVE,			// 离开
		TMOP_KICKMEMBER,	// 踢人
		TMOP_CHANGEPOS,		// 换位置
		TMOP_READY,			// 玩家开始
	};

	const int CMD_LOBBY_REQ_GATE_LOGIN			= 1;		// Lobby->GateSvr	请求登录
	const int CMD_GATE_RESP_LOBBY_LOGIN			= 2;		// GateSvr->Lobby	返回请求登录

	const int CMD_LOBBY_REQ_GATE_USERGAME		= 3;		// Lobby->GateSvr	请求玩家的游戏信息
	const int CMD_GATE_RESP_LOBBY_USERGAME		= 4;		// GateSvr->Lobby	返回玩家的游戏信息

	const int CMD_LOBBY_REQ_GATE_MATCHING		= 5;		// Lobby->GateSvr	请求约战
	const int CMD_GATE_RESP_LOBBY_MATCHING		= 6;		// GateSvr->Lobby	返回约战结果

	const int CMD_GATE_NOTE_LOBBY_MATCHED		= 7;		// GateSvr->Lobby	通知配对结果

	const int CMD_GATE_NOTE_LOBBY_OUT_GAME		= 8;		// GateSvr->Lobby	通知"被"离开游戏
	const int CMD_GATE_NOTE_LOBBY_DESTROY_GAME	= 9;		// GateSvr->Lobby	通知游戏结束

	const int CMD_LOBBY_NOTE_GATE_ALIVE			= 10;		// Lobby->GateSvr	心跳
	const int CMD_GATE_NOTE_LOBBY_ALIVE			= 11;		// GateSvr->Lobby	心跳

	const int CMD_LOBBY_NOTE_GATE_QUIT			= 12;		// Lobby->GateSvr	通知离开
	const int CMD_GATE_NOTE_LOBBY_QUIT			= 13;		// GateSvr->Lobby	通知关闭连接

	const int CMD_LOBBY_NOTE_GATE_LOCAL_INFO	= 14;		// Lobby->GateSvr	通知硬件信息

	const int CMD_LOBBY_REQ_GATE_QUIT_MATCHING	= 15;		// Lobby->GateSvr	请求"取消"约战
	const int CMD_GATE_RESP_LOBBY_QUIT_MATCHING	= 16;		// GateSvr->Lobby	返回"取消"约战的结果

	const int CMD_GATE_NOTE_LOBBY_SYSMSG		= 17;		// GateSvr->Lobby	发送公告
	const int CMD_LOBBY_NOTE_GATE_ERRMSG		= 18;		// Lobby->GateSvr	错误日志

	const int CMD_LOBBY_REQ_GATE_OP2TEAM		= 19;		// Lobby->GateSvr   组的操作
	const int CMD_GATE_RESP_LOBBY_OP2TEAM		= 20;

	const int CMD_GATE_NOTE_LOBBY_UPDATETEAM	= 21;		// 

	const int CMD_LOBBY_REQ_GATE_OP2TEAMMEMBER	= 22;		//
	const int CMD_GATE_RESP_LOBBY_OP2TEAMMEMBER	= 23;

	const int CMD_GATE_NOTE_LOBBY_UPDATETEAMMEMBER=24;		// 

	const int CMD_GATE_NOTE_LOBBY_EigenCodes	= 25;		// GateSvr->Lobby	发送外挂数据
	const int CMD_POST_CHATMSG					= 26;		// 


	const int CMD_GATE_NOTE_LOBBY_USER_CNT		= 200;		// GateSvr->Lobby	通知人数
	const int CMD_GATE_NOTE_LOBBY_USER_ACT		= 201;		// GateSvr->Lobby	通知人员变化

	struct Lobby_Req_Gate_Login : public Imba_TCPXY<CMD_LOBBY_REQ_GATE_LOGIN>
	{
		unsigned int user_id;
		unsigned int session_id;

		Lobby_Req_Gate_Login( void )
		{
			memset(this, 0, sizeof(Lobby_Req_Gate_Login));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Req_Gate_Login);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Req_Gate_Login);
		}
	};

	// GateSvr->Lobby	返回请求登录
	struct Gate_Resp_Lobby_Login : public Imba_TCPXY<CMD_GATE_RESP_LOBBY_LOGIN>
	{
		enum LOGIN_RES
		{
			LOGIN_SUCCESS=0,
			NOUSER,
			ERRDB,
			ERRPASSWD,
			LOGIN_TIMEOUT,
		};

		unsigned char res;
		unsigned char test_param;

		Gate_Resp_Lobby_Login( void )
		{
			memset(this, 0, sizeof(Gate_Resp_Lobby_Login));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Resp_Lobby_Login);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Resp_Lobby_Login);
		}
	};

	// Lobby->GateSvr	请求玩家的游戏信息
	struct Lobby_Req_Gate_UserGame : public Imba_TCPXY<CMD_LOBBY_REQ_GATE_USERGAME>
	{
		unsigned int user_id;
		unsigned int session_id;
		unsigned int game_id;

		Lobby_Req_Gate_UserGame( void )
		{
			memset(this, 0, sizeof(Lobby_Req_Gate_UserGame));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Req_Gate_UserGame);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Req_Gate_UserGame);
		}
	};

	struct Gate_Resp_Lobby_UserGame : public Imba_TCPXY<CMD_GATE_RESP_LOBBY_USERGAME>
	{
		unsigned int	coin;
		unsigned int	medal;
		unsigned int	expr;
		unsigned int	escape;

		unsigned int	total;
		unsigned int	win;
		unsigned char	continues;
		unsigned char	lastres;
		unsigned int	admin;

		Gate_Resp_Lobby_UserGame( void )
		{
			memset(this, 0, sizeof(Gate_Resp_Lobby_UserGame));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Resp_Lobby_UserGame);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Resp_Lobby_UserGame);
		}
	};

	struct Lobby_Req_Gate_Matching : public Imba_TCPXY<CMD_LOBBY_REQ_GATE_MATCHING>
	{
		unsigned int user_id;
		unsigned int session_id;
		unsigned int game_id;
		unsigned int mode_id;

		Lobby_Req_Gate_Matching( void )
		{
			memset(this, 0, sizeof(Lobby_Req_Gate_Matching));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Req_Gate_Matching);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Req_Gate_Matching);
		}
	};

	struct Gate_Resp_Lobby_Matching : public Imba_TCPXY<CMD_GATE_RESP_LOBBY_MATCHING>
	{
		enum {SUCCESS=0,AUTH_ERR,ERR_GAME,ADD_ERR};

		unsigned char	res;
		unsigned short	time;	// 估计用时

		Gate_Resp_Lobby_Matching( void )
		{
			memset(this, 0, sizeof(Gate_Resp_Lobby_Matching));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Resp_Lobby_Matching);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Resp_Lobby_Matching);
		}
	};

	// GateSvr->Lobby	通知配对结果
	struct Gate_Note_Lobby_Matched : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_MATCHED>
	{
		enum { MAX_DATA_LEN = 200 };
		enum {SUCCESS=0,FAIL,TIMEOUT};
		unsigned char	res;
		unsigned char	why;
		unsigned int	group_id;
		unsigned char	team_id;
		unsigned int	ip;
		unsigned short	port;
		unsigned int	broadcast_len;
		char			data[MAX_DATA_LEN];

		Gate_Note_Lobby_Matched( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_Matched));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_Matched)-MAX_DATA_LEN;
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_Matched)-MAX_DATA_LEN+broadcast_len;
		}
	};

	// GateSvr->Lobby	通知"被"离开游戏
	struct Gate_Note_Lobby_Out_Game	: public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_OUT_GAME>
	{
		enum {LEAVEGAME=0, GAMEEND};

		unsigned char why;

		Gate_Note_Lobby_Out_Game( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_Out_Game));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_Out_Game);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_Out_Game);
		}
	};

	// GateSvr->Lobby	通知游戏结束
	struct Gate_Note_Lobby_Destroy_Game : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_DESTROY_GAME>
	{
		enum RS_DESTROY_GAME
		{
			RS_DESTROY_GAME_NORMAL=0,	// 正常解散
			RS_DESTROY_GAME_NOUSER,	// 没用户
			RS_DESTROY_GAME_END,
		};

		unsigned char	res;
		// 游戏结果
		unsigned char	cur_res;
		unsigned char	dirty_data;			// 0xff
		unsigned char	b_add;				// 0xff 1(+) 0(-)
		unsigned int	upscore;
		unsigned int	coin;
		unsigned int	medal;
		unsigned int	escape;

		Gate_Note_Lobby_Destroy_Game( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_Destroy_Game));
			dirty_data = 0xff;
			b_add	   = 0xff;
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_Destroy_Game);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_Destroy_Game);
		}
	};

	struct Lobby_Note_Gate_Alive : public Imba_TCPXY<CMD_LOBBY_NOTE_GATE_ALIVE>
	{
		unsigned int user_id;
		unsigned int session_id;
		
		Lobby_Note_Gate_Alive( void )
		{
			memset(this, 0, sizeof(Lobby_Note_Gate_Alive));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Note_Gate_Alive);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Note_Gate_Alive);
		}
	};

	// GateSvr->Lobby	心跳
	struct Gate_Note_Lobby_Alive : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_ALIVE>
	{
		Gate_Note_Lobby_Alive( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_Alive));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_Alive);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_Alive);
		}
	};

	// Lobby->GateSvr	通知离开
	struct Lobby_Note_Gate_Quit : public Imba_TCPXY<CMD_LOBBY_NOTE_GATE_QUIT>
	{
		unsigned int user_id;
		unsigned int session_id;

		Lobby_Note_Gate_Quit( void )
		{
			memset(this, 0, sizeof(Lobby_Note_Gate_Quit));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Note_Gate_Quit);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Note_Gate_Quit);
		}
	};

	// GateSvr->Lobby	通知关闭连接
	struct Gate_Note_Lobby_Quit	: public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_QUIT>
	{
		enum {INVALID=0,RELOGIN};

		unsigned char res;

		Gate_Note_Lobby_Quit( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_Quit));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_Quit);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_Quit);
		}
	};

	// 不需要处理
	struct Lobby_Note_Gate_Local_Info : public Imba_TCPXY<CMD_LOBBY_NOTE_GATE_LOCAL_INFO>
	{
		enum { MAX_DATA_LEN = 200 };
		unsigned int	user_id;
		unsigned int	session_id;

		unsigned int	game_id;
		unsigned char	os_ver_cnt;
		unsigned char	game_ver_cnt;
		char			data[MAX_DATA_LEN];

		Lobby_Note_Gate_Local_Info( void )
		{
			memset(this, 0, sizeof(Lobby_Note_Gate_Local_Info));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Note_Gate_Local_Info)-MAX_DATA_LEN;
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Note_Gate_Local_Info)-MAX_DATA_LEN+os_ver_cnt+game_ver_cnt;
		}
	};

	struct Lobby_Req_Gate_Quit_Matching : public Imba_TCPXY<CMD_LOBBY_REQ_GATE_QUIT_MATCHING>
	{
		unsigned int	user_id;
		unsigned int	session_id;

		Lobby_Req_Gate_Quit_Matching( void )
		{
			memset(this, 0, sizeof(Lobby_Req_Gate_Quit_Matching));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Req_Gate_Quit_Matching);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Req_Gate_Quit_Matching);
		}
	};

	struct Gate_Resp_Lobby_Quit_Matching : public Imba_TCPXY<CMD_GATE_RESP_LOBBY_QUIT_MATCHING>
	{
		unsigned int res;

		Gate_Resp_Lobby_Quit_Matching( void )
		{
			memset(this, 0, sizeof(Gate_Resp_Lobby_Quit_Matching));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Resp_Lobby_Quit_Matching);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Resp_Lobby_Quit_Matching);
		}
	};

	struct Gate_Note_Lobby_SysMsg : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_SYSMSG>
	{
		enum{ TYPE_SYS = 0, TYPE_USER };
		enum{ MAX_NAME_LEN=50 };
		enum{ MAX_MESSAGE_LEN=512 };

		int		msgtype;					// 系统消息
		char	sender[MAX_NAME_LEN+1];		// 发送者
		int		msglen;						// 将\0计算在内
		char	message[MAX_MESSAGE_LEN+1];	// 

		Gate_Note_Lobby_SysMsg( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_SysMsg));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_SysMsg)-(MAX_MESSAGE_LEN+1);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_SysMsg)-(MAX_MESSAGE_LEN+1)+msglen;
		}
	};

	struct Lobby_Note_Gate_ErrMsg : public Imba_TCPXY<CMD_LOBBY_NOTE_GATE_ERRMSG>
	{
		enum { MAX_ERRMSG_LEN=1024 };

		unsigned int	user_id;
		unsigned int	session_id;
		int				logmode;				// logmode由客户端定义， 外挂，没有地图，数据传输错误
		int				len;					// 包括\0 的长度
		char			msg[MAX_ERRMSG_LEN+1];	// string \0结束

		Lobby_Note_Gate_ErrMsg( void )		
		{ 
			memset(this, 0, sizeof(Lobby_Note_Gate_ErrMsg));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Lobby_Note_Gate_ErrMsg)-(MAX_ERRMSG_LEN+1);
		}
		const int get_minlen( void )
		{
			return sizeof(Lobby_Note_Gate_ErrMsg)-(MAX_ERRMSG_LEN+1)+len;
		}
	};

	struct Lobby_Req_Gate_OP2Team : public Imba_TCPXY<CMD_LOBBY_REQ_GATE_OP2TEAM>
	{
		unsigned int	user_id;
		unsigned int	session_id;
		int		op;
		int		config;			// 扩展配置

		Lobby_Req_Gate_OP2Team( void )
		{
			memset(this, 0, sizeof(Lobby_Req_Gate_OP2Team));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Req_Gate_OP2Team);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Req_Gate_OP2Team);
		}

	};

	struct Gate_Resp_Lobby_OP2Team : public Imba_TCPXY<CMD_GATE_RESP_LOBBY_OP2TEAM>
	{
		enum FLAG
		{
			SUCCEED = 0,
			ERR_OP,
			ERR_GAME,
			NEED_LEADERPOWER,	// 需要队长权限
			ERR_TEAMSTATUS,		// 错误的队伍状态
			WAIT_FULL_MEMBER,
		};

		int		flag;		
		int		op;

		Gate_Resp_Lobby_OP2Team( void )
		{
			memset(this, 0, sizeof(Gate_Resp_Lobby_OP2Team));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Resp_Lobby_OP2Team);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Resp_Lobby_OP2Team);
		}
	};

	struct Gate_Note_Lobby_UpdateTeam : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_UPDATETEAM>
	{
		enum TEAM_OPERATOR
		{
			TEAM_APPEAR = 0,	// 出现小组
			TEAM_DISAPPEAR,		// 消失小组
			TEAM_UPDATE,
		};
		enum TEAM_STATUS
		{ 
			TEAM_NORMAL,		// 小组普通状态
			TEAM_START,			// 小组游戏状态
		};

		int		teamid;
		int		op;
		int		status;

		Gate_Note_Lobby_UpdateTeam( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_UpdateTeam));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_UpdateTeam);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_UpdateTeam);
		}
	};

	struct Lobby_Req_Gate_OP2TeamMember : public Imba_TCPXY<CMD_LOBBY_REQ_GATE_OP2TEAMMEMBER>
	{
		unsigned int	user_id;
		unsigned int	session_id;
		int				op;
		union
		{
			int		kicked_userid;	// 踢人的时候有效, 
			int		teamid;			// 加入的时候有效
			int		pos;			// 换位置的位置
		};

		Lobby_Req_Gate_OP2TeamMember( void )
		{
			memset(this, 0, sizeof(Lobby_Req_Gate_OP2TeamMember));
		}

		const int get_minlen( void )
		{
			return sizeof(Lobby_Req_Gate_OP2TeamMember);
		}

		const int get_msglen( void )
		{
			return sizeof(Lobby_Req_Gate_OP2TeamMember);
		}
	};

	struct Gate_Resp_Lobby_OP2TeamMember : public Imba_TCPXY<CMD_GATE_RESP_LOBBY_OP2TEAMMEMBER>
	{
		enum FLAG
		{
			SUCCEED = 0,
			ERR_OP,
			ERR_GAME,
			NEED_LEADERPOWER,	// 需要队长权限
			OVERSIZE,			// 队伍人员已满
			ERR_TEAMSTATUS,		// 错误的队伍状态, 队伍已经开始或者队伍游戏中
			NO_KICKEDUSER,		// 找不到被踢的人
			NO_KICKSELF,		// 不能踢自己
			ERR_TEAM,
			ERR_POS,			// 错误的位置
			HAS_USERPOS,		// 座位已经有人了
			SAME_USERPOS,		// 换位置的时候，这个位置就是当前位置
		};

		int		flag;
		int		op;

		Gate_Resp_Lobby_OP2TeamMember( void )
		{
			memset(this, 0, sizeof(Gate_Resp_Lobby_OP2TeamMember));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Resp_Lobby_OP2TeamMember);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Resp_Lobby_OP2TeamMember);
		}
	};

	struct Gate_Note_Lobby_UpdateTeamMember : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_UPDATETEAMMEMBER>
	{
		enum OPERATE
		{ 
			ADD_TEAMMEMBER = 0,	// 添加一个成员
			DEC_TEAMMEMBER,		// 减少
			UPDATE_TEAMMEMBER,	// 更新成员
		};

		enum UPDATETYPE
		{
			UPDATE_BY_SELF = 0,
			UPDATE_BY_KICK,		// 只有踢人的时候出现
			UPDATE_BY_SYS,
		};

		enum ROLE
		{
			MEMBER	= 0,
			LEADER,
		};

		enum STATUS
		{
			S_WAIT	= 0,
			S_READY,
			S_INQUEUE,
			S_GAME,
		};

		int		op;
		int		type;
		int		userid;
		char	nickname[51];
		int		win;			//  赢的盘数
		int		lose;			// 逃跑率
		int		medal;			// 计分等级
		int		role;
		int		teamid;
		int		pos;
		int		status;

		Gate_Note_Lobby_UpdateTeamMember( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_UpdateTeamMember));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_UpdateTeamMember);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_UpdateTeamMember);
		}
	};

	struct Gate_Note_Lobby_EigenCodes : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_EigenCodes>
	{
		enum { MAX_EIGENCODE_LEN = 2000 };
		enum { MAX_EIGENCODE_LEN2 = MAX_EIGENCODE_LEN+1 };

		unsigned int	game_id;
		unsigned int	totalcodecount;
		unsigned int	codelen;
		char			eigencodes[MAX_EIGENCODE_LEN2];

		Gate_Note_Lobby_EigenCodes( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_EigenCodes));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_EigenCodes)-MAX_EIGENCODE_LEN2*sizeof(char);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_EigenCodes)+(codelen-MAX_EIGENCODE_LEN2)*sizeof(char);
		}
	};

	struct Post_ChatMsg : public Imba_TCPXY<CMD_POST_CHATMSG>
	{
		enum { MAX_MSGLEN = 2000 };
		enum MsgType
		{  
			MT_P2P	   = 0,	// 单对单
			MT_ORGTEAM,		// 共同组队的队伍内广播
			MT_GAMETEAM,	// 游戏队伍内广播
			MT_GATE,		// gate服务广播
		};

		int		msgtype;
		int		fromuserid;
		int		touserid;
		char	fromnickname[LL_NICKNAME_MAX+1];
		char	tonickname[LL_NICKNAME_MAX+1];
		int		msglen;
		char	msg[MAX_MSGLEN];	// utf8

		Post_ChatMsg( void )
		{
			memset(this, 0, sizeof(Post_ChatMsg));
		}

		const int get_minlen( void )
		{
			return sizeof(Post_ChatMsg)-MAX_MSGLEN*sizeof(char);
		}

		const int get_msglen( void )
		{
			return sizeof(Post_ChatMsg)+(msglen-MAX_MSGLEN)*sizeof(char);
		}
	};

	struct Gate_Note_Lobby_User_Cnt : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_USER_CNT>
	{
		enum { MAX_REPORT_CNT = 500 };
		unsigned int	game_id;
		unsigned int	cnt;
		User_Report		users[MAX_REPORT_CNT];

		Gate_Note_Lobby_User_Cnt( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_User_Cnt));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_User_Cnt)-MAX_REPORT_CNT*sizeof(User_Report);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_User_Cnt)+(cnt-MAX_REPORT_CNT)*sizeof(User_Report);
		}
	};

	struct Gate_Note_Lobby_User_Act : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_USER_ACT>
	{
		enum { MAX_USER_COUNT=20 };
		unsigned char	move_from;	// 参考LIST_TYPE
		unsigned char	move_to;
		unsigned int	cnt;
		unsigned int	user_ids[MAX_USER_COUNT];

		Gate_Note_Lobby_User_Act( void )
		{
			memset(this, 0, sizeof(Gate_Note_Lobby_User_Act));
		}

		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Lobby_User_Act)-MAX_USER_COUNT*sizeof(unsigned int);
		}

		const int get_msglen( void )
		{
			return sizeof(Gate_Note_Lobby_User_Act)+(cnt-MAX_USER_COUNT)*sizeof(unsigned int);
		}
	};
};

#pragma pack(pop)

