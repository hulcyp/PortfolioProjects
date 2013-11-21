#pragma once

#include "TimeUtils.h"
#include "StdLibraryIncludes.h"


namespace Monky
{
	class Timer
	{
	public:
		Timer();
		Timer( double time );
		~Timer();

		void setTime( double time );
		void start();
		void pause();
		void unPause();
		void stop();

		bool isTimeUp();
		bool wasStarted() { return m_wasStarted; }


		static void pauseTimers();
		static void unPauseTimers();

	protected:
		static void addTimer( Timer* timer );
		static void removeTimer( Timer* timer );

		double m_timerTime;

		double m_startTime;
		double m_endTime;

		bool m_wasStarted;

		double m_pauseStart;
		double m_timePaused;

		static std::list< Timer* > m_timers;


	};
}