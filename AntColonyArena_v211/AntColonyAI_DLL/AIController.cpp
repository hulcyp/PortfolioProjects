#include "AIController.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "RandNumGen.h"
#include "MathFuncs.h"

AIController::Agent::Agent()
	:	entityID( -1 )
	,	specialStatus( -1 )
	,	currentTargetAgentID( -1 )
	,	currentEnemyTargetID( -1 )
	,	waitingForPath( false )
{}
//------------------------------------------------------------
AIController::AIController()
	:	m_playerID( 0 )
	,	MAX_NUM_SCOUTS( 5 )
	,	MIN_NUM_WORKERS( 8 )
	,	m_spawnedWorkerLastFrame( false )
	,	TURN_UPKEEP_TOLERANCE( 50 )
	,	MAX_NUM_A_STAR_PATHS_PER_TURN( 50 )
	,	QUEEN_RANGE_FACTOR( 4 )
	,	TURN_INTERVAL_QUEEN_MOVES_ON( 50 )
	,	NUM_WORKERS_IDLE_BEFORE_QUEEN_MOVES( 3 )
	,	m_currentQueenState( GATHER_RESOURCES )
	,	m_queen( nullptr )
	,	m_numWorkersWithFood( 0 )
{
	m_cardinalDirRanPool.addItem( ORDER_MOVE_NORTH );
	m_cardinalDirRanPool.addItem( ORDER_MOVE_WEST );
	m_cardinalDirRanPool.addItem( ORDER_MOVE_SOUTH );
	m_cardinalDirRanPool.addItem( ORDER_MOVE_EAST );
	InitializeCriticalSection( &m_agentsCS );
}
//------------------------------------------------------------
void AIController::initialize( int playerID, const ArenaInfo& arenaInfo )
{
	m_playerID = playerID;
	m_arenaInfo = arenaInfo;
}
//------------------------------------------------------------
void AIController::updateControllerState( int turnNumber, int currentNutrients )
{
	m_turnNumber = turnNumber;
	m_currentNutrients = currentNutrients;
	if( turnNumber > m_arenaInfo.numTurnsBeforeSuddenDeath )
		m_currentQueenState = ATTACK;
}
//------------------------------------------------------------
void AIController::update( ArenaMap& currentMap, const AgentReports& agentReports, const ObservedEntities& entities, Orders& tentativeOrders )
{
	EnterCriticalSection( &m_agentsCS );
	generateNewOrders( currentMap, agentReports, entities, tentativeOrders );	
	LeaveCriticalSection( &m_agentsCS );
}
//----------------------------------------------------------------------
void AIController::renderPathsForAgents() const
{
	EnterCriticalSection( &m_agentsCS );
	
	glColor4ub( 0, 255, 0, 255 );
	for( auto iter = m_agents.begin(); iter != m_agents.end(); ++iter )
	{
		std::map< int, Agent* >::const_iterator agentIter = iter;
		const Agent* agent = agentIter->second;
		//if( agent->entityType == ENTITY_TYPE_WORKER )
		{
			const std::vector< OrderCode >& path = agent->currentPath;
			TileCoords currentLoc =  iter->second->pos;
	
			for( std::vector< OrderCode >::const_reverse_iterator revIter = path.rbegin(); revIter != path.rend() ; ++revIter )
			{			
				OrderCode orderCode = *revIter;
				if( orderCode != ORDER_HOLD )
				{
					TileCoords dir = AStarPathGenerator::getTileCoordsDirFromOrderCode( orderCode );
					glBegin( GL_LINES );
					{
						glVertex2f( (float)currentLoc.x, (float)currentLoc.y );
						currentLoc += dir;
						glVertex2f( (float)currentLoc.x, (float)currentLoc.y );
					}
					glEnd();
				}
			}
		}
	}
	LeaveCriticalSection( &m_agentsCS );
}
//----------------------------------------------------------------------
int AIController::getUpkeepCostFromAgentType( EntityType entityType ) const
{
	int upkeep = 0;
	switch( entityType )
	{
	case ENTITY_TYPE_WORKER:
		upkeep = m_arenaInfo.upkeepNutrientsPerTurnForWorker;
		break;
	case ENTITY_TYPE_SOLDIER:
		upkeep = m_arenaInfo.upkeepNutrientsPerTurnForSoldier;
		break;
	case ENTITY_TYPE_SCOUT:
		upkeep = m_arenaInfo.upkeepNutrientsPerTurnForScout;
		break;
	case ENTITY_TYPE_QUEEN:
		upkeep = m_arenaInfo.upkeepNutrientsPerTurnForQueen;
		break;
	}
	return upkeep;
}
//----------------------------------------------------------------------
void AIController::handleAgentReports( ArenaMap& currentMap, const AgentReports& agentReports, const ObservedEntities& entities, Orders& tentativeOrders )
{
	EnterCriticalSection( &m_agentsCS );
	for( int i = 0; i < agentReports.m_numberAgentReports; ++i )
	{
		Agent*& currentAgent = m_agents[ agentReports.m_agentReports[i].m_entityID ];
		//Indicates this is a new entity
		if( currentAgent == nullptr )
		{
			createNewAgent( currentAgent, agentReports.m_agentReports[i] );
		}
		else
		{
			if( agentReports.m_agentReports[i].m_reportCode == REPORT_WAS_KILLED_IN_COMBAT || 
				agentReports.m_agentReports[i].m_reportCode == REPORT_STARVED_TO_DEATH ||
				agentReports.m_agentReports[i].m_reportCode == REPORT_SUICIDE_SUCCESSFUL )
			{
				handleAgentDied( currentAgent, currentMap, agentReports.m_agentReports[i] );
				m_currentQueenState = DEFEND;
			}
			else
			{
				if( agentReports.m_agentReports[i].m_agentType == ENTITY_TYPE_WORKER && agentReports.m_agentReports[i].m_reportCode == REPORT_TAKE_SUCCESSFUL )
					++m_numWorkersWithFood;

				currentAgent->pos.x = agentReports.m_agentReports[i].m_newPositionX;
				currentAgent->pos.y = agentReports.m_agentReports[i].m_newPositionY;
				currentAgent->specialStatus = agentReports.m_agentReports[i].m_specialStatus;
				if( agentReports.m_agentReports[i].m_reportCode == REPORT_MOVE_SUCCESSFUL )
				{
					if( !currentAgent->currentPath.empty() )
						currentAgent->currentPath.pop_back();
					if( currentAgent->currentPath.empty() )
						currentAgent->waitingForPath = false;
				}
				else if( agentReports.m_agentReports[i].m_reportCode == REPORT_HOLD_SUCCESSFUL )
				{
					if( currentAgent == m_queen && !m_queen->currentPath.empty() && m_queen->currentPath.back() == ORDER_HOLD )
						currentAgent->currentPath.pop_back();
					if( currentAgent->currentPath.empty() )
						currentAgent->waitingForPath = false;
				}
				else if( agentReports.m_agentReports[i].m_reportCode == REPORT_ERROR_BLOCKED_BY_ROCK )
				{
					currentAgent->currentPath.clear();
					m_agentsRequestingPaths.push( currentAgent->entityID );
				}
				else if( currentAgent->entityType == ENTITY_TYPE_QUEEN )
				{
					if( agentReports.m_agentReports[i].m_reportCode == REPORT_QUEEN_WAS_FED )
						currentMap.getMapTile( m_queen->pos.x, m_queen->pos.y ).isClaimed = false;	
					else if( agentReports.m_agentReports[i].m_reportCode == REPORT_CREATE_SUCCESSFUL && m_currentQueenState == DEFEND )
					{
						m_currentQueenState = GATHER_RESOURCES;
					}
					else if( agentReports.m_agentReports[i].m_reportCode == REPORT_QUEEN_WAS_ASSAULTED && m_queen->currentPath.empty() )
					{
						tentativeOrders.AddOrder( m_queen->entityID, ORDER_CREATE_SOLDIER, true );
					}
				}
			}
		}
	}
	LeaveCriticalSection( &m_agentsCS );
}
//----------------------------------------------------------------------
void AIController::processAgentPathOrders( ArenaMap& currentMap, Orders& tentativeOrders )
{
	EnterCriticalSection( &m_agentsCS );
	
	for( auto agentIter = m_agents.begin(); agentIter != m_agents.end(); ++agentIter )
	{
		Agent* currAgent = agentIter->second;
		if( !currAgent->currentPath.empty() )
		{
			if( currAgent->entityType == ENTITY_TYPE_WORKER && currAgent->specialStatus != ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
			{
				currentMap.getMapTile( currAgent->currentTargetLoc.x, currAgent->currentTargetLoc.y ).isClaimed = true;
			}
			tentativeOrders.AddOrder( currAgent->entityID, currAgent->currentPath.back(), true );
		}
	}

	LeaveCriticalSection( &m_agentsCS );
}
//----------------------------------------------------------------------
void AIController::generateNewOrders( ArenaMap& currentMap, const AgentReports& agentReports, const ObservedEntities& entities, Orders& tentativeOrders )
{	
	m_numWorkersThatCouldntFindFoodThisFrame = 0;
	for( auto iter = m_agents.begin(); iter != m_agents.end(); ++iter )
	{
		Agent* currentAgent = iter->second;	
		if( currentAgent->entityType == ENTITY_TYPE_WORKER )
		{
			handleWorkerOrders( currentAgent, currentMap, entities, tentativeOrders );
		}
		else if( currentAgent->entityType == ENTITY_TYPE_SOLDIER )
		{
			handleSoldierOrders( currentAgent, currentMap, entities, tentativeOrders );
		}
		else if( currentAgent->entityType == ENTITY_TYPE_QUEEN )
		{
			handleQueenOrders( currentAgent, currentMap, entities, tentativeOrders );			
		}
		else if( currentAgent->entityType == ENTITY_TYPE_SCOUT )
		{
			handleScoutOrders( currentAgent, currentMap, entities, tentativeOrders );	
		}
	}	
}
//----------------------------------------------------------------------
void AIController::processAStarPathRequests( ArenaMap& currentMap, const ObservedEntities& entities )
{
	int numPathsGen = 0;
	while( !m_agentsRequestingPaths.empty() && numPathsGen < MAX_NUM_A_STAR_PATHS_PER_TURN )
	{
		auto agentIter = m_agents.find( m_agentsRequestingPaths.front() );
		if( agentIter != m_agents.end() )
		{
			Agent* currentAgent = agentIter->second;
			if( currentAgent->currentTargetAgentID != -1 )
			{
				auto agentIter = m_agents.find( currentAgent->currentTargetAgentID );
				if( agentIter != m_agents.end() )
				{
					currentAgent->currentTargetLoc = agentIter->second->pos;
				}
			}
			else if( currentAgent->currentEnemyTargetID != -1 )
			{
				currentAgent->currentTargetLoc = getTileCoordsForEnemyAgent( currentAgent->currentEnemyTargetID, entities );
			}
			currentMap.requestAStarPath( (EntityType)currentAgent->entityType, currentAgent->pos, currentAgent->currentTargetLoc, currentAgent->currentPath );
			++numPathsGen;
			
			currentAgent->waitingForPath = false;
			m_agentsRequestingPaths.pop();
			
		}
		else
		{
			m_agentsRequestingPaths.pop();
		}
	}
}
//----------------------------------------------------------------------
bool AIController::validPath( ArenaMap& currentMap, const TileCoords& start, const std::vector< OrderCode >& path ) const
{
	bool validPath = true;
	TileCoords currentTile = start;
	for( auto revIter = path.rbegin(); revIter != path.rend(); ++revIter )
	{
		if( *revIter != ORDER_HOLD )
		{
			currentTile += AStarPathGenerator::getTileCoordsDirFromOrderCode( *revIter );
			if( !currentMap.canMoveOnTile( currentTile.x, currentTile.y ) )
				validPath = false;
		}
	}
	return validPath;
}
//----------------------------------------------------------------------
OrderCode AIController::getOrderToMoveTowardsTarget( ArenaMap& currentMap, const Vector2s& start, const Vector2s& goal ) const
{
	Vector2s deltaVec = goal - start;
	OrderCode order = ORDER_MOVE_NORTH;
	if( deltaVec.x > deltaVec.y )
	{
		int sign = Monky::MathFuncs<int>::getSign( deltaVec.x );
		if( sign > 0 )
			order = ORDER_MOVE_SOUTH;
		else
			order = ORDER_MOVE_NORTH;
	}
	else
	{
		int sign = Monky::MathFuncs<int>::getSign( deltaVec.y );
		if( sign > 0 )
			order = ORDER_MOVE_EAST;
		else
			order = ORDER_MOVE_WEST;
	}
	TileCoords dir = AStarPathGenerator::getTileCoordsDirFromOrderCode( order );
	TileCoords nextTile = start + dir;
	if( !currentMap.canMoveOnTile( nextTile.x, nextTile.y ) )
		order = AStarPathGenerator::getOrderCodeForCardinalDirection( dir.yx() );
	nextTile = start + dir;
	if( !currentMap.canMoveOnTile( nextTile.x, nextTile.y ) )
		order = AStarPathGenerator::getOrderCodeForCardinalDirection( -dir );
	nextTile = start + dir;
	if( !currentMap.canMoveOnTile( nextTile.x, nextTile.y ) )
		order = AStarPathGenerator::getOrderCodeForCardinalDirection( dir.yx() );
	
	return order;
}
//----------------------------------------------------------------------
void AIController::handleAgentDied( Agent*& agentThatDied, ArenaMap& currentMap, const AgentReport& agentReport )
{
	m_currentUpkeep -= getUpkeepCostFromAgentType( (EntityType)agentThatDied->entityType );
	if( agentReport.m_reportCode == REPORT_WAS_KILLED_IN_COMBAT )
		currentMap.addAgentKilledToTile( agentThatDied->pos.x, agentThatDied->pos.y );
	switch( agentThatDied->entityType ) 
	{
	case ENTITY_TYPE_WORKER:
		--m_currentNumWorkers;
		if( agentThatDied->specialStatus == ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
			--m_numWorkersWithFood;
		currentMap.getMapTile( agentThatDied->currentTargetLoc.x, agentThatDied->currentTargetLoc.y ).isClaimed = false;
		break;
	case ENTITY_TYPE_SCOUT:
		--m_currentNumScouts;
		break;
	case ENTITY_TYPE_SOLDIER:
		--m_currentNumSoldiers;
		break;
	}
	delete agentThatDied;
	agentThatDied = nullptr;
	m_agents.erase( agentReport.m_entityID );
	--m_currentSupply;		
}
//----------------------------------------------------------------------
void AIController::createNewAgent( Agent*& newAgent, const AgentReport& agentReport )
{
	newAgent = new Agent();
	newAgent->entityID = agentReport.m_entityID;
	newAgent->entityType = agentReport.m_agentType;
	newAgent->pos.x = agentReport.m_newPositionX;
	newAgent->pos.y = agentReport.m_newPositionY;
	newAgent->currentTargetLoc = newAgent->pos;
	m_currentUpkeep += getUpkeepCostFromAgentType( (EntityType)newAgent->entityType );
	++m_currentSupply;
	switch( newAgent->entityType ) 
	{
	case ENTITY_TYPE_WORKER:
		++m_currentNumWorkers;
		break;
	case ENTITY_TYPE_SCOUT:
		++m_currentNumScouts;
		break;
	case ENTITY_TYPE_SOLDIER:
		++m_currentNumSoldiers;
		break;
	case ENTITY_TYPE_QUEEN:
		m_queen = newAgent;
		break;
	}	
}
//----------------------------------------------------------------------
void AIController::handleWorkerOrders( Agent* worker, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders )
{
	if( m_currentQueenState == ATTACK && currentMap.hasNoMoreResources() && worker->specialStatus != ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
	{
		tentativeOrders.AddOrder( worker->entityID, ORDER_SUICIDE );
	}
	else
	{
		if( worker->currentPath.empty() && !worker->waitingForPath )
		{
			if( worker->pos == m_queen->pos && worker->specialStatus == ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
			{
				tentativeOrders.AddOrder( worker->entityID, ORDER_DROP_FOOD, true );
				--m_numWorkersWithFood;
				worker->currentTargetAgentID = -1;
			}
			else if( worker->pos == worker->currentTargetLoc && currentMap.getMapTileType( worker->pos.x, worker->pos.y ) == ARENA_SQUARE_TYPE_FOOD )
			{
				tentativeOrders.AddOrder( worker->entityID, ORDER_TAKE_FOOD, true );
				m_agentsRequestingPaths.push( worker->entityID );
				worker->currentTargetLoc = m_queen->pos;
				worker->currentTargetAgentID = m_queen->entityID;
				worker->waitingForPath = true;
				currentMap.getMapTile( worker->pos.x, worker->pos.y ).isClaimed = false;
			}	
			else if( worker->specialStatus != ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
			{
				int x, y;
				if( currentMap.getClosestNutrient( m_arenaInfo.visibilityRangeQueen*QUEEN_RANGE_FACTOR, m_queen->pos, x, y ) )
				{
					m_agentsRequestingPaths.push( worker->entityID );
					worker->currentTargetLoc = TileCoords( x, y );
					worker->currentTargetAgentID = -1;
					worker->waitingForPath = true;
					currentMap.getMapTile( x, y ).isClaimed = true;
				}
				else
				{
					++m_numWorkersThatCouldntFindFoodThisFrame;
					if( m_numWorkersThatCouldntFindFoodThisFrame >= NUM_WORKERS_IDLE_BEFORE_QUEEN_MOVES )
					{
						worker->currentTargetAgentID = m_queen->entityID;
					}
					else
					{
						worker->currentTargetAgentID = -1;
						worker->currentTargetLoc = currentMap.getRandomLocationInArea( worker->pos, m_arenaInfo.visibilityRangeQueen*QUEEN_RANGE_FACTOR );
					}
					m_agentsRequestingPaths.push( worker->entityID );
					worker->waitingForPath = true;
				}
			}
			else
			{
				worker->currentTargetAgentID =  m_queen->entityID;
				m_agentsRequestingPaths.push( worker->entityID );
				worker->waitingForPath = true;
			}
		}
		else if( currentMap.getMapTileType( worker->currentTargetLoc.x, worker->currentTargetLoc.y ) != ARENA_SQUARE_TYPE_FOOD && worker->specialStatus != ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
		{
			int x, y;
			if( currentMap.getClosestNutrient( m_arenaInfo.visibilityRangeQueen*QUEEN_RANGE_FACTOR, m_queen->pos, x, y ) )
			{
				m_agentsRequestingPaths.push( worker->entityID );
				worker->currentTargetLoc = TileCoords( x, y );
				worker->currentTargetAgentID = -1;
				worker->waitingForPath = true;
				currentMap.getMapTile( x, y ).isClaimed = true;
			}
		}
		else if( currentMap.hasNewMapInfo() && !worker->waitingForPath && !validPath( currentMap, worker->pos, worker->currentPath ) )
		{
			m_agentsRequestingPaths.push( worker->entityID );
			worker->waitingForPath = true;
		}
	}
}
//----------------------------------------------------------------------
void AIController::handleSoldierOrders( Agent* soldier, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders )
{
	if( soldier->currentPath.empty() )
	{
		soldier->currentTargetLoc = currentMap.getLocationForScoutToGoTo();
		switch( m_currentQueenState )
		{
		case GATHER_RESOURCES:
		case DEFEND:
		case ATTACK:
			if( !getEntityIDForClosestEnemyQueen( soldier->pos, entities, soldier->currentEnemyTargetID ) )
				soldier->currentTargetLoc = currentMap.getLocationForScoutToGoTo();
			break;
		}
		m_agentsRequestingPaths.push( soldier->entityID );
	}
	else if( m_currentQueenState == ATTACK && soldier->currentEnemyTargetID == -1 )
	{
		if( !getEntityIDForClosestEnemyQueen( soldier->pos, entities, soldier->currentEnemyTargetID ) )
		{
			if( soldier->currentTargetLoc == soldier->pos )
			{
				soldier->currentTargetLoc = currentMap.getLocationForScoutToGoTo();
				m_agentsRequestingPaths.push( soldier->entityID );
			}
		}
		else
			m_agentsRequestingPaths.push( soldier->entityID );
	}
	else
	{
		if( currentMap.hasNewMapInfo()  && !validPath( currentMap, soldier->pos, soldier->currentPath ) )
		{
			if( soldier->pos == soldier->currentTargetLoc || currentMap.getMapTileType( soldier->currentTargetLoc.x, soldier->currentTargetLoc.y ) == ARENA_SQUARE_TYPE_STONE )
				soldier->currentTargetLoc = currentMap.getLocationForScoutToGoTo();
			m_agentsRequestingPaths.push( soldier->entityID );
		}
	}
}
//----------------------------------------------------------------------
void AIController::handleQueenOrders( Agent* queen, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders )
{
	if( ( m_turnNumber > 0 && m_turnNumber % TURN_INTERVAL_QUEEN_MOVES_ON == 0 ) || m_numWorkersThatCouldntFindFoodThisFrame >= NUM_WORKERS_IDLE_BEFORE_QUEEN_MOVES || m_currentNumWorkers <= 0 )
	{
		if( m_currentNumWorkers <= 0 && m_currentUpkeep*TURN_UPKEEP_TOLERANCE + m_arenaInfo.nutrientCostToBirthWorker )
			tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_WORKER );
		else if( queen->currentPath.empty() && !queen->waitingForPath )
		{
			queen->currentTargetLoc = getLocationForQueenToGoTo( currentMap );
			m_agentsRequestingPaths.push( queen->entityID );
			queen->waitingForPath = true;
		}
	}
	else 
	{
		switch( m_currentQueenState )
		{
		case GATHER_RESOURCES:
			if( m_currentNutrients > m_currentUpkeep*TURN_UPKEEP_TOLERANCE + m_arenaInfo.nutrientCostToBirthWorker )//&& !m_spawnedWorkerLastFrame )
			{
				tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_WORKER );
				m_spawnedWorkerLastFrame = true;
			}
			else if( m_currentNutrients > m_currentUpkeep*TURN_UPKEEP_TOLERANCE + m_arenaInfo.nutrientCostToBirthScout )
			{
				if( m_currentNumScouts < MAX_NUM_SCOUTS )
					tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_SCOUT );
				m_spawnedWorkerLastFrame = false;	
			}
			break;
		case DEFEND:
			if( m_currentNutrients > m_currentUpkeep*TURN_UPKEEP_TOLERANCE + m_arenaInfo.nutrientCostToBirthSoldier && m_currentNumWorkers >= MIN_NUM_WORKERS )
			{
				tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_SOLDIER, true );
				m_spawnedWorkerLastFrame = false;	
			}
			else if( m_currentNumScouts < MAX_NUM_SCOUTS )
				tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_SCOUT );
			else if( m_currentNutrients > m_currentUpkeep*TURN_UPKEEP_TOLERANCE + m_arenaInfo.nutrientCostToBirthWorker && !m_spawnedWorkerLastFrame && !currentMap.hasNoMoreResources() )
			{
				tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_WORKER );
				m_spawnedWorkerLastFrame = true;
			}
			m_spawnedWorkerLastFrame = false;
			break;
		case ATTACK:
			if( m_currentNutrients > m_currentUpkeep*TURN_UPKEEP_TOLERANCE + m_arenaInfo.nutrientCostToBirthSoldier && ( currentMap.hasNoMoreResources() || m_currentNumWorkers >= MIN_NUM_WORKERS ) )
			{
				tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_SOLDIER );
				m_spawnedWorkerLastFrame = false;	
			}
			else if( m_currentNutrients > m_currentUpkeep*TURN_UPKEEP_TOLERANCE + m_arenaInfo.nutrientCostToBirthWorker && !m_spawnedWorkerLastFrame && !currentMap.hasNoMoreResources() )
			{
				tentativeOrders.AddOrder( queen->entityID, ORDER_CREATE_WORKER );
				m_spawnedWorkerLastFrame = true;
			}
			else
				m_spawnedWorkerLastFrame = false;
			break;		
		}
	}
}
//----------------------------------------------------------------------
void AIController::handleScoutOrders( Agent* scout, ArenaMap& currentMap, const ObservedEntities& entities, Orders& tentativeOrders )
{
	if( scout->currentPath.empty() )
	{
		scout->currentTargetLoc = currentMap.getLocationForScoutToGoTo();
		m_agentsRequestingPaths.push( scout->entityID );
	}
	else
	{
		if( currentMap.hasNewMapInfo()  && !validPath( currentMap, scout->pos, scout->currentPath ) )
		{
			if( scout->pos == scout->currentTargetLoc || currentMap.getMapTileType( scout->currentTargetLoc.x, scout->currentTargetLoc.y ) == ARENA_SQUARE_TYPE_STONE )
				scout->currentTargetLoc = currentMap.getLocationForScoutToGoTo();
			m_agentsRequestingPaths.push( scout->entityID );
		}
	}
}
//----------------------------------------------------------------------
TileCoords AIController::getLocationForQueenToGoTo( const ArenaMap& currentMap )
{
	static OrderCode orderCodeToStartWith = ORDER_MOVE_NORTH;
	TileCoords newQueenLoc = m_queen->pos;
	if( m_currentNumWorkers <= 0 )
	{
		OrderCode dirCode = orderCodeToStartWith;
		float lowestGCost = 100.0f;
		for( int i = 0; i < 4; ++i )
		{
			dirCode = (OrderCode)Monky::MathFuncs<int>::wrap( orderCodeToStartWith + i, ORDER_MOVE_EAST, ORDER_MOVE_SOUTH ); 
			TileCoords newDir = AStarPathGenerator::getTileCoordsDirFromOrderCode( dirCode );
			TileCoords tempNewQueenLoc;
			newDir = newDir * (float)m_arenaInfo.visibilityRangeQueen;
			tempNewQueenLoc = m_queen->pos + newDir;
			float gCostToEnterTile = currentMap.getGCostToEnterTile( ENTITY_TYPE_QUEEN, tempNewQueenLoc.x, tempNewQueenLoc.y );
			if( currentMap.isInMap( tempNewQueenLoc.x, tempNewQueenLoc.y ) && currentMap.canMoveOnTile( tempNewQueenLoc.x, tempNewQueenLoc.y ) && gCostToEnterTile < lowestGCost )
			{
				newQueenLoc = tempNewQueenLoc;
				lowestGCost = gCostToEnterTile;
			}
		}
	
		int x, y;
		if( currentMap.getClosestNutrient( m_arenaInfo.visibilityRangeQueen * QUEEN_RANGE_FACTOR, newQueenLoc, x, y ) )
			newQueenLoc = TileCoords( x, y );

	
		orderCodeToStartWith = dirCode;
	}
	else
	{
		int x, y;
		if( currentMap.getClosestNutrient( m_arenaInfo.visibilityRangeQueen * QUEEN_RANGE_FACTOR, m_queen->pos, x, y ) )
			newQueenLoc = TileCoords( x, y );
	}

	if( newQueenLoc == m_queen->pos )
		newQueenLoc = currentMap.getRandomLocationInArea( m_queen->pos, (int)( m_arenaInfo.width*0.1 ) );
	
	return newQueenLoc;
}
//----------------------------------------------------------------------
bool AIController::getEntityIDForClosestEnemyQueen( const TileCoords& pos, const ObservedEntities& entities, int& closestQueenID ) const
{
	int visibleQueenIndices[8] = { -1, -1, -1, -1, -1, -1 };
	int queenIndex = 0;
	for( int i = 0; i < entities.m_numberOfObservedEntities; ++i )
	{
		if( entities.m_entities[i].m_agentType == ENTITY_TYPE_QUEEN && queenIndex < 8 )
			visibleQueenIndices[ queenIndex++ ] = i;
	}
	TileCoords closestQueenTC(  -1, -1 );
	float closestDeltaSqrd = (float)m_arenaInfo.width;
	int closestQueenIndex = -1;
	for( int q = 0; q < queenIndex; ++q )
	{
		if( visibleQueenIndices[q] != -1 )
		{
			TileCoords nextQueenLoc = TileCoords( entities.m_entities[ visibleQueenIndices[q] ].m_positionX, entities.m_entities[ visibleQueenIndices[q] ].m_positionY );
			float deltaSqrd = ( nextQueenLoc - pos ).lengthSqrd();
			if( deltaSqrd < closestDeltaSqrd )
			{
				closestQueenTC = nextQueenLoc;
				closestDeltaSqrd = deltaSqrd;
				closestQueenIndex = visibleQueenIndices[q];
			}
		}
	}
	if( closestQueenIndex != -1 )
		closestQueenID = entities.m_entities[ closestQueenIndex ].m_entityID;
	else
		closestQueenID = -1;
	return closestQueenIndex != -1;
}
//----------------------------------------------------------------------
TileCoords AIController::getTileCoordsForEnemyAgent( int agentID, const ObservedEntities& entities ) const
{
	TileCoords agentLoc( -1, -1 );
	for( int i = 0; i < entities.m_numberOfObservedEntities; ++i )
	{
		if( entities.m_entities[i].m_entityID == agentID )
		{
			agentLoc = TileCoords( entities.m_entities[i].m_positionX, entities.m_entities[i].m_positionY );
			break;
		}
	}
	return agentLoc;
}