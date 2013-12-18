#include "Assertion.h"
#include "GameApp.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include <StringUtils.h>

namespace Monky
{
	const int Assertion::MAX_MESSAGE_SIZE = 256;
	const char* Assertion::ms_DebugMsg = "Debug?\n";
	const char* Assertion::ms_ErrorLocation = "Assertion failed at %s(%d)\n";
	const char* Assertion::ms_Title = "Assertion Failed"; 
	
	Assertion::Assertion( bool condition, const char* file, int line, const char* format, ... )
	{
		if( condition )
			return;
	
		char locMsg[256];
	
		sprintf_s( locMsg, ms_ErrorLocation, file, line );
		
		va_list args;
		va_start( args, format );
		char errorMsg[ MAX_MESSAGE_SIZE ];
		vsprintf_s( errorMsg, format, args );
		va_end( args );
		
		std::string msg = std::string( locMsg ) + "\n" + std::string( errorMsg ) + "\n" + std::string( ms_DebugMsg );
		
		int button = MessageBoxA( NULL, msg.c_str(), ms_Title, MB_YESNO | MB_ICONERROR | MB_SETFOREGROUND );

		std::string consoleMsg = file;
		consoleMsg += "(";
		consoleMsg += toString( line );
		consoleMsg += "): ";
		consoleMsg += errorMsg;
		consoleMsg += "\n";

		OutputDebugStringA( consoleMsg.c_str() );

		switch( button )
		{
		case IDYES:
			__debugbreak();
			break;
		case IDNO:
			GameApp::getInstance()->exitProgram( -1 );
			break;
		}

	}
	
	Assertion::~Assertion()
	{}
}