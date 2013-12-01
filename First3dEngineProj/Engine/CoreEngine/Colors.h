#pragma once

namespace pdh
{
	struct Color4f
	{
		Color4f( float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f )
			:	r( r )
			,	g( g )
			,	b( b )
			,	a( a )
		{}
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
	}
}