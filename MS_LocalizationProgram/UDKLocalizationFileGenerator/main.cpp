#include "StringUtils.h"
#include <string>
#include <fstream>

using namespace Monky;

void parserCSVFile( const std::string& fileName );

int main( int argc, char** argv )
{
	if( argc > 1 )
	{
		std::string fileName = argv[1];
		if( fileName.size() > 0 )
		{
			parserCSVFile( fileName );	
		}
	}
	return 0;
}

void parserCSVFile( const std::string& fileName )
{
	std::ifstream csvFile( fileName );
	std::ofstream localFile( "Bjeardsoft.int" );
	std::string row;
	if( csvFile.is_open() )
	{
		std::getline( csvFile, row );
		while( !csvFile.eof() )
		{
			std::getline( csvFile, row );
			std::vector< std::string > columns;
			stringTokenizer( row, columns, "\t" );
		
			if( columns.size() > 1 )
			{
				if( columns[0] != "" )
				{
					localFile << columns[0] << std::endl;
				}
				else if( columns[1] == "" )
				{
					continue;
				}
				else
				{
					localFile << columns[1] << "= \"";		
		
					for( int i = 2; i < columns.size(); )
					{
						std::string tempStr = columns[i];
						if( tempStr[0] == '"' )
						{
							tempStr = tempStr.substr( 1, tempStr.size() - 2 );
						}

						localFile << tempStr; 
						++i;
						if( i != columns.size() )
						{
							localFile << "^";
						}
					}
					localFile << "\"\n";
				}
			}
		}
	}
}