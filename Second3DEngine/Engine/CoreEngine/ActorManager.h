#pragma once
#include "StdLibraryIncludes.h"


namespace Monky
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
		void killAll();
		void update( double deltaTime );
		void renderActors();
		void listActors();
		void playAllAnimations();
		void pauseAllAnimations();
		void stopAllAnimations();

	private:
		std::map< std::string, Actor* > m_actors;

	};
}