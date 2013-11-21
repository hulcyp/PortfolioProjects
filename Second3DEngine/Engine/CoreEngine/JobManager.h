#pragma once
#include "CommonEngineIncludes.h"
#include "ThreadUtils.h"

namespace Monky
{

	unsigned int __stdcall workerThread( void* parameters );

	class Job;

	class JobManager
	{
	public:
		friend unsigned int __stdcall workerThread( void* parameters );
		
		JobManager();
		JobManager( int numOfThreads );
		void init();
		
		void setNumThreads( int numOfThreads );
		void update();
		void fireEventsForMyCompletedJobs();
		void postJob( Job* job );
		void updatePriorities();

	private:
		Job* claimPendingJob();
		void postJobFinished( Job* job );
		int getDesiredNumberOfWorkerThreads();

		std::vector< Job* > m_pendingJobs;
		std::vector< Job* > m_completedJobs;
		int m_desiredNumberOfWorkerThreads;
		int m_currentNumberOfWorkerThreads;
	};

	JobManager& getJobManager();
}