#include "CollectResource.h"
#include "TerrainType.h"
#include "Unit.h"
#include "ReturnResource.h"
#include "RTSWorld.h"

namespace pdh
{
	CollectResource::CollectResource( Map::MapTile& mapTile )
		:	UnitOrders( vec2f( (float)mapTile.getTilecoords().x, (float)mapTile.getTilecoords().y ) )	
		,	m_mapTile( mapTile )
	{
		m_collectionTimer = new Timer();
		m_collectionTimer->setTime( mapTile.getTerrainType()->getResourceInfo().timePerResource );
		m_readableDescription = "Collecting Resources";
		m_collectionTimer->start();
		m_unitOrderType = HARVEST_RESOURCE;
	}
	//-------------------------------------------
	void CollectResource::followOrders( float deltaTime )
	{
		if( m_unit )
		{
			if( m_collectionTimer->isTimeUp() )
			{

				if( m_mapTile.hasResource() && m_unit->m_resource.currentAmount < m_mapTile.getMaxCollectAmountPerTrip() )
				{
					m_unit->m_resource.currentAmount += m_unit->m_world->getMap()->collectResourceFromTile( m_mapTile );

				}
				else
				{
					m_ordersComplete = true;
					// Find closes place to take resource to
					Building* toReturnTo = m_unit->m_world->getBuildingToReturnResources( m_unit );
					m_unit->pushOrders( new ReturnResource( m_mapTile, toReturnTo ) );
				}
				m_collectionTimer->start();
			}
		}		
	}
	//----------------------------------------------
	void CollectResource::assignUnit( Unit* unit )
	{
		UnitOrders::assignUnit( unit );
		unit->m_resource.resourceType = m_mapTile.getResourceType();
	}
}