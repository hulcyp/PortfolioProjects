#include "Controller.h"
#include "RTSWorld.h"
#include "BuildingBlueprint.h"
#include "Unit.h"
#include "AttackOrders.h"
#include "MoveOrders.h"
#include <functional>
#include "GotoResource.h"
#include "BuildOrders.h"
#include "Building.h"
#include "BuildUnitOrders.h"
#include "AttackOrders.h"
#include "UnitBlueprint.h"
#include "AttackMoveOrders.h"
#include "HoldOrders.h"
#include "TerrainType.h"

namespace pdh
{
	Controller::Controller( RTSWorld* world, int playerNum, const Color4f& teamColor )
		:	m_world( world )
		,	m_playerNumber( playerNum )
		,	m_fishFingers( 0 )
		,	m_custard( 0 )
		,	m_timeEnergy( 0 )
		,	m_currentSupplyCount( 0 )
		,	m_maxSupplyCount( 0 )
		,	MAX_SUPPLY_COUNT( 200 )
		,	m_teamColor( teamColor )
		,	m_currentFrame( 0 )
	{
		registerForEvent( "GiveResource" + toString( m_playerNumber ), *this, &Controller::giveResourceEvt );
		registerForEvent( "EntityKilled", *this, &Controller::entityKilled );
		
		m_unitsSelected.clear();
		m_buildingsSelected.clear();

		m_currentMapView = new Map( m_world );
		m_currentMapView->initializeMap( "none", world->getMap()->getMapWidth(), world->getMap()->getMapHeight() );

		registerConsoleCommand( "GiveResource" + toString( m_playerNumber ), "Give resources to player. Usage: [fishFinger] [custard] [timeEnergy]",
			[&]( const std::string& args )
		{
			std::vector< std::string > tokens;
			stringTokenizer( args, tokens, " " );
			
			NamedProperties params;
			
			if( tokens.size() > 0 )
			{
				int fishFingers;
				if( !stringToType( tokens[0], &fishFingers ) )
					consolePrintColorf( "Must be integer value for fish fingers, see usage", color::RED );
				else
					params.set( "fishFingers", fishFingers );
			}
			if( tokens.size() > 1 )
			{
				int custard;
				if( !stringToType( tokens[1], &custard ) )
					consolePrintColorf( "Must be integer value for custard, see usage", color::RED );
				else
					params.set( "custard", custard );
			}
			if( tokens.size() > 2 )
			{
				int timeEnergy;
				if( !stringToType( tokens[2], &timeEnergy ) )
					consolePrintColorf( "Must be integer value for timeEnergy, see usage", color::RED );
				else
					params.set( "timeEnergy", timeEnergy );
			}
			fireEvent( "GiveResource" + toString( m_playerNumber ), params );
		} );
	}
	//--------------------------------------------------
	Controller::~Controller()
	{
		m_unitsSelected.clear();
		m_buildingsSelected.clear();
		SAFE_DELETE( m_currentMapView );
		m_visibleEntities.clear();
		m_entitiesOwned.clear();
		m_entitesToRender.clear();
	}
	//--------------------------------------------------
	void Controller::renderViewOfWorld()
	{
		updateMapView();
		updateEntitiesToRender();
		m_currentMapView->render();
		for( size_t i = 0; i < m_entitesToRender.size(); ++i )
		{
			m_entitesToRender[i]->render();
		}
		m_visibleEntities.clear();
		++m_currentFrame;
	}
	//--------------------------------------------------
	void Controller::issueAttackOrdersToLocation( const vec2f& targLoc )
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
				unit->assignOrders( new AttackMoveOrders( targLoc ) );
		} );
	}
	//--------------------------------------------------
	void Controller::issueAttackOrdersOnTargetEntity( Entity* entity )
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
				unit->assignOrders( new AttackOrders( entity ) );
		} );
	}
	//--------------------------------------------------
	void Controller::issueMoveOrders( const vec2f& loc )
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
				unit->assignOrders( new MoveOrders( loc ) );
		} );
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{			
			if( building->getOwner() == this )
				building->assignInitialOrderForUnits( new MoveOrders( loc ) );
		} );
	}
	//--------------------------------------------------
	void Controller::issueBuildOrders( const std::string& blueprint, const vec2f& loc )
	{
		BuildingBlueprint* buildingBP = BuildingBlueprint::getBlueprint( blueprint );
		if( m_world->canBuildAtWorldCoords( this, buildingBP, loc ) )
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
	//--------------------------------------------------
	void Controller::issueBuildOrders( Building* building )
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
			{
				if( building )
				{
					unit->assignOrders( new MoveOrders( building->getPosition(), building->getWidth() * building->getWidth() ) );
					unit->pushOrders( new BuildOrders( building ) );
				}
			}
		} );
	}
	//--------------------------------------------------
	void Controller::issueHarvestOrders( Map::MapTile& mapTile )
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
			{
				if( unit->getUnitBlueprint()->isPeon() )
					unit->assignOrders( new GotoResource( mapTile ) );
				else
				{
					TileCoords tileCoords = m_world->getTileCoordsForCollectingResource( unit, mapTile );
					unit->assignOrders( new MoveOrders( m_world->getMap()->getWorldCoordsFromTileCoords( tileCoords ) ) );
				}
			}
		} );
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{
			if( building->getOwner() == this )
				building->assignInitialOrderForUnits( new GotoResource( mapTile ) );
		} );
	}
	//--------------------------------------------------
	void Controller::issueHoldOrders()
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
			{
				unit->assignOrders( new HoldOrders( unit->getPosition() ) );
			}
		} );
	}
	//--------------------------------------------------
	void Controller::issueStopOrder()
	{
		std::for_each( m_unitsSelected.begin(), m_unitsSelected.end(),
			[&] ( Unit* unit )
		{
			if( unit->getOwner() == this )
				unit->cancelOrders();
		} );
	}
	//--------------------------------------------------
	void Controller::issueBuildUnitOrder( const std::string& unitType )
	{
		std::for_each( m_buildingsSelected.begin(), m_buildingsSelected.end(),
			[&] ( Building* building )
		{
			if( building->isComplete() && building->getOwner() == this )
			{
				if( hasEnoughSupply( unitType ) )
					building->pushOrders( new BuildUnitOrders( building, unitType ) );
			}
		} );
	}
	//--------------------------------------------------
	void Controller::entityKilled( NamedProperties& params )
	{
		Entity* entity = nullptr;
		if( params.get( "entity", entity ) == NamedProperties::SUCCESS )
		{
			m_unitsSelected.erase( std::remove_if( m_unitsSelected.begin(), m_unitsSelected.end(),
				[&]( Unit* unit )
			{
				return unit == entity;
			} ), m_unitsSelected.end() );
			
			m_buildingsSelected.erase( std::remove_if( m_buildingsSelected.begin(), m_buildingsSelected.end(),
				[&]( Building* building )
			{
				return building == entity;
			} ), m_buildingsSelected.end() );

			m_visibleEntities.erase( std::remove_if( m_visibleEntities.begin(), m_visibleEntities.end(),
				[&]( Entity* visibleEntity )
			{
				return visibleEntity == entity;
			} ), m_visibleEntities.end() );

			m_entitiesOwned.erase( std::remove_if( m_entitiesOwned.begin(), m_entitiesOwned.end(),
				[&]( Entity* entityControlled )
			{
				return entityControlled == entity;
			} ), m_entitiesOwned.end() );
			
		}
	}
	//--------------------------------------------------
	void Controller::giveResource( Map::ResourceType type, int amount )
	{
		switch( type )
		{
		case Map::FISH_FINGERS:
			m_fishFingers += amount;
			break;
		case Map::CUSTARD:
			m_custard += amount;
			break;
		case Map::TIME_ENERGY:
			m_timeEnergy += amount;
			break;
		}
	}
	//--------------------------------------------------
	void Controller::giveResourceEvt( NamedProperties& params )
	{
		int fishFingers = 0;
		int custard = 0;
		int timeEnergy = 0;
		params.get( "fishFingers", fishFingers );
		params.get( "custard", custard );
		params.get( "timeEnergy", timeEnergy );

		m_fishFingers += fishFingers;
		m_custard += custard;
		m_timeEnergy += timeEnergy;
	}
	//--------------------------------------------------
	Controller::ResourceRequestError Controller::requestResource( int fishFingers, int custard, int timeEnergy )
	{
		if( m_fishFingers < fishFingers )
			return NOT_ENOUGH_FISH_FINGERS;
		
		if( m_custard < custard )
			return NOT_ENOUGH_CUSTARD;
		
		if( m_timeEnergy < timeEnergy )
			return NOT_ENOUGH_TIME_ENERGY;
		
		m_fishFingers -= fishFingers;
		m_custard -= custard;
		m_timeEnergy -= timeEnergy;

		return SUCCESS;
	}
	//--------------------------------------------------
	void Controller::addToMaxSupplyCount( int amountToAdd )
	{		
		m_maxSupplyCount += amountToAdd;
		m_maxSupplyCount = MathFuncs<int>::clamp( m_maxSupplyCount, 0, MAX_SUPPLY_COUNT );
	}
	//--------------------------------------------------
	void Controller::removeFromMaxSupplyCount( int amountToRemove )
	{
		m_maxSupplyCount -= amountToRemove;
		m_maxSupplyCount = MathFuncs<int>::clamp( m_maxSupplyCount, 0, MAX_SUPPLY_COUNT );
	}
	//--------------------------------------------------
	void Controller::addToCurrentSupplyCount( int amountToAdd )
	{
		m_currentSupplyCount += amountToAdd;
	}
	//--------------------------------------------------
	void Controller::removeFromCurrentSupplyCount( int amountToRemove )
	{		
		m_currentSupplyCount -= amountToRemove;
		m_currentSupplyCount = MathFuncs<int>::clamp( m_currentSupplyCount, 0, m_maxSupplyCount );
	}
	//--------------------------------------------------
	bool Controller::hasEnoughSupply( const std::string& blueprintName )
	{
		UnitBlueprint* blueprint = UnitBlueprint::getBluePrint( blueprintName );
		return ( m_currentSupplyCount + blueprint->getUnitCost().supply ) <= m_maxSupplyCount;
	}
	//--------------------------------------------------
	void Controller::notifyOfVisibleTiles( const std::vector< TileCoords >& tiles )
	{
		for( size_t i = 0; i < tiles.size(); ++i )
		{
			Map::MapTile& realMapTile = m_world->getMap()->getMapTile( tiles[i] );
			Map::MapTile& curMapTile = m_currentMapView->getMapTile( tiles[i] );
			if( realMapTile.getTerrainType() != curMapTile.getTerrainType() )
			{
				m_currentMapView->changeTileType( curMapTile, realMapTile.getTerrainType() );
				curMapTile = realMapTile;
			}
			curMapTile.lastFrameUpdated = m_currentFrame;
		}
	}
	//--------------------------------------------------
	void Controller::notifyOfVisibleEntites( const std::vector< Entity* >& entities )
	{
		for( size_t i = 0; i < entities.size(); ++i )
		{
			m_visibleEntities.push_back( entities[i] );
		}
	}
	//--------------------------------------------------
	void Controller::giveEntityToControl( Entity* entity )
	{
		m_entitiesOwned.push_back( entity );
	}
	//--------------------------------------------------
	const std::vector< Entity* >& Controller::getListOfVisibleEntities() const
	{
		return m_visibleEntities;
	}
	//--------------------------------------------------
	const std::vector< Entity* >& Controller::getListofEntitesOwned() const
	{
		return m_entitiesOwned;
	}
	//--------------------------------------------------
	void Controller::updateMapView()
	{
		for( int y = 0; y < m_currentMapView->getMapHeight(); ++y )
		{
			for( int x = 0; x < m_currentMapView->getMapWidth(); ++x )
			{
				TileCoords tileCoords( x, y );
				int index = m_currentMapView->getTileIndexFromTileCoords( tileCoords );
				Map::MapTile mapTile = m_currentMapView->getMapTile( tileCoords );
				if( mapTile.lastFrameUpdated != m_currentFrame && mapTile.lastFrameUpdated != m_currentFrame - 1 )
				{
					m_currentMapView->changeTileColor( index, mapTile.getTerrainType()->getColor() * color::GREY );								
				}
				else
				{
					m_currentMapView->changeTileColor( index, mapTile.getTerrainType()->getColor() );								
				}
			}
		}
	}
	//--------------------------------------------------
	void Controller::updateEntitiesToRender()
	{
		m_entitesToRender.clear();
		for( size_t i = 0; i < m_entitiesOwned.size(); ++i )
		{
			m_entitesToRender.push_back( m_entitiesOwned[i] );
		}
		for( size_t i = 0; i < m_visibleEntities.size(); ++i )
		{
			m_entitesToRender.push_back( m_visibleEntities[i] );
		}

		std::sort( m_entitesToRender.begin(), m_entitesToRender.end(),
			[&] ( Entity* A, Entity* B )
		{
			return A->getType() > B->getType();
		} );
	}
}