#pragma once
#include "Camera.h"

namespace Monky
{
	class P3DCamera : public Camera
	{
	public:
		P3DCamera( const std::string& id, const vec3f& pos, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );
		virtual ~P3DCamera();

		virtual const mat4f& getViewMatrix();
	};
}