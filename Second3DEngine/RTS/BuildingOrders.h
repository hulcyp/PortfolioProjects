#pragma once
#include "Orders.h"

namespace pdh
{
	class Building;
	class UnitBlueprint;

	class BuildingOrders : public Orders
	{
	public:
		enum OrderType
		{
			BUILD_UNIT
		};

		BuildingOrders( Building* building );
		virtual ~BuildingOrders();

	protected:
		Building* m_building;

	};
}