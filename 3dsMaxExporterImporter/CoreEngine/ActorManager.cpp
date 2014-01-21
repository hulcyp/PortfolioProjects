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

	void ActorManager::removeActor( const std::string& id )
	{
		m_actors.erase( id );
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

	void ActorManager::update( double deltaTime )
	{
		for( aIter iter = m_actors.begin(); iter != m_actors.end(); ++iter )
		{
			iter->second->update( deltaTime );
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

	void ActorManager::playAllAnimations()
	{
		for( aIter iter = m_actors.begin(); iter != m_actors.end(); ++iter )
		{
			iter->second->play();
		}
	}

	void ActorManager::pauseAllAnimations()
	{
		for( aIter iter = m_actors.begin(); iter != m_actors.end(); ++iter )
		{
			iter->second->pause();
		}
	}

	void ActorManager::stopAllAnimations()
	{
		for( aIter iter = m_actors.begin(); iter != m_actors.end(); ++iter )
		{
			iter->second->stop();
		}
	}
}