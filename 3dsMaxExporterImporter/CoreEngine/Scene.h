#pragma once
#include <string>
#include <vector>


namespace pdh
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