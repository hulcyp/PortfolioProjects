#include "RTSDebuggingStats.h"

namespace pdh
{	
	int RTSDebuggingStats::ASTAR_PATHS_GEN = 0;
	float RTSDebuggingStats::ASTAR_PATH_GEN_TIME = 0.0f;

	void RTSDebuggingStats::clearStats()
	{		
		DebugStatistics::clearStats();
		ASTAR_PATHS_GEN = 0;
		ASTAR_PATH_GEN_TIME = 0.0f;
	}
}