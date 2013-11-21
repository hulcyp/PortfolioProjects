#pragma once
#include "UnitOrders.h"
#include "Map.h"
#include <Timer.h>

namespace pdh
{
	class CollectResource : public UnitOrders
	{
	public:
		CollectResource( Map::MapTile& mapTile );

		virtual void followOrders( float deltaTime );
		virtual void assignUnit( Unit* unit );

	private:
		Map::MapTile& m_mapTile;
		float m_timeToCollect;
		int m_timePerResource;
		Timer* m_collectionTimer;
	};
}