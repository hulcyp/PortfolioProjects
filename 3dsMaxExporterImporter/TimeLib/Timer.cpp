#include "Timer.h"
#include <algorithm>

namespace pdh
{
	Timer::Timer()
		:	m_timerTime( 0 )
		,	m_startTime( 0 )
		,	m_endTime( 0 )
		,	m_wasStarted( false )
		,	m_pauseStart( 0 )
		,	m_timePaused( 0 )
	{
		Timer::addTimer( this );
	}
		
	Timer::Timer( double time )
		:	m_timerTime( SecondsToClocks( time ) )
		,	m_startTime( 0 )
		,	m_endTime( 0 )
		,	m_wasStarted( false )
		,	m_pauseStart( 0 )
		,	m_timePaused( 0 )
	{
		Timer::addTimer( this );
	}

	Timer::~Timer()
	{
		Timer::removeTimer( this );
	}

	//Member functions

	void Timer::setTime( double time )
	{
		m_timerTime = SecondsToClocks( time );
	}

	void Timer::setTime( SystemClocks time )
	{
		m_timerTime = time;
	}

	void Timer::start()
	{
		m_startTime = GetAbsoluteTimeClocks();
		m_endTime = m_startTime + m_timerTime;
		m_wasStarted = true;
	}

	void Timer::pause()
	{
		m_pauseStart = GetAbsoluteTimeClocks();
	}

	void Timer::unPause()
	{
		m_timePaused = GetAbsoluteTimeClocks() - m_pauseStart;
	}

	void Timer::stop()
	{
		m_wasStarted = false;
		m_timePaused = 0;
	}

	bool Timer::isTimeUp()
	{
		SystemClocks time = m_endTime + m_timePaused;
		
		if( GetAbsoluteTimeClocks() > time && m_wasStarted )
		{
			m_wasStarted = false;
			m_timePaused = 0;
			return true;
		}

		return false;
	}

	//Static functions
	void Timer::pauseTimers()
	{
		std::for_each( m_timers.begin(), m_timers.end(),
			[&]( Timer* timer )
		{
			timer->pause();
		} );
	}

	void Timer::unPauseTimers()
	{
		std::for_each( m_timers.begin(), m_timers.end(),
			[&]( Timer* timer )
		{
			timer->unPause();
		} );
	}

	void Timer::addTimer( Timer* timer )
	{
		m_timers.push_front( timer );
	}

	void Timer::removeTimer( Timer* timer )
	{
		m_timers.remove( timer );
	}

	std::list< Timer* > Timer::m_timers = std::list< Timer* >();
}