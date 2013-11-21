#pragma once
#include <vector>
#include <CommonEngineIncludes.h>
#include "Map.h"
#include <EventSystemInterface.h>

namespace pdh
{
	class Unit;
	class Building;
	class RTSWorld;
	class BuildingBlueprint;
	class Entity;
	class Map;

	class Controller
	{
	public:

		enum ResourceRequestError
		{
			SUCCESS,
			NOT_ENOUGH_FISH_FINGERS,
			NOT_ENOUGH_CUSTARD,
			NOT_ENOUGH_TIME_ENERGY
		};

		Controller( RTSWorld* world, int playerNum, const Color4f& teamColor = color::WHITE );
		virtual ~Controller();

		void renderViewOfWorld();

		virtual void issueAttackOrdersToLocation( const vec2f& targLoc );
		virtual void issueAttackOrdersOnTargetEntity( Entity* entity );
		virtual void issueMoveOrders( const vec2f& loc );
		virtual void issueBuildOrders( const std::string& blueprint, const vec2f& loc );
		virtual void issueBuildOrders( Building* building );
		virtual void issueHarvestOrders( Map::MapTile& mapTile );
		virtual void issueHoldOrders();
		virtual void issueStopOrder();
		virtual void issueBuildUnitOrder( const std::string& unitType );

		void entityKilled( NamedProperties& params );

		RTSWorld* getWorld() { return m_world; }
		int getPlayerNumber() { return m_playerNumber; }

		void giveResource( Map::ResourceType type, int amount );
		ResourceRequestError requestResource( int fishFingers, int custard, int timeEnergy );
		void giveResourceEvt( NamedProperties& params );

		void addToMaxSupplyCount( int amountToAdd );
		void removeFromMaxSupplyCount( int amountToRemove );
		void addToCurrentSupplyCount( int amountToAdd );
		void removeFromCurrentSupplyCount( int amountToRemove );

		bool hasEnoughSupply( const std::string& blueprintName );

		const Color4f& getTeamColor() const { return m_teamColor; }

		void notifyOfVisibleTiles( const std::vector< TileCoords >& tiles );
		void notifyOfVisibleEntites( const std::vector< Entity* >& entities );
		void giveEntityToControl( Entity* entity );

		const std::vector< Entity* >& getListOfVisibleEntities() const;
		const std::vector< Entity* >& getListofEntitesOwned() const;
		const Map* getCurrentViewOfMap() const { return m_currentMapView; }
		Map* getCurrentViewOfMap() { return m_currentMapView; }
		

	protected:
		void updateMapView();
		void updateEntitiesToRender();

		std::vector< Unit* > m_unitsSelected;
		std::vector< Building* > m_buildingsSelected;
		int m_playerNumber;
		RTSWorld* m_world;

		Color4f m_teamColor;
		
		int m_fishFingers;
		int m_custard;
		int m_timeEnergy;
		int m_currentSupplyCount;
		int m_maxSupplyCount;
		const int MAX_SUPPLY_COUNT;

		int m_currentFrame;

		Map* m_currentMapView;
		std::vector< Entity* > m_visibleEntities;
		std::vector< Entity* > m_entitiesOwned;

		std::vector< Entity* > m_entitesToRender;
	};
}