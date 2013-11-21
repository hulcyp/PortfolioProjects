#include "Job.h"
#include "EventSystemInterface.h"

namespace Monky
{
	Job::Job( const std::string& eventToFire, float priority )
		:	m_eventToFire( eventToFire )
		,	m_priority( priority )
	{
		m_threadID = Monky::getCurrentThreadID();
		Monky::initializeCriticalSection( m_jobCriticalSection );
	}
	//-------------------------------------------
	Job::Job( const Job& copy )
		: m_eventToFire( copy.m_eventToFire )
	{
		Monky::initializeCriticalSection( m_jobCriticalSection );
	}
	//-------------------------------------------
	Job::~Job()
	{}
	//-------------------------------------------
	bool Job::operator<( const Job& rhs ) const
	{
		return m_priority < rhs.m_priority;
	}
	//-------------------------------------------
	void Job::fireCompletedEvent()
	{
		fireEvent( m_eventToFire );
	}
	//-------------------------------------------
	void Job::setPriority( float priority )
	{
		m_priority = priority;
	}
}