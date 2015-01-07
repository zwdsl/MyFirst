// simpleservice.h
//
// an simple Windows NT service. 
//
// -- if this code works, it was written by wangxinyue.hz
// -- if not, I don't know who wrote it.
// 

/*
 * 1) install Service
 *		Start->StartNTService()
 *			Service enter ...
 *			Service install
 *		Return<-StartNTService()
 *			
 * 2) Start Service
 *		Start->StartNTService()
 *			Service enter ...
 *			Starting NTService ...
 *			OnStartProc() - user
 *			Service Start Succeed!
 *			wait...
 *		
 * 3) Stop Service
 *			Terminate "SimpleNTService" because receive stop service control code.
 *			wait end. g_hTerminateEvent is restart.
 *			OnEndProc() - user 
 *			service exit
 *		Return<-StartNTService()	
 *		
 * 4) uninstall Service
 *		Start->StartNTService()
 *			Service enter ...
 *			Service uninstall
 *		Return<-StartNTService()
 *
 **/

#pragma once

#ifndef _WINDOWS_
	#include <windows.h>
#endif

#ifndef _WINSVC_
	#include <winsvc.h>
#endif

#ifndef _ALGORITHM_
	#include <algorithm>
#endif

#include "minidump.h"

static void LogText( std::string message )
{
	//LOGD( message );
	std::cout << message << std::endl;
}

//==================================
// 名称空间 simpleservice::xxxx
//==================================
namespace simpleservice
{
	#define FS_CTRL_INTERVAL	5000
	typedef BOOL (*lpServiceControlProc)(void);

//==========
// 全局函数
//==========
	void StopNTService();
	bool IsRunAsService();
	void UnInitService();
	unsigned int GetTerminateEvent();
	BOOL UpdateServiceStatus ( SERVICE_STATUS_HANDLE, DWORD, DWORD, DWORD );

//==========
// 全局变量
//==========
	static BOOL g_bRunAsConsoleMode = FALSE;
	static std::string g_configfile;

	static char g_ServiceName[64] = {0};
	static HANDLE g_hTerminateEvent = NULL;

	static SERVICE_STATUS_HANDLE  g_hSvcStatus = 0L;
	static SERVICE_STATUS  g_ServiceStatus;

	static CMiniDumper	g_minidump;
//==========
// 回调函数
//==========
	static lpServiceControlProc OnStartProc;
	static lpServiceControlProc OnEndProc;
	static lpServiceControlProc OnHelpProc;


//==========
// 函数实现
//==========
	static
	bool IsRunAsService()
	{
		return (!g_bRunAsConsoleMode);
	}

	static
	unsigned int GetTerminateEvent()
	{
		return (unsigned int)g_hTerminateEvent;
	}

	static
	void StopNTService()
	{
		UpdateServiceStatus ( g_hSvcStatus, SERVICE_STOP_PENDING, NO_ERROR, FS_CTRL_INTERVAL );
		::SetEvent ( g_hTerminateEvent );
	}

	static
	bool CreateTerminateEvent()
	{
		if ( g_hTerminateEvent && g_hTerminateEvent != INVALID_HANDLE_VALUE )
			return true;

		g_hTerminateEvent = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
		return (g_hTerminateEvent == NULL ? false:true);
	}

	static
	BOOL UpdateServiceStatus ( SERVICE_STATUS_HANDLE hStatus, DWORD dwStatus, DWORD dwErrorCode, DWORD dwWaitHint )
	{
		if ( !IsRunAsService() ) return TRUE;
		if ( hStatus == 0 ) return FALSE;

		static DWORD dwCheckpoint = 1;
		DWORD  dwControls = SERVICE_ACCEPT_STOP;

		ZeroMemory ( &g_ServiceStatus, sizeof(g_ServiceStatus) );
		g_ServiceStatus.dwServiceType = SERVICE_WIN32;
		g_ServiceStatus.dwCurrentState = dwStatus;
		g_ServiceStatus.dwWaitHint = dwWaitHint;

		if(dwErrorCode)
		{
			g_ServiceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
			g_ServiceStatus.dwServiceSpecificExitCode = dwErrorCode;
		}

		// special cases that depend on the new state
		switch ( dwStatus )
		{
			case SERVICE_START_PENDING:
				dwControls = 0;
				break;

			case SERVICE_RUNNING:
			case SERVICE_STOPPED:
				dwCheckpoint = 0;
				break;
		}
		g_ServiceStatus.dwCheckPoint = dwCheckpoint ++;
		g_ServiceStatus.dwControlsAccepted = dwControls;
		return ::SetServiceStatus ( hStatus, &g_ServiceStatus );
	}

	static
	void UnInitService()
	{
		if ( g_hTerminateEvent && g_hTerminateEvent != INVALID_HANDLE_VALUE )
		{
			::CloseHandle ( g_hTerminateEvent );
			g_hTerminateEvent = NULL;
		}
	}

	static
	VOID WINAPI CtrlHandler ( DWORD dwControl )
	{
		switch ( dwControl )
		{
			// SCM request stop server
			case SERVICE_CONTROL_SHUTDOWN:
			case SERVICE_CONTROL_STOP:
				{
					LogText("receive stop service control code, stop \"" + std::string(g_ServiceName) + "\"");
					StopNTService ();
				}
				break;

			// returns the current state of the service to the SCM.
			case SERVICE_CONTROL_INTERROGATE:
				{
					if ( g_hSvcStatus != 0 )
						::SetServiceStatus ( g_hSvcStatus, &g_ServiceStatus);
				}
				break;
		}
	}

	static
	void ReleaseServiceResource()
	{
		try
		{
			UpdateServiceStatus ( g_hSvcStatus, SERVICE_STOP_PENDING, ::GetLastError(), FS_CTRL_INTERVAL );
			UnInitService();

			if ( OnEndProc ) 
				OnEndProc();

			/* Elvis has left the building */
			UpdateServiceStatus ( g_hSvcStatus, SERVICE_STOPPED, NO_ERROR, FS_CTRL_INTERVAL );
		}
		catch (...){}
	}

	static
	void ServiceMain(DWORD argc, char *argv[])
	{
		if ( IsRunAsService() )
		{
			g_hSvcStatus = ::RegisterServiceCtrlHandler ( g_ServiceName, CtrlHandler );
			if(g_hSvcStatus == 0)
			{
				LogText("RegisterServiceCtrlHandler failed" );
				return;
			}
		}

		UpdateServiceStatus ( g_hSvcStatus, SERVICE_START_PENDING, NO_ERROR, FS_CTRL_INTERVAL );

		try
		{
			if( !OnStartProc || !OnStartProc() )
			{
				ReleaseServiceResource();
			}
			else
			{
				UpdateServiceStatus ( g_hSvcStatus, SERVICE_RUNNING, NO_ERROR, FS_CTRL_INTERVAL );
				
				LogText(std::string(g_ServiceName) + " Start Succeed!");

				// 等待系统结束
				::WaitForSingleObject ( g_hTerminateEvent, INFINITE);
			}
			
		}
		catch (...)
		{
			ReleaseServiceResource();
		}

		ReleaseServiceResource();	
	}
}


//==================================
// 名称空间 serviceinstall::xxxx
//==================================
namespace serviceinstall
{
	using namespace std;

	#define SI_SERVICE_SUCCEED				0x00000000
	#define SI_SERVICE_OPERATE_SUCCEED		0x00000001
	#define SI_SERVICE_INSTALL_SUCCEED		0x00000010
	#define SI_SERVICE_UNINSTALL_SUCCEED	0x00000100
	#define SI_SERVICE_INSTALL_ABORT		0x00001000
	#define SI_SERVICE_UNINSTALL_ABORT		0x00010000

	static char m_szDescription[256] = {0};
	static unsigned int m_idStart = 3;

	static
	bool Install()
	{
		SC_HANDLE hSCM = ::OpenSCManager(NULL,						// local machine
										 NULL,						// ServicesActive database
										 SC_MANAGER_ALL_ACCESS);	// full access
		if (!hSCM) return false;

		// Get the executable file path
		char szFilePath[_MAX_PATH];
		::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));

		// Create the service
		SC_HANDLE hService = ::CreateService(hSCM, 
			simpleservice::g_ServiceName, simpleservice::g_ServiceName,
			SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, 
			SERVICE_ERROR_NORMAL, szFilePath, NULL, NULL, NULL, NULL, NULL);

		if (!hService)
		{
			::CloseServiceHandle(hSCM);
			return false;
		}

		// make registry entries to support logging messages
		// Add the source name as a subkey under the Application
		// key in the EventLog service portion of the registry.
		char szKey[256];
		HKEY hKey = NULL;
		strcpy_s(szKey, 256, "SYSTEM\\CurrentControlSet\\Services\\");
		strcat_s(szKey, 256, simpleservice::g_ServiceName);
		if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) != ERROR_SUCCESS)
		{
			::CloseServiceHandle(hService);
			::CloseServiceHandle(hSCM);
			return false;
		}

		if(m_szDescription[0] != '0')
			::RegSetValueEx(hKey, "Description", 0, REG_SZ, (LPBYTE)   m_szDescription, strlen(m_szDescription) + 1);
		if(m_idStart != 3)
			::RegSetValueEx(hKey, "Start", 0, REG_DWORD, (LPBYTE)   &m_idStart, sizeof(unsigned int) );

		::RegCloseKey(hKey);
		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);
		return true;
	}

	static
	bool Uninstall()
	{
		SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (!hSCM) return false;

		bool bResult = false;
		SC_HANDLE hService = ::OpenService(hSCM, simpleservice::g_ServiceName, SERVICE_ALL_ACCESS);
		if (hService)
		{
			if ( ::DeleteService(hService) )
				bResult = true;

			::CloseServiceHandle(hService);
		}
	    
		::CloseServiceHandle(hSCM);
		return bResult;
	}

	static
	BOOL IsInstalled()
	{
		bool bResult = false;

		SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (hSCM)
		{
			// Try to open the service
			SC_HANDLE hService = ::OpenService(hSCM, simpleservice::g_ServiceName, SERVICE_QUERY_CONFIG);
			if (hService)
			{
				bResult = true;
				::CloseServiceHandle(hService);
			}

			::CloseServiceHandle(hSCM);
		}
	    
		return bResult;
	}

	static
	int parsecmdline( int argc, char* argv[], bool& bConsole, std::string& configfile )
	{
		int result = SI_SERVICE_SUCCEED;
		for (int i=1; i<argc; i++)
		{
			if ( argv[i][0]=='-' )
			{
				if ( strchr(argv[i], 'i') || strchr(argv[i], 'I') )
				{
					LogText("Service install");
					if( !Install() )
						result |= SI_SERVICE_INSTALL_ABORT;
					else
					{
						result |= SI_SERVICE_INSTALL_SUCCEED;
					}
				}

				if ( strchr(argv[i], 'u') || strchr(argv[i], 'U') )
				{
					LogText("Service uninstall");
					if( !Uninstall() )
						result |= SI_SERVICE_UNINSTALL_ABORT;
					else
						result |= SI_SERVICE_UNINSTALL_SUCCEED;
				}

				if ( strchr(argv[i], 'd') || strchr(argv[i], 'D') )
				{
					bConsole = true;
				}

				if ( strchr(argv[i], 'v') || strchr(argv[i], 'V') )
				{
					if ( simpleservice::OnHelpProc )
					{
						simpleservice::OnHelpProc( );
					}

					result |= SI_SERVICE_OPERATE_SUCCEED;
				}

				if ( strchr(argv[i], 'h') || strchr(argv[i], 'H') )
				{
					std::cout<<"Usage: "<<argv[0]<<" [OPTION]..."<<std::endl;
					std::cout<<"Mandatory arguments to long options are mandatory for short options too."<<std::endl;
					std::cout<<"  -d			        console mode"<<std::endl;
					std::cout<<"  -f=[FILE]	            use config file"<<std::endl;
					std::cout<<"  -h                    display this help"<<std::endl;
					std::cout<<"  -v                    display version"<<std::endl;
					std::cout<<"  -i                    register server"<<std::endl;
					std::cout<<"  -u                    unregister server"<<std::endl;

					result |= SI_SERVICE_OPERATE_SUCCEED;
				}

				if ( ( strchr(argv[i], 'f') || strchr(argv[i], 'F') ) && i+1<argc )
				{
					configfile = argv[i+1];
					i ++;
					continue;
				}
			}
		}

		return result;
	}
}


static bool IsRunAsService()
{
	return simpleservice::IsRunAsService();
}

static BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		{
			if ( simpleservice::g_bRunAsConsoleMode )
			{
				simpleservice::StopNTService();
				//Sleep(100);
			}
		}
		return TRUE;
	default:
		return FALSE;
	}
}

static void SetServiceDesc(char *szServiceDesc, bool StartType)
{
	int len = strlen(szServiceDesc);
	if(len >= 255) len = 255;
	memcpy(serviceinstall::m_szDescription, szServiceDesc, len);

	if(StartType)
		serviceinstall::m_idStart = 2;
}

static int StartNTService ( const char *szServiceName, 
						   simpleservice::lpServiceControlProc OnStartFunc, 
						   simpleservice::lpServiceControlProc OnEndFunc, 
						   simpleservice::lpServiceControlProc OnHelpFunc, 
						   int argc, char *argv[] )
{
	if ( !szServiceName || !(strlen(szServiceName) > 0) || !OnStartFunc || !OnEndFunc )
		return -2;

	LogText("Service enter ...");

	int len = strlen(szServiceName);
	if(len >= 64) len = 63;
	memcpy(simpleservice::g_ServiceName, szServiceName, len);

	bool bConsole = false;
	int  result = serviceinstall::parsecmdline( argc, argv, bConsole, simpleservice::g_configfile );
	if ( result != SI_SERVICE_SUCCEED )
	{
		getchar();
		return result;
	}

	LogText("Starting NTService ...");

	simpleservice::OnStartProc = OnStartFunc;
	simpleservice::OnEndProc = OnEndFunc;
	simpleservice::OnHelpProc = OnHelpFunc;

	simpleservice::CreateTerminateEvent ();

	// run as a console-mode
	if	( !bConsole )
	{
		/*
		* Service table entry that specifies Displays for the
		* services in this module.
		*/

		SERVICE_TABLE_ENTRY ste[] = { simpleservice::g_ServiceName, (LPSERVICE_MAIN_FUNCTION)simpleservice::ServiceMain, NULL, NULL	};

		if ( ::StartServiceCtrlDispatcher ( ste ) )
		{
			return result;
		}
		else
		{
			DWORD dwErr = ::GetLastError();
			if ( dwErr != ERROR_FAILED_SERVICE_CONTROLLER_CONNECT )
			{
				LogText("StartServiceCtrlDispatcher failed ...");
				return -1;
			}
		}
	}

	simpleservice::g_bRunAsConsoleMode = TRUE;
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
	simpleservice::ServiceMain ( argc, argv );

	return result;
}

