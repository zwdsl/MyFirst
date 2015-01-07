
#pragma once

#include "imba_basexy.h"
#include "imba_structs.h"
#include "ImbaUUID.h"

namespace Imba_Client2GateXY
{
	const int XY_BEGIN				= IMBA_CLIENT2GATEXY_BEGIN;
	const int XY_END				= IMBA_CLIENT2GATEXY_END;

	const int UDP_GETXY_BEGIN			= XY_BEGIN+60;
	const Imba_UdpXY UDP_GET_SVRSTATUS	= Imba_UdpXY(UDP_GETXY_BEGIN+1, "get svrstatus");	// ��ȡ�������״̬
	const Imba_UdpXY UDP_GET_ONLINES	= Imba_UdpXY(UDP_GETXY_BEGIN+2, "get onlines");		// ��ȡ������������
	const Imba_UdpXY UDP_GET_GAMESTATUS	= Imba_UdpXY(UDP_GETXY_BEGIN+3, "get gamestatus");	// ��ȡ��Ϸ״̬
	const Imba_UdpXY UDP_GET_GAMECONFIG	= Imba_UdpXY(UDP_GETXY_BEGIN+4, "get gameconfig");	// ��ȡ��Ϸ����״̬
	const Imba_UdpXY UDP_GET_GHOSTDNS	= Imba_UdpXY(UDP_GETXY_BEGIN+5, "get ghostdns");	// ���ghost·�ɷ�ʽ
	const Imba_UdpXY UDP_GET_GHOSTONLINE= Imba_UdpXY(UDP_GETXY_BEGIN+6, "get ghostonline");	// ���ghost��������
	const Imba_UdpXY UDP_GET_USERID		= Imba_UdpXY(UDP_GETXY_BEGIN+7, "get userid", 1);	// �����û����ǳƻ���û�������id
	const Imba_UdpXY UDP_GET_USERCOUNT	= Imba_UdpXY(UDP_GETXY_BEGIN+8, "get usercount", 2);// get usercount >(<) 200 ��ȡ������medal>200����������
	const Imba_UdpXY UDP_GET_EIGENCODES = Imba_UdpXY(UDP_GETXY_BEGIN+9, "get eigencodes");	// ��ȡeigencodes����
	const Imba_UdpXY UDP_GET_MATCHLIST	= Imba_UdpXY(UDP_GETXY_BEGIN+10,"get matchlist", 1);	// ���´�db��ȡmatchlist����


	const int UDP_SETXY_BEGIN			= XY_BEGIN+80;
	const Imba_UdpXY UDP_SET_LOGLEVEL	= Imba_UdpXY(UDP_SETXY_BEGIN+1, "set loglevel", 1);		// ������־�ȼ�
	const Imba_UdpXY UDP_SET_GAMECONFIG	= Imba_UdpXY(UDP_SETXY_BEGIN+2, "set gameconfig", 1);	// 
	const Imba_UdpXY UDP_SET_GHOSTDNS	= Imba_UdpXY(UDP_SETXY_BEGIN+3, "set ghostdns", 1);		// ����ghost·�ɷ�ʽ
	const Imba_UdpXY UDP_SET_MAINGHOST	= Imba_UdpXY(UDP_SETXY_BEGIN+4, "set mainghost", 1);	// ��������·�ɷ�ʽ�� ����
	const Imba_UdpXY UDP_SET_GHOSTMAXONLINE=Imba_UdpXY(UDP_SETXY_BEGIN+5, "set ghostmaxonline", 2);// ����ghost�������
	const Imba_UdpXY UDP_KICK_USER		= Imba_UdpXY(UDP_SETXY_BEGIN+6, "kick user", 1);		// ����
	const Imba_UdpXY UDP_SEND_SYSMSG	= Imba_UdpXY(UDP_SETXY_BEGIN+7, "send sysmsg", 2);		// ����ϵͳ��Ϣ
	const Imba_UdpXY UDP_BANON_USER		= Imba_UdpXY(UDP_SETXY_BEGIN+8, "ban on", 1);			// ���ʺ�
	const Imba_UdpXY UDP_BANOFF_USER	= Imba_UdpXY(UDP_SETXY_BEGIN+9, "ban off", 1);			// ����ʺ�
	const Imba_UdpXY UDP_UPDATE_GHOSTCFG= Imba_UdpXY(UDP_SETXY_BEGIN+10,"update ghostcfg");		// ���´�db��ȡghost����
	const Imba_UdpXY UDP_UPDATE_EIGENCODES=Imba_UdpXY(UDP_SETXY_BEGIN+11,"update eigencodes" );	// ���´�db��ȡeigencodes����
	const Imba_UdpXY UDP_UPDATE_MATCHLIST=Imba_UdpXY(UDP_SETXY_BEGIN+12,"update matchlist", 1);	// ���´�db��ȡmatchlist����
	const Imba_UdpXY UDP_END_MATCHLIST	 =Imba_UdpXY(UDP_SETXY_BEGIN+13,"end matchlist", 1);			// ��ֹmatchlist����
	const Imba_UdpXY UDP_ENABLE_CHRISTMAS	 =Imba_UdpXY(UDP_SETXY_BEGIN+14,"enable christmas", 1);			// christmas����
	

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
		TOP_CREATE = 0,	// ����С��
		TOP_DESTORY,	// ����С��	,, �����ʱ��ʵ��
		TOP_START,		// С�鿪ʼ
		TOP_CANCELSTART	// С��ȡ����ʼ
	};

	enum TEAMMEMBER_OPERATE
	{ 
		TMOP_JOIN = 0,		// ����
		TMOP_LEAVE,			// �뿪
		TMOP_KICKMEMBER,	// ����
		TMOP_CHANGEPOS,		// ��λ��
		TMOP_READY,			// ��ҿ�ʼ
	};

	const int CMD_LOBBY_REQ_GATE_LOGIN			= 1;		// Lobby->GateSvr	�����¼
	const int CMD_GATE_RESP_LOBBY_LOGIN			= 2;		// GateSvr->Lobby	���������¼

	const int CMD_LOBBY_REQ_GATE_USERGAME		= 3;		// Lobby->GateSvr	������ҵ���Ϸ��Ϣ
	const int CMD_GATE_RESP_LOBBY_USERGAME		= 4;		// GateSvr->Lobby	������ҵ���Ϸ��Ϣ

	const int CMD_LOBBY_REQ_GATE_MATCHING		= 5;		// Lobby->GateSvr	����Լս
	const int CMD_GATE_RESP_LOBBY_MATCHING		= 6;		// GateSvr->Lobby	����Լս���

	const int CMD_GATE_NOTE_LOBBY_MATCHED		= 7;		// GateSvr->Lobby	֪ͨ��Խ��

	const int CMD_GATE_NOTE_LOBBY_OUT_GAME		= 8;		// GateSvr->Lobby	֪ͨ"��"�뿪��Ϸ
	const int CMD_GATE_NOTE_LOBBY_DESTROY_GAME	= 9;		// GateSvr->Lobby	֪ͨ��Ϸ����

	const int CMD_LOBBY_NOTE_GATE_ALIVE			= 10;		// Lobby->GateSvr	����
	const int CMD_GATE_NOTE_LOBBY_ALIVE			= 11;		// GateSvr->Lobby	����

	const int CMD_LOBBY_NOTE_GATE_QUIT			= 12;		// Lobby->GateSvr	֪ͨ�뿪
	const int CMD_GATE_NOTE_LOBBY_QUIT			= 13;		// GateSvr->Lobby	֪ͨ�ر�����

	const int CMD_LOBBY_NOTE_GATE_LOCAL_INFO	= 14;		// Lobby->GateSvr	֪ͨӲ����Ϣ

	const int CMD_LOBBY_REQ_GATE_QUIT_MATCHING	= 15;		// Lobby->GateSvr	����"ȡ��"Լս
	const int CMD_GATE_RESP_LOBBY_QUIT_MATCHING	= 16;		// GateSvr->Lobby	����"ȡ��"Լս�Ľ��

	const int CMD_GATE_NOTE_LOBBY_SYSMSG		= 17;		// GateSvr->Lobby	���͹���
	const int CMD_LOBBY_NOTE_GATE_ERRMSG		= 18;		// Lobby->GateSvr	������־

	const int CMD_LOBBY_REQ_GATE_OP2TEAM		= 19;		// Lobby->GateSvr   ��Ĳ���
	const int CMD_GATE_RESP_LOBBY_OP2TEAM		= 20;

	const int CMD_GATE_NOTE_LOBBY_UPDATETEAM	= 21;		// 

	const int CMD_LOBBY_REQ_GATE_OP2TEAMMEMBER	= 22;		//
	const int CMD_GATE_RESP_LOBBY_OP2TEAMMEMBER	= 23;

	const int CMD_GATE_NOTE_LOBBY_UPDATETEAMMEMBER=24;		// 

	const int CMD_GATE_NOTE_LOBBY_EigenCodes	= 25;		// GateSvr->Lobby	�����������
	const int CMD_POST_CHATMSG					= 26;		// 


	const int CMD_GATE_NOTE_LOBBY_USER_CNT		= 200;		// GateSvr->Lobby	֪ͨ����
	const int CMD_GATE_NOTE_LOBBY_USER_ACT		= 201;		// GateSvr->Lobby	֪ͨ��Ա�仯

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

	// GateSvr->Lobby	���������¼
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

	// Lobby->GateSvr	������ҵ���Ϸ��Ϣ
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
		unsigned short	time;	// ������ʱ

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

	// GateSvr->Lobby	֪ͨ��Խ��
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

	// GateSvr->Lobby	֪ͨ"��"�뿪��Ϸ
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

	// GateSvr->Lobby	֪ͨ��Ϸ����
	struct Gate_Note_Lobby_Destroy_Game : public Imba_TCPXY<CMD_GATE_NOTE_LOBBY_DESTROY_GAME>
	{
		enum RS_DESTROY_GAME
		{
			RS_DESTROY_GAME_NORMAL=0,	// ������ɢ
			RS_DESTROY_GAME_NOUSER,	// û�û�
			RS_DESTROY_GAME_END,
		};

		unsigned char	res;
		// ��Ϸ���
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

	// GateSvr->Lobby	����
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

	// Lobby->GateSvr	֪ͨ�뿪
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

	// GateSvr->Lobby	֪ͨ�ر�����
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

	// ����Ҫ����
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

		int		msgtype;					// ϵͳ��Ϣ
		char	sender[MAX_NAME_LEN+1];		// ������
		int		msglen;						// ��\0��������
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
		int				logmode;				// logmode�ɿͻ��˶��壬 ��ң�û�е�ͼ�����ݴ������
		int				len;					// ����\0 �ĳ���
		char			msg[MAX_ERRMSG_LEN+1];	// string \0����

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
		int		config;			// ��չ����

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
			NEED_LEADERPOWER,	// ��Ҫ�ӳ�Ȩ��
			ERR_TEAMSTATUS,		// ����Ķ���״̬
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
			TEAM_APPEAR = 0,	// ����С��
			TEAM_DISAPPEAR,		// ��ʧС��
			TEAM_UPDATE,
		};
		enum TEAM_STATUS
		{ 
			TEAM_NORMAL,		// С����ͨ״̬
			TEAM_START,			// С����Ϸ״̬
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
			int		kicked_userid;	// ���˵�ʱ����Ч, 
			int		teamid;			// �����ʱ����Ч
			int		pos;			// ��λ�õ�λ��
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
			NEED_LEADERPOWER,	// ��Ҫ�ӳ�Ȩ��
			OVERSIZE,			// ������Ա����
			ERR_TEAMSTATUS,		// ����Ķ���״̬, �����Ѿ���ʼ���߶�����Ϸ��
			NO_KICKEDUSER,		// �Ҳ������ߵ���
			NO_KICKSELF,		// �������Լ�
			ERR_TEAM,
			ERR_POS,			// �����λ��
			HAS_USERPOS,		// ��λ�Ѿ�������
			SAME_USERPOS,		// ��λ�õ�ʱ�����λ�þ��ǵ�ǰλ��
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
			ADD_TEAMMEMBER = 0,	// ���һ����Ա
			DEC_TEAMMEMBER,		// ����
			UPDATE_TEAMMEMBER,	// ���³�Ա
		};

		enum UPDATETYPE
		{
			UPDATE_BY_SELF = 0,
			UPDATE_BY_KICK,		// ֻ�����˵�ʱ�����
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
		int		win;			//  Ӯ������
		int		lose;			// ������
		int		medal;			// �Ʒֵȼ�
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
			MT_P2P	   = 0,	// ���Ե�
			MT_ORGTEAM,		// ��ͬ��ӵĶ����ڹ㲥
			MT_GAMETEAM,	// ��Ϸ�����ڹ㲥
			MT_GATE,		// gate����㲥
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
		unsigned char	move_from;	// �ο�LIST_TYPE
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

