
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

	//signal( SIGHUP, SIG_IGN );	// SIGHUP (����) �����н��̵��û�ע��ʱ֪ͨ�ý��̣�ʹ������ֹ
	//signal( SIGALRM, SIG_IGN ); // SIGALRM (��ʱ) alarm����ʹ�ø��źţ�ʱ�Ӷ�ʱ����ʱ��Ӧ
	//signal( SIGTERM, SIG_IGN ); // SIGTERM �������(terminate)�ź�, ��SIGKILL��ͬ���Ǹ��źſ��Ա������ʹ���. 
	//		ͨ������Ҫ������Լ������˳�. shell����killȱʡ��������ź�. 
	signal( SIGPIPE, SIG_IGN ); // SIGPIPE д���޶����̵Ĺܵ�, ����Socketͨ��SOCT_STREAM�Ķ������Ѿ���ֹ������д�롣
	//signal( SIGXCPU, SIG_IGN ); // SIGXCPU ����CPUʱ����Դ����. ������ƿ�����getrlimit/setrlimit����ȡ/�ı�
	//signal( SIGXFSZ, SIG_IGN ); // SIGXFSZ ��������ͼ�����ļ������ڳ����ļ���С��Դ����
	//signal( SIGPROF, SIG_IGN ); // SIGPROF (����ʱ�䳬ʱ) setitimer(2)�������õĹ���ͳ�Ƽ����ʱ��(profiling interval timer)
	//signal( SIGUSR1, SIG_IGN );
	//signal( SIGUSR2, SIG_IGN );
	//signal( SIGVTALRM, SIG_IGN ); // SIGVTALRM ����ʱ���ź�. ������SIGALRM, ���Ǽ�����Ǹý���ռ�õ�CPUʱ��

	signal( SIGQUIT, SIG_IGN ); // SIGQUIT (�˳�) �û����»�ʱ֪ͨ���̣�ʹ������ֹ
	signal( SIGINT, on_ctrl_message );  // SIGINT (�ж�) ���û�����ʱ,֪ͨǰ̨��������ֹ����
	//signal( SIGPOLL, SIG_IGN );
	//signal( SIGSYS, SIG_IGN );// SIGSYS �Ƿ���ϵͳ����
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

