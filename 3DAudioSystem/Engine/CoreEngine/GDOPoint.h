#pragma once
#include "GraphicDebugObject.h"
#include "Colors.h"

namespace Monky
{
	class GDOPoint : public GraphicDebugObject
	{
	public:
		GDOPoint( float radius, const vec3f& pos, float showUntilTime = 0.0f, Color4f color = color::WHITE );
	};
}