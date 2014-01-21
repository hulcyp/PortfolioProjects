#pragma once

namespace pdh
{
	class DebugStatistics
	{
	public:
		static int RENDER_CALLS;
		static int MATERIAL_SWAPS;

		static void clearStats();
	};
}