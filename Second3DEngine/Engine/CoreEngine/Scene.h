#pragma once
#include "CommonEngineIncludes.h"


namespace Monky
{
	class Camera;
	class Actor;

	class Scene
	{
	public:
		Scene( const std::string& name )
			:	name( name )
		{}
		std::vector< Actor* > actors;
		std::string name;
		std::vector< Camera* > cameras;
	};
}