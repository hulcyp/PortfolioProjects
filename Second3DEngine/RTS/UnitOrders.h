#pragma once
#include "Orders.h"
#include <CommonEngineIncludes.h>
#include "Map.h"

namespace pdh
{
	class Unit;

	class UnitOrders : public Orders
	{
	public:
		enum UnitOrderType
		{
			NONE,
			ATTACK,
			ATTACK_MOVE,
			MOVE,
			GO_TO_RESOURCE,
			HARVEST_RESOURCE,
			RETURN_RESOURCE,
			BUILD
		};

		UnitOrders( const vec2f& targLoc, float tolerance = 0.001f );
		virtual ~UnitOrders();

		virtual void assignUnit( Unit* unit );
		
		virtual void renderPath( bool renderAStarPath = false ) const;
		virtual void givePath();
		const vec2f& getCurrentLoc() const;
		void setStartLoc( const vec2f& startLoc ) { m_startingLoc = startLoc; }

		UnitOrderType getUnitOrderType() const { return m_unitOrderType; }

	protected:
		vec2f getNextNaiveTarget( const vec2f& currentLoc ) const;

		Unit* m_unit;
		std::vector < TileCoords > m_path;
		int m_currentPathIndex;
		bool m_needNewNaiveTarget;
		bool m_recievedPath;
		float m_tolerance;
		vec2f m_currentGoal;
		vec2f m_startingLoc;
		UnitOrderType m_unitOrderType;
	};
}