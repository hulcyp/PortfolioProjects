#include "DevConsole.h"
#include <stdarg.h>
#include <string>
#include "ConsoleSystem.h"
#include "StringUtils.h"

namespace pdh
{
	void consolePrintf( const char* text, Color4f color, ... )
	{
		std::string sText;
		va_list args;
		va_start( args, color );

		for( int i = 0; text[i] != '\0'; ++i )
		{
			if( text[i] == '%' && text[ i + 1 ] != '\0' )
			{
				switch( text[ ++i ] )
				{
				case 'd':
				case 'i':
					{
					int arg = va_arg( args, int );
					sText += toString( arg );
					break;
					}
				case 'u':
					{
					unsigned arg = va_arg( args, unsigned int );
					sText += toString( arg );
					break;
					}
				case 'f':
				case 'F':
					{
					double arg = va_arg( args, double );
					sText += toString( arg );
					break;
					}
				case 'c':
					{
					char arg = va_arg( args, char );
					sText += arg;
					break;
					}
				case 's':
					{
					const char* arg = va_arg( args, char* );
					sText += arg;
					break;
					}
				default:
					sText += text[i];
				}
			}
			else
			{
				sText += text[i];
			}
		}

		ConsoleSystem::getInstance()->printToConsole( sText, color );
	}
}