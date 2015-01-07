
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
		unsigned int	user_id;				// �û�id
		unsigned int	session_id;				// ��̬����
		char			nick_name[30];
		int				coin;					// �����Ϣ
		int				medal;					// ������Ϣ
		unsigned char	team_id;				// ս��id
	};

	struct User_GhostResult
	{
		unsigned int	user_id;				// �û�id
		
		int				hero;					// Ӣ��
		int				herolevel;				// Ӣ�۵ȼ�

		int				herokill;				// ɱ���Է�Ӣ�۴���
		int				herodeath;				// ��������
		int				soldierkill;			// ˢ������
		int				soldierkillB;			// ˢ����(��)
		int				heroassist;				//	������
		int				gold;					//	��Ǯ
		int				soldierkillanimal;		//	ɱҰ��

		int				chenghao;				//(mvp,ɱ,������)
		int				dianjishu;				//apmֵ
		int				pos_id;					//��λid

		int 			clientip;				// �ͻ���ip
		int 			ghostsvrip;				// ������ip
		
		long long		leave_tm;				// 1 ����
		unsigned char	game_team;				// ��Ϸ��Ӫ
		unsigned char	game_result;			// 0 lose ; 1 win;
		unsigned char	game_rank;				// ��Ϸ����

		unsigned char	cheater;				// 0 ������1ͼ�� 2������
		int				chaita;					// ����
		int				chaitaB;				// ������

		int				ImbaCoin;				//����

		int				messenger;				//������ʹ
		int				flymessenger;			//������ʹ
		int				guard;					//��������
		int				guardB;					//�������
		int				fog;					//����֮��
		int				dust;					//��Ӱ֮��
		int				drummer;				//ս��
		int				mekansm;				//÷��

		int				doublekill;				//˫ɱ
		int				triplekill;				//��ɱ
		int				ultrakill;				//��ɱ
		int				rampage;				//��ɱ
	};

	const int GHOST_NOTE_GATE_OK				= 1;	// GhostSvr->GateSvr���ͳ�ʼ�������Ϣ

	const int GATE_REQ_GHOST_CREATE_GAME		= 4;	// GateSvr->GhostSvr���󴴽���Ϸ
	const int GHOST_RESP_GATE_CREATE_GAME		= 5;	// GhostSvr->GateSvr�ظ���Ϸ�������

	const int GHOST_NOTE_GATE_START_GAME		= 6;	// GhostSvr->GateSvr֪ͨ��ʼ��Ϸ
	const int GHOST_NOTE_GATE_DESTROY_GAME		= 7;	// GhostSvr->GateSvr֪ͨ������Ϸ

	const int GHOST_REQ_GATE_BEG_USER			= 8;	// GhostSvr->GateSvr����������
	const int GATE_RESP_GHOST_BEG_USER			= 9;	// GateSvr->GhostSvr������

	const int GATE_NODE_GHOST_DESTROY_GAME		= 10;	// GateSvr->GhostSvrǿ�ƹر���Ϸ

	const int GHOST_NOTE_GATE_REMOVE_USER		= 11;	// GhostSvr->GateSvrɾ�����
	const int GATE_NOTE_GHOST_REMOVE_USER		= 12;	// GateSvr->GhostSvrɾ�����

	const int GHOST_NOTE_GATE_USECOIN			= 13;	// GhostSvr->GateSvr ʹ�ý��
	const int GHOST_NOTE_GATE_GETPROP			= 14;	// GhostSvr->GateSvr ��õ���

	const int GHOST_NOTE_GATE_LOGFATAL			= 15;	// GhostSvr->GateSvr дghost�쳣��־
	const int GHOST_NOTE_GATE_BEG_RES			= 16;	// GhostSvr->GateSvr֪ͨ������ӵĽ��

	const int TCP_HEARTBEAT						= 1000;	// 


	struct	Ghost_Note_Gate_OK : public Imba_TCPXY<GHOST_NOTE_GATE_OK>
	{
		int				ghostid;	// gate����appid
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
		unsigned int		game_id;		// ��Ϸid
		unsigned short		model_id;		// ģʽid(��:��ͼid)
		unsigned int		group_id;		// gatesvr���� ����id(���������ս��)
		char				props[MAX_PROP_COUNT+1];// �������ֵ,	\0				
		int					modeid;
		char				modeinfo[MAX_MODE_INFO_COUNT];
		unsigned char		user_cnt;		// �û���
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
			//����
			RS_CREATE_GAME_END,
		};
		unsigned int	game_id;
		unsigned int	group_id;
		unsigned char	result;
		unsigned short	data_cnt;
		char			game_data[MAX_GAME_DATA_CNT];	// ��Ϸ��ص���Ϣ(��ħ������-�㲥����)

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

	// GhostSvr->GateSvr֪ͨ��ʼ��Ϸ 
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


	// GhostSvr->GateSvr֪ͨ������Ϸ
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

	// GhostSvr->GateSvr����������
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

	// GateSvr->GhostSvr������
	struct Gate_Resp_Ghost_Beg_User : public Imba_TCPXY<GATE_RESP_GHOST_BEG_USER>
	{
		enum { MAX_USER_COUNT=20 };
		unsigned int	game_id;
		unsigned int	group_id;
		unsigned char	user_cnt;	// �û���(��0��ʾ���ʧ��)
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

	// GhostSvr->GateSvr֪ͨ������ӵĽ��
	struct Ghost_Note_Gate_Beg_Res : public Imba_TCPXY<GHOST_NOTE_GATE_BEG_RES>
	{
		enum { MAX_GAME_DATA_CNT=2048 };
		enum RS_BEG
		{
			RS_BEG_SUCCESS=0,
			//����
			RS_BEG_END,
		};

		unsigned int	game_id;
		unsigned int	group_id;
		unsigned char	result;
		unsigned short	data_cnt;
		char			game_data[MAX_GAME_DATA_CNT];	// ��Ϸ��ص���Ϣ(��ħ������-�㲥����)

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
		unsigned int game_id;	// ��game=0,��ghost����game
		unsigned int group_id;	// ��group_id=0,����Ϸ����group

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

	// GhostSvr->GateSvr֪ͨɾ�����
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

	// GateSvr->GhostSvr֪ͨɾ�����
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
		int				logmode;	// logmode��ghost���壬 ��ң�û�е�ͼ�����ݴ������
		unsigned int	user_id;
		unsigned int	game_id;
		unsigned int	group_id;
		char			user_ip[32];	// string \0���� 
		int				len;			// ����\0 �ĳ���
		char			msg[MAX_FATAL_LEN+1];// string \0����

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

