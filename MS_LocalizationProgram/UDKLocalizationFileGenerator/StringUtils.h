#pragma once
#include <string>
#include <vector>

namespace Monky
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

	std::string floatToString( float num, int prec );
	std::string doubleToString( double num, int prec );

	void stringToLowerCase( std::string& text );
	void stringToUpperCase( std::string& text );
	void stringTokenizer( const std::string& s, std::vector< std::string >& tokens, const std::string& pattern = " " );
	int findStringInList( const std::string& str, const std::vector< std::string >& list );
}
