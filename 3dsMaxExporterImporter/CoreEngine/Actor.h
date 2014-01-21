#pragma once
#include <Vector3.h>
#include "CommonEngineIncludes.h"
#include "Object.h"

namespace pdh
{
	class Actor : public Object
	{
	public:
		Actor( const std::string& id, Mesh* mesh = nullptr );			
	};
}