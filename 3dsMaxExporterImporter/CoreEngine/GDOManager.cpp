#include "GDOManager.h"
#include "GraphicDebugObject.h"
#include "ActorManager.h"
#include "CommonEngineIncludes.h"
#include <algorithm>
#include <functional>

namespace pdh
{
	GDOManager::GDOManager( ActorManager* aManager )
		:	m_actorManager( aManager )
	{}

	GDOManager::~GDOManager()
	{
		LIST_CLEANUP( m_GDOs );	
	}
	
	void GDOManager::addGDO( GraphicDebugObject* gdo )
	{
		m_GDOs.push_back( gdo );
	}

	void GDOManager::renderGDOs( Renderer* renderer, float currentTime )
	{
		m_currentTime = currentTime;
		std::for_each(	m_GDOs.begin(),
						m_GDOs.end(),
		[&] ( GraphicDebugObject* gdo )
		{
			gdo->render( renderer, currentTime );
		} );

		m_GDOs.remove_if( std::mem_fn( &GraphicDebugObject::isDead ) );		
	}

	void GDOManager::clearAllGDOs()
	{
		std::for_each(	m_GDOs.begin(),
						m_GDOs.end(),
		[&] ( GraphicDebugObject* gdo )
		{
			gdo->setIsDead( true );
		} );
	}
}