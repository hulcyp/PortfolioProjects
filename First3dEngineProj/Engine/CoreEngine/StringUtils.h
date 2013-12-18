#pragma once
#include <string>
#include <sstream>
#include <vector>

namespace pdh
{
	template< typename T >
	std::string toString( T i )
	{
		std::stringstream out;
		out << i;
		return out.str();
	}

	template< typename T >
	bool stringToType( const std::string& str, T* type )
	{
		std::stringstream ss;
		ss << str;
		ss >> *type;
		
		return !ss.fail();
	}

	void stringToLowerCase( std::string& text );
	void stringTokenizer( const std::string& s, std::vector< std::string >& tokens, const std::string& pattern = " " );
}