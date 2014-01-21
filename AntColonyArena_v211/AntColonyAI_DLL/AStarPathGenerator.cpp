#include "AStarPathGenerator.h"
#include "ArenaMap.h"

AStarPathGenerator::AStarPathGenerator()
{
	if( !m_dirInitialized )
	{
		m_cardinalDirections[0] = CD_NORTH;
		m_cardinalDirections[1] = CD_EAST;
		m_cardinalDirections[2] = CD_SOUTH;
		m_cardinalDirections[3] = CD_WEST;
		m_dirInitialized = true;
	}
}
//-------------------------------------------------------------------------------------------
AStarPathGenerator::AStarPathError AStarPathGenerator::getAStarPathToGoal
	( ArenaMap& arenaMap, EntityType entityType, const TileCoords& start, const TileCoords& goal, std::vector< OrderCode >& pathOut )
{
	AStarPathError errorStatus = SUCCESS;

	if( !arenaMap.isInMap( goal.x, goal.y ) )
		return FAILED_TO_FIND_PATH;

	TileCoords currentTileCoords = start;
	float currentF = (float)arenaMap.getHCost( currentTileCoords.x, currentTileCoords.y, goal.x, goal.y );
	float nextF = 0;
	float nextG = 0;
	TileCoords nextTileCoords;

	
	sm_minHeap.pushElement( currentF, currentTileCoords );

	AStarMinHeap::HeapElement currentElement;
	while( sm_minHeap.popSmallestElement( currentElement ) != AStarMinHeap::EMPTY )
	{			
		if( arenaMap.getLastAIPathClosedOn( goal.x, goal.y ) == sm_currentPathNumber )
			break;			
			
		arenaMap.setLastAIPathClosedOn( currentTileCoords.x, currentTileCoords.y, sm_currentPathNumber );
		currentTileCoords = currentElement.mapTile;

		for( int i = 0; i < 4; ++i )
		{
			nextTileCoords = currentTileCoords + m_cardinalDirections[i];
			if( arenaMap.isInMap( nextTileCoords.x, nextTileCoords.y ) )
			{				
				bool canMoveOnTile = arenaMap.canMoveOnTile( nextTileCoords.x, nextTileCoords.y ); 
				ArenaSquareType squaretype = (ArenaSquareType)arenaMap.getMapTileType( nextTileCoords.x, nextTileCoords.y ); 
				if( arenaMap.getLastAIPathClosedOn( nextTileCoords.x, nextTileCoords.y ) != sm_currentPathNumber && canMoveOnTile )
				{
					if( arenaMap.getLastAIPathOpenedOn( nextTileCoords.x, nextTileCoords.y ) != sm_currentPathNumber )
					{
						arenaMap.setCurrentGValueForTile( nextTileCoords.x, nextTileCoords.y, 
							arenaMap.getCurrentGValueForTile( currentTileCoords.x, currentTileCoords.y ) + arenaMap.getGCostToEnterTile( entityType, nextTileCoords.x, nextTileCoords.y ) );
						
						nextF = arenaMap.getCurrentGValueForTile( nextTileCoords.x, nextTileCoords.y ) + arenaMap.getHCost( nextTileCoords.x, nextTileCoords.y, goal.x, goal.y );
						arenaMap.setMoveOrderToGetToTileForCurrentPath( nextTileCoords.x, nextTileCoords.y, getOrderCodeForCardinalDirection( m_cardinalDirections[i] ) );
						sm_minHeap.pushElement( nextF, nextTileCoords );
						arenaMap.setLastAIPathOpenedOn( nextTileCoords.x, nextTileCoords.y, sm_currentPathNumber );
					}
					else
					{
						nextG = arenaMap.getCurrentGValueForTile( currentTileCoords.x, currentTileCoords.y ) + arenaMap.getGCostToEnterTile( entityType, nextTileCoords.x, nextTileCoords.y );
						if( nextG < arenaMap.getCurrentGValueForTile( nextTileCoords.x, nextTileCoords.y ) )
						{
							arenaMap.setCurrentGValueForTile( nextTileCoords.x, nextTileCoords.y, nextG );
							nextF = nextG + arenaMap.getHCost( nextTileCoords.x, nextTileCoords.y, goal.x, goal.y );
							arenaMap.setMoveOrderToGetToTileForCurrentPath( nextTileCoords.x, nextTileCoords.y, getOrderCodeForCardinalDirection( m_cardinalDirections[i] ) );
							sm_minHeap.updateElement( arenaMap.getCurrentFValue( nextTileCoords.x, nextTileCoords.y ), nextF, nextTileCoords );
							arenaMap.setCurrentFValue( nextTileCoords.x, nextTileCoords.y, nextF );
						}
					}
				}
			}
		}
	}
	
	pathOut.clear();
	if( arenaMap.getLastAIPathClosedOn( goal.x, goal.y ) == sm_currentPathNumber )
	{
		nextTileCoords = goal;		
		while( nextTileCoords != start )
		{
			OrderCode code = arenaMap.getMoveOrderToGetToTileForCurentPath( nextTileCoords.x, nextTileCoords.y );
			pathOut.push_back( code );
			nextTileCoords -= getTileCoordsDirFromOrderCode( code );
		}
		errorStatus = SUCCESS;
	}
	else
	{
		errorStatus = FAILED_TO_FIND_PATH;
	}

	++sm_currentPathNumber;
	sm_minHeap.resetHeap();
	return errorStatus;
}
//--------------------------------------------------------------------------------
OrderCode AStarPathGenerator::getOrderCodeForCardinalDirection( const TileCoords& dir )
{
	OrderCode order = ORDER_HOLD;
	if( dir == CD_NORTH )
		order = ORDER_MOVE_NORTH;
	else if( dir == CD_EAST )
		order = ORDER_MOVE_EAST;
	else if( dir == CD_SOUTH )
		order = ORDER_MOVE_SOUTH;
	else if( dir == CD_WEST )
		order = ORDER_MOVE_WEST;
	return order;
}
//--------------------------------------------------------------------------------
TileCoords AStarPathGenerator::getTileCoordsDirFromOrderCode( OrderCode code )
{
	switch( code )
	{
	case ORDER_MOVE_NORTH:
		return CD_NORTH;
	case ORDER_MOVE_EAST:
		return CD_EAST;
	case ORDER_MOVE_SOUTH:
		return CD_SOUTH;
	case ORDER_MOVE_WEST:
		return CD_WEST;
	default:
		return CD_NORTH;
	}
}
	
int AStarPathGenerator::sm_currentPathNumber = 0;
AStarMinHeap AStarPathGenerator::sm_minHeap;
TileCoords AStarPathGenerator::m_cardinalDirections[4];
bool AStarPathGenerator::m_dirInitialized = false;

const TileCoords AStarPathGenerator::CD_NORTH( 0, -1 );
const TileCoords AStarPathGenerator::CD_EAST( 1, 0 );
const TileCoords AStarPathGenerator::CD_SOUTH( 0, 1 );
const TileCoords AStarPathGenerator::CD_WEST( -1 , 0 );