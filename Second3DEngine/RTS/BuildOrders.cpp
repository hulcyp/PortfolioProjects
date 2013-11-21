#include "BuildOrders.h"
#include "BuildingBlueprint.h"
#include "RTSWorld.h"
#include <Font.h>
#include "Building.h"
#include "MoveOrders.h"
#include "Unit.h"
#include "Controller.h"

namespace pdh
{
	BuildOrders::BuildOrders( const std::string& buildingType, const vec2f& loc )
		:	UnitOrders( loc )
		,	m_building( nullptr ) 
	{
		m_readableDescription = "Building";
		m_buildingBP = BuildingBlueprint::getBlueprint( buildingType );
		m_unitOrderType = BUILD;
	}
	//-------------------------------------------------------
	BuildOrders::BuildOrders( Building* building )
		:	UnitOrders( building->getPosition() )
		,	m_building( building )
		,	m_buildingBP( BuildingBlueprint::getBlueprint( building->getBlueprintName() ) )
	{
		m_readableDescription = "Building";
		if( m_building )
		{
			m_building->addUnitWorkingOnBuilding();
			m_healthIncreaseRate = m_building->m_maxHealth / m_building->m_blueprint->getBuildTime();
			m_healthIncreaseRate *= m_building->getCurrentHealthIncreaseFactor();
			m_buildTimeIncreaseFactor = m_building->getCurrentBuildRateFactor();
		}
	}
	//-------------------------------------------------------
	BuildOrders::~BuildOrders()
	{
		if( m_building )
			m_building->removeUnitWorkdingOnBuilding();
	}
	//-------------------------------------------------------
	void BuildOrders::followOrders( float deltaTime )
	{
		if( m_unit )
		{							
			if( m_building == nullptr )
			{
				m_building = m_unit->m_world->spawnBuilding( m_buildingBP, m_targetLocation, m_unit->m_owner );	
				m_building->addUnitWorkingOnBuilding();
				m_healthIncreaseRate = m_building->m_maxHealth / m_building->m_blueprint->getBuildTime();
				m_healthIncreaseRate *= m_building->getCurrentHealthIncreaseFactor();
				m_buildTimeIncreaseFactor = m_building->getCurrentBuildRateFactor();
			}
			else
			{
				//m_building->setCurrentBuildTime( m_currentBuildTime );

				if( !m_building->isComplete() )
				{					
					m_building->addToCurrentBuildTime( deltaTime*m_buildTimeIncreaseFactor );
					if( m_building->m_currentHealth < m_building->m_maxHealth )
						m_building->m_currentHealth += m_healthIncreaseRate * deltaTime;
				}
				else
				{
					m_ordersComplete = true;
				}	
			}
		}
	}
}