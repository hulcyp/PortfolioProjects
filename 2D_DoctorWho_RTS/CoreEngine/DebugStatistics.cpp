#include "DebugStatistics.h"

namespace pdh
{
	int DebugStatistics::RENDER_CALLS = 0;
	int DebugStatistics::MATERIAL_SWAPS = 0;

	void DebugStatistics::clearStats()
	{		
		MATERIAL_SWAPS = 0;
		RENDER_CALLS = 0;
	}
}