#pragma once
#include "UnitOrders.h"
#include <CommonEngineIncludes.h>
#include "Map.h"

namespace pdh
{
	class MoveOrders : public UnitOrders
	{
	public:
		MoveOrders( const vec2f& pos, float m_tolerance = 0.001f );

		virtual void followOrders( float deltaTime );		
	};
}