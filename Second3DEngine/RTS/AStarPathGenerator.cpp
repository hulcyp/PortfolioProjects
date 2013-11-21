#include "AStarPathGenerator.h"
#include "RTSWorld.h"
#include "Map.h"

namespace pdh
{
	AStarPathGenerator::AStarPathGenerator( RTSWorld* world )
		:	m_world( world )
	{
		if( !m_dirInitialized )
		{
			m_cardinalDirections[0] = Map::CD_NORTH;
			m_cardinalDirections[1] = Map::CD_EAST;
			m_cardinalDirections[2] = Map::CD_SOUTH;
			m_cardinalDirections[3] = Map::CD_WEST;
			m_dirInitialized = true;
		}
	}
	//-----------------------------------------------------------------
	AStarPathGenerator::AStarPathError AStarPathGenerator::getAStarPathToGoal
		( const Unit* unit, const TileCoords& start, const TileCoords& goal, std::vector< TileCoords >& pathOut )
	{
		AStarPathError errorStatus = SUCCESS;

		Map* map = m_world->getMap();
		Map::MapTile* currentMapTile = &map->getMapTile( start );
		Map::MapTile* nextMapTile = nullptr;
		currentMapTile->m_G = 0;
		int currentF = currentMapTile->getH( goal );
		int nextF = 0;
		int nextG = 0;
		currentMapTile->m_F = currentF;
		TileCoords nextTileCoords;

		Map::MapTile* goalTile = &map->getMapTile( goal );

		sm_minHeap.pushElement( currentF, currentMapTile );

		AStarMinHeap::HeapElement currentElement;
		while( sm_minHeap.popSmallestElement( currentElement ) != AStarMinHeap::EMPTY )
		{			
			if( goalTile->m_lastAIPathNumberClosedOn == sm_currentPathNumber )
				break;			
			
			currentElement.mapTile->m_lastAIPathNumberClosedOn = sm_currentPathNumber;
			currentMapTile = currentElement.mapTile;

			for( int i = 0; i < 4; ++i )
			{
				nextTileCoords = currentMapTile->getTilecoords() + m_cardinalDirections[i];
				if( nextTileCoords.x > 0 && nextTileCoords.y > 0 )
				{
					nextMapTile = &map->getMapTile( nextTileCoords );
				
					if( nextMapTile->m_lastAIPathNumberClosedOn != sm_currentPathNumber &&
						m_world->canEntityMoveOnTile( unit, nextMapTile->getTilecoords() ) )
					{
						if( nextMapTile->m_lastAIPathNumberOpenedOn != sm_currentPathNumber )
						{
							nextMapTile->m_G = currentMapTile->m_G + nextMapTile->m_GCost;
							nextF = nextMapTile->m_G + nextMapTile->getH( goal );
							nextMapTile->m_directionToGetToTileForCurrentPath = m_cardinalDirections[i];
							sm_minHeap.pushElement( nextF, nextMapTile );
							nextMapTile->m_lastAIPathNumberOpenedOn = sm_currentPathNumber;
						}
						else
						{
							nextG = currentMapTile->m_G + nextMapTile->m_GCost;
							if( nextG < nextMapTile->m_G )
							{
								nextMapTile->m_G = nextG;
								nextF = nextG + nextMapTile->getH( goal );
								nextMapTile->m_directionToGetToTileForCurrentPath = m_cardinalDirections[i];
								sm_minHeap.updateElement( nextMapTile->m_F, nextF, nextMapTile );
								nextMapTile->m_F = nextF;
							}
						}
					}
				}
			}
		}
		
		if( goalTile->m_lastAIPathNumberClosedOn == sm_currentPathNumber )
		{
			nextTileCoords = goalTile->m_tileCoords;
			while( nextTileCoords != start )
			{
				pathOut.push_back( nextTileCoords );
				nextTileCoords -= map->getMapTile( nextTileCoords ).m_directionToGetToTileForCurrentPath;
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
	
	int AStarPathGenerator::sm_currentPathNumber = 0;
	AStarMinHeap AStarPathGenerator::sm_minHeap;
	TileCoords AStarPathGenerator::m_cardinalDirections[4];
	bool AStarPathGenerator::m_dirInitialized = false;

}