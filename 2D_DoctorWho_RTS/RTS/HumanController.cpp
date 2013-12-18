#include "HumanController.h"
#include "Map.h"
#include "RTSWorld.h"
#include "Entity.h"
#include "Unit.h"
#include "Building.h"
#include "MoveOrders.h"
#include <EventSystemInterface.h>
#include "BuildingBlueprint.h"
#include <Mesh.h>
#include <MeshFactory.h>
#include <Texture.h>
#include <Font.h>
#include <Renderer.h>
#include "BuildOrders.h"
#include "Building.h"
#include "BuildUnitOrders.h"
#include "BuildingBlueprint.h"
#include "UnitBlueprint.h"
#include "HoldOrders.h"

namespace pdh
{
	HumanController::HumanController( RTSWorld* world, int playerNum, Camera* HUDCamera, const Color4f& teamColor )
		:	Controller( world, playerNum, teamColor )
		,	m_currentOrder( NONE )
		,	m_isSelecting( false )
		,	m_hudCamera( HUDCamera )
		,	m_resourceIconWidth( 12.0f )
		,	m_outOfResourceMsgTimer( 3.0f )
		,	m_notEnoughSupplyMsgTimer( 3.0f )
		,	m_validBuildingSelected( false )
	{
		registerForEvent( "onKeyDown", *this, &HumanController::onKeyDown );
		registerForEvent( "onKeyUp", *this, &HumanController::onKeyUp );
		registerForEvent( "onMouseButtonDown", *this, &HumanController::onMouseButtonDown );
		registerForEvent( "onMouseButtonUp", *this, &HumanController::onMouseButtonUp );
		registerForEvent( "onMouseMove", *this, &HumanController::onMouseMove );
		registerForEvent( "onMouseMove", *this, &HumanController::onMouseMove );
		registerForEvent( "NotEnoughResources", *this, &HumanController::notEnoughtResources );
		
		m_fishFingersMesh = MeshFactory::generate2DOrthoRectangle( m_resourceIconWidth, m_resourceIconWidth, "FishFingersMaterial" );
		m_custardMesh = MeshFactory::generate2DOrthoRectangle( m_resourceIconWidth, m_resourceIconWidth, "CustardMaterial" );
		m_timeEnergyMesh = MeshFactory::generate2DOrthoRectangle( m_resourceIconWidth, m_resourceIconWidth, "TimeEnergyMaterial" );
		m_supplyMesh = MeshFactory::generate2DOrthoRectangle( m_resourceIconWidth, m_resourceIconWidth, "TimeEnergyMaterial" );

		m_fishFingersIconPos = vec3f( m_world->getScreenWidth() - m_resourceIconWidth*16, m_world->getScreenHeight() - m_resourceIconWidth, 0.0f );
		m_custardIconPos = vec3f( m_world->getScreenWidth() - m_resourceIconWidth*12, m_world->getScreenHeight() - m_resourceIconWidth, 0.0f );
		m_timeEnergyIconPos = vec3f( m_world->getScreenWidth() - m_resourceIconWidth*8, m_world->getScreenHeight() - m_resourceIconWidth, 0.0f );
		m_supplyIconPos = vec3f( m_world->getScreenWidth() - m_resourceIconWidth*4, m_world->getScreenHeight() - m_resourceIconWidth, 0.0f );

		std::string fontName = "mainFont_72";
		float fontHeight = 12.0f;	
		Color4f color = color::WHITE;
		vec2f posRel = Font::TOPLEFT;
		m_resourcesTextParams.set( "fontName", fontName );
		m_resourcesTextParams.set( "fontHeight", fontHeight );
		m_resourcesTextParams.set( "color", color );
		m_resourcesTextParams.set( "posRel", posRel );
		m_resourcesTextParams.set( "camera", m_hudCamera );

		m_fishFingersResourcePos = vec3f( (float)m_world->getScreenWidth() - m_resourceIconWidth*15, (float)m_world->getScreenHeight(), 0.0f );
		m_custardResourcePos = vec3f( (float)m_world->getScreenWidth() - m_resourceIconWidth*11, (float)m_world->getScreenHeight(), 0.0f );
		m_timeEnergyResourcePos = vec3f( (float)m_world->getScreenWidth() - m_resourceIconWidth*7,(float) m_world->getScreenHeight(), 0.0f );
		m_supplyPos = vec3f( (float)m_world->getScreenWidth() - m_resourceIconWidth*3,(float) m_world->getScreenHeight(), 0.0f );
		
		fontHeight = 24.0f;
		color = color::RED;
		std::string text = "Not enough resources";
		vec3f pos = vec3f( 0.0f,(float) m_world->getScreenHeight() * 0.5f, 0.0f );
		m_outOfResourceMsgParams.set( "fontName", fontName );
		m_outOfResourceMsgParams.set( "fontHeight", fontHeight );
		m_outOfResourceMsgParams.set( "color", color );
		m_outOfResourceMsgParams.set( "posRel", posRel );
		m_outOfResourceMsgParams.set( "camera", m_hudCamera );
		m_outOfResourceMsgParams.set( "pos", pos );
		m_outOfResourceMsgParams.set( "text", text );

		text = "Not enough supply";
		pos = vec3f( 0.0f, (float) m_world->getScreenHeight() * 0.5f - 24.0f, 0.0f );
		m_notEnoughSupplyMsgParams.set( "fontName", fontName );
		m_notEnoughSupplyMsgParams.set( "fontHeight", fontHeight );
		m_notEnoughSupplyMsgParams.set( "color", color );
		m_notEnoughSupplyMsgParams.set( "posRel", posRel );
		m_notEnoughSupplyMsgParams.set( "camera", m_hudCamera );
		m_notEnoughSupplyMsgParams.set( "pos", pos );
		m_notEnoughSupplyMsgParams.set( "text", text );
	}
	//-----------------------------------------------------------
	HumanController::~HumanController()
	{
		unregisterForEvent( *this );
	}
	//-----------------------------------------------------------
	void HumanController::render()
	{
				matStackf matStack;
		aabb2f currentSelectionWorldSpace( m_world->getWorldCoordsFromScreenCoords( m_currentSelection.minPoint() ), m_world->getWorldCoordsFromScreenCoords( m_currentSelection.maxPoint() ) );
			
		if( m_currentOrder == BUILD_BUILDING && m_validBuildingSelected )
		{
			BuildingBlueprint* blueprint = BuildingBlueprint::getBlueprint( m_currentBuildingSelectedToBuild );

			WorldCoords mouseWorldCoords = m_world->getWorldCoordsFromScreenCoords( m_currentCursorPos );
			mouseWorldCoords = m_world->getNearestWorldCoordsToBuildBulidingOnGrid( mouseWorldCoords, blueprint->getWidth(), blueprint->getHeight() ); 

			Material* mat = Renderer::getMaterial( blueprint->getMesh()->getTriangleBatch( 0 )->material );
			Color4f colorTint = color::WHITE;
			if( !m_world->canBuildAtWorldCoords( this, blueprint, mouseWorldCoords ) )
			{
				colorTint = color::RED;
			}
			
			mat->updateUniform( "uColorTint", colorTint );
		
			float hWidth = blueprint->getWidth() * 0.5f;
			float hHeight = blueprint->getHeight() * 0.5f;
			matStack.translate( (mouseWorldCoords.x - hWidth), (mouseWorldCoords.y - hHeight), 0.1f );
			NamedProperties params;
			params.set( "mesh", blueprint->getMesh() );
			params.set( "modelMatrix", matStack.top() );
			fireEvent( "renderMesh", params );
		}
		if( m_unitsSelected.size() > 0 )
		{
			std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(), 
				[&]( Unit* unit )
			{
				unit->renderOrders();
			} );
		}

		NamedProperties hudParams;
		matStack.loadIdentity();
		matStack.translate( m_fishFingersIconPos );
		hudParams.set( "mesh", m_fishFingersMesh );
		hudParams.set( "camera", m_hudCamera );
		hudParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", hudParams );		
		
		matStack.loadIdentity();
		matStack.translate( m_custardIconPos );
		hudParams.set( "mesh", m_custardMesh );
		hudParams.set( "camera", m_hudCamera );
		hudParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", hudParams );		
		
		matStack.loadIdentity();
		matStack.translate( m_timeEnergyIconPos );
		hudParams.set( "mesh", m_timeEnergyMesh );
		hudParams.set( "camera", m_hudCamera );
		hudParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", hudParams );		
		
		matStack.loadIdentity();
		matStack.translate( m_supplyIconPos );
		hudParams.set( "mesh", m_supplyMesh );
		hudParams.set( "camera", m_hudCamera );
		hudParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", hudParams );

		matStack.loadIdentity();
		std::string fishFingersText = toString( m_fishFingers );
		m_resourcesTextParams.set( "text",  fishFingersText );
		m_resourcesTextParams.set( "pos", m_fishFingersResourcePos );
		m_resourcesTextParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderText", m_resourcesTextParams );

		std::string custardText = toString( m_custard );
		m_resourcesTextParams.set( "text",  custardText );
		m_resourcesTextParams.set( "pos", m_custardResourcePos );
		m_resourcesTextParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderText", m_resourcesTextParams );

		std::string timeEnergyText = toString( m_timeEnergy );
		m_resourcesTextParams.set( "text",  timeEnergyText );
		m_resourcesTextParams.set( "pos", m_timeEnergyResourcePos );
		m_resourcesTextParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderText", m_resourcesTextParams );
		
		std::string supplyText = toString( m_currentSupplyCount ) + "/" + toString( m_maxSupplyCount );
		m_resourcesTextParams.set( "text",  supplyText );
		m_resourcesTextParams.set( "pos", m_supplyPos );
		m_resourcesTextParams.set( "modelMatrix", matStack.top() );
		fireEvent( "renderText", m_resourcesTextParams );

		if( !m_outOfResourceMsgTimer.isTimeUp() && m_outOfResourceMsgTimer.wasStarted() )
		{
			m_outOfResourceMsgParams.set( "modelMatrix", matStack.top() );
			fireEvent( "renderText", m_outOfResourceMsgParams );
		}

		if( !m_notEnoughSupplyMsgTimer.isTimeUp() && m_notEnoughSupplyMsgTimer.wasStarted() )
		{
			fireEvent( "renderText", m_notEnoughSupplyMsgParams );
		}

		if( m_isSelecting && currentSelectionWorldSpace.minPoint() != currentSelectionWorldSpace.maxPoint() )
		{			
			currentSelectionWorldSpace.adjustMinMaxPoints();			
		
			Mesh* mesh = MeshFactory::generate2DOrthoRectangleBorder( (float)currentSelectionWorldSpace.getWidth(), (float)currentSelectionWorldSpace.getHeight() );
			matStack.loadIdentity();
			matStack.translate( (float)currentSelectionWorldSpace.minPoint().x, (float)currentSelectionWorldSpace.minPoint().y, 0.1f );
			NamedProperties params;
			params.set( "mesh", mesh );
			params.set( "modelMatrix", matStack.top() );
			fireEvent( "renderMesh", params );
		}
	}
	//-----------------------------------------------------------
	void HumanController::onKeyDown( NamedProperties& params )
	{
		unsigned char keyCode;
		if( params.get( "keyCode", keyCode ) == NamedProperties::SUCCESS )
		{
			keyCode = toupper( keyCode );
			char numKeyCode = getNumericalCharFromShiftedChar( keyCode );
			if( numKeyCode != '0' - 1 )
				keyCode = numKeyCode;
			if( ( '0' <= keyCode && keyCode <= '9' ) )
			{
				if( glutGetModifiers() == GLUT_ACTIVE_SHIFT )
				{
					addToControlGroup( keyCode );
				}
				else
				{
					selectControlGroup( keyCode );
				}
			}
			else if( m_currentOrder == BUILD_BUILDING )
			{
				m_validBuildingSelected = m_unitsSelected[0]->getBuildingNameFromHotkey( (char)keyCode, m_currentBuildingSelectedToBuild );
			}
			else if( m_buildingsSelected.size() > 0 )
			{
				std::string unitName;
				if( m_buildingsSelected[0]->getUnitNameFromHotkey( (char)keyCode, unitName ) )
					issueBuildUnitOrder( unitName );
				else
				{
					if( keyCode == (char)27 )
					{
						if( m_buildingsSelected.size() > 0 && m_buildingsSelected[0]->hasOrders() )
						{
							popBackBuildUnitOrder();
						}
					}
				}
			}
			else
			{
				switch( keyCode )
				{
				case 'A':
					m_currentOrder = ATTACK_MOVE;
					break;
				case 'M':
					m_currentOrder = MOVE;
					break;
				case 'G':
					m_currentOrder = HARVEST;
					break;
				case 'P':
					m_currentOrder = SPAWN;
					break;
				case 'B':
					if( ( m_unitsSelected.size() > 0 ) && m_unitsSelected[0]->getUnitBlueprint()->getBuildingsCanProduce().size() > 0 )
						m_currentOrder = BUILD_BUILDING;
					break;
				case 'S':
					issueStopOrder();
					break;
				case 'H':
					if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
						issueHoldOrders();
					else
						queueHoldOrders();
					break;
				case (char)27:
					if( m_currentOrder != NONE )
						m_currentOrder = NONE;
					else
					{
						deselectAll();
					}
					break;					
				}
			}
		}
	}
	//-----------------------------------------------------------
	void HumanController::onKeyUp( NamedProperties& params )
	{}
	//-----------------------------------------------------------
	void HumanController::onMouseButtonDown( NamedProperties& params )
	{
		vec2i screenCoords;
		int keyCode;
		if( params.get( "screenCoords", screenCoords ) == NamedProperties::SUCCESS )
		{
			m_currentSelection.setMinPoint( screenCoords );
		}
		
		if( params.get( "keyCode", keyCode ) == NamedProperties::SUCCESS )
		{
			if( keyCode == GLUT_LEFT_BUTTON )
				m_isSelecting = true;
		}
	}
	//-----------------------------------------------------------
	void HumanController::onMouseButtonUp( NamedProperties& params )
	{
		vec2i screenCoords;
		int keyCode = 0;

		if( params.get( "screenCoords", screenCoords ) == NamedProperties::SUCCESS )
		{
			m_currentSelection.setMaxPoint( screenCoords );
		}

		params.get( "keyCode", keyCode );
		
		//Entity* entity = nullptr;
		Map::MapTile* mapTile = nullptr;
		if( keyCode == GLUT_LEFT_BUTTON )
		{

			std::vector< Entity* > selectedEntities;
			float mapWidth = (float)m_world->getMap()->getMapWidth() - 1;
			float mapHeight = (float)m_world->getMap()->getMapHeight() - 1;

			WorldCoords minWorldCoords = m_world->getWorldCoordsFromScreenCoords( m_currentSelection.minPoint() );
			WorldCoords maxWorldCoords = m_world->getWorldCoordsFromScreenCoords( m_currentSelection.maxPoint() );			

			aabb2f currentSelectionWorldSpace = aabb2f( minWorldCoords, maxWorldCoords );
			
			currentSelectionWorldSpace.adjustMinMaxPoints();

			WorldCoords clampedMin = currentSelectionWorldSpace.minPoint().clamp( WorldCoords( 0, 0 ), WorldCoords( mapWidth, mapHeight ) );
			WorldCoords clampedMax = currentSelectionWorldSpace.maxPoint().clamp( WorldCoords( 0, 0 ), WorldCoords( mapWidth, mapHeight ) );
			
			currentSelectionWorldSpace.setMinPoint( clampedMin );
			currentSelectionWorldSpace.setMaxPoint( clampedMax );
						
			m_world->getSelectedEntitesFromAABB2( this, currentSelectionWorldSpace, selectedEntities );
			if( selectedEntities.empty() )
			{
				Entity* entity = m_world->getSelectedEntityFromWorldCoords( this, clampedMax );
				if( entity )
					selectedEntities.push_back( entity );
			}
		
			m_isSelecting = false;
						
			mapTile = &m_world->getSelectedMapTileFromWorldCoord( this, clampedMax );


			if( selectedEntities.size() > 0 )
			{
				Entity* unitEntity = getFirstUnitEntity( selectedEntities );
				Entity* buildingEntity = getFirstBuildingEntity( selectedEntities );
				if( unitEntity != nullptr )
				{
					if( glutGetModifiers() == GLUT_ACTIVE_CTRL )
					{
						deselectAll();
						std::vector< Entity* > selectedEntities;
						m_world->getAllEntitiesOfTypeOnScreen( unitEntity, selectedEntities );
						for( size_t i = 0; i < selectedEntities.size(); ++i )
						{
							if( selectedEntities[i]->getType() == Entity::UNIT )
							{
								selectedEntities[i]->setSelected( true, selectedEntities[i]->getOwner() == this );
								m_unitsSelected.push_back( reinterpret_cast< Unit* >( selectedEntities[i] ) );
							}
						}
					}
					else if( glutGetModifiers() == GLUT_ACTIVE_SHIFT )
					{
						
						for( size_t i = 0; i < selectedEntities.size(); ++i )
						{
							if( selectedEntities[i]->getType() == Entity::UNIT )
							{
								selectedEntities[i]->setSelected( true, selectedEntities[i]->getOwner() == this );
								m_unitsSelected.push_back( reinterpret_cast< Unit* >( selectedEntities[i] ) );
							}
						}
					}
					else
					{	
						deselectAll();
						for( size_t i = 0; i < selectedEntities.size(); ++i )
						{
							if( selectedEntities[i]->getType() == Entity::UNIT )
							{
								selectedEntities[i]->setSelected( true, selectedEntities[i]->getOwner() == this );
								m_unitsSelected.push_back( reinterpret_cast< Unit* >( selectedEntities[i] ) );
							}
						}
					}
				}
				else if( buildingEntity != nullptr )
				{
					if( glutGetModifiers() == GLUT_ACTIVE_CTRL )
					{
						deselectAll();
						std::vector< Entity* > selectedEntities;
						m_world->getAllEntitiesOfTypeOnScreen( buildingEntity, selectedEntities );
						for( size_t i = 0; i < selectedEntities.size(); ++i )
						{
							if( selectedEntities[i]->getType() == Entity::BUILDING )
							{
								selectedEntities[i]->setSelected( true, selectedEntities[i]->getOwner() == this );
								m_buildingsSelected.push_back( reinterpret_cast< Building* >( selectedEntities[i] ) );
							}
						}
					}
					else if( glutGetModifiers() == GLUT_ACTIVE_SHIFT )
					{						
						for( size_t i = 0; i < selectedEntities.size(); ++i )
						{
							if( selectedEntities[i]->getType() == Entity::BUILDING )
							{
								selectedEntities[i]->setSelected( true, selectedEntities[i]->getOwner() == this );
								m_buildingsSelected.push_back( reinterpret_cast< Building* >( selectedEntities[i] ) );
							}
						}
					}
					else
					{
						deselectAll();
						for( size_t i = 0; i < selectedEntities.size(); ++i )
						{
							if( selectedEntities[i]->getType() == Entity::BUILDING )
							{
								selectedEntities[i]->setSelected( true, selectedEntities[i]->getOwner() == this );
								m_buildingsSelected.push_back( reinterpret_cast< Building* >( selectedEntities[i] ) );
							}
						}
					}
				}
			}
			else
			{
				switch( m_currentOrder )
				{
				case NONE:	
					deselectAll();
					break;
				case ATTACK:
					break;
				case ATTACK_MOVE:
					issueAttackOrdersToLocation( maxWorldCoords );
					m_currentOrder = NONE;
					break;
				case MOVE:
					if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
					{
						issueMoveOrders( maxWorldCoords );
						m_currentOrder = NONE;
					}
					else
					{
						queueMoveOrders( maxWorldCoords );
					}					
					break;
				case BUILD_BUILDING:
					if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
					{
						BuildingBlueprint* blueprint = BuildingBlueprint::getBlueprint( m_currentBuildingSelectedToBuild );
						WorldCoords onGridCoords = m_world->getNearestWorldCoordsToBuildBulidingOnGrid( maxWorldCoords, blueprint->getWidth(), blueprint->getHeight() );
						issueBuildOrders( m_currentBuildingSelectedToBuild, onGridCoords );
						m_currentOrder = NONE;
						m_currentBuildingSelectedToBuild = "";
						m_validBuildingSelected = false;
					}
					else
					{
						queueBuildOrders( m_currentBuildingSelectedToBuild, maxWorldCoords );
					}
					break;
				case HOLD:
					break;
				case HARVEST:
					if( mapTile )
						issueHarvestOrders( *mapTile );
					m_currentOrder = NONE;
					break;
				case SPAWN:
					m_world->spawnUnit( "Probe", maxWorldCoords, this );
					m_currentOrder = NONE;
					break;
				}
			}
		}
		else if( keyCode == GLUT_RIGHT_BUTTON )
		{		
			Entity* entity = nullptr;

			WorldCoords worldCoords = m_world->getWorldCoordsFromScreenCoords( screenCoords );
			TileCoords tileCoords = m_world->getMap()->getTileCoordsFromWorldCoords( worldCoords );
			entity = m_world->getSelectedEntityFromWorldCoords( this, worldCoords );

			if( tileCoords.x != -1  )
				mapTile = &m_world->getSelectedMapTileFromTileCoord( this, tileCoords );

			m_isSelecting = false;

			if( entity )
			{
				if( entity->getOwner() != this )
				{
					if( m_unitsSelected.size() > 0 )
					{
						issueAttackOrdersOnTargetEntity( entity );
					}
				}
				else if( entity->getType() == Entity::UNIT )
				{
					if( m_unitsSelected.size() > 0 )
					{
						if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
						{
							issueMoveOrders( worldCoords );
							m_currentOrder = NONE;
						}
						else
							queueMoveOrders( worldCoords );
					}
				}
				else if( entity->getType() == Entity::BUILDING )
				{
					if( m_unitsSelected.size() > 0 )
					{
						Building* building = reinterpret_cast< Building* >( entity );
						if( !building->isComplete() )
						{
							Controller::issueBuildOrders( building );
							m_currentOrder = NONE;
						}
						else
						{
							issueMoveOrders( worldCoords );
							m_currentOrder = NONE;
						}
					}
				}
			}
			else if( mapTile )
			{
				if( mapTile->hasResource() )
				{
					issueHarvestOrders( *mapTile );
					m_currentOrder = NONE;
				}
				else
				{
					if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
					{						
						issueMoveOrders( worldCoords );
						m_currentOrder = NONE;
					}
					else
					{
						queueMoveOrders( worldCoords );
					}
					
				}
			}
		}
	}
	//-------------------------------------------------
	void HumanController::onMouseMove( NamedProperties& params )
	{
		vec2i mouseCoords;
		if( params.get( "mouseCoords", mouseCoords ) == NamedProperties::SUCCESS )
		{
			m_currentCursorPos = mouseCoords;
		}
		m_currentSelection.setMaxPoint( mouseCoords );
		
	}
	//-------------------------------------------------
	void HumanController::queueMoveOrders( const vec2f& loc )
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
				unit->pushOrders( new MoveOrders( loc ) );
		} );
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{
			building->pushInitialOrderForUnits( new MoveOrders( loc ) );
		} );
	}
	void HumanController::queueBuildOrders( const std::string& buildingType, const vec2f& loc )
	{
		BuildingBlueprint* buildingBP = BuildingBlueprint::getBlueprint( buildingType );
		if( m_world->canBuildAtWorldCoords( this, buildingBP, loc ) )
		{
			Controller::ResourceRequestError requestResourceError = 
				requestResource( buildingBP->getBuildingCost().fishFingers, buildingBP->getBuildingCost().custard, buildingBP->getBuildingCost().timeEnergy );
			if( requestResourceError == Controller::NOT_ENOUGH_FISH_FINGERS )
			{
				NamedProperties params;
				std::string text = "Not enough Fish Fingers";
				params.set( "text", text );
				fireEvent( "NotEnoughResources", params );
			}
			else if( requestResourceError == Controller::NOT_ENOUGH_CUSTARD )
			{
				NamedProperties params;
				std::string text = "Not enough Custard";
				params.set( "text", text );
				fireEvent( "NotEnoughResources", params );
			}
			else if( requestResourceError == Controller::NOT_ENOUGH_TIME_ENERGY )
			{
				NamedProperties params;
				std::string text = "Not enough Time Energy";
				params.set( "text", text );
				fireEvent( "NotEnoughResources", params );
			}
			else if( requestResourceError == SUCCESS )
			{
				std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
					[&] ( Unit* unit )
				{		
					if( unit->getOwner() == this )	
					{
						unit->pushOrders( new MoveOrders( loc, buildingBP->getWidth() * buildingBP->getWidth() ) ); 
						unit->pushOrders( new BuildOrders( buildingType, loc ) );
					}
				} );
			}
		}
	}
	//-------------------------------------------------
	void HumanController::queueHoldOrders()
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
			{
				const Orders* orders = unit->getLastOrderInQueue();
				if( orders )
					unit->pushOrders( new HoldOrders( orders->getTargetLocation() ) );
				else
					unit->pushOrders( new HoldOrders( unit->getPosition() ) );
			}
		} );
	}
	//-------------------------------------------------
	void HumanController::notEnoughtResources( NamedProperties& params )
	{
		Controller* owner = nullptr;
		params.get( "owner", owner );
		if( owner == this )
		{
			m_outOfResourceMsgTimer.start();
			std::string text;
			if( params.get( "text", text ) == NamedProperties::SUCCESS )
				m_outOfResourceMsgParams.set( "text", text );
		}
	}
	//-------------------------------------------------
	void HumanController::notEnoughSupply()
	{
		m_notEnoughSupplyMsgTimer.start();		
	}
	//-------------------------------------------------
	void HumanController::issueBuildOrders( const std::string& blueprint, const vec2f& loc )
	{
		BuildingBlueprint* buildingBP = BuildingBlueprint::getBlueprint( blueprint );
		if( buildingBP != nullptr )
		{
			if( m_world->canBuildAtWorldCoords( this, buildingBP, loc ) )
			{
				Controller::ResourceRequestError requestResourceError = 
					requestResource( buildingBP->getBuildingCost().fishFingers, buildingBP->getBuildingCost().custard, buildingBP->getBuildingCost().timeEnergy );
				if( requestResourceError == Controller::NOT_ENOUGH_FISH_FINGERS )
				{
					NamedProperties params;
					std::string text = "Not enough Fish Fingers";
					params.set( "text", text );
					fireEvent( "NotEnoughResources", params );
				}
				else if( requestResourceError == Controller::NOT_ENOUGH_CUSTARD )
				{
					NamedProperties params;
					std::string text = "Not enough Custard";
					params.set( "text", text );
					fireEvent( "NotEnoughResources", params );
				}
				else if( requestResourceError == Controller::NOT_ENOUGH_TIME_ENERGY )
				{
					NamedProperties params;
					std::string text = "Not enough Time Energy";
					params.set( "text", text );
					fireEvent( "NotEnoughResources", params );
				}
				else if( requestResourceError == SUCCESS )
				{
					std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
						[&] ( Unit* unit )
					{
						if( unit->getOwner() == this )			
						{
							unit->assignOrders( new MoveOrders( loc, buildingBP->getWidth() * buildingBP->getWidth() ) ); 
							unit->pushOrders( new BuildOrders( blueprint, loc ) );
						}
					} );
				}
			}
		}
	}
	//-------------------------------------------------
	void HumanController::issueBuildUnitOrder( const std::string& unitType )
	{
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{
			if( building->isComplete() && building->getOwner() == this )
			{
				if( hasEnoughSupply( unitType ) )
					building->pushOrders( new BuildUnitOrders( building, unitType ) );
				else
					notEnoughSupply();
			}
		} );
	}
	//-------------------------------------------------
	void HumanController::deselectAll()
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			unit->setSelected( false, false );
		} );
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{
			building->setSelected( false, false );
		} );
		m_unitsSelected.clear();
		m_buildingsSelected.clear();
	}
	//-------------------------------------------------
	Entity* HumanController::getFirstUnitEntity( const std::vector< Entity* > entities ) const
	{
		for( size_t i = 0; i < entities.size(); ++i )
		{
			if( entities[i]->getType() == Entity::UNIT )
			{
				return entities[i];
			}
		}
		return nullptr;
	}
	//-------------------------------------------------
	Entity* HumanController::getFirstBuildingEntity( const std::vector< Entity* > entities ) const
	{
		for( size_t i = 0; i < entities.size(); ++i )
		{
			if( entities[i]->getType() == Entity::BUILDING )
			{
				return entities[i];
			}
		}
		return nullptr;
	}
	//-------------------------------------------------
	void HumanController::popBackBuildUnitOrder()
	{
		for( size_t i = 0; i < m_buildingsSelected.size(); ++i )
		{
			if( m_buildingsSelected[i]->hasOrders() )
			{
				m_buildingsSelected[i]->popOrder();
				break;
			}
		}
	}
	//-------------------------------------------------
	void HumanController::createControlGroup( char ctrlGroup )
	{
		std::vector< Unit* >& unitCtrlGroup = m_unitControlGroups[ ctrlGroup ];
		std::vector< Building* >& buildingCtrlGroup = m_buildingControlGroups[ ctrlGroup ];
		unitCtrlGroup.clear();
		buildingCtrlGroup.clear();

		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			unitCtrlGroup.push_back( unit );
		} );
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{
			buildingCtrlGroup.push_back( building );
		} );
	}
	//-------------------------------------------------
	void HumanController::addToControlGroup( char ctrlGroup )
	{
		std::vector< Unit* >& unitCtrlGroup = m_unitControlGroups[ ctrlGroup ];
		std::vector< Building* >& buildingCtrlGroup = m_buildingControlGroups[ ctrlGroup ];
		
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
				unitCtrlGroup.push_back( unit );
		} );
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{
			if( building->getOwner() == this )
				buildingCtrlGroup.push_back( building );
		} );
	}
	//-------------------------------------------------
	void HumanController::selectControlGroup( char ctrlGroup )
	{
		std::vector< Unit* >& unitCtrlGroup = m_unitControlGroups[ ctrlGroup ];
		std::vector< Building* >& buildingCtrlGroup = m_buildingControlGroups[ ctrlGroup ];

		deselectAll();
		std::for_each( unitCtrlGroup.begin(), unitCtrlGroup.end(),
			[&] ( Unit* unit )
		{
			unit->setSelected( true, true );
			m_unitsSelected.push_back( unit );
		} );
		std::for_each( buildingCtrlGroup.begin(), buildingCtrlGroup.end(),
			[&] ( Building* building )
		{
			building->setSelected( true, true );
			m_buildingsSelected.push_back( building );
		} );
	}
	//-------------------------------------------------
	char HumanController::getNumericalCharFromShiftedChar( char keyCode )
	{
		char numKeyCode = '0' - 1;
		switch( keyCode )
		{
		case '!':
			numKeyCode = '1';
			break;
		case '@':
			numKeyCode = '2';
			break;
		case '#':
			numKeyCode = '3';
			break;
		case '$':
			numKeyCode = '4';
			break;
		case '%':
			numKeyCode = '5';
			break;
		case '^':
			numKeyCode = '6';
			break;
		case '&':
			numKeyCode = '7';
			break;
		case '*':
			numKeyCode = '8';
			break;
		case '(':
			numKeyCode = '9';
			break;
		case ')':
			numKeyCode = '0';
			break;
		}
		return numKeyCode;
	}

}