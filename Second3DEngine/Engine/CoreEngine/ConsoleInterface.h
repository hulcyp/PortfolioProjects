#pragma once
#include "StdLibraryIncludes.h"
#include "Colors.h"

namespace Monky
{
	class ConsoleSystem;

	ConsoleSystem * initConsole( int width, int height, int bufferSize );
	void cleanupConsole();
	void consolePrintf( const char* text, ... );
	void consolePrintColorf( const char* text, Color4f color, ... );
	void vConsolePrintf( const char* text, Color4f color, va_list args );
	void executeConsoleCommand( const std::string& cmd, const std::string& args );
	void registerConsoleCommand( const std::string& cmd, const std::string& helpText, std::function< void( const std::string& ) > cmdFunc );	
}