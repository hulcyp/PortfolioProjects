#include <DebugStatistics.h>

namespace pdh
{
	class RTSDebuggingStats : public DebugStatistics
	{
	public:
		static int ASTAR_PATHS_GEN;
		static float ASTAR_PATH_GEN_TIME;

		static void clearStats();
	};
}