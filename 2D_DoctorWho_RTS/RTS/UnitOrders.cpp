#include "UnitOrders.h"
#include "Unit.h"
#include "RTSWorld.h"
#include <MeshFactory.h>
#include <Mesh.h>

namespace pdh
{
	UnitOrders::UnitOrders( const vec2f& targLoc, float tolerance )
		:	Orders( UNIT, targLoc )
		,	m_unit( nullptr )
		,	m_currentPathIndex( -1 )
		,	m_needNewNaiveTarget( true )
		,	m_recievedPath( false )
		,	m_tolerance( tolerance )
	{
		m_unitOrderType = NONE;
	}
	//-------------------------------------
	UnitOrders::~UnitOrders()
	{
		if( m_unit != nullptr && !m_recievedPath )
			m_unit->getWorld()->unregisterForPath( this );
	}
	//-------------------------------------
	void UnitOrders::assignUnit( Unit* unit ) 
	{ 
		m_unit = unit; 
		m_unit->getWorld()->requestPath( this, m_unit, m_path );
		m_currentPathIndex = m_path.size() - 1;
	}
	//-------------------------------------
	void UnitOrders::renderPath( bool renderAStarPath ) const
	{
		WorldCoords start;
		if( !m_unit )
			start = m_startingLoc;
		else
			start = m_unit->m_position;

		if( m_path.size() > 0 && renderAStarPath )
		{
			for( int i = m_currentPathIndex; i >=  0; --i )
			{			
				WorldCoords end = WorldCoords( (float)m_path[ i ].x, (float)m_path[ i ].y );
				Mesh* line = MeshFactory::generateLine( start, end );
				NamedProperties params;
				params.set( "mesh", line );
				fireEvent( "renderMesh", params );
			
				start = WorldCoords( (float)m_path[i].x, (float)m_path[i].y );
				SAFE_DELETE( line );			
			}
		}

		if( !m_unit )
			start = m_startingLoc;
		else
			start = m_unit->m_position;

		Mesh* line = MeshFactory::generateLine( start, getTargetLocation(), "SimpleColorMaterial", color::BLUE );
		NamedProperties params;
		params.set( "mesh", line );
		fireEvent( "renderMesh", params );
		SAFE_DELETE( line );
	}
	//-----------------------------------------------------------------
	void UnitOrders::givePath()
	{
		m_currentPathIndex = m_path.size() - 1;
		m_recievedPath = true;
	}
	//-----------------------------------------------------------------
	const vec2f& UnitOrders::getCurrentLoc() const
	{
		return m_unit->m_position;
	}
	//-----------------------------------------------------------------
	// Protected member functions
	//-----------------------------------------------------------------
	vec2f UnitOrders::getNextNaiveTarget( const vec2f& currentLoc ) const
	{
		vec2f delta = m_targetLocation - currentLoc;
		//float xDist = m_targetLocation.x - currentLoc.x;
		//float yDist = m_targetLocation.y - currentLoc.y;

		int xSign = MathFuncs<float>::getSign( delta.x );
		int ySign = MathFuncs<float>::getSign( delta.y );

		delta.x = std::abs( delta.x );
		delta.y = std::abs( delta.y );

		vec2f dir;

		float distSqrd = delta.dot( delta );
		if( delta.x > delta.y && distSqrd > m_tolerance )
		{
			if( xSign > 0 )
				dir.x += 1.0f;
			else
				dir.x -= 1.0f;
		}
		else if( distSqrd > m_tolerance )
		{
			if( ySign > 0 )
				dir.y += 1.0f;
			else
				dir.y -= 1.0f;
		}

		vec2f newTargLoc = currentLoc;
		bool canMoveOnTile = false;
		
		for( int i = 0; i < 3 && !canMoveOnTile; ++i ) 
		{
			canMoveOnTile = m_unit->getWorld()->canEntityMoveOnTile( m_unit, m_unit->getMap()->getTileCoordsFromWorldCoords( newTargLoc + dir ) );
			if( canMoveOnTile )
			{
				newTargLoc = newTargLoc + dir;
			}
			else
			{
				if( i == 1 )
					dir = -dir;
				dir = dir.yx();
			}
		}

		return newTargLoc;
	}
}