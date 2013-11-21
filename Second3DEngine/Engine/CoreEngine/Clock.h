#pragma once
#include "CommonEngineIncludes.h"
#include "Uncopyable.h"
#include "TimeUtils.h"

namespace Monky
{
	class Clock : public Uncopyable
	{
	public:
		Clock( Clock* parent, double maxDeltaSeconds = 0.1 );
		~Clock();

		void advance( double deltaSeconds );
		bool isPaused() const;		

		double getElapsedSecondsDouble() const;
		double getDeltaSecondsDouble() const;
		float getElapsedSecondsFloat() const;
		float getDeltaSecondsFloat() const;

		double getElapsedTimeDouble( TimeUtils::Units units ) const;
		double getDeltaTimeDouble( TimeUtils::Units units ) const;
		float getElapsedTimeFloat( TimeUtils::Units units ) const;
		float getDeltaTimeFloat( TimeUtils::Units units ) const;
				
		void setElapsedTime( double elapsedTimeSeconds );
		void setElapsedTime( double elapsedTime, TimeUtils::Units units );
		void setTimeScale( double timeScale );
		void reset();
		void pause();
		void unpause();
		
		// Alarm functions
		void setAlarmForElapsedSeconds( const std::string& eventToFire, double elapsedSeconds );
		void setAlarmForElapsedSeconds( const std::string& eventToFire, NamedProperties& paramsToPass, double elapsedSeconds );
		void setAlarmForSecondsAway( const std::string& eventToFire, double secondsAway );
		void setAlarmForSecondsAway( const std::string& eventToFire, NamedProperties& paramsToPass, double secondsAway );
		
	protected:
		
		struct Alarm
		{
			Alarm( const std::string& eventToFire, double elapsedSeconds )
				:	eventToFire( eventToFire )
				,	elapsedSecondsToFireAt( elapsedSeconds )
			{}
			Alarm( const std::string& eventToFire, NamedProperties& paramsToPass, double elapsedSeconds )
				:	eventToFire( eventToFire )
				,	paramsToPass( paramsToPass )
				,	elapsedSecondsToFireAt( elapsedSeconds )
			{}
			std::string eventToFire;
			NamedProperties paramsToPass;
			double elapsedSecondsToFireAt;
		};

		void sortAlarms();
		void checkAlarms();

	private:
		double m_elapsedSeconds;
		double m_deltaSeconds;
		double m_timeScale;
		bool m_isPaused;
		Clock* m_parent;
		std::vector< Clock* > m_children;
		std::vector< Alarm > m_alarms;

		const double MAX_DELTA_SECONDS;
	};
}