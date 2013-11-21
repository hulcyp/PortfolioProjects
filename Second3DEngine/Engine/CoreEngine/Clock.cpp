#include "Clock.h"
#include <algorithm>
#include "EventSystemInterface.h"


namespace Monky
{
	Clock::Clock( Clock* parent, double maxDeltaSeconds )
		:	m_parent( parent )
		,	m_elapsedSeconds( 0.0 )
		,	m_deltaSeconds( 0.0 )
		,	m_timeScale( 1.0 )
		,	m_isPaused( false )
		,	MAX_DELTA_SECONDS( maxDeltaSeconds )
	{
		if( m_parent )
		{
			m_parent->m_children.push_back( this );
		}
	}
	//------------------------------------------------------------------
	Clock::~Clock()
	{
		if( !m_children.empty() )
		{
			for( size_t i = 0; i < m_children.size(); ++i )
			{
				m_children[i]->m_parent = m_parent;
				if( m_parent )
					m_parent->m_children.push_back( m_children[i] );
			}
		}
		m_children.clear();
	}
	//------------------------------------------------------------------
	void Clock::advance( double deltaSeconds )
	{
		double dtSeconds = deltaSeconds;
		if( m_isPaused )
			dtSeconds = 0.0;
		else
			dtSeconds *= m_timeScale;

		m_deltaSeconds = dtSeconds;
		m_elapsedSeconds += m_deltaSeconds;

		checkAlarms();

		for( size_t i = 0; i < m_children.size(); ++i )
		{
			m_children[i]->advance( dtSeconds );
		}
	}
	//------------------------------------------------------------------
	bool Clock::isPaused() const
	{
		if( m_parent )
			return m_isPaused || m_parent->isPaused();
		else
			return m_isPaused;
	}
	//------------------------------------------------------------------
	double Clock::getElapsedSecondsDouble() const
	{
		return m_elapsedSeconds;
	}
	//------------------------------------------------------------------
	double Clock::getDeltaSecondsDouble() const
	{
		return m_deltaSeconds;
	}
	//------------------------------------------------------------------
	float Clock::getElapsedSecondsFloat() const
	{
		return (float)m_elapsedSeconds;
	}
	//------------------------------------------------------------------
	float Clock::getDeltaSecondsFloat() const
	{
		return (float)m_deltaSeconds;
	}
	//------------------------------------------------------------------
	double Clock::getElapsedTimeDouble( TimeUtils::Units units ) const
	{
		return TimeUtils::ConvertTimeFromSecondsTo( m_elapsedSeconds, units );
	}
	//------------------------------------------------------------------
	double Clock::getDeltaTimeDouble( TimeUtils::Units units ) const
	{
		return TimeUtils::ConvertTimeFromSecondsTo( m_deltaSeconds, units );
	}
	//------------------------------------------------------------------
	float Clock::getElapsedTimeFloat( TimeUtils::Units units ) const
	{
		return (float)TimeUtils::ConvertTimeFromSecondsTo( m_elapsedSeconds, units );
	}
	//------------------------------------------------------------------
	float Clock::getDeltaTimeFloat( TimeUtils::Units units ) const
	{
		return (float)TimeUtils::ConvertTimeFromSecondsTo( m_deltaSeconds, units );
	}
	//------------------------------------------------------------------
	void Clock::setElapsedTime( double elapsedTimeSeconds )
	{
		m_elapsedSeconds = elapsedTimeSeconds;
	}
	//------------------------------------------------------------------
	void Clock::setElapsedTime( double elapsedTime, TimeUtils::Units units )
	{
		m_elapsedSeconds = TimeUtils::ConvertFromUnitsToSeconds( elapsedTime, units );
	}
	//------------------------------------------------------------------
	void Clock::setTimeScale( double timeScale )
	{
		m_timeScale = timeScale;
	}
	//------------------------------------------------------------------
	void Clock::reset()
	{
		m_elapsedSeconds = 0.0;
		m_deltaSeconds = 0.0;
		m_isPaused = false;
	}
	//------------------------------------------------------------------
	void Clock::pause()
	{
		m_isPaused = true;
	}
	//------------------------------------------------------------------
	void Clock::unpause()
	{
		m_isPaused = false;
	}
	//------------------------------------------------------------------
	void Clock::setAlarmForElapsedSeconds( const std::string& eventToFire, double elapsedSeconds )
	{
		m_alarms.push_back( Alarm( eventToFire, elapsedSeconds ) );
		sortAlarms();
	}
	//------------------------------------------------------------------
	void Clock::setAlarmForElapsedSeconds( const std::string& eventToFire, NamedProperties& paramsToPass, double elapsedSeconds )
	{
		m_alarms.push_back( Alarm( eventToFire, paramsToPass, elapsedSeconds ) );
		sortAlarms();
	}
	//------------------------------------------------------------------
	void Clock::setAlarmForSecondsAway( const std::string& eventToFire, double secondsAway )
	{
		setAlarmForElapsedSeconds( eventToFire, m_elapsedSeconds + secondsAway );
	}
	//------------------------------------------------------------------
	void Clock::setAlarmForSecondsAway( const std::string& eventToFire, NamedProperties& paramsToPass, double secondsAway )
	{
		setAlarmForElapsedSeconds( eventToFire, paramsToPass, m_elapsedSeconds + secondsAway );
	}
	//------------------------------------------------------------------
	void Clock::sortAlarms()
	{
		std::sort( m_alarms.begin(), m_alarms.end(),
			[&]( Alarm& A, Alarm& B )
		{
			return A.elapsedSecondsToFireAt > B.elapsedSecondsToFireAt;
		} );
	}
	//------------------------------------------------------------------
	void Clock::checkAlarms()
	{
		while( !m_alarms.empty() )
		{
			size_t index = m_alarms.size() - 1;
			if( m_alarms[ index ].elapsedSecondsToFireAt <= m_elapsedSeconds )
			{
				if( m_alarms[ index ].paramsToPass.isEmpty() )
					fireEvent( m_alarms[ index ].eventToFire );
				else
					fireEvent( m_alarms[ index ].eventToFire, m_alarms[ index ].paramsToPass );
				
				m_alarms.pop_back();
			}
			else
			{
				break;
			}
		}
	}
}