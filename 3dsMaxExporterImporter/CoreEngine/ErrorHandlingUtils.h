#pragma once

/*************************
* author: Preston Hulcy
* last updated: 9-21-12
*************************/

#include <cassert>
#include <iostream>

#define ASSERT assert

namespace pdh
{
	void reportError( const char* title, const char* format, ... );
	void fatalError( const char* title, const char* format, ... );
	void printErrorToOutputConsole( const char* error );
	bool displayYesNoMessage( const char* title, const char* msg );
}

//HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE ); \
	//SetConsoleTextAttribute( hstdout, C_FG_WHITE | C_BG_BLACK ); \

/****************************************
#ifdef _DEBUG
	int main()
	{
		return _tWinMain( GetModuleHandle( NULL ), NULL, GetCommandLine(), SW_SHOW );
	}
#endif

	Set subsystem to "not set"
*****************************************/