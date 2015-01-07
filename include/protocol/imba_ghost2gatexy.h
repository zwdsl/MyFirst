
#pragma once

#include "imba_basexy.h"
#include "imba_structs.h"
#include "ImbaUUID.h"

#pragma pack(push)
#pragma pack(1)

namespace Imba_Ghost2GateXY_Old
{
	struct User_Del_Ghost
	{
		unsigned int user_id;
		unsigned char team_id;
	};

	struct User_Add_Ghost
	{
		unsigned int	user_id;				// 用户id
		unsigned int	session_id;				// 动态密码
		char			nick_name[30];
		int				coin;					// 金币信息
		int				medal;					// 荣誉信息
		unsigned char	team_id;				// 战队id
	};

	struct User_GhostResult
	{
		unsigned int	user_id;				// 用户id
		
		int				hero;					// 英雄
		int				herolevel;				// 英雄等级

		int				herokill;				// 杀死对方英雄次数
		int				herodeath;				// 死亡次数
		int				soldierkill;			// 刷兵个数
		int				soldierkillB;			// 刷兵数(反)
		int				heroassist;				//	助攻数
		int				gold;					//	金钱
		int				soldierkillanimal;		//	杀野数

		int				chenghao;				//(mvp,杀,死，助)
		int				dianjishu;				//apm值
		int				pos_id;					//槽位id

		int 			clientip;				// 客户端ip
		int 			ghostsvrip;				// 服务器ip
		
		long long		leave_tm;				// 1 逃跑
		unsigned char	game_team;				// 游戏阵营
		unsigned char	game_result;			// 0 lose ; 1 win;
		unsigned char	game_rank;				// 游戏名次

		unsigned char	cheater;				// 0 正常，1图挂 2其他挂
		int				chaita;					// 拆塔
		int				chaitaB;				// 反补塔

		int				ImbaCoin;				//活动金币

		int				messenger;				//动物信使
		int				flymessenger;			//飞行信使
		int				guard;					//岗哨守卫
		int				guardB;					//侦查守卫
		int				fog;					//诡异之雾
		int				dust;					//显影之尘
		int				drummer;				//战鼓
		int				mekansm;				//梅肯

		int				doublekill;				//双杀
		int				triplekill;				//三杀
		int				ultrakill;				//四杀
		int				rampage;				//五杀
	};

	const int GHOST_NOTE_GATE_OK				= 1;	// GhostSvr->GateSvr发送初始化完成信息

	const int GATE_REQ_GHOST_CREATE_GAME		= 4;	// GateSvr->GhostSvr请求创建游戏
	const int GHOST_RESP_GATE_CREATE_GAME		= 5;	// GhostSvr->GateSvr回复游戏创建结果

	const int GHOST_NOTE_GATE_START_GAME		= 6;	// GhostSvr->GateSvr通知开始游戏
	const int GHOST_NOTE_GATE_DESTROY_GAME		= 7;	// GhostSvr->GateSvr通知结束游戏

	const int GHOST_REQ_GATE_BEG_USER			= 8;	// GhostSvr->GateSvr请求添加玩家
	const int GATE_RESP_GHOST_BEG_USER			= 9;	// GateSvr->GhostSvr添加玩家

	const int GATE_NODE_GHOST_DESTROY_GAME		= 10;	// GateSvr->GhostSvr强制关闭游戏

	const int GHOST_NOTE_GATE_REMOVE_USER		= 11;	// GhostSvr->GateSvr删除玩家
	const int GATE_NOTE_GHOST_REMOVE_USER		= 12;	// GateSvr->GhostSvr删除玩家

	const int GHOST_NOTE_GATE_USECOIN			= 13;	// GhostSvr->GateSvr 使用金币
	const int GHOST_NOTE_GATE_GETPROP			= 14;	// GhostSvr->GateSvr 获得道具

	const int GHOST_NOTE_GATE_LOGFATAL			= 15;	// GhostSvr->GateSvr 写ghost异常日志
	const int GHOST_NOTE_GATE_BEG_RES			= 16;	// GhostSvr->GateSvr通知请求添加的结果

	const int TCP_HEARTBEAT						= 1000;	// 


	struct	Ghost_Note_Gate_OK : public Imba_TCPXY<GHOST_NOTE_GATE_OK>
	{
		int				ghostid;	// gate服务appid
		unsigned int	port;

		Ghost_Note_Gate_OK( void )		{ memset(this, 0, sizeof(Ghost_Note_Gate_OK));	}
		const int get_msglen( void )	{ return sizeof(Ghost_Note_Gate_OK);			}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_OK);
		}
	};

	struct Gate_Req_Ghost_Create_Game : public Imba_TCPXY<GATE_REQ_GHOST_CREATE_GAME>
	{
		enum { MAX_USER_COUNT=20, MAX_PROP_COUNT=10,MAX_MODE_INFO_COUNT=10, };
		unsigned int		game_id;		// 游戏id
		unsigned short		model_id;		// 模式id(如:地图id)
		unsigned int		group_id;		// gatesvr生成 队伍id(队伍里包含战队)
		char				props[MAX_PROP_COUNT+1];// 道具随机值,	\0				
		int					modeid;
		char				modeinfo[MAX_MODE_INFO_COUNT];
		unsigned char		user_cnt;		// 用户数
		User_Add_Ghost		users[MAX_USER_COUNT];

		

		Gate_Req_Ghost_Create_Game( void )		
		{ 
			memset(this, 0, sizeof(Gate_Req_Ghost_Create_Game));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Gate_Req_Ghost_Create_Game)
				-MAX_USER_COUNT*sizeof(User_Add_Ghost)
				+user_cnt*sizeof(User_Add_Ghost);
		}
		const int get_minlen( void )
		{
			return sizeof(Gate_Req_Ghost_Create_Game)-MAX_USER_COUNT*sizeof(User_Add_Ghost);
		}
	};

	struct	Ghost_Resp_Gate_Create_Game : public Imba_TCPXY<GHOST_RESP_GATE_CREATE_GAME>
	{
		enum { MAX_GAME_DATA_CNT=2048 };
		enum RS_CREATE_GAME
		{
			RS_CREATE_GAME_SUCCESS=0,
			//……
			RS_CREATE_GAME_END,
		};
		unsigned int	game_id;
		unsigned int	group_id;
		unsigned char	result;
		unsigned short	data_cnt;
		char			game_data[MAX_GAME_DATA_CNT];	// 游戏相关的信息(如魔兽争霸-广播内容)

		Ghost_Resp_Gate_Create_Game( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Resp_Gate_Create_Game));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Resp_Gate_Create_Game)
				-MAX_GAME_DATA_CNT*sizeof(char)
				+data_cnt*sizeof(char);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Resp_Gate_Create_Game)-MAX_GAME_DATA_CNT*sizeof(char);
		}
	};

	// GhostSvr->GateSvr通知开始游戏 
	struct	Ghost_Note_Gate_Start_Game : public Imba_TCPXY<GHOST_NOTE_GATE_START_GAME>
	{
		unsigned int	game_id;
		unsigned int	group_id;

		Ghost_Note_Gate_Start_Game( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Note_Gate_Start_Game));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Note_Gate_Start_Game);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_Start_Game);
		}
	};


	// GhostSvr->GateSvr通知结束游戏
	struct	Ghost_Note_Gate_Destroy_Game : public Imba_TCPXY<GHOST_NOTE_GATE_DESTROY_GAME>
	{
		enum { MAX_USER_COUNT = 20 };
		enum GameEndType
		{
			GET_NORMAL = 0,
			GET_INVALID,
			GET_ILLEGAL,
		};

		unsigned int		game_id;
		unsigned int		group_id;
		unsigned char		endtype;		
		unsigned char		count;
		long long			begintm;
		long long			endtm;
		User_GhostResult	users[MAX_USER_COUNT];

		Ghost_Note_Gate_Destroy_Game( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Note_Gate_Destroy_Game));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Note_Gate_Destroy_Game)
				-MAX_USER_COUNT*sizeof(User_GhostResult)
				+count*sizeof(User_GhostResult);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_Destroy_Game)-MAX_USER_COUNT*sizeof(User_GhostResult);
		}
	};

	// GhostSvr->GateSvr请求添加玩家
	struct	Ghost_Req_Gate_Beg_User : public Imba_TCPXY<GHOST_REQ_GATE_BEG_USER>
	{
		enum { MAX_USER_COUNT=20 };
		unsigned int	game_id;
		unsigned int	group_id;
		unsigned char	user_cnt;
		User_Del_Ghost	users[MAX_USER_COUNT];

		Ghost_Req_Gate_Beg_User( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Req_Gate_Beg_User));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Req_Gate_Beg_User)
				-MAX_USER_COUNT*sizeof(User_Del_Ghost)
				+user_cnt*sizeof(User_Del_Ghost);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Req_Gate_Beg_User)-MAX_USER_COUNT*sizeof(User_Del_Ghost);
		}
	};

	// GateSvr->GhostSvr添加玩家
	struct Gate_Resp_Ghost_Beg_User : public Imba_TCPXY<GATE_RESP_GHOST_BEG_USER>
	{
		enum { MAX_USER_COUNT=20 };
		unsigned int	game_id;
		unsigned int	group_id;
		unsigned char	user_cnt;	// 用户数(若0表示添加失败)
		User_Add_Ghost	users[MAX_USER_COUNT];

		Gate_Resp_Ghost_Beg_User( void )		
		{ 
			memset(this, 0, sizeof(Gate_Resp_Ghost_Beg_User));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Gate_Resp_Ghost_Beg_User)
				-MAX_USER_COUNT*sizeof(User_Add_Ghost)
				+user_cnt*sizeof(User_Add_Ghost);
		}
		const int get_minlen( void )
		{
			return sizeof(Gate_Resp_Ghost_Beg_User)-MAX_USER_COUNT*sizeof(User_Add_Ghost);
		}
	};

	// GhostSvr->GateSvr通知请求添加的结果
	struct Ghost_Note_Gate_Beg_Res : public Imba_TCPXY<GHOST_NOTE_GATE_BEG_RES>
	{
		enum { MAX_GAME_DATA_CNT=2048 };
		enum RS_BEG
		{
			RS_BEG_SUCCESS=0,
			//……
			RS_BEG_END,
		};

		unsigned int	game_id;
		unsigned int	group_id;
		unsigned char	result;
		unsigned short	data_cnt;
		char			game_data[MAX_GAME_DATA_CNT];	// 游戏相关的信息(如魔兽争霸-广播内容)

		Ghost_Note_Gate_Beg_Res( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Note_Gate_Beg_Res));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Note_Gate_Beg_Res)
				-MAX_GAME_DATA_CNT*sizeof(char)
				+data_cnt*sizeof(char);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_Beg_Res)-MAX_GAME_DATA_CNT*sizeof(char);
		}
	};

	struct Gate_Note_Ghost_Destroy_Game : public Imba_TCPXY<GATE_NODE_GHOST_DESTROY_GAME>
	{
		unsigned int game_id;	// 若game=0,该ghost所有game
		unsigned int group_id;	// 若group_id=0,该游戏所有group

		Gate_Note_Ghost_Destroy_Game( void )		
		{ 
			memset(this, 0, sizeof(Gate_Note_Ghost_Destroy_Game));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Gate_Note_Ghost_Destroy_Game);
		}
		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Ghost_Destroy_Game);
		}
	};

	// GhostSvr->GateSvr通知删除玩家
	struct Ghost_Note_Gate_Remove_User : public Imba_TCPXY<GHOST_NOTE_GATE_REMOVE_USER>
	{
		unsigned int game_id;
		unsigned int group_id;
		unsigned int user_id;

		Ghost_Note_Gate_Remove_User( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Note_Gate_Remove_User));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Note_Gate_Remove_User);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_Remove_User);
		}
	};

	// GateSvr->GhostSvr通知删除玩家
	struct Gate_Note_Ghost_Remove_User : public Imba_TCPXY<GATE_NOTE_GHOST_REMOVE_USER>
	{
		unsigned int game_id;
		unsigned int group_id;
		unsigned int user_id;

		Gate_Note_Ghost_Remove_User( void )		
		{ 
			memset(this, 0, sizeof(Gate_Note_Ghost_Remove_User));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Gate_Note_Ghost_Remove_User);
		}
		const int get_minlen( void )
		{
			return sizeof(Gate_Note_Ghost_Remove_User);
		}
	};

	struct Ghost_Note_Gate_UseCoin : public Imba_TCPXY<GHOST_NOTE_GATE_USECOIN>
	{
		unsigned int		user_id;
		unsigned int		game_id;
		unsigned int		group_id;
		unsigned int		used_coin;
		unsigned int		propid;

		Ghost_Note_Gate_UseCoin( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Note_Gate_UseCoin));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Note_Gate_UseCoin);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_UseCoin);
		}
	};

	struct Ghost_Note_Gate_GetProp : public Imba_TCPXY<GHOST_NOTE_GATE_GETPROP>
	{
		unsigned int		user_id;
		unsigned int		game_id;
		unsigned int		group_id;
		unsigned char		got_prop;

		Ghost_Note_Gate_GetProp( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Note_Gate_GetProp));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Note_Gate_GetProp);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_GetProp);
		}
	};

	struct Ghost_Note_Gate_LogFatal : public Imba_TCPXY<GHOST_NOTE_GATE_LOGFATAL>
	{	
		enum { MAX_FATAL_LEN=1024 };
		int				logmode;	// logmode由ghost定义， 外挂，没有地图，数据传输错误
		unsigned int	user_id;
		unsigned int	game_id;
		unsigned int	group_id;
		char			user_ip[32];	// string \0结束 
		int				len;			// 包括\0 的长度
		char			msg[MAX_FATAL_LEN+1];// string \0结束

		Ghost_Note_Gate_LogFatal( void )		
		{ 
			memset(this, 0, sizeof(Ghost_Note_Gate_LogFatal));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(Ghost_Note_Gate_LogFatal)-(MAX_FATAL_LEN+1);
		}
		const int get_minlen( void )
		{
			return sizeof(Ghost_Note_Gate_LogFatal)-(MAX_FATAL_LEN+1)+len;
		}
	};

	struct HeartBeat : public Imba_TCPXY<TCP_HEARTBEAT>
	{
		HeartBeat( void )		
		{ 
			memset(this, 0, sizeof(HeartBeat));	
		}
		const int get_msglen( void )			
		{ 
			return sizeof(HeartBeat);
		}
		const int get_minlen( void )
		{
			return sizeof(HeartBeat);
		}
	};
};

#pragma pack(pop)

