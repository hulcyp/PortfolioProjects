#pragma once
#include "Mesh.h"
#include <Vector3.h>
#include "Colors.h"

namespace pdh
{
	typedef Vector3<float> vec3f;

	std::shared_ptr< Mesh > generateCube( float size, Color4f color = color::WHITE );
}