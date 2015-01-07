
#pragma once

#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <dbghelp.h>
#include <string>

#ifdef UNICODE
	#define tstring wstring 
#else
	#define tstring string 
#endif

#pragma comment(lib, "dbghelp.lib")

class CMiniDumper
{ 
public:
	CMiniDumper( void )
	{
		m_OriginalFilter = ::SetUnhandledExceptionFilter(ExceptionFilter);
	}

	~CMiniDumper( void )
	{
		::SetUnhandledExceptionFilter(m_OriginalFilter);
	}

private:
	LPTOP_LEVEL_EXCEPTION_FILTER m_OriginalFilter;

	static LONG WINAPI ExceptionFilter( struct _EXCEPTION_POINTERS* ExceptionInfo )
	{
		TCHAR szPath[MAX_PATH];

		if (GetModuleFileName(NULL, szPath, sizeof(szPath)))
		{
			time_t nowtime;
			time(&nowtime);
			struct tm *pTime = localtime(&nowtime);
			
			TCHAR filename[MAX_PATH];
			_stprintf(filename, _T("_%4d-%02d-%02d_%02d-%02d-%02d.dmp"), pTime->tm_year+1900, 
				pTime->tm_mon+1, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, 
				pTime->tm_sec);

			TCHAR drive[_MAX_DRIVE];
			TCHAR dir[_MAX_DIR];
			TCHAR fname[_MAX_FNAME];
			TCHAR ext[_MAX_EXT];
			_tsplitpath( szPath, drive, dir, fname, ext ); 

			std::tstring strDumpFileName = drive;
			strDumpFileName += dir;
			strDumpFileName += fname;
			strDumpFileName += filename;

			HANDLE hFile = CreateFile(strDumpFileName.c_str(), FILE_ALL_ACCESS, 
				0, NULL, CREATE_ALWAYS, NULL, NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				MINIDUMP_EXCEPTION_INFORMATION exception_information;
				exception_information.ThreadId			= GetCurrentThreadId();
				exception_information.ExceptionPointers = ExceptionInfo;
				exception_information.ClientPointers	= TRUE;

				MiniDumpWriteDump(GetCurrentProcess( ), GetCurrentProcessId( ), 
					hFile, 
					(MINIDUMP_TYPE)
					(MiniDumpWithPrivateReadWriteMemory| 
					MiniDumpWithDataSegs| 
					MiniDumpWithHandleData|
					MiniDumpWithFullMemory| 
					MiniDumpWithThreadInfo| 
					MiniDumpWithUnloadedModules), &exception_information, NULL, NULL);
				CloseHandle(hFile);
			}
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}
};


