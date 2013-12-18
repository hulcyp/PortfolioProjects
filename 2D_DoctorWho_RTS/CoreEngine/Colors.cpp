#include "Colors.h"

namespace pdh
{
	Color4f::Color4f( float r, float g, float b, float a )
		:	r( r )
		,	g( g )
		,	b( b )
		,	a( a )
	{}

	Color4f::Color4f( unsigned long hexCol )
		:	r( ( ( hexCol & 0x00FF0000 ) >> 16 ) / 255.0f)
		,	g( ( ( hexCol & 0x0000FF00 ) >> 8 ) / 255.0f )
		,	b(   ( hexCol & 0x000000FF ) / 255.0f )
		,	a( 1.0f )//( ( hexCol & 0xFF000000 ) >> 24 ) / 255.0f )
	{}

	float& Color4f::operator[]( size_t i )
	{
		return *(&r + i);
	}

	Color4f Color4f::operator*( const Color4f& rhs ) const
	{
		return Color4f( r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a );
	}

	float Color4f::getElement( size_t i ) const
	{
		return *( &r + i );
	}

	std::string Color4f::toString()
	{
		std::stringstream ss;
		ss << "( " << r << ", " << g << ", " << b << ", " << a << " )";
		return std::string( ss.str() );
	}

	Color4f parseStringToColor4f( const std::string& str, const Color4f& defaultValue )
	{
		Color4f temp = defaultValue;
		
		if( str[0] == '#' )
		{
			char* res;
			unsigned long hex = strtoul( str.c_str() + 1, &res, 16 );
			if( res == nullptr )
				throw std::exception( "Invalid hexidecimal value" );
			
			temp = Color4f( hex );
		}
		else
		{
			std::vector< std::string > tokens;
			stringTokenizer( str, tokens, "," );
			size_t numValues = tokens.size();

			if( numValues < 3 )
				throw std::exception( "Not enough values to fill a Color4" );

			for( size_t i = 0; i < numValues && i < 4; ++i )
			{
				if( !stringToType( tokens[i], &temp[i] ) )
				{
					temp[i] = defaultValue.getElement( i );
				}
				temp[i] /= 255.0f;
			}
		}

		return temp;
	}
}