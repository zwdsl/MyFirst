
#pragma once

#include "biostream.h"

enum IMBA_SVRTYPE
{
	IMBASVRTYPE_CONFIG = 0, 

	IMBASVRTYPE_LOGIN, 
	IMBASVRTYPE_GATE, 
	IMBASVRTYPE_GHOST, 
	IMBASVRTYPE_DB, 

	IMBASVRTYPE_COUNT
};

static
const char* IMBA_SERVERNAME[] =
{
	"IMBA_ConfigServer",
	"IMBA_LoginServer",
	"IMBA_GateServer",
	"IMBA_GhostServer",
	"IMBA_DBServer",
	NULL
};

enum Length_Limit
{
	LL_USERNAME_MIN	= 3,
	LL_USERNAME_MAX = 15,

	LL_PASSWORD		= 32,

	LL_NICKNAME_MIN = 3,
	LL_NICKNAME_MAX = 31,
};

enum USER_POWER
{
	// ����λ������Ȩ��
	UP_IMBAGM		= 0x00000001,	// ����Ա

	// ����λ��
	UP_BANNED		= 0x00010000,	// �ʺű���
};

enum GameResultType
{
	GRT_LOSE	= 0,
	GRT_WIN		= 1,
	GRT_INVALID = 255,
};

enum NetType
{
	NT_CTC	= 0,	// ����
	NT_CNC,			// ��ͨ
	NT_CEN,			// ������
	NT_OTHER,		// ����
	NT_COUNT
};

struct DBConfig
{
	int			dbid;
	std::string	odbc;

	DBConfig( void ) { reset( ); }
	void reset( void ) 
	{
		dbid = 0;
		odbc.clear();
	}

	friend bostream& operator<<(bostream& bos, const DBConfig& msg)
	{
		bos << msg.dbid;
		bos << msg.odbc;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, DBConfig& msg)
	{
		msg.reset();

		bis >> msg.dbid;
		bis >> msg.odbc;

		return bis;
	}
};

struct ImbaGameLogDB
{
	enum LogType
	{
		LT_RESULT = 0,
		LT_USECOIN,
		LT_AWARDPROP,
		LT_LOGIN,
		LT_ONLINE,
		LT_SERVERLOG,
	};
	int			gameid;
	int			logtype;
	int			dbid;
	std::string	tablename;

	ImbaGameLogDB( void ) { reset(); }

	void reset( void ) 
	{
		gameid = logtype = dbid = 0;
		tablename.clear();
	}

	friend bostream& operator<<(bostream& bos, const ImbaGameLogDB& msg)
	{
		bos << msg.gameid;
		bos << msg.logtype;
		bos << msg.dbid;
		bos << msg.tablename;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaGameLogDB& msg)
	{
		msg.reset();

		bis >> msg.gameid;
		bis >> msg.logtype;
		bis >> msg.dbid;
		bis >> msg.tablename;

		return bis;
	}
};

// �����ݿ�����
struct ImbaUserDB
{
	int			id;				// id
	std::string odb_config;		// odbc config
	std::string	login_user;		// �ʺſ��ʺ�
	std::string	login_passwd;	// �ʺſ�����
	std::string db_ip;			// �ʺſ����ڻ���
	int			db_port;		// �ʺſ�db�����˿�
	std::string db_name;		// �ʺſ�db��
	std::string table_name;		// �ʺſ����

	ImbaUserDB( void ) { reset(); }

	void reset() 
	{
		id = 0;
		odb_config.clear();
		login_user.clear();
		login_passwd.clear();
		db_ip.clear();
		db_port = 0;
		db_name.clear();
		table_name.clear();
	}

	friend bostream& operator<<(bostream& bos, const ImbaUserDB& msg)
	{
		bos << msg.id;
		bos << msg.odb_config;
		bos << msg.login_user;
		bos << msg.login_passwd;
		bos << msg.db_ip;
		bos << msg.db_port;
		bos << msg.db_name;
		bos << msg.table_name;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaUserDB& msg)
	{
		msg.reset();

		bis >> msg.id;
		bis >> msg.odb_config;
		bis >> msg.login_user;
		bis >> msg.login_passwd;
		bis >> msg.db_ip;
		bis >> msg.db_port;
		bis >> msg.db_name;
		bis >> msg.table_name;

		return bis;
	}
};

// ��Ϸ������Ϣ
struct ImbaGameDB
{
	int			id;				// id
	std::string name;			// ��Ϸ��

	int			supportmode;	// ��Ϸ֧��ģʽ
	std::string config;			// ��Ϸ�ڲ�����

	std::string mapversion;		// ��Ϸ��ͼ�汾
	std::string mapdownloadurl;	// ��Ϸ��ͼ���ص�ַ

	std::string odb_config;		// odbc config
	std::string	login_user;		// ��Ϸ���¼�û�
	std::string	login_passwd;	// ��Ϸ���¼����
	std::string db_ip;			// ��Ϸ�����ڻ���
	int			db_port;		// ��Ϸ�����ݿ�����˿�
	std::string db_name;		// ��Ϸ��db��
	std::string gametable_name;	// ��Ϸ����
	std::string proptable_name;	// ��Ϸ���߱���
	std::string	mapname;		// ��Ϸ��ͼ����
	std::string coinconfig;
	std::string modeconfig;		// ��ͼģʽ

	ImbaGameDB( void ) { reset(); }

	void reset() 
	{
		id = 0;
		name.clear();

		supportmode = 0;
		config.clear();

		mapname.clear();
		mapversion.clear();
		mapdownloadurl.clear();

		odb_config.clear();
		login_user.clear();
		login_passwd.clear();
		db_ip.clear();
		db_port = 0;
		db_name.clear();
		gametable_name.clear();
		proptable_name.clear();
		coinconfig.clear();
		modeconfig.clear();
	}

	friend bostream& operator<<(bostream& bos, const ImbaGameDB& msg)
	{
		bos << msg.id;
		bos << msg.name;

		bos << msg.supportmode;
		bos << msg.config;

		bos << msg.mapversion;
		bos << msg.mapdownloadurl;
		

		bos << msg.odb_config;
		bos << msg.login_user;
		bos << msg.login_passwd;
		bos << msg.db_ip;
		bos << msg.db_port;
		bos << msg.db_name;
		bos << msg.gametable_name;
		bos << msg.proptable_name;
		bos << msg.mapname;
		bos << msg.coinconfig;
		bos << msg.modeconfig;
		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaGameDB& msg)
	{
		msg.reset();

		bis >> msg.id;
		bis >> msg.name;

		bis >> msg.supportmode;
		bis >> msg.config;

		bis >> msg.mapversion;
		bis >> msg.mapdownloadurl;

		bis >> msg.odb_config;
		bis >> msg.login_user;
		bis >> msg.login_passwd;
		bis >> msg.db_ip;
		bis >> msg.db_port;
		bis >> msg.db_name;
		bis >> msg.gametable_name;
		bis >> msg.proptable_name;
		bis >> msg.mapname;
		bis >> msg.coinconfig;
		bis >> msg.modeconfig;
		return bis;
	}
};

// LoginSvr ������Ϣ
struct ImbaLoginSvrConfig
{
	int				id;				// id

	std::string		toclient_ip;	// �Կͻ��˼���ip
	int				toclient_port;	// �Կͻ��˼����˿�
	std::string		publish_ips;	// �Կͻ��˷�����ip�������ip��x.x.x.x����ͨip��x.x.x.x

	int				togate_dns;		// ���ж��gatesvr���ӵ�loginsvrʱ�����������ָ��ؾ���

	std::string		cmdudp_ip;		// udp����ͨ��
	int				cmdudp_port;

	int				thread_count;	// �߳���Ŀ
	int				dbsvr_id;		// ָ��db����

	std::string		config;
	std::string		version;		// ����汾��
	std::string		dir;			// �����ڷ��������е�Ŀ¼���ɷ����¼��ʱ���ύ
	

	ImbaLoginSvrConfig( void ) { reset(); }

	void reset( void )
	{
		id = 0;

		toclient_ip.clear();
		toclient_port = 0;
		publish_ips.clear();

		togate_dns  = 0;

		cmdudp_ip.clear();
		cmdudp_port = 0;

		thread_count = 0;
		dbsvr_id = 0;

		config.clear();
		version.clear();
		dir.clear();
	}

	friend bostream& operator<<(bostream& bos, const ImbaLoginSvrConfig& msg)
	{
		bos << msg.id;

		bos << msg.toclient_ip;
		bos << msg.toclient_port;
		bos << msg.publish_ips;

		bos << msg.togate_dns;

		bos << msg.cmdudp_ip;
		bos << msg.cmdudp_port;

		bos << msg.thread_count;
		bos << msg.dbsvr_id;

		bos << msg.config;
		bos << msg.version;
		bos << msg.dir;
		

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaLoginSvrConfig& msg)
	{
		msg.reset();

		bis >> msg.id;

		bis >> msg.toclient_ip;
		bis >> msg.toclient_port;
		bis >> msg.publish_ips;

		bis >> msg.togate_dns;

		bis >> msg.cmdudp_ip;
		bis >> msg.cmdudp_port;

		bis >> msg.thread_count;
		bis >> msg.dbsvr_id;

		bis >> msg.config;
		bis >> msg.version;
		bis >> msg.dir;
		

		return bis;
	}
};


// 
struct ImbaGateSvrConfig
{
	int			id;

	std::string		toclient_ip;	// �Կͻ��˼���ip
	int				toclient_port;	// �Կͻ��˼����˿�
	std::string		publish_ips;	// �Կͻ��˷�����ip�������ip��x.x.x.x����ͨip��x.x.x.x

	std::string		toghost_ip;
	int				toghost_port;
	int				toghost_dns;

	int				thread_count;	// �߳���Ŀ
	int				dbsvr_id;

	int				gameid;
	int				gamesupportmode;
	std::string		gameconfig;

	std::string		cmdudp_ip;		// udp����ͨ��
	int				cmdudp_port;

	std::string		version;		// ����汾��
	std::string		dir;			// �����ڷ��������е�Ŀ¼���ɷ����¼��ʱ���ύ
	ImbaGateSvrConfig( void ) { reset(); }

	void reset( void )
	{
		id = 0;

		toclient_ip.clear();
		toclient_port = 0;
		publish_ips.clear();

		toghost_ip.clear();
		toghost_port = 0;
		toghost_dns  = 0;

		thread_count = 0;
		dbsvr_id	 = 0;

		gameid = 0;
		gamesupportmode = 0;
		gameconfig.clear();

		cmdudp_ip.clear();
		cmdudp_port = 0;

		version.clear();
		dir.clear();
	}

	friend bostream& operator<<(bostream& bos, const ImbaGateSvrConfig& msg)
	{
		bos << msg.id;

		bos << msg.toclient_ip;
		bos << msg.toclient_port;
		bos << msg.publish_ips;

		bos << msg.toghost_ip;
		bos << msg.toghost_port;
		bos << msg.toghost_dns;

		bos << msg.thread_count;
		bos << msg.dbsvr_id;

		bos << msg.gameid;
		bos << msg.gamesupportmode;
		bos << msg.gameconfig;

		bos << msg.cmdudp_ip;
		bos << msg.cmdudp_port;

		bos << msg.version;
		bos << msg.dir;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaGateSvrConfig& msg)
	{
		msg.reset();

		bis >> msg.id;

		bis >> msg.toclient_ip;
		bis >> msg.toclient_port;
		bis >> msg.publish_ips;

		bis >> msg.toghost_ip;
		bis >> msg.toghost_port;
		bis >> msg.toghost_dns;

		bis >> msg.thread_count;
		bis >> msg.dbsvr_id;

		bis >> msg.gameid;
		bis >> msg.gamesupportmode;
		bis >> msg.gameconfig;

		bis >> msg.cmdudp_ip;
		bis >> msg.cmdudp_port;

		bis >> msg.version;
		bis >> msg.dir;

		return bis;
	}
};


//
struct ImbaGhostSvrConfig
{
	int				id;

	std::string		toclient_ip;	// �Կͻ��˼���ip
	int				toclient_port;	// �Կͻ��˼����˿�
	std::string		publish_ips;	// �Կͻ��˷�����ip�������ip��x.x.x.x����ͨip��x.x.x.x

	int				thread_count;
	int				gatesvr_id;

	std::string		version;		// ����汾��
	std::string		dir;			// �����ڷ��������е�Ŀ¼���ɷ����¼��ʱ���ύ

	ImbaGhostSvrConfig( void ) { reset(); }

	void reset( void )
	{
		id = 0;

		toclient_ip.clear();
		toclient_port = 0;
		publish_ips.clear();

		thread_count = 0;
		gatesvr_id = 0;

		version.clear();
		dir.clear();
	}

	friend bostream& operator<<(bostream& bos, const ImbaGhostSvrConfig& msg)
	{
		bos << msg.id;

		bos << msg.toclient_ip;
		bos << msg.toclient_port;
		bos << msg.publish_ips;

		bos << msg.thread_count;
		bos << msg.gatesvr_id;

		bos << msg.version;
		bos << msg.dir;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaGhostSvrConfig& msg)
	{
		msg.reset();

		bis >> msg.id;

		bis >> msg.toclient_ip;
		bis >> msg.toclient_port;
		bis >> msg.publish_ips;

		bis >> msg.thread_count;
		bis >> msg.gatesvr_id;

		bis >> msg.version;
		bis >> msg.dir;

		return bis;
	}	
};

//
struct ImbaDBSvrConfig
{
	int				id;

	std::string		listen_ip;
	int				listen_port;

	int				thread_count;
	int				userdb_id;		// ʹ���ĸ��ʺſ�
	std::string		game_ids;		// ֧����Щ��Ϸ��

	std::string		version;		// ����汾��
	std::string		dir;			// �����ڷ��������е�Ŀ¼���ɷ����¼��ʱ���ύ

	ImbaDBSvrConfig( void ) { reset(); }

	void reset( void )
	{
		id = 0;

		listen_ip.clear();
		listen_port = 0;

		thread_count = 0;
		userdb_id = 0;
		game_ids.clear();

		version.clear();
		dir.clear();
	}

	friend bostream& operator<<(bostream& bos, const ImbaDBSvrConfig& msg)
	{
		bos << msg.id;

		bos << msg.listen_ip;
		bos << msg.listen_port;

		bos << msg.thread_count;
		bos << msg.userdb_id;
		bos << msg.game_ids;

		bos << msg.version;
		bos << msg.dir;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaDBSvrConfig& msg)
	{
		msg.reset();

		bis >> msg.id;

		bis >> msg.listen_ip;
		bis >> msg.listen_port;

		bis >> msg.thread_count;
		bis >> msg.userdb_id;
		bis >> msg.game_ids;

		bis >> msg.version;
		bis >> msg.dir;

		return bis;
	}	
};


// �û��ʺ���Ϣ
struct ImbaUserInfo
{
	int			userid;
	std::string	user;
	std::string nickname;
	int			headid;
	char		sex;
	int			money;
	int			vip;
	long long	vipexp_tm;
	long long	lastlogin_tm;

	ImbaUserInfo( void ) { reset(); }

	void reset( void )
	{
		userid = 0;

		user.clear();
		nickname.clear();

		headid = 0;
		sex = 0;
		money = 0;
		vip = 0;
		vipexp_tm = 0;
		lastlogin_tm = 0;
	}

	friend bostream& operator<<(bostream& bos, const ImbaUserInfo& msg)
	{
		bos << msg.userid;

		bos << msg.user;
		bos << msg.nickname;

		bos << msg.headid;
		bos << msg.sex;
		bos << msg.money;
		bos << msg.vip;
		bos << msg.vipexp_tm;
		bos << msg.lastlogin_tm;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaUserInfo& msg)
	{
		msg.reset();

		bis >> msg.userid;

		bis >> msg.user;
		bis >> msg.nickname;

		bis >> msg.headid;
		bis >> msg.sex;
		bis >> msg.money;
		bis >> msg.vip;
		bis >> msg.vipexp_tm;
		bis >> msg.lastlogin_tm;

		return bis;
	}	
};

// �û���Ϸ��Ϣ
struct ImbaGameInfo
{
	int		userid;
	__int64 hfid;			// hfid
	int		gameid;

	int		coin;
	int		medal;			// ��ҫ
	int		level;

	int		vip, goldvip, netvip; // 
	int		escaperate;

	int		win;			// Ӯ
	int		lose;			// ��
	int		earlyleave;		// ����
	int		escape;			// ����
	int		hold;			// ���

	std::string hfdata;		// �Ʒ���������

	int		room_ip;		//room_ip
	int		gate_ip;		//gate_ip
	int		fight_value;	//fight_value;
	int		point;
	__int64	locktime;
	ImbaGameInfo( void ) { reset(); }

	void reset( void )
	{
		userid	=  gameid	= 0;
		hfid	= 0;
		coin	= medal	= level		= 0;
		vip		= goldvip=netvip	= 0;
		escaperate = 0;

		win	= lose = earlyleave = escape = hold = 0;

		hfdata.clear();
		room_ip = gate_ip = fight_value = 0;
		point = 0;
		locktime = 0;
	}

	friend bostream& operator<<(bostream& bos, const ImbaGameInfo& msg)
	{
		bos << msg.userid;
		bos << msg.hfid;
		bos << msg.gameid;

		bos << msg.coin;
		bos << msg.medal;
		bos << msg.level;

		bos << msg.vip;
		bos << msg.goldvip;
		bos << msg.netvip;
		bos << msg.escaperate;

		bos << msg.win;
		bos << msg.lose;
		bos << msg.earlyleave;
		bos << msg.escape;
		bos << msg.hold;

		bos << msg.hfdata;

		bos << msg.room_ip;
		bos << msg.gate_ip;
		bos << msg.fight_value;

		bos << msg.point;
		bos << msg.locktime;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaGameInfo& msg)
	{
		msg.reset();

		bis >> msg.userid;
		bis >> msg.hfid;
		bis >> msg.gameid;

		bis >> msg.coin;
		bis >> msg.medal;
		bis >> msg.level;

		bis >> msg.vip;
		bis >> msg.goldvip;
		bis >> msg.netvip;
		bis >> msg.escaperate;

		bis >> msg.win;
		bis >> msg.lose;
		bis >> msg.earlyleave;
		bis >> msg.escape;
		bis >> msg.hold;

		bis >> msg.hfdata;

		bis >> msg.room_ip;
		bis >> msg.gate_ip;
		bis >> msg.fight_value;

		bis >> msg.point;
		bis >> msg.locktime;
		return bis;
	}
};


// �û���Ϸ��Ϣ
struct ImbaGameResult
{
	int		userid;
	__int64		hfid;

	unsigned char	game_team;				// ��Ϸ��Ӫ
	unsigned char	game_result;			// 0 lose ; 1 win;
	unsigned char	game_rank;				// ��Ϸ����

	int				hero;					// Ӣ��
	int				herolevel;				// Ӣ�۵ȼ�

	int				herokill;				// ɱ���Է�Ӣ�۴���
	int				herodeath;				// ��������
	int				soldierkill;			// ˢ������(��)
	int				soldierkillB;			// ˢ����(��)
	int				heroassist;				//	������
	int				gold;					//	��Ǯ
	int				soldierkillanimal;		//	ɱҰ��

	unsigned char	cheater;				// 0 ������1ͼ�� 2������ 

	long long		queuetm;				// ������е�ʱ��
	long long		endtm;					// �˳���Ϸʱ��

	int				medal;
	int				coin;
	int				level;
	int				escaperate;
	int				win;
	int				vip,goldvip,netvip;
	std::string		hfdata;

	int				user_ip;				//�û�ip
	int				host_svr_ip;			//����ip��ghost svr ip��
	int				chenghao;				//(mvp,ɱ,������)
	int				dianjishu;				//apmֵ
	int				match_srv_ip;			//ƥ�������ip
	int				pos_id;					//��λid
	int				fight_value;			//ս����

	int				chaita;
	int				chaitaB;

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

	int				imbacoin;				//����
	
	int				point;
	long long		locktime;

	ImbaGameResult( void ) 
	{ 
		reset(); 
	}

	void reset( void )
	{
		userid =0;
		 hfid = 0;

		game_team = game_result = game_rank = 0;

		hero = herolevel = 0;
		herokill = herodeath = soldierkill = 0;
		soldierkillB = heroassist = gold = soldierkillanimal = 0;

		cheater = 0;

		queuetm = endtm = 0;

		medal = 0;
		coin = 0;
		level = 0;
		escaperate = 0;
		win = 0;
		vip = goldvip = netvip = 0;
		hfdata.clear();
		user_ip = host_svr_ip = chenghao = dianjishu = match_srv_ip = fight_value = pos_id = 0;
		chaita = chaitaB = 0;
		messenger = flymessenger = guard = guardB = fog = dust = drummer = mekansm = 0;
		doublekill = triplekill = ultrakill = rampage = 0;
		imbacoin = 0;
		point = locktime = 0;
	}

	friend bostream& operator<<(bostream& bos, const ImbaGameResult& msg)
	{
		bos << msg.userid;
		bos << msg.hfid;

		bos << msg.game_team;
		bos << msg.game_result;
		bos << msg.game_rank;

		bos << msg.hero;
		bos << msg.herolevel;

		bos << msg.herokill;
		bos << msg.herodeath;
		bos << msg.soldierkill;

		bos << msg.soldierkillB;
		bos << msg.heroassist;
		bos << msg.gold;
		bos << msg.soldierkillanimal;

		bos << msg.cheater;	

		bos << msg.queuetm;
		bos << msg.endtm;

		bos << msg.medal;
		bos << msg.coin;
		bos << msg.level;
		bos << msg.escaperate;
		bos << msg.win;
		bos << msg.vip;
		bos << msg.goldvip;
		bos << msg.netvip;
		bos << msg.hfdata;

		bos << msg.user_ip;
		bos << msg.host_svr_ip;
		bos << msg.chenghao;
		bos << msg.dianjishu;
		bos << msg.match_srv_ip;

		bos << msg.pos_id;
		bos << msg.fight_value;

		bos << msg.chaita;
		bos << msg.chaitaB;

		bos << msg.messenger;
		bos << msg.flymessenger;
		bos << msg.guard;
		bos << msg.guardB;
		bos << msg.fog;
		bos << msg.dust;
		bos << msg.drummer;
		bos << msg.mekansm;
		
		bos << msg.doublekill;
		bos << msg.triplekill;
		bos << msg.ultrakill;
		bos << msg.rampage;

		bos << msg.imbacoin;
		bos << msg.point;
		bos << msg.locktime;
		return bos;
	}

	friend bistream& operator>>(bistream& bis, ImbaGameResult& msg)
	{
		msg.reset();

		bis >> msg.userid;
		bis >> msg.hfid;

		bis >> msg.game_team;
		bis >> msg.game_result;
		bis >> msg.game_rank;

		bis >> msg.hero;
		bis >> msg.herolevel;

		bis >> msg.herokill;
		bis >> msg.herodeath;
		bis >> msg.soldierkill;

		bis >> msg.soldierkillB;
		bis >> msg.heroassist;
		bis >> msg.gold;
		bis >> msg.soldierkillanimal;

		bis >> msg.cheater;	

		bis >> msg.queuetm;
		bis >> msg.endtm;

		bis >> msg.medal;
		bis >> msg.coin;
		bis >> msg.level;
		bis >> msg.escaperate;
		bis >> msg.win;
		bis >> msg.vip;
		bis >> msg.goldvip;
		bis >> msg.netvip;
		bis >> msg.hfdata;

		bis >> msg.user_ip;
		bis >> msg.host_svr_ip;
		bis >> msg.chenghao;
		bis >> msg.dianjishu;
		bis >> msg.match_srv_ip;

		bis >> msg.pos_id;
		bis >> msg.fight_value;

		bis >> msg.chaita;
		bis >> msg.chaitaB;
		

		bis >> msg.messenger;
		bis >> msg.flymessenger;
		bis >> msg.guard;
		bis >> msg.guardB;
		bis >> msg.fog;
		bis >> msg.dust;
		bis >> msg.drummer;
		bis >> msg.mekansm;

		bis >> msg.doublekill;
		bis >> msg.triplekill;
		bis >> msg.ultrakill;
		bis >> msg.rampage;

		bis >> msg.imbacoin;
		bis >> msg.point;
		bis >> msg.locktime;
		return bis;
	}
};
struct UserGatePos
{
	int		gameid;
	int		appid;			// gate id
	int		appsession;		// gate session
	int		userid;			// �û� id
	int		usersession;	// �û� session

	UserGatePos( void ) { reset(); }

	void reset( void )
	{
		gameid= appid= appsession = userid = usersession = 0;
	}

	friend bostream& operator<<(bostream& bos, const UserGatePos& msg)
	{
		bos << msg.gameid;
		bos << msg.appid;
		bos << msg.appsession;
		bos << msg.userid;
		bos << msg.usersession;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, UserGatePos& msg)
	{
		msg.reset();

		bis >> msg.gameid;
		bis >> msg.appid;
		bis >> msg.appsession;
		bis >> msg.userid;
		bis >> msg.usersession;

		return bis;
	}
};
struct User_Note_Calculate
{
	unsigned __int64	user_id;
	unsigned int	room_ip;
	unsigned int	gate_ip;

	User_Note_Calculate()
	{
		reset();
	}

	void reset()
	{
		user_id = 0;
		room_ip = 0;
		gate_ip = 0;
	}
	friend bostream& operator<<(bostream& bos, const User_Note_Calculate& msg)
	{
		bos << msg.user_id;
		bos << msg.room_ip;
		bos << msg.gate_ip;

		return bos;
	}

	friend bistream& operator>>(bistream& bis, User_Note_Calculate& msg)
	{
		msg.reset();

		bis >> msg.user_id;
		bis >> msg.room_ip;
		bis >> msg.gate_ip;

		return bis;
	}
};