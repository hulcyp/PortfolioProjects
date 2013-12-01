#pragma once
#include "GraphicDebugObject.h"

namespace pdh
{
	class GDOOrientation : public GraphicDebugObject
	{
	public:
		GDOOrientation( float size, const vec3f& pos, const mat4f& rotation, float showUntilTime = 0.0f );
	};
}