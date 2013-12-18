#include "ConsoleInterface.h"
#include "ConsoleSystem.h"
#include "Logging.h"

namespace Monky
{
	ConsoleSystem * g_ConsoleSystem = nullptr;
	
	ConsoleSystem * initConsole( int width, int height, int bufferSize )
	{
		if( !g_ConsoleSystem )
		{
			g_ConsoleSystem = new ConsoleSystem( width, height, bufferSize );
			return g_ConsoleSystem;
		}
		return nullptr;
	}

	void cleanupConsole()
	{
		SAFE_DELETE( g_ConsoleSystem );
	}

	void consolePrintf( const char* format, ... )
	{
		va_list args;
		va_start( args, format );
		vConsolePrintf( format, color::WHITE, args );
		Log( format, args );
	}

	void consolePrintColorf( const char* format, Color4f color, ... )
	{
		va_list args;
		va_start( args, color );
		vConsolePrintf( format, color, args );
		Log( format, args );
	}

	void vConsolePrintf( const char* format, Color4f color, va_list args )
	{
		char text[256];
		vsprintf_s( text, format, args );
		va_end( args );
		if( g_ConsoleSystem )
		{
			g_ConsoleSystem->printToConsole( text, color );
		}
		std::string str = text;
		str += '\n';
		printf( str.c_str() );
	}

	void executeConsoleCommand( const std::string& cmd, const std::string& args )
	{
		if( g_ConsoleSystem != nullptr )
			g_ConsoleSystem->executeCommand( cmd, args );
	}

	void registerConsoleCommand( const std::string& cmd, const std::string& helpText, std::function< void( const std::string& ) > cmdFunc )
	{
		if( g_ConsoleSystem != nullptr )
			g_ConsoleSystem->registerCommand( cmd, helpText, cmdFunc );
	}
}