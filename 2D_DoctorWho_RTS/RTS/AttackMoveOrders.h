#pragma once
#include "AttackOrders.h"
#include <EventSystemInterface.h>

namespace pdh
{
	class Entity;
	class Timer;

	class AttackMoveOrders : public UnitOrders
	{
	public:
		AttackMoveOrders( const vec2f& targLoc );
		virtual ~AttackMoveOrders();

		virtual void followOrders( float deltaTime );
		
		void entityKilled( NamedProperties& params );

		virtual void givePath();

	protected:
		void searchForTarget();
		bool isTargetStillInRange();
		void attack();
		void move( float deltaTime );

	private:
		Entity* m_currentTargetEntity;
		Timer* m_attackTimer;
		vec2f m_finalTargetLocation;
		float m_finalTargetTolerance;
		std::vector< TileCoords > m_pathToTargetEntity;
		int m_currentPathToTargetIndex;
		bool m_recievedPathToTarget;
	};
}