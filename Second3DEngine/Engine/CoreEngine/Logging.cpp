#include "Logging.h"
#include "CommonEngineIncludes.h"

#define MONKY_VERBOSE_DEBUGGING

namespace Monky
{
	const char* g_logFileName = "log.txt";
	std::ofstream g_logFile( g_logFileName, std::ios_base::trunc );

	//----------------------------------------------------------------------
	void Log( const char* format, ... )
	{
		va_list args;
		va_start( args, format );
		Log( format, args );
	}
	//----------------------------------------------------------------------
	void Log( const char* format, va_list args )
	{
		char text[512];
		vsprintf_s( text, format, args );
		va_end( args );
		if( !g_logFile.is_open() )
			g_logFile.open( g_logFileName );
		g_logFile << text << '\n';
	}
	//----------------------------------------------------------------------
	void vLog( const char* format, ... )
	{
#ifdef MONKY_VERBOSE_DEBUGGING
		va_list args;
		va_start( args, format );
		Log( format, args );	
#endif
	}
	//----------------------------------------------------------------------
	void Flush()
	{
		g_logFile.flush();
	}
}