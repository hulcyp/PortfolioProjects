#include "MoveOrders.h"
#include "Unit.h"

namespace pdh
{
	MoveOrders::MoveOrders( const vec2f& pos, float tolerance )
		:	UnitOrders( pos, tolerance )
	{
		m_readableDescription = "Moving to location";
		m_unitOrderType = MOVE;
	}
	//---------------------------------------------------
	void MoveOrders::followOrders( float deltaTime )
	{
		if( m_unit )
		{
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
			if( distSqrd < m_tolerance || distSqrd < m_unit->m_movement.speed*deltaTime )
			{
				if( m_currentPathIndex >= 0 )
				{
					--m_currentPathIndex;
					if( m_currentPathIndex < 0 )
						m_ordersComplete = true;
				}
				else
				{
					delta = m_currentGoal - m_targetLocation;
					distSqrd = delta.dot( delta );
					if( distSqrd < m_tolerance )
						m_ordersComplete = true;
					else
						m_needNewNaiveTarget = true;
				}
			}
		}
	}
}