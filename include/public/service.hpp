
#pragma once

#include <iostream>


#if defined _WINDOWS_ || defined WIN32

#include "ntservice.hpp"

static HANDLE g_thread_handle = 0;
static unsigned int __stdcall ServiceBeginProc_ThreadFunc(void *pval)
{
	CService::get_instance().m_running = true;
	CService::get_instance().m_configfile = simpleservice::g_configfile;
	CService::get_instance().m_runfunction();

	return 0;
}


static
BOOL OnServiceBeginProc( void )
{
	g_thread_handle = (HANDLE)_beginthreadex(0, 0, ServiceBeginProc_ThreadFunc, NULL, 0, 0);
	
	return TRUE;
}

static
BOOL OnServiceEndProc()
{
	CService::get_instance().m_running = false;
	CService::get_instance().m_stopfunction();

	if ( g_thread_handle )
	{
		CloseHandle( g_thread_handle );
	}

	return TRUE;
}

static
BOOL OnServiceHelpProc()
{
	CService::get_instance().m_helpfunction();
	return TRUE;
}

inline
void CService::run( const std::string& servername, Signal runf, Signal stopf, Signal helpf, int argc, char** argv )
{
	m_servername  = servername;
	m_runfunction = runf;
	m_stopfunction= stopf;
	m_helpfunction= helpf;
	m_running	  = false;

	StartNTService(m_servername.c_str(), OnServiceBeginProc, OnServiceEndProc, OnServiceHelpProc, argc, argv);
}

#else

#include <getopt.h>
#include <signal.h>
#include <pthread.h>

#include <sys/time.h>

static const char* option_format = "df:hv";

static option long_options[] = {
	{"daemon",              0, 0, 'd'},
	{"config-file",         1, 0, 'f'},
	{"help",                0, 0, 'h'},
	{"version",             0, 0, 'v'},
	{NULL,                  0, 0, 0}
};

inline
void usage( const char* program )
{
	std::cout<<"Usage: "<<program<<" [OPTION]..."<<std::endl;
	std::cout<<"Mandatory arguments to long options are mandatory for short options too."<<std::endl;
	std::cout<<"  -d, --daemon             daemon mode"<<std::endl;
	std::cout<<"  -f, --config-file=[FILE] use config file"<<std::endl;
	std::cout<<"  -h, --help               display this help and exit"<<std::endl;
	std::cout<<"  -v, --version            display version"<<std::endl;
}

static void on_ctrl_message(int sg)
{
	CService::get_instance().m_running = false;
	CService::get_instance().m_stopfunction()
}

inline 
void daemon_init( bool bdaemon )
{
	if ( bdaemon )
	{
		int pid;
		if( (pid=fork())>0 )	exit(0);
		else if( pid<0 )		exit(-1);
	}

	//signal( SIGHUP, SIG_IGN );	// SIGHUP (挂起) 当运行进程的用户注销时通知该进程，使进程终止
	//signal( SIGALRM, SIG_IGN ); // SIGALRM (超时) alarm函数使用该信号，时钟定时器超时响应
	//signal( SIGTERM, SIG_IGN ); // SIGTERM 程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理. 
	//		通常用来要求程序自己正常退出. shell命令kill缺省产生这个信号. 
	signal( SIGPIPE, SIG_IGN ); // SIGPIPE 写至无读进程的管道, 或者Socket通信SOCT_STREAM的读进程已经终止，而再写入。
	//signal( SIGXCPU, SIG_IGN ); // SIGXCPU 超过CPU时间资源限制. 这个限制可以由getrlimit/setrlimit来读取/改变
	//signal( SIGXFSZ, SIG_IGN ); // SIGXFSZ 当进程企图扩大文件以至于超过文件大小资源限制
	//signal( SIGPROF, SIG_IGN ); // SIGPROF (梗概时间超时) setitimer(2)函数设置的梗概统计间隔计时器(profiling interval timer)
	//signal( SIGUSR1, SIG_IGN );
	//signal( SIGUSR2, SIG_IGN );
	//signal( SIGVTALRM, SIG_IGN ); // SIGVTALRM 虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间

	signal( SIGQUIT, SIG_IGN ); // SIGQUIT (退出) 用户按下或时通知进程，使进程终止
	signal( SIGINT, on_ctrl_message );  // SIGINT (中断) 当用户按下时,通知前台进程组终止进程
	//signal( SIGPOLL, SIG_IGN );
	//signal( SIGSYS, SIG_IGN );// SIGSYS 非法的系统调用
}

inline
void CService::run( const std::string& servername, Signal runf, Signal stopf, Signal helpf, int argc, char** argv )
{
	m_servername  = servername;
	m_runfunction = runf;
	m_stopfunction= stopf;
	m_helpfunction= helpf;
	m_running	  = false;

	bool	bdaemon = false;
	char*	configfile = 0;

	while(true)
	{
		int option_index = 0;

		int c = getopt_long(argc, argv, option_format, long_options, &option_index);
		if (c == -1) break; 

		switch(c)
		{
		case 0: // long option, not need process
			break;
		case 'd':
			bdaemon = true; 
			break;
		case 'f':
			configfile = optarg; 
			break;
		case 'v':
			m_helpfunction( );
			return;
		case 'h':
			usage(argv[0]);
			return;
		default:
			return;
		}
	}

	daemon_init( bdaemon );
	if ( configfile )
	{
		CService::get_instance().m_configfile = std::string(configfile);
	}

	m_runfunction( );
	m_running = true;

	while ( m_running )
	{
		usleep( 2000 );
	}

	m_stopfunction( );
}

#endif

inline
CService& CService::get_instance( void )
{
	static CService service;

	return service;
}

inline
std::string CService::get_configfile( const std::string& defaultfile )
{
	return m_configfile.length()==0?defaultfile:m_configfile;
}

