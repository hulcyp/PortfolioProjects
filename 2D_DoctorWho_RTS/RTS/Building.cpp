#include "Building.h"
#include <CommonEngineIncludes.h>
#include <EventSystemInterface.h>
#include "BuildingBlueprint.h"
#include <Font.h>
#include <Renderer.h>
#include "Orders.h"
#include "UnitOrders.h"
#include <MeshFactory.h>
#include "Controller.h"
#include "RTSWorld.h"
#include "UnitBlueprint.h"

namespace pdh
{
	Building::Building( const BuildingBlueprint* blueprint, RTSWorld* world  )
		:	Entity( world )
		,	m_blueprint( blueprint )
		,	m_isComplete( false )
		,	m_currentNumUnitsWorkingOnThisBuilding( 0 )
	{
		m_entityType = BUILDING;
	}
	//----------------------------------------------------------------
	Building::~Building()
	{	
		if( m_owner != nullptr )
			m_owner->removeFromMaxSupplyCount( m_blueprint->getSupplyProvided() );
	}
	//----------------------------------------------------------------
	void Building::update( float deltaTime )
	{
		getVisibleTiles( m_visionRange, m_visibleTiles );
		m_world->getVisibleEntitesInRadius( this, m_position, m_visionRange, m_visibleEntities );
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
			}
		}
		else
		{
			// IDLE WAIT FOR ORDERS
		}
	}
	//----------------------------------------------------------------
	void Building::render()
	{
		Material* mat = Renderer::getMaterial( m_blueprint->getMesh()->getMaterial( 0 ) );
		mat->updateUniform( "uColorTint", m_colorTint );
		
		float hWidth = m_blueprint->getWidth() * 0.5f;
		float hHeight = m_blueprint->getHeight() * 0.5f;
		matStackf matStack;
		matStack.translate( (m_position.x - hWidth), (m_position.y - hHeight), 0.1f );
		if( m_owner )
		{
			Material* buildingMaterial = Renderer::getMaterial( m_blueprint->getMesh()->getMaterial( 0 ) );
			buildingMaterial->updateUniform( "uTeamColor", m_owner->getTeamColor() );
		}
		NamedProperties params;
		params.set( "mesh", m_blueprint->getMesh() );
		params.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", params );
		if( m_isSelected )
		{
			Material* selMat = Renderer::getMaterial( m_selectionMesh->getMaterial( 0 ) );
			if( m_isAllySelected )
				selMat->updateUniform( "uColorTint", color::GREEN );
			else
				selMat->updateUniform( "uColorTint", color::RED );
			params.set( "mesh", m_selectionMesh );
			fireEvent( "renderMesh", params );
			renderInitialUnitOrders();
		}

		if( !m_isComplete )
		{
			std::string fontName = "mainFont_72";
			std::string text = toString( (int)( m_currentBuildTime/m_blueprint->getBuildTime() * 100 ) ) + "%";
			vec3f pos = vec3f( m_position, 0.3f );
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

		if( m_orders.size() > 0 )
		{
			m_orders.front()->renderOrders();
		}
	}
	//----------------------------------------------------------------
	void Building::renderInitialUnitOrders()
	{
		vec2f currentPos = m_position;
		for( size_t i = 0; i <  m_initialOrdersForUnits.size(); ++i )
		{
			Mesh* line = MeshFactory::generateLine( currentPos, m_initialOrdersForUnits[i]->getTargetLocation(), "SimpleColorMaterial", color::BLUE );
			NamedProperties params;
			params.set( "mesh", line );
			fireEvent( "renderMesh", params );
				
			currentPos = m_initialOrdersForUnits[i]->getTargetLocation();
			SAFE_DELETE( line );
		}
	}
	//----------------------------------------------------------------
	aabb2f Building::getAABB2() const
	{
		return aabb2f( m_position - vec2f( getWidth() * 0.5f, getHeight() * 0.5f ), getWidth(), getHeight() );
	}
	//----------------------------------------------------------------
	void Building::addToCurrentBuildTime( float timeToAdd )
	{
		m_currentBuildTime += timeToAdd;
		m_colorTint.r = m_currentBuildTime / m_blueprint->getBuildTime();
		if( m_currentBuildTime >= m_blueprint->getBuildTime() )
		{
			m_isComplete = true;
			m_colorTint = color::WHITE;
			if( m_owner != nullptr )
				m_owner->addToMaxSupplyCount( m_blueprint->getSupplyProvided() );
		}
	}
	//----------------------------------------------------------------
	void Building::setCurrentBuildTime( float currentBuildTime )
	{
		m_currentBuildTime = currentBuildTime;
		m_colorTint.r = m_currentBuildTime / m_blueprint->getBuildTime();
		if( m_currentBuildTime >= m_blueprint->getBuildTime() )
		{
			m_isComplete = true;
			m_colorTint = color::WHITE;
			if( m_owner != nullptr )
				m_owner->addToMaxSupplyCount( m_blueprint->getSupplyProvided() );
		}
	}
	//----------------------------------------------------------------
	int Building::getTimeToBuild()
	{
		return m_blueprint->getBuildTime();
	}
	//----------------------------------------------------------------
	float Building::getWidth() const 
	{ 
		return m_blueprint->getWidth(); 
	}
	//----------------------------------------------------------------
	float Building::getHeight() const
	{ 
		return m_blueprint->getHeight(); 
	}
	//----------------------------------------------------------------
	const std::string& Building::getBlueprintName() const
	{
		return m_blueprint->getName();
	}
	//----------------------------------------------------------------
	float Building::getCurrentBuildRateFactor() const
	{
		return 1.0f/m_currentNumUnitsWorkingOnThisBuilding;
	}
	//----------------------------------------------------------------
	float Building::getCurrentHealthIncreaseFactor() const
	{
		return 1.0f/m_currentNumUnitsWorkingOnThisBuilding;
	}
	//----------------------------------------------------------------
	void Building::addUnitWorkingOnBuilding()
	{
		++m_currentNumUnitsWorkingOnThisBuilding;
	}
	//----------------------------------------------------------------
	void Building::removeUnitWorkdingOnBuilding()
	{
		--m_currentNumUnitsWorkingOnThisBuilding;
	}
	//----------------------------------------------------------------
	bool Building::getUnitNameFromHotkey( char hotkey, std::string& unitName ) const
	{
		auto iter = m_blueprint->getUnitsProduced().find( hotkey );
		if( iter != m_blueprint->getUnitsProduced().end() )
		{
			unitName = iter->second;
			return true;
		}
		return false;
	}
	//----------------------------------------------------------------
	bool Building::getUnitNameBasedOnResources( int fishFingers, int custard, int timeEnergy, std::string& unitType ) const
	{
		auto iter = m_blueprint->getUnitsProduced().begin();
		for( ; iter != m_blueprint->getUnitsProduced().end(); ++iter )
		{
			UnitBlueprint* bp = UnitBlueprint::getBluePrint( iter->second );
			if( bp->getUnitCost().fishFingers <= fishFingers &&
				bp->getUnitCost().custard <= custard &&
				bp->getUnitCost().timeEnergy <= timeEnergy )
			{
				unitType = iter->second;
				return true;		
			}
		}
		return false;
	}
	//----------------------------------------------------------------
	void Building::assignInitialOrderForUnits( Orders* initialOrder )
	{
		VECTOR_CLEANUP( m_initialOrdersForUnits );
		m_initialOrdersForUnits.push_back( initialOrder );
	}
	//----------------------------------------------------------------
	void Building::pushInitialOrderForUnits( Orders* initialOrder )
	{
		if( initialOrder != nullptr )
			m_initialOrdersForUnits.push_back( initialOrder );
	}
}