#include "StringUtils.h"
#include <algorithm>

namespace pdh
{
	void stringToLowerCase( std::string& text )
	{
		std::transform( text.begin(), text.end(), text.begin(), ::tolower );
	}

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
}