#include "RTSWorld.h"
#include "Map.h"
#include "Entity.h"
#include "UnitBlueprint.h"
#include "BuildingBlueprint.h"
#include "Unit.h"
#include "Building.h"
#include "MoveOrders.h"
#include "HumanController.h"
#include "GotoResource.h"
#include <Renderer.h>
#include <Camera.h>
#include "MapTypeBlueprint.h"
#include "UnitOrders.h"
#include "RTSDebuggingStats.h"
#include <Time.h>
#include "TerrainType.h"
#include "AIController.h"

namespace pdh
{
#pragma warning( disable:4355 )
	RTSWorld::RTSWorld( int numAI, const std::string& mapType, int mapWidth, int mapHeight, Renderer* renderer )
		:	m_renderer( renderer )
		,	m_debugMode( false )
		,	m_mapWidth( mapWidth )
		,	m_mapHeight( mapHeight )
		,	m_aStarPathGen( this )
		,	NUM_PATHS_GENERATED_PER_FRAME( 4 )
		,	m_playerToRenderPOVFrom( 0 )
	{
		m_hudCamera = new Camera( "HUD Camera", vec3f(), mat4f::ortho( 0.0f, (float)m_renderer->getScreenWidth(), (float)m_renderer->getScreenHeight(), 0.0f, -1.0f, 1.0f ) );
		
		m_currentNumPlayers = numAI + 1;
		m_map = createMap( mapType );
		loadTeamColors( "PlayerColors.xml" );

		m_humanController = new HumanController( this, 1, m_hudCamera, m_playerColors[0] );
		spawnInitialEntitiesForController( m_humanController, "K-9", "Tardis" );
		for( int i = 0; i < numAI; ++i )
		{
			Color4f color;
			if( i + 1 < (int)m_playerColors.size() )
				color = m_playerColors[ i + 1 ];
			else
				color = color::RED;
			m_aiControllers.push_back( new AIController( this,  i + 2, m_map->getWorldCoordsFromTileCoords( m_map->getPlayerStartLocation( i + 1 ) ), color ) );
			spawnInitialEntitiesForController( m_aiControllers[i], "Cyberman", "DalekScoutship" );
		}

		registerConsoleCommand( "debugMode", "Enables the rendering of debug graphics",
			[&] ( const std::string& args )
		{
			m_debugMode = !m_debugMode; 
		} );

		registerConsoleCommand( "spawnUnit", "Spawn a unit. Usage: [UnitType] [x loc] [y loc]",
			[&] ( const std::string& args )
		{
			std::vector< std::string > tokens;
			stringTokenizer( args, tokens, " " );
			if( tokens.size() != 3 )
			{
				consolePrintColorf( "Invalid number of arguments. See usage", color::RED );
				return;
			}

			float x;
			float y;

			if( !stringToType( tokens[1], &x ) )
			{
				consolePrintColorf( "Invalid x coord: %s. See usage", color::RED, tokens[1].c_str() );
				return;
			}
			if( !stringToType( tokens[2], &y ) )
			{
				consolePrintColorf( "Invalid y coord: %s. See usage", color::RED, tokens[2].c_str() );
				return;
			}
			
			spawnUnit( tokens[0], vec2f( x, y ), m_humanController );

		} );

		registerConsoleCommand( "renderFromPlayerPOV", "Sets which player POV to render from. Usage: renderFromPlayerPOV [playerNum 0-numPlayers or -1 for the entire map]",
			[&]( const std::string& args )
		{
			int playerNum;
			if( stringToType( args, &playerNum ) )
			{
				m_playerToRenderPOVFrom = playerNum;
			}
			else
			{
				consolePrintColorf( "Invalid player number: %s", color::RED, args.c_str() );
			}
		} );


		
		
		//WorldCoords startLoc = m_map->getWorldCoordsFromTileCoords( m_map->getPlayerStartLocation( 0 ) );
		//
		//startLoc = getNearestWorldCoordsToBuildBulidingOnGrid( startLoc, BuildingBlueprint::getBlueprint( "Tardis" )->getWidth(), BuildingBlueprint::getBlueprint( "Tardis" )->getHeight() );
		//Building* building = spawnBuilding( "DalekScoutship", startLoc, m_humanController );
		//building->setCurrentBuildTime( (float)building->getTimeToBuild() );
		//building->setHealth( building->getBlueprint()->getMaxHealth() );
		//
		//startLoc = m_map->getWorldCoordsFromTileCoords( m_map->getPlayerStartLocation( 1 ) );
		//startLoc = getNearestWorldCoordsToBuildBulidingOnGrid( startLoc, BuildingBlueprint::getBlueprint( "DalekSpaceship" )->getWidth(), BuildingBlueprint::getBlueprint( "DalekSpaceship" )->getHeight() );
		//
		//building = spawnBuilding( "DalekScoutship", startLoc, m_aiControllers[0] );
		//building->setCurrentBuildTime( (float)building->getTimeToBuild() );
		//building->setHealth( building->getBlueprint()->getMaxHealth() );

		//Unit* unit = spawnUnit( "WhiteDalek", WorldCoords( 15.0f, 15.0f ), m_aiControllers[0] );
		//unit->assignOrders( new MoveOrders( WorldCoords( 50.0f, 50.0f ) ) );
		
		m_cardinalDirections[0] = Map::CD_NORTH;
		m_cardinalDirections[1] = Map::CD_EAST;
		m_cardinalDirections[2] = Map::CD_SOUTH;
		m_cardinalDirections[3] = Map::CD_WEST;
	}
#pragma warning( default:4355 )
	//--------------------------------------------------------
	RTSWorld::~RTSWorld()
	{
		VECTOR_CLEANUP( m_entities );
		VECTOR_CLEANUP( m_aiControllers );
		SAFE_DELETE( m_humanController );
		SAFE_DELETE( m_hudCamera );
		SAFE_DELETE( m_map );
	}
	//--------------------------------------------------------
	Unit* RTSWorld::spawnUnit( const std::string& unitName, const vec2f& pos, Controller* owner )
	{
		UnitBlueprint* blueprint = UnitBlueprint::getBluePrint( unitName );
		Unit* unit  = nullptr;
		if( blueprint )
		{
			unit = blueprint->createUnit( this );
			unit->setPosition( pos );
			m_entities.push_back( unit );
			unit->setOwner( owner );
		}
		return unit;
	}
	//--------------------------------------------------------
	Unit* RTSWorld::spawnUnit( const UnitBlueprint* unitBP, const vec2f& pos, Controller* owner )
	{
		Unit* unit = unitBP->createUnit( this );
		unit->setPosition( pos );
		m_entities.push_back( unit );
		unit->setOwner( owner );
		return unit;
	}
	//--------------------------------------------------------
	Building* RTSWorld::spawnBuilding( const std::string& buildingName, const vec2f& pos, Controller* owner )
	{
		BuildingBlueprint* blueprint = BuildingBlueprint::getBlueprint( buildingName ); 
		Building* building = nullptr;
		if( blueprint )
		{
			building = blueprint->createBuilding( this );
			building->setPosition( pos );
			building->setOwner( owner );
			m_entities.push_back( building );
		}
		return building;
	}
	//--------------------------------------------------------
	Building* RTSWorld::spawnBuilding( const BuildingBlueprint* buildingBP, const vec2f& pos, Controller* owner )
	{
		Building* building = buildingBP->createBuilding( this );
		building->setPosition( pos );
		building->setOwner( owner );
		m_entities.push_back( building );
		return building;
	}
	//--------------------------------------------------------
	void RTSWorld::addEntity( Entity* entity )
	{
		m_entities.push_back( entity );
	}
	//--------------------------------------------------------
	void RTSWorld::killEntity( Entity* entityToKill )
	{
		m_pendingUnitsToDie.push_back( entityToKill );
	}
	//--------------------------------------------------------
	WorldCoords RTSWorld::getNearestWorldCoordsToBuildBulidingOnGrid( const WorldCoords& worldCoords, float width, float height ) const
	{
		float hWidth = width * 0.5f;
		float hHeight = height * 0.5f;
		WorldCoords newWorldCoords = worldCoords;
		WorldCoords offset( hWidth, hHeight );

		WorldCoords cornerWorldCoords = worldCoords - offset;
		TileCoords closestTileCoords = m_map->getTileCoordsFromWorldCoords( cornerWorldCoords );
		if( closestTileCoords.x != -1 )
			newWorldCoords = m_map->getWorldCoordsFromTileCoords( closestTileCoords ) + offset - WorldCoords( 0.5f, 0.5f );

		return newWorldCoords;
	}
	//--------------------------------------------------------
	Entity* RTSWorld::getClosestEnemyEntityInRadius( const Entity* entityLooking, float radius ) const
	{
		std::vector< std::pair< Entity*, float > > inRangeEntities;
		float radiusSqrd = radius * radius;
		std::for_each( m_entities.begin(), m_entities.end(), 
			[&] ( Entity* entity )
		{
			WorldCoords delta = entity->getPosition() - entityLooking->getPosition();
			float distSqrd = delta.dot( delta );
			if( distSqrd < radiusSqrd )
			{
				if( entity->getOwner() != entityLooking->getOwner() )
					inRangeEntities.push_back( std::make_pair( entity, distSqrd ) );
			}
		} );

		std::pair< Entity*, float > enemyEntity = std::pair< Entity*, float >( nullptr, radiusSqrd + 1.0f );
		std::for_each( inRangeEntities.begin(), inRangeEntities.end(),
			[&]( std::pair< Entity*, float >& inRangeEntity )
		{
			if( inRangeEntity.second < enemyEntity.second )
			{
				enemyEntity.first = inRangeEntity.first;
				enemyEntity.second = inRangeEntity.second;
			}
		} );

		return enemyEntity.first;
	}
	//--------------------------------------------------------
	void RTSWorld::updateSimulation( float deltaTime )
	{
		for( size_t i = 0; i < m_entities.size(); ++i )
		{
			m_entities[i]->update( deltaTime );
		}
		for( size_t i = 0; i < m_aiControllers.size(); ++i )
		{
			m_aiControllers[i]->update( deltaTime );
		}
		killPendingEntitiesReadyToDie();
		generateAStarPaths();
	}
	//--------------------------------------------------------
	void RTSWorld::updateDisplay()
	{
		if( m_playerToRenderPOVFrom == -1 )
		{
			m_map->enableDebugRender( m_debugMode );
			m_map->render();
			for( size_t i = 0; i < m_entities.size(); ++i )
			{
				m_entities[i]->render();
				if( m_debugMode )
					m_entities[i]->debugRender();
			}
		}
		else if( m_playerToRenderPOVFrom == 0 )
		{
			m_humanController->renderViewOfWorld();
		}
		else
		{
			if( m_playerToRenderPOVFrom < (int)m_aiControllers.size() + 1 )
			{
				m_aiControllers[ m_playerToRenderPOVFrom - 1 ]->renderViewOfWorld();
			}
		}

		m_humanController->render();
	}
	//--------------------------------------------------------
	int RTSWorld::getScreenWidth() const
	{
		return m_renderer->getScreenWidth();
	}
	//--------------------------------------------------------
	int RTSWorld::getScreenHeight() const
	{
		return m_renderer->getScreenHeight();
	}
	//--------------------------------------------------------
	WorldCoords RTSWorld::getWorldCoordsFromScreenCoords( const vec2i& screenCoords ) const
	{
		float x = 2.0f * screenCoords.x / m_renderer->getScreenWidth() - 1.0f;
		float y = -2.0f * screenCoords.y / m_renderer->getScreenHeight() + 1.0f;

		mat4f viewProj = m_renderer->getViewMatrix() * m_renderer->getProjMatrix();
		mat4f viewProjInverse = viewProj.inverse();
		vec3f pointInClipSpace( x, y, 0 );
		vec3f worldCoords = transformVector( pointInClipSpace, viewProjInverse );
		return WorldCoords( worldCoords.x, worldCoords.y );
	}
	//--------------------------------------------------------
	Entity* RTSWorld::getSelectedEntityFromWorldCoords( const Controller* requestor, const WorldCoords worldCoords ) const
	{
		for( size_t i = 0; i < requestor->getListofEntitesOwned().size(); ++i )
		{
			if( requestor->getListofEntitesOwned()[i]->getAABB2().contains( worldCoords ) )
			{
				return requestor->getListofEntitesOwned()[i];
			}
		}
		for( size_t i = 0; i < requestor->getListOfVisibleEntities().size(); ++i )
		{
			if( requestor->getListOfVisibleEntities()[i]->getAABB2().contains( worldCoords ) )
			{
				return requestor->getListOfVisibleEntities()[i];
			}
		}
		return nullptr;
	}
	//--------------------------------------------------------
	Map::MapTile& RTSWorld::getSelectedMapTileFromWorldCoord( Controller* requestor, const WorldCoords worldCoords ) const
	{
		return m_map->getMapTile( requestor->getCurrentViewOfMap()->getTileCoordsFromWorldCoords( worldCoords ) );
	}
	//--------------------------------------------------------
	Map::MapTile& RTSWorld::getSelectedMapTileFromTileCoord( Controller* requestor, const TileCoords& tileCoords ) const
	{
		return requestor->getCurrentViewOfMap()->getMapTile( tileCoords );
	}
	//--------------------------------------------------------
	Entity* RTSWorld::getSelectedEntityFromAABB2( const Controller* requestor, const aabb2f& selection ) const
	{
		for( size_t i = 0; i < requestor->getListOfVisibleEntities().size(); ++i )
		{
			if( requestor->getListOfVisibleEntities()[i]->getAABB2().intersects( selection ) )
			{
				return requestor->getListOfVisibleEntities()[i];
			}
		}
		for( size_t i = 0; i < requestor->getListofEntitesOwned().size(); ++i )
		{
			if( requestor->getListofEntitesOwned()[i]->getAABB2().intersects( selection ) )
			{
				return requestor->getListofEntitesOwned()[i];
			}
		}
		return nullptr;
	}
	//--------------------------------------------------------
	void RTSWorld::getSelectedEntitesFromAABB2( const Controller* requestor, const aabb2f& selection, std::vector< Entity* >& entitiesSelection ) const
	{
		for( size_t i = 0; i < requestor->getListofEntitesOwned().size(); ++i )
		{
			if( selection.intersects( requestor->getListofEntitesOwned()[i]->getAABB2() ) )
			{
				entitiesSelection.push_back( requestor->getListofEntitesOwned()[i] );
			}
		}
		for( size_t i = 0; i < requestor->getListOfVisibleEntities().size(); ++i )
		{
			if( selection.intersects( requestor->getListOfVisibleEntities()[i]->getAABB2() ) )
			{
				entitiesSelection.push_back( requestor->getListOfVisibleEntities()[i] );
			}
		}
	}
	//--------------------------------------------------------
	void RTSWorld::getAllEntitiesOfTypeOnScreen( const Entity* entity, std::vector< Entity* >& entitiesSelected ) const
	{
		aabb2f entireScreen( getWorldCoordsFromScreenCoords( vec2i( 0, m_renderer->getScreenHeight() ) ), getWorldCoordsFromScreenCoords( vec2i( m_renderer->getScreenWidth(), 0 ) ) );
		for( size_t i = 0; i < entity->getOwner()->getListofEntitesOwned().size(); ++i )
		{
			if( entireScreen.contains( entity->getOwner()->getListofEntitesOwned()[i]->getPosition() ) )
			{
				if( entity->getOwner()->getListofEntitesOwned()[i]->getBlueprintName() == entity->getBlueprintName() )
				{					
					entitiesSelected.push_back( entity->getOwner()->getListofEntitesOwned()[i] );
				}
			}
		}
	}
	//--------------------------------------------------------
	Building* RTSWorld::getBuildingToReturnResources( Unit* unit ) const
	{
		Entity* closest = nullptr;
		float distSqrdToClosest = (float)m_map->getMapWidth();
		distSqrdToClosest *= distSqrdToClosest;
		for( size_t i = 0; i < unit->getOwner()->getListofEntitesOwned().size(); ++i )
		{
			if( unit->getOwner()->getListofEntitesOwned()[i]->getType() == Entity::BUILDING )
			{
				vec2f dx = unit->getOwner()->getListofEntitesOwned()[i]->getPosition() - unit->getPosition();
				float distSqrd = dx.dot( dx );
				if( distSqrd < distSqrdToClosest )
				{
					distSqrdToClosest = distSqrd;
					closest = unit->getOwner()->getListofEntitesOwned()[i];
				}
			}
		}
		return reinterpret_cast< Building* >( closest );
	}
	//--------------------------------------------------------
	TileCoords RTSWorld::getTileCoordsForCollectingResource( const Unit* unit, const Map::MapTile& mapTile ) const
	{
		TileCoords closestTC( -1, -1 );
		float closestDistSqrd = (float)m_map->getMapWidth() * m_map->getMapWidth();
		for( int i = 0; i < 4; ++i )
		{
			TileCoords tileCoords = m_cardinalDirections[i] + mapTile.getTilecoords();
			if( m_map->areTileCoordsInBounds( tileCoords ) )
			{
				if( canEntityMoveOnTile( unit, tileCoords ) )
				{
					WorldCoords delta = m_map->getWorldCoordsFromTileCoords( tileCoords ) - unit->getPosition();
					float distSqrd = delta.dot( delta );
					if( distSqrd < closestDistSqrd )
					{
						closestTC = tileCoords;
						closestDistSqrd = distSqrd;
					}
				}
			}
		}
		return closestTC;
	}
	//--------------------------------------------------------
	TileCoords RTSWorld::getTileCoordsForCollectingResource( const Unit* unit, const TileCoords& start, Map::ResourceType type ) const
	{
		float closestDistSqrd = (float)m_map->getMapWidth() * m_map->getMapWidth();
		TileCoords closestTileCoords( -1, -1 );
		WorldCoords startWC = m_map->getWorldCoordsFromTileCoords( start );
		for( int y = 0; y < m_map->getMapHeight(); ++y )
		{
			for( int x = 0; x < m_map->getMapWidth(); ++x )
			{
				TileCoords tileCoords( x, y );
				Map::MapTile& mapTile = unit->getOwner()->getCurrentViewOfMap()->getMapTile( tileCoords );
				if( mapTile.getResourceType() == type )
				{
					WorldCoords delta = m_map->getWorldCoordsFromTileCoords( tileCoords ) - startWC;
					float distSqrd =  delta.dot( delta );
					if( distSqrd < closestDistSqrd )
					{
						closestTileCoords = tileCoords;
					}
				}
			}
		}
		return closestTileCoords;
	}
	//--------------------------------------------------------
	bool RTSWorld::canEntityMoveOnTile( const Unit* unit, const TileCoords& tileCoords ) const
	{
		bool canMove = false;
		if( unit )
			canMove = unit->canMoveOnTileWithWorldCoords( unit->getOwner()->getCurrentViewOfMap()->getWorldCoordsFromTileCoords( tileCoords ) );
		return canMove;
	}
	//----------------------------------------------------------------
	bool RTSWorld::canBuildAtWorldCoords( const Controller* requestor, const BuildingBlueprint* buildingBP, const WorldCoords& worldCoords ) const
	{
		float width = buildingBP->getWidth();
		float height = buildingBP->getHeight();
		WorldCoords minPoint = worldCoords - WorldCoords( width * 0.5f, height * 0.5f );
		aabb2f boundingBox = aabb2f( minPoint, width, height );

		TileCoords minTileCoords = requestor->getCurrentViewOfMap()->getTileCoordsFromWorldCoords( boundingBox.minPoint() );
		TileCoords maxTileCoords = requestor->getCurrentViewOfMap()->getTileCoordsFromWorldCoords( boundingBox.maxPoint() );

		bool canBuildOn = true;
		const std::vector< std::string >& canBuildOnRules = buildingBP->getConstructionRules().canBeBuiltOn;
		const std::vector< std::string >& mustBeBuildAdjToRules = buildingBP->getConstructionRules().mustBeBuiltAdjTo;

		if( mustBeBuildAdjToRules.size() > 0 )
		{
			canBuildOn = false;
			for( int y = minTileCoords.y - 1; y < maxTileCoords.y + 1 && !canBuildOn; y += (int)height + 1 )
			{
				for( int x = minTileCoords.x; x < maxTileCoords.x && !canBuildOn; ++x )
				{
					TileCoords tileCoords( x, y );
					int i = requestor->getCurrentViewOfMap()->getTileIndexFromTileCoords( tileCoords );
					if( i != -1 )
					{
						const Map::MapTile& mapTile = requestor->getCurrentViewOfMap()->getMapTile( tileCoords );
						auto mustBuildAdjToIter = std::find( mustBeBuildAdjToRules.begin(), mustBeBuildAdjToRules.end(), mapTile.getTerrainType()->getName() );
						if( mustBuildAdjToIter != mustBeBuildAdjToRules.end() )
							canBuildOn = true;
					}
				}
			}
			for( int x = minTileCoords.x - 1; x < maxTileCoords.x + 1 && !canBuildOn; x += (int)width + 1 )
			{
				for( int y = minTileCoords.y; y < maxTileCoords.y && !canBuildOn; ++y )
				{
					TileCoords tileCoords( x, y );
					int i = requestor->getCurrentViewOfMap()->getTileIndexFromTileCoords( tileCoords );
					if( i != -1 )
					{
						const Map::MapTile& mapTile = requestor->getCurrentViewOfMap()->getMapTile( tileCoords );
						auto mustBuildAdjToIter = std::find( mustBeBuildAdjToRules.begin(), mustBeBuildAdjToRules.end(), mapTile.getTerrainType()->getName() );
						if( mustBuildAdjToIter != mustBeBuildAdjToRules.end() )
							canBuildOn = true;
					}
				}
			}	
		}
		
		for( int x = minTileCoords.x; x < maxTileCoords.x; ++x )
		{
			for( int y = minTileCoords.y; y < maxTileCoords.y; ++y )
			{
				TileCoords tileCoords( x, y );
				if( tileCoords.x >= 0 && tileCoords.y >= 0 )
				{
					const Map::MapTile& mapTile = requestor->getCurrentViewOfMap()->getMapTile( tileCoords );
					auto canBuildOnIter = std::find( canBuildOnRules.begin(), canBuildOnRules.end(), mapTile.getTerrainType()->getName() );
					if( canBuildOnIter == canBuildOnRules.end() )
						canBuildOn = false;
				}
			}
		}

		

		return canBuildOn;
	}
	//----------------------------------------------------------------
	void RTSWorld::requestPath
		( UnitOrders* orders, const Unit* unit, std::vector< TileCoords >& pathOut )
	{
		m_entitesRequestingPath.push_back( AStarPathRequestor( orders, unit, pathOut ) );
		//m_aStarPathGen.getAStarPathToGoal( unit, m_map->getTileCoordsFromWorldCoords( orders->getCurrentLoc() ), 
		//	m_map->getTileCoordsFromWorldCoords( orders->getTargetLocation() ), pathOut );
	}
	void RTSWorld::unregisterForPath( UnitOrders* orders )
	{
		m_entitesRequestingPath.erase( std::remove_if( m_entitesRequestingPath.begin(), m_entitesRequestingPath.end(), 
			[&] ( AStarPathRequestor& requestor ) -> bool
			{
				return requestor.orders == orders;
			} )
			, m_entitesRequestingPath.end() );
	}
	//--------------------------------------------------------
	void RTSWorld::getVisibleEntitesInRadius( const Entity* entity, const vec2f& pos, float radius, std::vector< Entity* >& entities )
	{
		for( size_t i = 0; i < m_entities.size(); ++i )
		{
			vec2f delta = m_entities[i]->getPosition() - pos;
			float distSqrd = delta.dot( delta );
			if( distSqrd < radius*radius )
			{
				if( entity->getOwner() != m_entities[i]->getOwner() )
					entities.push_back( m_entities[i] );
			}
		}
	}
	//--------------------------------------------------------
	// Protected memeber functions
	//--------------------------------------------------------
	void RTSWorld::spawnInitialEntitiesForController( Controller* controller, const std::string& peonType, const std::string& buildingType )
	{
		WorldCoords startLoc = m_map->getWorldCoordsFromTileCoords( m_map->getPlayerStartLocation( controller->getPlayerNumber() - 1 ) );
		float width = BuildingBlueprint::getBlueprint( buildingType )->getWidth();
		float height = BuildingBlueprint::getBlueprint( buildingType )->getHeight();

		startLoc = getNearestWorldCoordsToBuildBulidingOnGrid( startLoc, width, height );
		Building* building = spawnBuilding( buildingType, startLoc, controller );
		building->setCurrentBuildTime( (float)building->getTimeToBuild() );
		building->setHealth( building->getBlueprint()->getMaxHealth() );

		UnitBlueprint* unitBP = UnitBlueprint::getBluePrint( peonType );
		if( unitBP != nullptr )
		{
			spawnUnit( peonType, startLoc + WorldCoords( width, 0.0f ), controller );
			spawnUnit( peonType, startLoc + WorldCoords( -width, 0.0f ), controller );
			spawnUnit( peonType, startLoc + WorldCoords( 0.0f, height ), controller );
			spawnUnit( peonType, startLoc + WorldCoords( 0.0f, -height ), controller );
			controller->addToCurrentSupplyCount( unitBP->getUnitCost().supply * 4 );
		}
		
	}
	//--------------------------------------------------------
	Map* RTSWorld::createMap( const std::string& mapType )
	{
		MapTypeBlueprint* blueprint = MapTypeBlueprint::getBlueprint( mapType ); 
		Map* map;
		if( blueprint == nullptr )
		{
			return nullptr;
		}
		map = blueprint->createMap( m_mapWidth, m_mapHeight, m_currentNumPlayers, this );

		return map;
	}
	//--------------------------------------------------------
	void RTSWorld::generateAStarPaths()
	{
		for( size_t i = 0; m_entitesRequestingPath.size() > 0 && i < NUM_PATHS_GENERATED_PER_FRAME; ++i )
		{
			SystemClocks startTime = GetAbsoluteTimeClocks();
		
			AStarPathRequestor& requestor = m_entitesRequestingPath.front();
			if( requestor.orders != nullptr )
			{
				requestor.pathOut.clear();
				if( m_aStarPathGen.getAStarPathToGoal( requestor.unit, m_map->getTileCoordsFromWorldCoords( requestor.orders->getCurrentLoc() ), 
					m_map->getTileCoordsFromWorldCoords( requestor.orders->getTargetLocation() ), requestor.pathOut ) == AStarPathGenerator::SUCCESS )
				{
					requestor.orders->givePath();
					++RTSDebuggingStats::ASTAR_PATHS_GEN;
				}
				m_entitesRequestingPath.pop_front();				
			}
			SystemClocks endTime = GetAbsoluteTimeClocks();
			RTSDebuggingStats::ASTAR_PATH_GEN_TIME += (float)ClocksToSeconds( endTime - startTime );
		}
	}
	//--------------------------------------------------------
	void RTSWorld::killPendingEntitiesReadyToDie()
	{
		NamedProperties params;
		
		if( m_pendingUnitsToDie.size() > 0 )
		{
			std::for_each( m_pendingUnitsToDie.begin(), m_pendingUnitsToDie.end(), 
				[&]( Entity* entityToKill )
			{
				params.set( "entity", entityToKill );
				fireEvent( "EntityKilled", params );

				m_entities.erase( std::remove_if( m_entities.begin(), m_entities.end(), 
					[&]( Entity* entity ) -> bool
				{
					return entityToKill == entity;
				} ), m_entities.end() );

				SAFE_DELETE( entityToKill );
			} );
			m_pendingUnitsToDie.clear();
		}
	}
	//--------------------------------------------------------
	RTSWorld::AStarPathRequestor::AStarPathRequestor( UnitOrders* orders, const Unit* unit, std::vector< TileCoords >& pathOut )
		:	orders( orders )	
		,	unit( unit )
		,	pathOut( pathOut )
	{}
	//--------------------------------------------------------
	RTSWorld::AStarPathRequestor& RTSWorld::AStarPathRequestor::operator=( const AStarPathRequestor& rhs )
	{
		orders = rhs.orders;
		unit = rhs.unit;
		pathOut = rhs.pathOut;

		return *this;
	}
	//----------------------------------------------------------
	void RTSWorld::loadTeamColors( const char* filePath )
	{
		XMLParser parser( filePath );
		const XMLNode* root = parser.getDocument().FirstChildElement( "PlayerColors" );
		if( root )
		{
			parser.validateXMLChildElements( root, "PlayerColor", "" );
			const XMLNode* playerColor = root->FirstChildElement( "PlayerColor" );
			for( ; playerColor != nullptr; playerColor = playerColor->NextSiblingElement( "PlayerColor" ) )
			{
				m_playerColors.push_back( parser.getXMLAttributeAsColor( playerColor, "color", color::WHITE ) );
			}
		}
	}
	//----------------------------------------------------------
	const int RTSWorld::MAX_NUM_PLAYERS = 8;
}