#include "StringUtils.h"
#include <string>
#include <fstream>

using namespace Monky;

void parseINTFile( const std::string& localFileName );

int main( int argc, char** argv )
{
	if( argc > 1 )
	{
		std::string fileName = argv[1];
		if( fileName.size() > 0 )
		{
			parseINTFile( fileName );	
		}
	}
	return 0;
}

void parseINTFile( const std::string& localFileName )
{
	std::ifstream localFile( localFileName );
	std::ofstream tabFile( "BS_Localization.txt" );
	std::string line;
	std::string row;

	if( localFile.is_open() )
	{
		tabFile << "\tVariable Name\tEnglish\n";
		while( !localFile.eof() )
		{
			std::getline( localFile, line );

			if( line.size() > 0 )
			{
				if( line[0] == '[' )
				{
					row = line + '\t' + '\n';
				}
				else
				{
					std::vector< std::string > variableValuePair;
					stringTokenizer( line, variableValuePair, "=" );
					if( variableValuePair.size() == 2 )
					{
						row = '\t';
						row += variableValuePair[0] + '\t';
						std::string value = variableValuePair[1].substr( 1 );
						value = value.substr( 0, value.size() - 1 );
						row += value + '\n';
					}
				}

				tabFile << row;
			}
		}
	}

}

