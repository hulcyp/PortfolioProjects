#pragma once
#include "UnitOrders.h"
#include "Map.h"

namespace pdh
{
	class GotoResource : public UnitOrders
	{
	public:
		GotoResource( Map::MapTile& mapTile );

		virtual void assignUnit( Unit* unit );

		virtual void followOrders( float deltaTime );

		const Map::MapTile& getMapTile() const { return m_mapTile; }

	private:
		Map::MapTile& m_mapTile;
	};
}