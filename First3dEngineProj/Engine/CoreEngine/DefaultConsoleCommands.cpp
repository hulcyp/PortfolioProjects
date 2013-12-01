#include "DefaultConsoleCommands.h"
#include "CommonEngineIncludes.h"
#include <map>

namespace pdh
{
	void printCmd( const std::string& args )
	{
		consolePrintf( args.c_str() );
	}
	void exitCmd( const std::string& args )
	{
		exit( 0 );
	}
	void dalekCmd( const std::string& args )
	{
		for( int i = 0; i < 10; ++i )
		{
			consolePrintColorf( "EXTERMINATE!!!", color::RED );
		}
	}
}