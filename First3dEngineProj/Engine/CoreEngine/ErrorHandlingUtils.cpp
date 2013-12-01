#include "ErrorHandlingUtils.h"
#include <windows.h>
#include <stdio.h>

namespace pdh
{
	void reportError( const char* title, const char* format, ... )
	{
		char msg[256];
		va_list args;
		va_start( args, format );
		vsprintf_s( msg, format, args );
		va_end( args );
		MessageBoxA( NULL, msg, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
	}
	void fatalError( const char* title, const char* format, ... )
	{
		char msg[256];
		va_list args;
		va_start( args, format );
		vsprintf_s( msg, format, args );
		va_end( args );
		MessageBoxA( NULL, msg, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
		exit( -1 );
	}

	void printErrorToOutputConsole( const char* error )
	{
		OutputDebugStringA( error );
	}

	
}