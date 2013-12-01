#pragma once
#include <map>
#include <string>


namespace pdh
{
	class Actor;
	class Renderer;

	typedef std::map< std::string, Actor* >::iterator aIter;

	class ActorManager
	{
	public:
		virtual ~ActorManager();

		void addActor( Actor* actor );
		Actor* getActor( const std::string& id );
		void renderActors( Renderer* renderer );
		void listActors();

	private:
		std::map< std::string, Actor* > m_actors;

	};
}