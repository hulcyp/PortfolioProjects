#pragma once
#include <vector>
#include "StringUtils.h"

namespace pdh
{
	struct Color4f
	{
		Color4f( float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f );
		Color4f( unsigned long hexCol );

		float& operator[]( size_t i );
		Color4f operator*( const Color4f& rhs ) const;
		float getElement( size_t i ) const;
		std::string toString();

		float r, g, b, a;
	};
	namespace color
	{
		const Color4f WHITE( 1.0f, 1.0f, 1.0f, 1.0f );
		const Color4f RED( 1.0f, 0.0f, 0.0f, 1.0f );
		const Color4f GREEN( 0.0f, 1.0f, 0.0f, 1.0f );
		const Color4f BLUE( 0.0f, 0.0f, 1.0f, 1.0f );
		const Color4f BLACK( 0.0f, 0.0f, 0.0f, 1.0f );
		const Color4f FADEDBLACK( 0.0f, 0.0f, 0.0f, 0.5f );
		const Color4f YELLOW( 1.0f, 1.0f, 0.0f, 1.0f );

		const Color4f DARK_GREEN( 0.0f, 0.39f, 0.0f );
		const Color4f FOREST_GREEN( 0.13f, 0.55f, 0.13f );

		const Color4f GREY( 0.5f, 0.5f, 0.5f, 1.0f );
	}

	Color4f parseStringToColor4f( const std::string& str, const Color4f& defaultValue = color::WHITE );
}