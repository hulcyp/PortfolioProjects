#pragma once
#include "UnitOrders.h"
#include <CommonEngineIncludes.h>
#include <Timer.h>
#include <EventSystemInterface.h>

namespace pdh
{
	class Entity;

	class AttackOrders : public UnitOrders
	{
	public:
		AttackOrders( Entity* target );
		virtual ~AttackOrders();

		virtual void followOrders( float deltaTime );
		
		void entityKilled( NamedProperties& params );

		virtual void assignUnit( Unit* unit );

	protected:
		void attack();

	private:
		Entity* m_targetEntity;
		Timer* m_attackTimer;
	};
}