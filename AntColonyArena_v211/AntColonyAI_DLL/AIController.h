#pragma once
#include <cassert>
#include <stdio.h>
#define _ASSERT( arg ) assert( arg );
#include "ArenaMap.h"
#include "CommonInterface.hpp"
#include "RandItemPool.h"
#include <map>
#include <queue>
#include "Vector2s.h"

typedef Vector2s TileCoords;

#define MAX_NUM_SQUADS 50

class AIController
{
public:
	
	struct Agent
	{
		Agent();
		int entityID;
		TileCoords pos;
		char entityType;
		char specialStatus;
		std::vector< OrderCode > currentPath;
		Vector2s currentTargetLoc;
		int currentTargetAgentID;
		bool waitingForPath;
		int currentEnemyTargetID;
	};

	enum State
	{
		GATHER_RESOURCES,
		DEFEND,
		ATTACK
	};


	AIController();
	void initialize( int playerID, const ArenaInfo& arenaInfo );
	void updateControllerState( int turnNumber, int currentNutrients );
	void update( ArenaMap& currentMap, const AgentReports& agentReports, const ObservedEntities& entities, Orders& tentativeOrders );
	
	void renderPathsForAgents() const;

	int getUpkeepCostFromAgentType( EntityType entityType ) const;

	void handleAgentReports( ArenaMap& currentMap, const AgentReports& agentReports, const ObservedEntities& entities, Orders& tentativeOrders );
	void processAgentPathOrders( ArenaMap& currentMap, Orders& tentativeOrders );
	void generateNewOrders( ArenaMap& currentMap, const AgentReports& agentReports, const ObservedEntities& entities, Orders& tentativeOrders );
	void processAStarPathRequests( ArenaMap& currentMap, const ObservedEntities& entities );

protected:
	bool validPath( ArenaMap& currentMap, const TileCoords& start, const std::vector< OrderCode >& path ) const;

	OrderCode getOrderToMoveTowardsTarget( ArenaMap& currentMap, const Vector2s& start, const Vector2s& goal ) const;

	void createNewAgent( Agent*& newAgent, const AgentReport& agentReport );
	void handleAgentDied( Agent*& agentThatDied, ArenaMap& currentMap, const AgentReport& agentReport );
	void handleWorkerOrders( Agent* worker, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders );
	void handleSoldierOrders( Agent* soldier, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders );
	void handleQueenOrders( Agent* queen, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders );
	void handleScoutOrders( Agent* scout, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders );

	TileCoords getLocationForQueenToGoTo( const ArenaMap& currentMap );

	bool getEntityIDForClosestEnemyQueen( const TileCoords& pos, const ObservedEntities& entities, int& closestQueenID ) const;
	TileCoords getTileCoordsForEnemyAgent( int agentID, const ObservedEntities& entities ) const;

private:
	int m_playerID;
	int m_turnNumber;
	int m_currentNutrients;
	int m_currentUpkeep;
	int m_currentSupply;
	int m_currentNumWorkers;
	int m_currentNumScouts;
	int m_currentNumSoldiers;
	Agent* m_queen;
	ArenaInfo m_arenaInfo;
	Monky::RandItemPool< OrderCode > m_cardinalDirRanPool;
	
	std::map< int, Agent* > m_agents;
	std::queue< int > m_agentsRequestingPaths;

	mutable CRITICAL_SECTION m_agentsCS;

	const int MAX_NUM_SCOUTS;
	const int MIN_NUM_WORKERS;
	const int TURN_UPKEEP_TOLERANCE;
	const int MAX_NUM_A_STAR_PATHS_PER_TURN;
	const int QUEEN_RANGE_FACTOR;
	const int TURN_INTERVAL_QUEEN_MOVES_ON;
	const int NUM_WORKERS_IDLE_BEFORE_QUEEN_MOVES;
	
	bool m_spawnedWorkerLastFrame;
	int m_numWorkersThatCouldntFindFoodThisFrame;
	int m_numWorkersWithFood;

	State m_currentQueenState;
};

