#pragma once
#include <CommonEngineIncludes.h>
#include <Uncopyable.h>
#include <ThreadUtils.h>

namespace Monky
{
	class Job : public Uncopyable
	{
	public:
		Job( const std::string& eventToFire, float priority = 1.0f );
		Job( const Job& copy );
		virtual ~Job();

		bool operator<( const Job& job ) const;

		virtual void execute() = 0;
		virtual void fireCompletedEvent();
		void setPriority( float priority );
		float getPriority() const { return m_priority; }	
		int getThreadID() const { return m_threadID; }

		MONKY_CRITICAL_SECTION m_jobCriticalSection;

	protected:
		std::string m_eventToFire;
		float m_priority;
		int m_threadID;
	};
}