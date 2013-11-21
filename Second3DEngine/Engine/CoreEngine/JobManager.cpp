#include "CommonEngineIncludes.h"
#include "JobManager.h"
#include "Job.h"

namespace Monky
{
	bool g_initializedCS = false;
	MONKY_CRITICAL_SECTION g_workThreadIDCS;
	MONKY_CRITICAL_SECTION g_JobManagerCS;

	JobManager& getJobManager()
	{
		static JobManager jobManager;
		return jobManager;
	}
	//-------------------------------------------
	JobManager::JobManager()
		:	m_desiredNumberOfWorkerThreads( 1 )
		,	m_currentNumberOfWorkerThreads( 0 )
	{
		init();
	}
	//-------------------------------------------
	JobManager::JobManager( int numOfThreads )
		:	m_desiredNumberOfWorkerThreads( numOfThreads )
		,	m_currentNumberOfWorkerThreads( 0 )
	{
		init();	
	}
	//-------------------------------------------	
	void JobManager::init()
	{
		if( !g_initializedCS )
		{
			Monky::initializeCriticalSection( g_workThreadIDCS );
			Monky::initializeCriticalSection( g_JobManagerCS );
			g_initializedCS = true;
		}
	}
	//-------------------------------------------	
	void JobManager::setNumThreads( int numOfThreads )
	{
		Monky::enterCriticalSection( g_JobManagerCS );
		m_desiredNumberOfWorkerThreads = numOfThreads;
		Monky::leaveCriticalSection( g_JobManagerCS );
	}
	//-------------------------------------------	
	void JobManager::update()
	{
		Monky::enterCriticalSection( g_JobManagerCS );
		int numWorkersToCreate = m_desiredNumberOfWorkerThreads - m_currentNumberOfWorkerThreads;
		Monky::leaveCriticalSection( g_JobManagerCS );

		while( numWorkersToCreate > 0 )
		{
			Monky::beginThread( workerThread, nullptr );
			--numWorkersToCreate;
			Monky::enterCriticalSection( g_JobManagerCS );			
			++m_currentNumberOfWorkerThreads;
			Monky::leaveCriticalSection( g_JobManagerCS );
			
		}
		
		Monky::enterCriticalSection( g_JobManagerCS );
		int currentNumWorkers = m_currentNumberOfWorkerThreads;
		Monky::leaveCriticalSection( g_JobManagerCS );
		
		if( currentNumWorkers <= 0 )
		{			
			Job* job = claimPendingJob();
			
			if( job != nullptr )
			{
				job->execute();
				postJobFinished( job );
			}
		}		
	}
	//-------------------------------------------	
	void JobManager::fireEventsForMyCompletedJobs()
	{
		Monky::enterCriticalSection( g_JobManagerCS );
		std::vector< Job* > completedJobs = m_completedJobs;
		Monky::leaveCriticalSection( g_JobManagerCS );

		std::for_each( completedJobs.begin(), completedJobs.end(),
			[&]( Job* job )
		{
			if( job->getThreadID() == Monky::getCurrentThreadID() )
			{
				job->fireCompletedEvent();
				SAFE_DELETE( job );
			}
			
		} );
		completedJobs.erase( 
			std::remove_if( completedJobs.begin(), completedJobs.end(),
			[&]( Job* job )
			{
				return job == nullptr;
			} ), completedJobs.end() );

		Monky::enterCriticalSection( g_JobManagerCS );
		m_completedJobs = completedJobs;
		Monky::leaveCriticalSection( g_JobManagerCS );
	}
	//-------------------------------------------	
	void JobManager::postJob( Job* job )
	{
		Monky::enterCriticalSection( g_JobManagerCS );
		m_pendingJobs.push_back( job );
		std::sort( m_pendingJobs.begin(), m_pendingJobs.end() );
		Monky::leaveCriticalSection( g_JobManagerCS );
	}
	//-------------------------------------------	
	void JobManager::updatePriorities()
	{
		Monky::enterCriticalSection( g_JobManagerCS );
		std::sort( m_pendingJobs.begin(), m_pendingJobs.end() );
		Monky::leaveCriticalSection( g_JobManagerCS );
	}
	//-------------------------------------------	
	// Private member functions
	//-------------------------------------------		
	Job* JobManager::claimPendingJob()
	{
		Job* jobToClaim = nullptr;
		Monky::enterCriticalSection( g_JobManagerCS );
		if( !m_pendingJobs.empty() )
		{
			jobToClaim = m_pendingJobs.back();
			m_pendingJobs.pop_back();
		}	
		Monky::leaveCriticalSection( g_JobManagerCS );
		return jobToClaim;
	}
	//-------------------------------------------		
	void JobManager::postJobFinished( Job* job )
	{
		Monky::enterCriticalSection( g_JobManagerCS );
		m_completedJobs.push_back( job );
		Monky::leaveCriticalSection( g_JobManagerCS );
	}
	//-------------------------------------------		
	int JobManager::getDesiredNumberOfWorkerThreads()
	{
		Monky::enterCriticalSection( g_JobManagerCS );
		int desiredNumberOfWorkerThreads = m_desiredNumberOfWorkerThreads;
		Monky::leaveCriticalSection( g_JobManagerCS );
		return desiredNumberOfWorkerThreads;
	}

	//-------------------------------------------		
	// Worker thread function
	//-------------------------------------------		
	unsigned int __stdcall workerThread( void* parameters )
	{
		Monky::enterCriticalSection( g_workThreadIDCS );
		static int nextWorkerThreadID = 0;
		Monky::leaveCriticalSection( g_workThreadIDCS );

		Monky::enterCriticalSection( g_workThreadIDCS );
		int workerThreadID = nextWorkerThreadID;
		++nextWorkerThreadID;
		Monky::leaveCriticalSection( g_workThreadIDCS );
	
		for( ;; )
		{
			Monky::enterCriticalSection( g_JobManagerCS );
			int desiredNumberOfWorkerThreads = getJobManager().getDesiredNumberOfWorkerThreads();	
			Monky::leaveCriticalSection( g_JobManagerCS );

			if( workerThreadID >= desiredNumberOfWorkerThreads )
				break;

			Monky::enterCriticalSection( g_JobManagerCS );
			Job* job = getJobManager().claimPendingJob();
			Monky::leaveCriticalSection( g_JobManagerCS );

			if( job )
			{
				job->execute();
				Monky::enterCriticalSection( g_JobManagerCS );
				getJobManager().postJobFinished( job );
				Monky::leaveCriticalSection( g_JobManagerCS );
			}
			Monky::sleep(0);
		}

		Monky::enterCriticalSection( g_JobManagerCS );
		--getJobManager().m_currentNumberOfWorkerThreads;
		Monky::leaveCriticalSection( g_JobManagerCS );

		return 0;
	}
}