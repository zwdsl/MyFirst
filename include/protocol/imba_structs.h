
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
	// 第四位，有益权限
	UP_IMBAGM		= 0x00000001,	// 管理员

	// 高四位，
	UP_BANNED		= 0x00010000,	// 帐号被封
};

enum GameResultType
{
	GRT_LOSE	= 0,
	GRT_WIN		= 1,
	GRT_INVALID = 255,
};

enum NetType
{
	NT_CTC	= 0,	// 电信
	NT_CNC,			// 网通
	NT_CEN,			// 教育网
	NT_OTHER,		// 其他
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

// 用数据库配置
struct ImbaUserDB
{
	int			id;				// id
	std::string odb_config;		// odbc config
	std::string	login_user;		// 帐号库帐号
	std::string	login_passwd;	// 帐号库密码
	std::string db_ip;			// 帐号库所在机器
	int			db_port;		// 帐号库db监听端口
	std::string db_name;		// 帐号库db名
	std::string table_name;		// 帐号库表名

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

// 游戏配置信息
struct ImbaGameDB
{
	int			id;				// id
	std::string name;			// 游戏名

	int			supportmode;	// 游戏支持模式
	std::string config;			// 游戏内部配置

	std::string mapversion;		// 游戏地图版本
	std::string mapdownloadurl;	// 游戏地图下载地址

	std::string odb_config;		// odbc config
	std::string	login_user;		// 游戏库登录用户
	std::string	login_passwd;	// 游戏库登录密码
	std::string db_ip;			// 游戏库所在机器
	int			db_port;		// 游戏库数据库监听端口
	std::string db_name;		// 游戏库db名
	std::string gametable_name;	// 游戏表名
	std::string proptable_name;	// 游戏道具表名
	std::string	mapname;		// 游戏地图名称
	std::string coinconfig;
	std::string modeconfig;		// 地图模式

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

// LoginSvr 配置信息
struct ImbaLoginSvrConfig
{
	int				id;				// id

	std::string		toclient_ip;	// 对客户端监听ip
	int				toclient_port;	// 对客户端监听端口
	std::string		publish_ips;	// 对客户端发布的ip，如电信ip是x.x.x.x，网通ip是x.x.x.x

	int				togate_dns;		// 当有多个gatesvr连接到loginsvr时，对其做何种负载均衡

	std::string		cmdudp_ip;		// udp命令通道
	int				cmdudp_port;

	int				thread_count;	// 线程数目
	int				dbsvr_id;		// 指向db服务

	std::string		config;
	std::string		version;		// 服务版本号
	std::string		dir;			// 服务在服务器运行的目录，由服务登录的时候提交
	

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

	std::string		toclient_ip;	// 对客户端监听ip
	int				toclient_port;	// 对客户端监听端口
	std::string		publish_ips;	// 对客户端发布的ip，如电信ip是x.x.x.x，网通ip是x.x.x.x

	std::string		toghost_ip;
	int				toghost_port;
	int				toghost_dns;

	int				thread_count;	// 线程数目
	int				dbsvr_id;

	int				gameid;
	int				gamesupportmode;
	std::string		gameconfig;

	std::string		cmdudp_ip;		// udp命令通道
	int				cmdudp_port;

	std::string		version;		// 服务版本号
	std::string		dir;			// 服务在服务器运行的目录，由服务登录的时候提交
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

	std::string		toclient_ip;	// 对客户端监听ip
	int				toclient_port;	// 对客户端监听端口
	std::string		publish_ips;	// 对客户端发布的ip，如电信ip是x.x.x.x，网通ip是x.x.x.x

	int				thread_count;
	int				gatesvr_id;

	std::string		version;		// 服务版本号
	std::string		dir;			// 服务在服务器运行的目录，由服务登录的时候提交

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
	int				userdb_id;		// 使用哪个帐号库
	std::string		game_ids;		// 支持哪些游戏库

	std::string		version;		// 服务版本号
	std::string		dir;			// 服务在服务器运行的目录，由服务登录的时候提交

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


// 用户帐号信息
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

// 用户游戏信息
struct ImbaGameInfo
{
	int		userid;
	__int64 hfid;			// hfid
	int		gameid;

	int		coin;
	int		medal;			// 荣耀
	int		level;

	int		vip, goldvip, netvip; // 
	int		escaperate;

	int		win;			// 赢
	int		lose;			// 输
	int		earlyleave;		// 早退
	int		escape;			// 逃跑
	int		hold;			// 坚持

	std::string hfdata;		// 浩方其他数据

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


// 用户游戏信息
struct ImbaGameResult
{
	int		userid;
	__int64		hfid;

	unsigned char	game_team;				// 游戏阵营
	unsigned char	game_result;			// 0 lose ; 1 win;
	unsigned char	game_rank;				// 游戏名次

	int				hero;					// 英雄
	int				herolevel;				// 英雄等级

	int				herokill;				// 杀死对方英雄次数
	int				herodeath;				// 死亡次数
	int				soldierkill;			// 刷兵个数(正)
	int				soldierkillB;			// 刷兵数(反)
	int				heroassist;				//	助攻数
	int				gold;					//	金钱
	int				soldierkillanimal;		//	杀野数

	unsigned char	cheater;				// 0 正常，1图挂 2其他挂 

	long long		queuetm;				// 加入队列的时间
	long long		endtm;					// 退出游戏时间

	int				medal;
	int				coin;
	int				level;
	int				escaperate;
	int				win;
	int				vip,goldvip,netvip;
	std::string		hfdata;

	int				user_ip;				//用户ip
	int				host_svr_ip;			//主机ip（ghost svr ip）
	int				chenghao;				//(mvp,杀,死，助)
	int				dianjishu;				//apm值
	int				match_srv_ip;			//匹配服务器ip
	int				pos_id;					//槽位id
	int				fight_value;			//战斗力

	int				chaita;
	int				chaitaB;

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

	int				imbacoin;				//活动金币
	
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
	int		userid;			// 用户 id
	int		usersession;	// 用户 session

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