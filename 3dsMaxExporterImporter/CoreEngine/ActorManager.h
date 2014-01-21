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
		void removeActor( const std::string& id );
		Actor* getActor( const std::string& id );
		void update( double deltaTime );
		void renderActors( Renderer* renderer );
		void listActors();
		void playAllAnimations();
		void pauseAllAnimations();
		void stopAllAnimations();

	private:
		std::map< std::string, Actor* > m_actors;

	};
}