#pragma once
#include <vector>
#include "AStarMinHeap.h"
#include "Vector2s.h"
#include "CommonInterface.hpp"

typedef Vector2s TileCoords;

class ArenaMap;
	
class AStarPathGenerator
{
public:

	static const TileCoords CD_NORTH;
	static const TileCoords CD_EAST;
	static const TileCoords CD_SOUTH;
	static const TileCoords CD_WEST;

	enum AStarPathError
	{
		SUCCESS,
		FAILED_TO_FIND_PATH,
		PATH_LIMIT_REACHED,
	};

	AStarPathGenerator();

	AStarPathError getAStarPathToGoal( ArenaMap& arenaMap, EntityType entityType, const TileCoords& start, const TileCoords& goal, std::vector< OrderCode >& pathOut );
	
	static OrderCode getOrderCodeForCardinalDirection( const TileCoords& dir );
	static TileCoords getTileCoordsDirFromOrderCode( OrderCode code );

		
private:
	static int sm_currentPathNumber;
	static AStarMinHeap sm_minHeap;
	static TileCoords m_cardinalDirections[4];
	static bool m_dirInitialized;
};