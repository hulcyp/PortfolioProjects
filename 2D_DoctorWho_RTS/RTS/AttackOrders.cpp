#pragma once
#include "AttackOrders.h"
#include "Unit.h"
#include "RTSWorld.h"


namespace pdh
{
	AttackOrders::AttackOrders( Entity* target )
		:	UnitOrders( target->getPosition() )
		,	m_targetEntity( target )
	{
		m_readableDescription = "Attack entity";
		m_unitOrderType = ATTACK;
		m_attackTimer = new Timer();
		registerForEvent( "EntityKilled", *this, &AttackOrders::entityKilled );
	}
	//----------------------------------------------------
	AttackOrders::~AttackOrders()
	{
		SAFE_DELETE( m_attackTimer );
	}
	//----------------------------------------------------
	void AttackOrders::followOrders( float deltaTime )
	{
		if( m_unit && m_targetEntity )
		{
			vec2f deltaBetweenCurrentAndPrevTargPos = m_targetEntity->getPosition() - m_targetLocation;
			float distSqrdBtwnCurAndPrevPos = deltaBetweenCurrentAndPrevTargPos.dot( deltaBetweenCurrentAndPrevTargPos );

			// If target has moved more than one square away
			if( distSqrdBtwnCurAndPrevPos >= 1.0f )
			{				
				// Requests a new path... The old path will still be accessibale until a new path is generated
				// so we will use that until we get the new one
				m_targetLocation = m_targetEntity->getPosition();
				m_unit->getWorld()->requestPath( this, m_unit, m_path );
			}

			if( m_currentPathIndex >= 0 )
			{				
				m_currentGoal = vec2f( (float)m_path[ m_currentPathIndex ].x, (float)m_path[ m_currentPathIndex ].y );
			}
			else if( m_needNewNaiveTarget )
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
				delta = m_currentGoal - m_targetLocation;
				distSqrd = delta.dot( delta );
				if( m_currentPathIndex >= 0 )
				{
					--m_currentPathIndex;
					if( m_currentPathIndex < 0 && distSqrd < m_tolerance )
						attack();
					else if( m_currentPathIndex < 0 )
					{					
						m_unit->getWorld()->requestPath( this, m_unit, m_path );
					}
				}
				else
				{
					if( distSqrd < m_tolerance )
					{
						attack();
					}
					else
						m_needNewNaiveTarget = true;
				}
			}
			if( m_targetEntity != nullptr )
				m_targetLocation = m_targetEntity->getPosition();
		}
	}
	//-------------------------------------------------------------------
	void AttackOrders::entityKilled( NamedProperties& params )
	{
		Entity* entityKilled = nullptr;
		if( params.get( "entity", entityKilled ) == NamedProperties::SUCCESS )
		{
			if( entityKilled == m_targetEntity )
			{
				m_targetEntity = nullptr;
				m_ordersComplete = true;
			}
		}
	}
	//-------------------------------------------------------------------
	void AttackOrders::assignUnit( Unit* unit )
	{
		UnitOrders::assignUnit( unit );
		if( unit != nullptr )
			m_tolerance = m_unit->m_attack.range;
	}
	//-------------------------------------------------------------------
	void AttackOrders::attack()
	{
		vec2f delta = m_targetEntity->getPosition() - m_unit->m_position;
		float distSqrd = delta.dot( delta );
		if( !m_attackTimer->wasStarted() && distSqrd <= m_unit->m_attack.range )
		{
			m_attackTimer->setTime( m_unit->m_attack.speed );
			m_attackTimer->start();
		}
		else if( m_attackTimer->isTimeUp() && distSqrd <= m_unit->m_attack.range )
		{
			m_targetEntity->dealDamage( m_unit->m_attack.damage );
			m_attackTimer->setTime( m_unit->m_attack.speed );
			m_attackTimer->start();
		}
		
		if( m_targetEntity == nullptr || m_targetEntity->m_currentHealth < 0.0f )
		{
			m_ordersComplete = true;
		}
	}
}