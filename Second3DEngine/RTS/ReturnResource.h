#pragma once
#include "UnitOrders.h"
#include "Map.h"

namespace pdh
{
	class Building;

	class ReturnResource : public UnitOrders
	{
	public:
		ReturnResource( Map::MapTile& mapTile, Building* buildingToReturnTo );

		virtual void followOrders( float deltaTime );

	private:
		Map::MapTile& m_mapTile;
		Building* m_buildingToReturnTo;
	};
}