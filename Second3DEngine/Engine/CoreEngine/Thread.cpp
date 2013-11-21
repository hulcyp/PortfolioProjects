#include "Thread.h"
#include <Windows.h>
#include <process.h>
#include <Assertion.h>

namespace Monky
{
	unsigned int __stdcall threadWrapper( void* parameters );

	class ThreadImplementation
	{
	public:
		ThreadImplementation::ThreadImplementation( std::function< void( void* ) > callee, void* args )
			:	m_callee( callee )
			,	m_args( args )
		{
			_beginthreadex( NULL, 0, &threadWrapper, this, 0, &m_threadID );
		}

	private:
		HANDLE m_thread;
		std::function< void( void* ) > m_callee;
		void* m_args;
		unsigned int m_threadID;

		void runInsideThread()
		{
			assertion( m_callee, "Invalid callee function" );
			
			m_callee( m_args );
		}

		friend unsigned int __stdcall threadWrapper( void* parameters );
	};

	Thread::Thread( std::function< void( void* ) > callee, void* args )
		:	m_pImpl( new ThreadImplementation( callee, args ) )
	{}
	//-------------------------------------------------------
	Thread::~Thread()
	{
		delete m_pImpl;
	}
	//-------------------------------------------------------
	unsigned int __stdcall threadWrapper( void* parameters )
	{
		ThreadImplementation* threadImpl = reinterpret_cast< ThreadImplementation* >( parameters );
		assertion( threadImpl != nullptr, "Invalid thread implenation passed to thread wrapper" );

		threadImpl->runInsideThread();

		return 0;
	}
}