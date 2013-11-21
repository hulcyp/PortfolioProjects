#include "BuildingOrders.h"
#include "Building.h"

namespace pdh
{
	BuildingOrders::BuildingOrders( Building* building )
		:	Orders( BUILDING, building->getPosition() ) 
		,	m_building( building )
	{}
	//------------------------------------------------------
	BuildingOrders::~BuildingOrders()
	{
		m_building = nullptr;
	}
}