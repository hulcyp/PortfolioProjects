#include "PrepareToBuildOrders.h"
#include "Building.h"
#include "BuildingBlueprint.h"
#include "Unit.h"
#include "Controller.h"

namespace pdh
{	
	PrepareToBuildOrders::PrepareToBuildOrders( const std::string& buildingType, const vec2f& loc )
		:	UnitOrders( loc )
		,	m_building( nullptr )
	{
		m_readableDescription = "Preparing to build";
		m_buildingBP = BuildingBlueprint::getBlueprint( buildingType );
	}
	//-------------------------------------------------------------
	PrepareToBuildOrders::PrepareToBuildOrders( Building* building )
		:	UnitOrders( building->getPosition() )
		,	m_building( building )
		,	m_buildingBP( BuildingBlueprint::getBlueprint( building->getBlueprintName() ) )
	{
		m_readableDescription = "Preparing to build";
	}
	//-------------------------------------------------------------
	void PrepareToBuildOrders::followOrders( float deltaTime )
	{
		if( m_unit )
		{							
			if( m_building == nullptr )
			{
				if( !m_unit->m_owner->requestResource( Map::FISH_FINGERS, m_buildingBP->getBuildingCost().fishFingers ) )
				{
					NamedProperties params;
					m_ordersComplete = true;
					std::string text = "Not enough Fish Fingers";
					params.set( "text", text );
					fireEvent( "NotEnoughResources", params );
				}
				if( !m_unit->m_owner->requestResource( Map::CUSTARD, m_buildingBP->getBuildingCost().custard ) )
				{
					NamedProperties params;
					m_ordersComplete = true;
					std::string text = "Not enough Custard";
					params.set( "text", text );
					fireEvent( "NotEnoughResources", params );
				}
				if( !m_unit->m_owner->requestResource( Map::TIME_ENERGY, m_buildingBP->getBuildingCost().timeEnergy ) )
				{
					NamedProperties params;
					m_ordersComplete = true;
					std::string text = "Not enough Time Energy";
					params.set( "text", text );
					fireEvent( "NotEnoughResources", params );
				}

				if( m_ordersComplete )
				{
					m_unit->cancelAllQueuedOrders();
				}
				
			}
			m_ordersComplete = true;
		}
	}
}