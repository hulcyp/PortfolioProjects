#pragma once
#include "BuildingOrders.h"

namespace pdh
{
	class BuildUnitOrders : public BuildingOrders
	{
	public:
		BuildUnitOrders( Building* building, const std::string& unitType );
		BuildUnitOrders( Building* building, const UnitBlueprint* unitBP );
				
		virtual void followOrders( float deltaTime );
		virtual void renderOrders();

	private:		
		const UnitBlueprint* m_unitBlueprint;
		float m_currentBuildTime;
		bool m_startedBuilding;
	};
}