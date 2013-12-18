#include "ErrorHandlingUtils.h"
#include <GlutApp.h>
#include <windows.h>
#include <stdio.h>

namespace pdh
{
	void reportError( const char* title, const char* format, ... )
	{
		char msg[1024];
		va_list args;
		va_start( args, format );
		vsprintf_s( msg, format, args );
		va_end( args );
		MessageBoxA( NULL, msg, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
	}
	void fatalError( const char* title, const char* format, ... )
	{
		char msg[1024];
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

	bool displayYesNoMessage( const char* title, const char* msg )
	{
		HWND hWnd = ::FindWindowA( NULL, GlutApp::sGetWindowTitle() );
		bool yesSelected = false;
		int selected = MessageBoxA( hWnd, msg, title, MB_YESNO | MB_ICONWARNING | MB_SETFOREGROUND );
		switch( selected )
		{
		case IDYES:
			yesSelected = true;
			break;
		case IDNO:
			yesSelected = false;
			break;
		}

		return yesSelected;
	}	
}