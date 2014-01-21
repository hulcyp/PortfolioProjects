#pragma once

#include "Time.h"
#include <list>


namespace pdh
{
	class Timer
	{
	public:
		Timer();
		Timer( double time );
		~Timer();

		void setTime( double time );
		void setTime( SystemClocks time );
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

		SystemClocks m_timerTime;


		SystemClocks m_startTime;
		SystemClocks m_endTime;

		bool m_wasStarted;

		SystemClocks m_pauseStart;
		SystemClocks m_timePaused;

		static std::list< Timer* > m_timers;


	};
}