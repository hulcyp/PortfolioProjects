#pragma once
#include <vector>
#include "AStarMinHeap.h"
#include <CommonEngineIncludes.h>

namespace pdh
{
	typedef vec2i TileCoords;

	class Unit;
	class RTSWorld;

	class AStarPathGenerator
	{
	public:

		enum AStarPathError
		{
			SUCCESS,
			FAILED_TO_FIND_PATH,
			PATH_LIMIT_REACHED,
		};

		AStarPathGenerator( RTSWorld* world );

		AStarPathError getAStarPathToGoal( const Unit* unit, const TileCoords& start, const TileCoords& goal, std::vector< TileCoords >& pathOut );

		
	private:
		RTSWorld* m_world;
		static int sm_currentPathNumber;
		static AStarMinHeap sm_minHeap;
		static TileCoords m_cardinalDirections[4];
		static bool m_dirInitialized;
	};
}