#pragma once
#include "GraphicDebugObject.h"

namespace Monky
{
	class GDOOrientation : public GraphicDebugObject
	{
	public:
		GDOOrientation( float size, const vec3f& pos, const mat3f& rotation, float showUntilTime = 0.0f );
		GDOOrientation( float size, const vec3f& pos, const mat3f& rotation, Color4f& color, float showUntilTime = 0.0f );
	};
}