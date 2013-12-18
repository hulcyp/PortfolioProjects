#pragma once
#include "GraphicDebugObject.h"

namespace pdh
{
	class GDOArrow : public GraphicDebugObject
	{
	public:
		GDOArrow( const vec3f& dir, float length, const vec3f& pos, float showUntilTime = 0.0f );
	};
}