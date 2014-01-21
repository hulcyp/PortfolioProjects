#pragma once

/*************************
* author: Preston Hulcy
* last updated: 9-21-12
*************************/

namespace pdh
{
	
	typedef __int64 SystemClocks;
	
	double GetAbsoluteTimeSeconds();
	SystemClocks GetAbsoluteTimeClocks();
	SystemClocks GetClockFrequency(); // clocks/sec
	double GetClockFrequencyDouble();
	double ClocksToSeconds( SystemClocks clocks );
	SystemClocks SecondsToClocks( double seconds );
	void TimeWaster( double seconds );

}