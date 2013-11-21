#include "AIController.h"
#include "Entity.h"
#include "Unit.h"
#include "UnitBlueprint.h"
#include "RTSWorld.h"
#include "Building.h"
#include "BuildUnitOrders.h"
#include "BuildingBlueprint.h"

namespace pdh
{
	AIController::AIController( RTSWorld* world, int teamNum, const WorldCoords& startLoc, const Color4f& teamColor )
		:	Controller( world, teamNum, teamColor )
		,	m_startingLocation( startLoc )
	{}
	//-----------------------------------------------------------------
	void AIController::update( float deltaTime )
	{
		selectIdalPeon();
		if( m_unitsSelected.size() > 0 )
		{
			if( m_fishFingers < 50 )
			{
				TileCoords tileCoords = m_world->getTileCoordsForCollectingResource( m_unitsSelected[0], m_currentMapView->getTileCoordsFromWorldCoords( m_startingLocation ), Map::FISH_FINGERS );
				if( tileCoords.x != -1 )
					issueHarvestOrders( m_currentMapView->getMapTile( tileCoords ) );
			}
			else if( m_custard < 50 )
			{
				TileCoords tileCoords = m_world->getTileCoordsForCollectingResource( m_unitsSelected[0], m_currentMapView->getTileCoordsFromWorldCoords( m_startingLocation ), Map::CUSTARD );
				if( tileCoords.x != -1 )
					issueHarvestOrders( m_world->getMap()->getMapTile( tileCoords ) );
			}
			else
			{
				char hotkey = m_unitsSelected[0]->getUnitBlueprint()->getHotkeyPool().getRandItem();
				std::string buildingName;
				m_unitsSelected[0]->getBuildingNameFromHotkey( hotkey, buildingName );
				TileCoords tileCoords = findLocationToBuildBuilding( buildingName );
				if( tileCoords.x != -1 )
				{
					Controller::ResourceRequestError requestResourceError = 
						requestResource( BuildingBlueprint::getBlueprint( buildingName )->getBuildingCost().fishFingers, 
						BuildingBlueprint::getBlueprint( buildingName )->getBuildingCost().custard, 
						BuildingBlueprint::getBlueprint( buildingName )->getBuildingCost().timeEnergy );
					if( requestResourceError == SUCCESS )
					{
						issueBuildOrders( buildingName, WorldCoords( (float)tileCoords.x, (float)tileCoords.y ) );
					}
				}
			}
		}
		else
		{
			if( m_fishFingers > 100 )
			{
				tryToIssueBuildUnitOrders();
			}
		}
	}
	//-----------------------------------------------------------------
	void AIController::selectPeon()
	{
		m_unitsSelected.clear();
		for( size_t i = 0; i < m_entitiesOwned.size(); ++i )
		{
			if( m_entitiesOwned[i]->getType() == Entity::UNIT )
			{
				Unit* unit = reinterpret_cast< Unit* >( m_entitiesOwned[i] );
				if( unit->getUnitBlueprint()->isPeon() )
				{
					m_unitsSelected.push_back( unit );
					break;
				}
			}
		}
	}
	//-----------------------------------------------------------------
	void AIController::selectIdalPeon()
	{
		m_unitsSelected.clear();
		for( size_t i = 0; i < m_entitiesOwned.size(); ++i )
		{
			if( m_entitiesOwned[i]->getType() == Entity::UNIT )
			{
				Unit* unit = reinterpret_cast< Unit* >( m_entitiesOwned[i] );
				if( unit->getUnitBlueprint()->isPeon() && !unit->hasOrders() )
				{
					m_unitsSelected.push_back( unit );
					break;
				}
			}
		}
	}
	//-----------------------------------------------------------------
	void AIController::tryToIssueBuildUnitOrders()
	{
		for( size_t i = 0; i < m_entitiesOwned.size(); ++i )
		{
			if( m_entitiesOwned[i]->getType() == Entity::BUILDING )
			{
				std::string unitType;
				Building* building = reinterpret_cast< Building* >( m_entitiesOwned[i] ); 
				if( building->getUnitNameBasedOnResources( m_fishFingers, m_custard, m_timeEnergy, unitType ) )
					building->pushOrders( new BuildUnitOrders( building, unitType ) );
			}
		}
	}
	//-----------------------------------------------------------------
	TileCoords AIController::findLocationToBuildBuilding( const std::string& buildingName )
	{
		int radius = (int)m_unitsSelected[0]->getVisionRadius();
		WorldCoords unitLoc = m_unitsSelected[0]->getPosition();
		for( int y = (int)unitLoc.y - radius; y < (int)unitLoc.y + radius; ++y )
		{
			for( int x = (int)unitLoc.x - radius; x < (int)unitLoc.x + radius; ++x )
			{
				TileCoords tileCoords( x, y ); 
				if( m_world->getMap()->areTileCoordsInBounds( tileCoords ) )
				{
					WorldCoords worldCoords = m_world->getMap()->getWorldCoordsFromTileCoords( tileCoords );
					if( m_world->canBuildAtWorldCoords( this, BuildingBlueprint::getBlueprint( buildingName ), worldCoords ) )
					{
						return tileCoords;
					}
				}
			}
		}
		return TileCoords( -1, -1 );
	}
}