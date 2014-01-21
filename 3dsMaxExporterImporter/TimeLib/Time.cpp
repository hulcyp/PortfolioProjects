
/*************************
* author: Preston Hulcy
* last updated: 9-21-12
*************************/
#include <windows.h>
#include "Time.h"


namespace pdh
{
	double GetAbsoluteTimeSeconds()
	{
		SystemClocks freq = GetClockFrequency();
		LARGE_INTEGER clocks;

		QueryPerformanceCounter( &clocks );

		return static_cast<double>( clocks.QuadPart ) / freq;
	}

	SystemClocks GetAbsoluteTimeClocks()
	{
		LARGE_INTEGER clocks;
		QueryPerformanceCounter( &clocks );
		return clocks.QuadPart;
	}

	SystemClocks GetClockFrequency()
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );
		return freq.QuadPart;
	}

	double GetClockFrequencyDouble()
	{
		return static_cast<double>( GetClockFrequency() );
	}

	double ClocksToSeconds( SystemClocks clocks )
	{
		SystemClocks freq = GetClockFrequency();
		return static_cast<double>( clocks ) / freq;
	}

	SystemClocks SecondsToClocks( double seconds )
	{
		SystemClocks freq = GetClockFrequency();
		return static_cast<SystemClocks>(freq * seconds);
	}

	void TimeWaster( double seconds )
	{
		SystemClocks now = GetAbsoluteTimeClocks();
		SystemClocks clocksToWasteTo = now + SecondsToClocks( seconds );
		
		while( now < clocksToWasteTo )
		{
			now = GetAbsoluteTimeClocks();
		}
	}

}