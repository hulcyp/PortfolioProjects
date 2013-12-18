#include "GotoResource.h"
#include "Unit.h"
#include "CollectResource.h"
#include "Building.h"
#include "RTSWorld.h"
#include "ReturnResource.h"


namespace pdh
{
	GotoResource::GotoResource( Map::MapTile& mapTile )
		:	UnitOrders( vec2f( (float)mapTile.getTilecoords().x, (float)mapTile.getTilecoords().y ) )	
		,	m_mapTile( mapTile )
	{
		m_readableDescription = "Going to resources";
		m_unitOrderType = GO_TO_RESOURCE;
	}
	//------------------------------------------------------
	void GotoResource::assignUnit( Unit* unit )
	{
		if( unit )
		{
			RTSWorld* world = unit->getWorld();
			TileCoords tileCoords = world->getTileCoordsForCollectingResource( unit, m_mapTile );
			if( tileCoords.x != -1 )
				m_targetLocation = unit->getMap()->getWorldCoordsFromTileCoords( tileCoords );
			else
			{
				// can't get to resource
			}

			UnitOrders::assignUnit( unit );
			if( m_unit->m_resource.currentAmount >= m_mapTile.getMaxCollectAmountPerTrip() )
			{
				Building* toReturnTo = m_unit->m_world->getBuildingToReturnResources( m_unit );
				m_unit->assignOrders( new ReturnResource( m_mapTile, toReturnTo ) );
			}
		}
	}
	//------------------------------------------------------
	void GotoResource::followOrders( float deltaTime )
	{
		if( m_unit )
		{
			if( m_currentPathIndex >= 0 )
			{				
				m_currentGoal = vec2f( (float)m_path[ m_currentPathIndex ].x, (float)m_path[ m_currentPathIndex ].y );
			}
			else if( m_needNewNaiveTarget )
			{
				m_currentGoal = getNextNaiveTarget( m_unit->m_position );
				m_needNewNaiveTarget = false;
			}

			Map* map = m_unit->getMap();
			
			vec2f dir =  m_currentGoal - m_unit->m_position;
			dir.normalize();
			vec2f dx = dir * m_unit->m_movement.speed * deltaTime;
			vec2f newPos = m_unit->m_position + dx;
			
			m_unit->setPosition( newPos );					

			vec2f delta = m_unit->m_position - m_currentGoal;
			float distSqrd = delta.dot( delta );
			if( distSqrd < m_tolerance )
			{
				if( m_currentPathIndex >= 0 )
				{
					--m_currentPathIndex;
					if( m_currentPathIndex < 0 )
					{
						m_unit->pushOrders( new CollectResource( m_mapTile ) );
						m_ordersComplete = true;
					}
				}
				else
				{
					delta = m_currentGoal - m_targetLocation;
					distSqrd = delta.dot( delta );
					if( distSqrd < m_tolerance )
					{
						m_unit->pushOrders( new CollectResource( m_mapTile ) );
						m_ordersComplete = true;
					}
					else
						m_needNewNaiveTarget = true;
				}
			}
		}
	}	
}