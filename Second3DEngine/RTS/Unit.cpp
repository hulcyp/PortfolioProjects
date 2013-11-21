#include "Unit.h"
#include "UnitOrders.h"
#include <Renderer.h>
#include "UnitBlueprint.h"
#include "TerrainType.h"
#include <Font.h>
#include <CommonEngineIncludes.h>
#include "RTSWorld.h"
#include <MeshFactory.h>
#include "Controller.h"
#include "AttackOrders.h"
#include "MoveOrders.h"
#include <RandNumGen.h>

namespace pdh
{
	Unit::Unit( const UnitBlueprint* blueprint, RTSWorld* world  )
		:	Entity( world )	
		,	m_blueprint( blueprint )
		,	m_renderAStarPath( false )
	{
		m_entityType = UNIT;
		registerForEvent( "renderAStar", *this, &Unit::toggleAStarRender );
	}
	//----------------------------------------------------------------
	Unit::~Unit()
	{
		unregisterForEvent( *this );
		if( m_owner != nullptr )
			m_owner->removeFromCurrentSupplyCount( m_blueprint->getUnitCost().supply );
	}
	//----------------------------------------------------------------
	void Unit::update( float deltaTime )
	{
		getVisibleTiles( m_movement.vision, m_visibleTiles );
		m_world->getVisibleEntitesInRadius( this, m_position, m_movement.vision, m_visibleEntities );
		if( m_owner )
		{
			m_owner->notifyOfVisibleTiles( m_visibleTiles );
			m_owner->notifyOfVisibleEntites( m_visibleEntities );
			m_visibleTiles.clear();
			m_visibleEntities.clear();
		}
		if( m_orders.size() > 0 )
		{
			m_orders.front()->followOrders( deltaTime );
			if( m_orders.front()->areOrdersComplete() )
			{
				SAFE_DELETE( m_orders.front() );
				m_orders.pop_front();
				if( m_orders.size() > 0 )
					reinterpret_cast< UnitOrders* >( m_orders.front() )->assignUnit( this );
			}
		}
		else
		{
			// IDLE WAIT FOR ORDERS
			// or attack enemy that comes in range and then return to current location
			searchForTarget();
		}		
	}
	//----------------------------------------------------------------
	void Unit::render()
	{		
		float hWidth = m_blueprint->getWidth() * 0.5f;
		float hHeight = m_blueprint->getHeight() * 0.5f;
		matStackf matStack;
		matStack.translate( (m_position.x - hWidth), (m_position.y - hHeight), 0.1f );
		if( m_owner )
		{
			Material* unitMaterial = Renderer::getMaterial( m_blueprint->getMesh()->getMaterial( 0 ) );
			unitMaterial->updateUniform( "uTeamColor", m_owner->getTeamColor() );
		}
		NamedProperties params;
		params.set( "mesh", m_blueprint->getMesh() );
		params.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", params );
		if( m_isSelected )
		{
			matStack.loadIdentity();
			matStack.translate( (m_position.x - hWidth), (m_position.y - hHeight), 0.1f );
		
			Material* selectMat = Renderer::getMaterial( m_selectionMesh->getMaterial( 0 ) );
			if( m_isAllySelected )
				selectMat->updateUniform( "uColorTint", color::GREEN );
			else
				selectMat->updateUniform( "uColorTint", color::RED );

			params.set( "mesh", m_selectionMesh );
			fireEvent( "renderMesh", params );
		}

		Material* healthMat = Renderer::getMaterial( m_healthBar->getMaterial( 0 ) );
		
		healthMat->updateUniform( "uColorTint", color::RED );
		matStack.loadIdentity();
		matStack.translate( (m_position.x - hWidth), (m_position.y - hHeight), 0.1f );
		matStack.translate( 0.0f, m_blueprint->getHeight() * 1.2f, 0.0f );
		params.set( "mesh", m_healthBar );
		params.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", params );

		healthMat->updateUniform( "uColorTint", color::GREEN );
		matStack.loadIdentity();
		matStack.scale( m_currentHealth / m_maxHealth, 1.0f, 1.0f );
		matStack.translate( (m_position.x - hWidth), (m_position.y - hHeight), 0.1f );
		matStack.translate( 0.0f, m_blueprint->getHeight() * 1.2f, 0.0f );
		params.set( "mesh", m_healthBar );
		params.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", params );
		

	}
	//----------------------------------------------------------------
	void Unit::debugRender()
	{
		vec2f dir = m_position - m_prevPosition;
		dir.normalize();
		
		createArrowGDO( dir, 1.0f, vec3f( m_position, 0.2f ) );

		Mesh* mesh = MeshFactory::generateArrow( dir, 1.0f, "DebugMaterial" );
		matStackf matStack;
		matStack.translate( m_position.x, m_position.y, 0.1f );
		NamedProperties params;
		params.set( "mesh", mesh );
		params.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", params );
		

		std::string fontName = "mainFont_72";
		std::string text = toString( m_resource.currentAmount );
		vec3f pos = vec3f( m_position, 0.1f );
		pos.y += m_blueprint->getHeight() * 0.5f;
		float fontHeight = 1.0f;
		Color4f color = color::WHITE;
		vec2f posRel = Font::BOTCENTER;
		NamedProperties textParams;
		textParams.set( "fontName", fontName );
		textParams.set( "text", text );
		textParams.set( "pos", pos );
		textParams.set( "fontHeight", fontHeight );
		textParams.set( "color", color );
		textParams.set( "posRel", posRel );
		fireEvent( "renderText", textParams );

		pos.y += 1.0f;
		textParams.set( "pos", pos );
		if( m_orders.size() > 0 )
			textParams.set( "text", m_orders.front()->getDescritpion() );
		else
			textParams.set( "text", std::string( "Idle" ) );

		fireEvent( "renderText", textParams );
	}
	//----------------------------------------------------------------
	void Unit::renderOrders()
	{
		vec2f currentPos = m_position;
		for( size_t i = 0; i <  m_orders.size(); ++i )
		{
			if( m_orders[i]->getType() == Orders::UNIT )
			{
				UnitOrders* unitOrder = reinterpret_cast< UnitOrders* >( m_orders[i] );
				unitOrder->renderPath( m_renderAStarPath );
			}
			else
			{
				Mesh* line = MeshFactory::generateLine( currentPos, m_orders[i]->getTargetLocation() );
				NamedProperties params;
				params.set( "mesh", line );
				fireEvent( "renderMesh", params );
				
				currentPos = m_orders[i]->getTargetLocation();
				SAFE_DELETE( line );
			}
		}
	}
	//----------------------------------------------------------------
	aabb2f Unit::getAABB2() const
	{
		return aabb2f( m_position - vec2f( getWidth() * 0.5f, getHeight() * 0.5f ), getWidth(), getHeight() );
	}
	//----------------------------------------------------------------
	float Unit::getWidth() const 
	{ 
		return m_blueprint->getWidth(); 
	}
	//----------------------------------------------------------------
	float Unit::getHeight() const
	{ 
		return m_blueprint->getHeight(); 
	}
	//----------------------------------------------------------------
	const std::string& Unit::getBlueprintName() const
	{
		return m_blueprint->getName();
	}
	//----------------------------------------------------------------
	bool Unit::canMoveToLocation( const WorldCoords& worldCoords ) const
	{
		// TODO
		return false;
	}
	//----------------------------------------------------------------
	bool Unit::canMoveOnTileWithWorldCoords( const WorldCoords& worldCoords ) const
	{
		Map::MapTile& mapTile = m_world->getMap()->getMapTile( m_world->getMap()->getTileCoordsFromWorldCoords( worldCoords ) );
		return canMoveOnTile( mapTile.getTerrainType()->getName() );
	}
	//----------------------------------------------------------------
	bool Unit::canMoveOnTile( const std::string& terrainType ) const
	{
		bool canMove = false;
		const std::vector< std::string >& canMoveOnList = m_blueprint->getCanMoveOnList();
		auto iter = std::find( canMoveOnList.begin(), canMoveOnList.end(), terrainType );
		
		if( iter != canMoveOnList.end() )
			canMove = true;

		return canMove;
	}
	//----------------------------------------------------------------
	bool Unit::getBuildingNameFromHotkey( char hotkey, std::string& buildingName ) const
	{
		auto iter = m_blueprint->getBuildingsCanProduce().find( hotkey );
		if( iter != m_blueprint->getBuildingsCanProduce().end() )
		{
			buildingName = iter->second;
			return true;
		}
		return false;
	}
	//----------------------------------------------------------------
	void Unit::assignOrders( Orders* orders )
	{
		DEQUE_CLEANUP( m_orders );
		if( orders->getType() == Orders::UNIT )
		{		
			m_orders.push_back( orders );
			reinterpret_cast< UnitOrders* >( orders )->assignUnit( this );
		}
	}
	//----------------------------------------------------------------
	void Unit::pushOrders( Orders* orders )
	{
		if( orders->getType() == Orders::UNIT )
		{
			if( m_orders.size() > 0 )
				reinterpret_cast< UnitOrders* >( orders )->setStartLoc( m_orders.back()->getTargetLocation() );
			else
				reinterpret_cast< UnitOrders* >( orders )->assignUnit( this );

			m_orders.push_back( orders );
		}
	}
	//----------------------------------------------------------------
	const Orders* Unit::getCurrentOrders() const
	{
		if( m_orders.size() > 0 )
			return m_orders.front();
		else
			return nullptr;
	}
	//----------------------------------------------------------------
	const Orders* Unit::getLastOrderInQueue() const
	{
		if( m_orders.size() > 0 )
			return m_orders.back();
		else
			return nullptr;
	}
	//----------------------------------------------------------------
	void Unit::toggleAStarRender( NamedProperties& params )
	{
		m_renderAStarPath = !m_renderAStarPath; 
	}
	//--------------------------------------------------------
	void Unit::searchForTarget()
	{
		Entity* newTarget = m_world->getClosestEnemyEntityInRadius( this, m_movement.vision );		
		if( newTarget )
		{
			assignOrders( new AttackOrders( newTarget ) );
			pushOrders( new MoveOrders( m_position ) );
		}
	}
}