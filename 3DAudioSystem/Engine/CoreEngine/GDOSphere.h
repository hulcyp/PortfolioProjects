#pragma once
#include "GraphicDebugObject.h"
#include "Colors.h"

namespace Monky
{
	class GDOSphere : public GraphicDebugObject
	{
	public:
		GDOSphere( float radius, int rings, int sectors, const vec3f& pos, float showUntilTime = 0.0f, Color4f color = color::WHITE );

	};
}