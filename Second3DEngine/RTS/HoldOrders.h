#pragma once
#include "UnitOrders.h"

namespace pdh
{
	class HoldOrders : public UnitOrders
	{
	public:
		HoldOrders( const vec2f& targLoc );

		virtual void followOrders( float deltaTime );
	};
}