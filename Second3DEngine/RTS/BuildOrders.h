#pragma once
#include "UnitOrders.h"
#include <CommonEngineIncludes.h>
#include <string>

namespace pdh
{
	class Building;
	class BuildingBlueprint;
	
	class BuildOrders : public UnitOrders
	{
	public:
		BuildOrders( const std::string& buildingType, const vec2f& loc );
		BuildOrders( Building* building );
		virtual ~BuildOrders();

		virtual void followOrders( float deltaTime );
		virtual void renderPath() const{}

	private:
		BuildingBlueprint* m_buildingBP;
		Building* m_building;
		float m_healthIncreaseRate;
		float m_buildTimeIncreaseFactor;
	};
}