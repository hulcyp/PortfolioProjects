#pragma once
#include <cassert>
#include <stdio.h>
#define _ASSERT( arg ) assert( arg );
#include "CommonInterface.hpp"
#include "AStarPathGenerator.h"
#include "RandItemPool.h"

class ArenaMap
{
public:

	struct MapTile
	{
		MapTile();
		char tileMaterial;
		float currentGCost;
		float gCostToEnterTile;
		float gCostToEnterTileForScout;
		float gCostToEnterTileForWorker;
		float gCostToEnterTileForSoldier;
		float gCostToEnterTileForQueen;
		int lastAIPathClosedOn;
		int lastAIPathOpenedOn;
		float currentF;
		mutable bool isClaimed;
		OrderCode moveOrderToGetToTileForCurrentPath;
		int lastTurnUpdated;
		int numAgentsKilledInTile;
		int enemyAgentsSeenInThisTile;
	};

	ArenaMap();

	void init();

	void updateMap( const ArenaSquares& currentViewOfArena, int currentTurn );
	void updateEnemySightingsHeatMap( const ObservedEntities& entities );

	char getMapTileType( int x, int y );
	char getMapTileType( int x, int y ) const;
	MapTile& getMapTile( int x, int y );
	const MapTile& getMapTile( int x, int y ) const;

	int getMapWidth() const;
	int getMapHeight() const;
	bool isInMap( int x, int y ) const;

	void setArenaInfo( const ArenaInfo& arenaInfo );
	const ArenaInfo& getArenaInfo() const;

	float getGCostToEnterTile( EntityType entityType, int x, int y ) const;
	float getCurrentGValueForTile( int x, int y ) const;
	void setCurrentGValueForTile( int x, int y, float g );
	int getLastAIPathClosedOn( int x, int y ) const;
	void setLastAIPathClosedOn( int x, int y, int lastAIPath );
	int getLastAIPathOpenedOn( int x, int y ) const;
	void setLastAIPathOpenedOn( int x, int y, int lastAIPath );
	float getCurrentFValue( int x, int y ) const;
	void setCurrentFValue( int x, int y, float f );

	int getHCost( int currentX, int currentY, int targetX, int targetY );
	bool canMoveOnTile( int x, int y );
	bool canMoveOnTile( int x, int y ) const;
	void setMoveOrderToGetToTileForCurrentPath( int x, int y, OrderCode order );
	OrderCode getMoveOrderToGetToTileForCurentPath( int x, int y ) const;
	
	void addAgentKilledToTile( int x, int y );

	bool getClosestNutrient( int startingRadius, const TileCoords& current, int& nutrientX, int& nutrientY ) const;
	
	void requestAStarPath( EntityType entityType, const TileCoords& start, TileCoords& goal, std::vector< OrderCode >& pathOut );

	float getGCostToEnterTileForEntityType( ArenaSquareType squareType, EntityType entityType ) const;

	void renderMapOverlay( unsigned int color, short inspectSquareX, short inspectSquareY ) const;

	bool hasNewMapInfo() const { return m_hasNewMapInfo; }

	bool shouldMoveSucceed( EntityType entityType, OrderCode orderCode, const TileCoords& currentLoc ) const;

	TileCoords getLocationForScoutToGoTo() const;
	TileCoords getRandomLocationInArea( const TileCoords& pos, int radius ) const;

	bool hasNoMoreResources() const { return m_hasNoMoreResources; }

private:
	bool searchHalfCircleUsingManhattenDistForNutrient( const TileCoords& pos, const TileCoords& dir, int radius, TileCoords& nutrientLoc ) const;
	bool searchInDirectionForNutrient( const TileCoords& pos, const TileCoords& dir, int radius, TileCoords& nutrientLoc ) const;
	bool checkTileForNutrient( const TileCoords& pos ) const;

	struct Nutrient
	{
		Nutrient( const TileCoords loc, bool claimed = false )
			:	loc( loc )
			,	claimed( claimed )
		{}
		TileCoords loc;
		bool claimed;
	};

	MapTile m_mapTiles[ MAX_ARENA_SQUARES ];
	ArenaInfo m_arenaInfo;
	AStarPathGenerator m_aStarPathGen;
	std::vector< Nutrient > m_visibleNutrients;
	bool m_hasNewMapInfo;
	short m_width;
	short m_height;
	int m_currentTurn;
	Monky::RandItemPool<TileCoords> m_tilesToScout;
	const int TOLERANCE_FOR_TILE_FRESHNESS;
	bool m_hasNoMoreResources;
	int m_maxAgentKillCountInSingleTile;
	int m_maxEnemySeenCountInSingleTile;
};