#include "Entity.h"
#include "RTSWorld.h"
#include "Controller.h"
#include "Orders.h"
#include <Mesh.h>
#include <EventSystemInterface.h>
#include <MeshFactory.h>

namespace pdh
{		
	Entity::Entity( RTSWorld* world )
		:	m_owner( nullptr )
		,	m_world( world )
		,	m_isSelected( false )
		,	m_entityType( NONE )
	{}
	//----------------------------------------------------------------
	Entity::~Entity()
	{		
		DEQUE_CLEANUP( m_orders );	
	}
	//----------------------------------------------------------------
	void Entity::debugRender()
	{}
	//----------------------------------------------------------------
	void Entity::renderOrders()
	{
		vec2f currentPos = m_position;
		for( size_t i = 0; i <  m_orders.size(); ++i )
		{
			Mesh* line = MeshFactory::generateLine( currentPos, m_orders[i]->getTargetLocation() );
			NamedProperties params;
			params.set( "mesh", line );
			fireEvent( "renderMesh", params );
			
			currentPos = m_orders[i]->getTargetLocation();
			SAFE_DELETE( line );
		}
	}
	//----------------------------------------------------------------
	void Entity::setPosition( const vec2f& pos )
	{ 
		m_prevPosition = m_position;
		m_position = pos; 
	}
	//----------------------------------------------------------------
	void Entity::setHealth( float health )
	{
		m_currentHealth = MathFuncs<float>::clamp( health, 0.0f, m_maxHealth );
	}
	//----------------------------------------------------------------
	void Entity::setSelected( bool selected, bool allySelected )
	{
		m_isSelected = selected;
		m_isAllySelected = allySelected;
	}
	//----------------------------------------------------------------
	Map* Entity::getMap() const
	{
		if( m_world )
			return m_world->getMap();
		else
			return nullptr;
	}
	RTSWorld* Entity::getWorld() const
	{
		return m_world;
	}
	//----------------------------------------------------------------
	Map::MapTile& Entity::getMapTileCurrentlyOn() const
	{
		assertion( m_world != nullptr, "Error entity does not know what world it exists in" ); 
		return m_world->getMap()->getMapTile( m_world->getMap()->getTileCoordsFromWorldCoords( WorldCoords( m_position.x, m_position.y ) ) );
	}
	//----------------------------------------------------------------
	void Entity::setOwner( Controller* owner )
	{
		m_owner = owner;
		if( m_owner )
			m_owner->giveEntityToControl( this );
	}
	//------------------------------------------
	Controller* Entity::getOwner() const
	{
		return m_owner;
	}
	//------------------------------------------
	void Entity::assignOrders( Orders* orders )
	{
		DEQUE_CLEANUP( m_orders );
		if( orders != nullptr )
			m_orders.push_back( orders );
	}
	//------------------------------------------
	void Entity::pushOrders( Orders* orders )
	{
		if( orders != nullptr )
			m_orders.push_back( orders );
	}
	//------------------------------------------
	void Entity::cancelOrders()
	{
		DEQUE_CLEANUP( m_orders );		
	}
	//------------------------------------------
	void Entity::cancelAllQueuedOrders()
	{
		while( m_orders.size() != 1 )
		{
			SAFE_DELETE( m_orders.back() );
			m_orders.pop_back();
		}
	}
	//------------------------------------------
	void Entity::popOrder()
	{
		if( m_orders.size() > 0 )
		{
			SAFE_DELETE( m_orders.back() );
			m_orders.pop_back();
		}
	}
	//------------------------------------------
	void Entity::dealDamage( float damage )
	{
		m_currentHealth -= damage;
		if( m_currentHealth <= 0.0f )
			m_world->killEntity( this );
	}
	//------------------------------------------
	void Entity::getVisibleTiles( float visionRadius, std::vector< TileCoords >& tiles )
	{
		TileCoords center = m_world->getMap()->getTileCoordsFromWorldCoords( m_position );
		for( int y = center.y - (int)visionRadius; y < center.y + (int)visionRadius; ++y )
		{
			for( int x = center.x - (int)visionRadius; x < center.x + (int)visionRadius; ++x )
			{
				TileCoords tileCoords( x, y );
				int index = m_world->getMap()->getTileIndexFromTileCoords( tileCoords );
				if( index != -1 )
				{
					TileCoords delta = tileCoords - center;
					float distSqrd = (float)delta.dot( delta );
					if( distSqrd < visionRadius * visionRadius )
						tiles.push_back( tileCoords );
				}
			}
		}
	}
}