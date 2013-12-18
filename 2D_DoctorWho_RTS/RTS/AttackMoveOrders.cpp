#include "AttackMoveOrders.h"
#include "Unit.h"
#include "RTSWorld.h"

namespace pdh
{
	AttackMoveOrders::AttackMoveOrders( const vec2f& targLoc )
		:	UnitOrders( targLoc )
		,	m_currentTargetEntity( nullptr )
		,	m_finalTargetLocation( targLoc )
	{
		m_readableDescription = "Attack Move";
		m_unitOrderType = ATTACK_MOVE;
		m_attackTimer = new Timer();
		registerForEvent( "EntityKilled", *this, &AttackMoveOrders::entityKilled );
		m_finalTargetTolerance = m_tolerance;
	}
	//-------------------------------------------------------
	AttackMoveOrders::~AttackMoveOrders()
	{
		SAFE_DELETE( m_attackTimer );
	}
	//-------------------------------------------------------
	void AttackMoveOrders::followOrders( float deltaTime )
	{
		if( m_currentTargetEntity == nullptr )
			searchForTarget();

		if( m_currentTargetEntity == nullptr )
		{
			m_targetLocation = m_finalTargetLocation;
			m_tolerance = m_finalTargetTolerance;
			move( deltaTime );
		}
		else
		{
			if( isTargetStillInRange() )
			{
				vec2f deltaBetweenCurrentAndPrevTargPos = m_currentTargetEntity->getPosition() - m_targetLocation;
				float distSqrdBtwnCurAndPrevPos = deltaBetweenCurrentAndPrevTargPos.dot( deltaBetweenCurrentAndPrevTargPos );

				// If target has moved more than one square away
				if( distSqrdBtwnCurAndPrevPos >= 1.0f )
				{				
					// Requests a new path... The old path will still be accessibale until a new path is generated
					// so we will use that until we get the new one
					m_targetLocation = m_currentTargetEntity->getPosition();
					m_unit->getWorld()->requestPath( this, m_unit, m_path );
				}

				m_tolerance = m_unit->m_attack.range;
				move( deltaTime );
				attack();
			}
			else
			{
				m_currentTargetEntity = nullptr;
			}
		}
	}
	//-------------------------------------------------------------------
	void AttackMoveOrders::entityKilled( NamedProperties& params )
	{
		Entity* entityKilled = nullptr;
		if( params.get( "entity", entityKilled ) == NamedProperties::SUCCESS )
		{
			if( entityKilled == m_currentTargetEntity )
			{
				m_currentTargetEntity = nullptr;
			}
		}
	}
	//--------------------------------------------------------
	void AttackMoveOrders::givePath()
	{
		if( !m_recievedPath )
			UnitOrders::givePath();
		else
		{
			m_currentPathToTargetIndex = m_pathToTargetEntity.size() - 1;
			m_recievedPathToTarget = true;
		}
	}
	//--------------------------------------------------------
	void AttackMoveOrders::searchForTarget()
	{
		if( m_unit != nullptr )
		{
			Entity* newTarget = m_unit->m_world->getClosestEnemyEntityInRadius( m_unit, m_unit->m_movement.vision );
			if( m_currentTargetEntity == nullptr && newTarget )
			{
				m_currentTargetEntity = newTarget;
				m_unit->m_world->requestPath( this, m_unit, m_pathToTargetEntity );
			}
		}
	}
	//--------------------------------------------------------
	bool AttackMoveOrders::isTargetStillInRange()
	{
		bool isInRange = false;
		if( m_currentTargetEntity != nullptr )
		{
			WorldCoords delta = m_currentTargetEntity->getPosition() - m_unit->m_position;
			float distSqrd = delta.dot( delta );
			if( distSqrd <= m_unit->m_movement.vision * m_unit->m_movement.vision )
				isInRange = true;
		}

		return isInRange;
	}
	//--------------------------------------------------------
	void AttackMoveOrders::attack()
	{
		vec2f delta = m_currentTargetEntity->getPosition() - m_unit->m_position;
		float distSqrd = delta.dot( delta );
		if( !m_attackTimer->wasStarted() && distSqrd <= m_unit->m_attack.range )
		{
			m_attackTimer->setTime( m_unit->m_attack.speed );
			m_attackTimer->start();
		}
		else if( m_attackTimer->isTimeUp() && distSqrd <= m_unit->m_attack.range )
		{
			m_currentTargetEntity->dealDamage( m_unit->m_attack.damage );
			m_attackTimer->setTime( m_unit->m_attack.speed );
			m_attackTimer->start();
		}
	}
	void AttackMoveOrders::move( float deltaTime )
	{
		if( m_unit )
		{
			if( m_currentPathIndex >= 0 && m_currentTargetEntity == nullptr )
			{				
				m_currentGoal = vec2f( (float)m_path[ m_currentPathIndex ].x, (float)m_path[ m_currentPathIndex ].y );
			}
			else if( m_currentPathToTargetIndex >= 0 && m_currentTargetEntity != nullptr )
			{
				m_currentGoal = vec2f( (float)m_pathToTargetEntity[ m_currentPathToTargetIndex ].x, 
					(float)m_pathToTargetEntity[ m_currentPathToTargetIndex ].y );
			}
			else if( m_needNewNaiveTarget || m_currentTargetEntity != nullptr )
			{
				m_currentGoal = getNextNaiveTarget( m_unit->m_position );
				m_needNewNaiveTarget = false;
			}

			vec2f dir = m_currentGoal - m_unit->m_position;
			dir.normalize();
			vec2f dx = dir * m_unit->m_movement.speed * deltaTime;
			vec2f newPos = m_unit->m_position + dx;

			m_unit->setPosition( newPos );

			vec2f delta = m_unit->m_position - m_currentGoal;
			float distSqrd = delta.dot( delta );
			if( distSqrd < m_tolerance )
			{
				if( m_currentPathIndex >= 0 && m_recievedPath && m_currentTargetEntity == nullptr  )
				{
					--m_currentPathIndex;
					if( m_currentPathIndex < 0 )
						m_ordersComplete = true;
				}
				else if( m_currentTargetEntity == nullptr )
				{
					delta = m_currentGoal - m_finalTargetLocation;
					distSqrd = delta.dot( delta );
					if( distSqrd < m_finalTargetTolerance )
						m_ordersComplete = true;
					else
						m_needNewNaiveTarget = true;
				}
				else
				{
					if( m_currentPathToTargetIndex >= 0 && m_recievedPathToTarget )
					{
						--m_currentPathToTargetIndex;						
					}
					else
					{
						delta = m_currentGoal - m_targetLocation;
						distSqrd = delta.dot( delta );
						if( distSqrd >= m_tolerance )
							m_needNewNaiveTarget = true;
					}
				}
			}
		}
	}
}