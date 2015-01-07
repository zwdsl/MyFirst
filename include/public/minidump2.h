#include <windows.h>
#include <imagehlp.h>
#include <tchar.h>
#pragma once
//#pragma comment(lib, "dbghelp.lib")

typedef BOOL (WINAPI *MiniDumpWriteDump_PTR)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
											 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
											 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
											 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
											 );

inline bool IsDataSectionNeeded( const WCHAR* pModuleName ) 
{
	if( pModuleName == NULL ) 
	{
		return false; 
	}
	//wprintf(L"%s\r\n",pModuleName);

	//WCHAR szFileName[_MAX_FNAME] = L""; 
	//_wsplitpath( pModuleName, NULL, NULL, szFileName, NULL ); 

	//if( wcsicmp( szFileName, L"ntdll" ) == 0 ) return true; 
	return true; 
}

inline BOOL CALLBACK MiniDumpCallback(
									  PVOID                            pParam, 
									  const PMINIDUMP_CALLBACK_INPUT   pInput, 
									  PMINIDUMP_CALLBACK_OUTPUT        pOutput 
									  ) 
{
	if( pInput == 0 || pOutput == 0) return FALSE; 
	switch( pInput->CallbackType ) 
	{
	case ModuleCallback: 
		if( pOutput->ModuleWriteFlags & ModuleWriteDataSeg ) 
			if( !IsDataSectionNeeded( pInput->Module.FullPath ) ) 
				pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg); 
		// fall through
	case IncludeModuleCallback: 
	case IncludeThreadCallback: 
	case ThreadCallback: 
	case ThreadExCallback: 
		return TRUE; 
	default:;
	}
	return FALSE; 
}

inline void CreateMiniDump( EXCEPTION_POINTERS* pep, LPCTSTR lpDllPath, LPCTSTR filename) 
{
	HMODULE hModule = ::LoadLibrary(lpDllPath);
	if( INVALID_HANDLE_VALUE == hModule )
	{
		MessageBox(NULL,_T("Load dbghelp.dll faild"), _T(""), MB_OK|MB_SYSTEMMODAL);
		return;
	}

	MiniDumpWriteDump_PTR pfnMiniDumpWriteDump = (MiniDumpWriteDump_PTR)GetProcAddress(hModule, "MiniDumpWriteDump");
	if( NULL == pfnMiniDumpWriteDump )
	{
		MessageBox(NULL,_T("Load MiniDumpWriteDump faild"), _T(""), MB_OK|MB_SYSTEMMODAL);
		FreeLibrary(hModule);
		return;
	}

	HANDLE hFile = CreateFile( filename, GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei; 
		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE; 
		MINIDUMP_CALLBACK_INFORMATION mci; 
		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback; 
		mci.CallbackParam       = 0; 
		MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(
			MiniDumpWithPrivateReadWriteMemory | 
			MiniDumpWithDataSegs | 
			MiniDumpWithHandleData |
			MiniDumpWithFullMemory | 
			MiniDumpWithThreadInfo | 
			MiniDumpWithUnloadedModules ); 
		pfnMiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci ); 

		CloseHandle( hFile ); 
	}

	FreeLibrary(hModule);
	return;
}

