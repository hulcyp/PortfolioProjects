#include "HoldOrders.h"
#include "Unit.h"

namespace pdh
{
	HoldOrders::HoldOrders( const vec2f& targLoc )
		:	UnitOrders( targLoc )
	{
		m_readableDescription = "Holding position";
	}
	//------------------------------------------------
	void HoldOrders::followOrders( float deltaTime )
	{
		// Just stay here until this order is canceled
	}
}