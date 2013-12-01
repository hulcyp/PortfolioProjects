#include "ActorManager.h"
#include "Actor.h"
#include "CommonEngineIncludes.h"

namespace pdh
{
	ActorManager::~ActorManager()
	{
		MAP_CLEANUP( m_actors );
	}

	void ActorManager::addActor( Actor* actor )
	{
		Actor*& tempActor = m_actors[ actor->getID() ];
		if( !tempActor )
		{
			tempActor = actor;
		}
		else
		{
			consolePrintColorf( "The Actor %s, already exists", color::YELLOW, actor->getID().c_str() );
		}
	}

	Actor* ActorManager::getActor( const std::string& id )
	{
		aIter iter = m_actors.find( id );
		if( iter != m_actors.end() )
		{
			return iter->second;
		}
		else
		{
			consolePrintColorf( "The Actor %s, does not exist", color::YELLOW, id.c_str() );
			return nullptr;
		}
	}

	void ActorManager::renderActors( Renderer* renderer )
	{
		for( aIter iter = m_actors.begin(); iter != m_actors.end(); ++iter )
		{
			iter->second->render( renderer );
		}
	}

	void ActorManager::listActors()
	{
		for( aIter iter = m_actors.begin(); iter != m_actors.end(); ++iter )
		{
			consolePrintColorf( iter->first.c_str(), color::YELLOW );
		}
	}
}