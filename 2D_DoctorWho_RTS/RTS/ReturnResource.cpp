#include "ReturnResource.h"
#include "Map.h"
#include "Building.h"
#include "Unit.h"
#include "GotoResource.h"
#include "Controller.h"

namespace pdh
{

	ReturnResource::ReturnResource( Map::MapTile& mapTile, Building* buildingToReturnTo )
		:	UnitOrders( buildingToReturnTo->getPosition() )
		,	m_mapTile( mapTile )
		,	m_buildingToReturnTo( buildingToReturnTo )
	{
		m_readableDescription = "Returning resources";
		m_unitOrderType = RETURN_RESOURCE;
	}
	//-------------------------------------------------------------------
	void ReturnResource::followOrders( float deltaTime )
	{
		if( m_unit && m_buildingToReturnTo )
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

			vec2f dir = m_currentGoal - m_unit->m_position;
			dir.normalize();
			vec2f dx = dir * m_unit->m_movement.speed * deltaTime;
			vec2f newPos = m_unit->m_position + dx;
							
			m_unit->setPosition( newPos );		

			vec2f delta = m_unit->m_position - m_currentGoal;
			float distSqrd = delta.dot( delta );
			float tolerance = m_tolerance;
			
			vec2f deltaBetweenCurrentAndTarget = m_targetLocation - m_currentGoal;
			float distSqrdBetweenCurrentAndTarget = deltaBetweenCurrentAndTarget.dot( deltaBetweenCurrentAndTarget );
			if( distSqrdBetweenCurrentAndTarget < m_tolerance )
			{
				float buildingTolerance = m_buildingToReturnTo->getWidth() * m_buildingToReturnTo->getWidth(); 
				tolerance = buildingTolerance;
			}

			if( distSqrd < tolerance )
			{
				if( m_currentPathIndex >= 0 )
				{
					--m_currentPathIndex;
					if( m_currentPathIndex < 0 )
					{
						m_ordersComplete = true;
						if( m_mapTile.hasResource() )
							m_unit->pushOrders( new GotoResource( m_mapTile ) );
						else
						{
							TileCoords newResourceTileCoords = m_unit->getMap()->getNextClosestTileCoordsWithResource( m_mapTile.getTilecoords(), m_unit->m_resource.resourceType );
							if( newResourceTileCoords.x != -1 )
								m_unit->pushOrders( new GotoResource( m_unit->getMap()->getMapTile( newResourceTileCoords ) ) );
						}

						// Dump resources to controller and remove from unit
						m_unit->m_owner->giveResource( m_unit->m_resource.resourceType, m_unit->m_resource.currentAmount );
						m_unit->m_resource.currentAmount = 0;
						m_unit->m_resource.resourceType = Map::NONE;
					}
				}
				else
				{
					delta = m_currentGoal - m_targetLocation;
					distSqrd = delta.dot( delta );
					if( distSqrd < tolerance )
						m_ordersComplete = true;
					else
						m_needNewNaiveTarget = true;
				}
			}
		}
		else
		{
			m_ordersComplete = true;
		}
	}
}