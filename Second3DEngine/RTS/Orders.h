#pragma once
#include <string>
#include <CommonEngineIncludes.h>

namespace pdh
{
	class Orders
	{
	public:
		enum OrderType
		{
			UNIT,
			BUILDING
		};

		Orders( OrderType type, const vec2f& targLoc );
		virtual ~Orders(){}

		virtual void followOrders( float deltaTime ) = 0;
		bool areOrdersComplete();
		OrderType getType() { return m_orderType; }
		const std::string& getDescritpion() { return m_readableDescription; }
		const vec2f& getTargetLocation() const { return m_targetLocation; }

		virtual void renderOrders(){}


	protected:		
		bool m_ordersComplete;
		OrderType m_orderType;
		std::string m_readableDescription;
		vec2f m_targetLocation;
	};
}