#include "Orders.h"

namespace pdh
{
	Orders::Orders( OrderType type, const vec2f& targLoc )
		:	m_ordersComplete( false )
		,	m_orderType( type )
		,	m_targetLocation( targLoc )
	{}
	//-------------------------------------
	bool Orders::areOrdersComplete() 
	{ 
		return m_ordersComplete; 
	}
}