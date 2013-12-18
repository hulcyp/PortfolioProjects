#pragma once
#include <vector>
#include <CommonEngineIncludes.h>
#include <EventSystemInterface.h>
#include "Map.h"
#include "AStarPathGenerator.h"
#include <deque>


namespace pdh
{
	class Unit;
	class Building;
	class Controller;
	class HumanController;
	class Entity;
	class Camera;
	class UnitOrders;
	class BuildingBlueprint;
	class UnitBlueprint;
	class AIController;

	class RTSWorld
	{
	public:

		RTSWorld( int numAI, const std::string& maptype, int mapWidth, int mapHeight, Renderer* renderer );
		~RTSWorld();
		
		Unit* spawnUnit( const std::string& unitName, const vec2f& pos, Controller* owner );
		Unit* spawnUnit( const UnitBlueprint* unitBP, const vec2f& pos, Controller* owner );
		Building* spawnBuilding( const std::string& buildingName, const vec2f& pos, Controller* owner );
		Building* spawnBuilding( const BuildingBlueprint* buildingPB, const vec2f& pos, Controller* owner );

		void addEntity( Entity* entity );
		void killEntity( Entity* entityToKill );

		WorldCoords getNearestWorldCoordsToBuildBulidingOnGrid( const WorldCoords& worldCoords, float width, float height ) const;
		
		Entity* getClosestEnemyEntityInRadius( const Entity* entityLooking, float radius ) const;

		void updateSimulation( float deltaTime );
		void updateDisplay();

		Map* getMap() const { return m_map; }
		int getScreenWidth() const;
		int getScreenHeight() const;

		WorldCoords getWorldCoordsFromScreenCoords( const vec2i& screenCoords ) const;
		Entity* getSelectedEntityFromWorldCoords( const Controller* requestor, const WorldCoords worldCoords ) const;
		Map::MapTile& getSelectedMapTileFromWorldCoord( Controller* requestor, const WorldCoords worldCoords ) const;
		Map::MapTile& getSelectedMapTileFromTileCoord( Controller* requestor, const TileCoords& tileCoords ) const;

		Entity* getSelectedEntityFromAABB2( const Controller* requestor, const aabb2f& selection ) const;
		
		void getSelectedEntitesFromAABB2( const Controller* requestor, const aabb2f& selection, std::vector< Entity* >& entitiesSelection ) const;
		void getAllEntitiesOfTypeOnScreen( const Entity* entity, std::vector< Entity* >& entitiesSelected ) const;
		Building* getBuildingToReturnResources( Unit* unit ) const;
				
		TileCoords getTileCoordsForCollectingResource( const Unit* unit, const Map::MapTile& mapTile ) const;
		TileCoords getTileCoordsForCollectingResource( const Unit* unit, const TileCoords& start, Map::ResourceType type ) const;

		bool canEntityMoveOnTile( const Unit* unit, const TileCoords& tileCoords ) const;
		bool canBuildAtWorldCoords( const Controller* requestor, const BuildingBlueprint* buildingBP, const WorldCoords& worldCoords ) const;

		void requestPath( UnitOrders* orders, const Unit* unit, std::vector< TileCoords >& pathOut );
		void unregisterForPath( UnitOrders* orders );

		void getVisibleEntitesInRadius( const Entity* entity, const vec2f& pos, float radius, std::vector< Entity* >& entities );

		static const int MAX_NUM_PLAYERS;
		
	protected:
		Map* createMap( const std::string& mapType );
		void spawnInitialEntitiesForController( Controller* controller, const std::string& peonType, const std::string& buildingType );

		void generateAStarPaths();

		struct AStarPathRequestor
		{
			AStarPathRequestor( UnitOrders* orders, const Unit* unit, std::vector< TileCoords >& pathOut ); 
			UnitOrders* orders; 
			const Unit* unit;
			std::vector< TileCoords >& pathOut;

			AStarPathRequestor& operator=( const AStarPathRequestor& rhs );
		};

		void killPendingEntitiesReadyToDie();

		void loadTeamColors( const char* filePath );


	private:
		std::vector< Entity* > m_entities;
		Map* m_map;
		int m_mapWidth;
		int m_mapHeight;
		HumanController* m_humanController;
		std::vector< AIController* > m_aiControllers;
		Renderer* m_renderer;
		Camera* m_hudCamera;
		bool m_debugMode;
		int m_currentNumPlayers;

		int m_playerToRenderPOVFrom;

		std::vector< Entity* > m_pendingUnitsToDie;

		AStarPathGenerator m_aStarPathGen;
		
		std::deque< AStarPathRequestor > m_entitesRequestingPath;
				
		TileCoords m_cardinalDirections[4];

		std::vector< Color4f > m_playerColors;

		const size_t NUM_PATHS_GENERATED_PER_FRAME;
	};
}