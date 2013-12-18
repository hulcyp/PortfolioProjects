
/*************************
* author: Preston Hulcy
* last updated: 6-5-13
*************************/
#include <windows.h>
#include "TimeUtils.h"
#include <stdint.h>


namespace Monky
{
	typedef int64_t SystemClocks;

	double GetClockFrequencyDouble();
	double ClocksToSeconds( SystemClocks clocks );
	SystemClocks GetAbsoluteTimeClocks();
	SystemClocks GetClockFrequency(); // clocks/sec
	SystemClocks SecondsToClocks( double seconds );

	double TimeUtils::GetAbsoluteTimeSeconds()
	{
		static SystemClocks freq = GetClockFrequency();
		static double inverseFreq = 1.0 / static_cast<double>( freq );

		LARGE_INTEGER clocks;
		QueryPerformanceCounter( &clocks );

		return clocks.QuadPart * inverseFreq;
	}
	//---------------------------------------------------------------
	double TimeUtils::GetAbsoluteTime( Units units )
	{
		return ConvertTimeFromSecondsTo( GetAbsoluteTimeSeconds(), units );
	}
	//---------------------------------------------------------------
	void TimeUtils::TimeWaster( double seconds )
	{
		SystemClocks now = GetAbsoluteTimeClocks();
		SystemClocks clocksToWasteTo = now + SecondsToClocks( seconds );
		
		while( now < clocksToWasteTo )
		{
			now = GetAbsoluteTimeClocks();
		}
	}
	//---------------------------------------------------------------
	double TimeUtils::ConvertTimeFromSecondsTo( double timeSeconds, Units units )
	{
		switch( units )
		{
		case SECONDS:
			return timeSeconds;
		case MILLI:
			return timeSeconds * 1000.0;
		case MICRO:
			return timeSeconds * 1000000.0;
		default:
			return timeSeconds;
		}
	}
	//---------------------------------------------------------------
	double TimeUtils::ConvertFromUnitsToSeconds( double time, Units units )
	{
		switch( units )
		{
		case SECONDS:
			return time;
		case MILLI:
			return time * 0.001;
		case MICRO:
			return time * 0.000001;
		default:
			return time;
		}
	}

	//---------------------------------------------------------------
	//	Utility functions
	//---------------------------------------------------------------
	double GetClockFrequencyDouble()
	{
		return static_cast<double>( GetClockFrequency() );
	}
	//---------------------------------------------------------------
	SystemClocks GetAbsoluteTimeClocks()
	{
		LARGE_INTEGER clocks;
		QueryPerformanceCounter( &clocks );
		return clocks.QuadPart;
	}
	//---------------------------------------------------------------
	SystemClocks GetClockFrequency()
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );
		return freq.QuadPart;
	}
	//---------------------------------------------------------------
	double ClocksToSeconds( SystemClocks clocks )
	{
		SystemClocks freq = GetClockFrequency();
		return static_cast<double>( clocks ) / freq;
	}
	//---------------------------------------------------------------
	SystemClocks SecondsToClocks( double seconds )
	{
		SystemClocks freq = GetClockFrequency();
		return static_cast<SystemClocks>(freq * seconds);
	}
}