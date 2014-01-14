#include "StringUtils.h"
#include <algorithm>
#include <sstream>

namespace Monky
{
	std::string floatToString( float num, int prec )
	{
		std::stringstream out;
		out.precision( prec );
		out << std::fixed << num;
		return out.str();
	}
	//--------------------------------------------------------------------------
	std::string doubleToString( double num, int prec )
	{
		std::stringstream out;
		out.precision( prec );
		out << std::fixed << num;
		return out.str();
	}
	//--------------------------------------------------------------------------
	void stringToLowerCase( std::string& text )
	{
		std::transform( text.begin(), text.end(), text.begin(), ::tolower );
	}
	//--------------------------------------------------------------------------
	void stringToUpperCase( std::string& text )
	{
		std::transform( text.begin(), text.end(), text.begin(), ::toupper );
	}
	//--------------------------------------------------------------------------
	void stringTokenizer( const std::string& s, std::vector< std::string >& tokens, const std::string& pattern )
	{
		size_t firstIndex = 0;
		size_t secondIndex = s.find_first_of( pattern ); 

		while( secondIndex != s.npos )
		{
			tokens.push_back( s.substr( firstIndex, secondIndex - firstIndex ) );
			firstIndex = secondIndex + 1;
			secondIndex = s.find( pattern, firstIndex );
		}

		tokens.push_back( s.substr( firstIndex, secondIndex ) );
	}	
	//--------------------------------------------------------------------------
	int findStringInList( const std::string& str, const std::vector< std::string >& list )
	{
		int strIndex = -1;
		for( size_t i = 0; i < list.size(); ++i )
		{
			if( list[i] == str )
			{
				strIndex = i;
				break;
			}
		}
		return strIndex;
	}
}