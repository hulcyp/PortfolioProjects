#include "ThreadUtils.h"
#include <process.h>

namespace Monky
{
	MONKY_HANDLE beginThread( unsigned int( __stdcall *functionPtr )( void* ), void* argList )
	{
		return (MONKY_HANDLE)_beginthreadex( NULL, 0, functionPtr, argList, 0, NULL );
	}
	//--------------------------------------------------------------------
	void initializeCriticalSection( MONKY_CRITICAL_SECTION &criticalSection )
	{
		InitializeCriticalSection( &criticalSection );
	}
	//--------------------------------------------------------------------
	void enterCriticalSection( MONKY_CRITICAL_SECTION &criticalSection )
	{
		EnterCriticalSection( &criticalSection );
	}
	//--------------------------------------------------------------------
	void leaveCriticalSection( MONKY_CRITICAL_SECTION &criticalSection )
	{
		LeaveCriticalSection( &criticalSection );
	}
	//--------------------------------------------------------------------
	void sleep( unsigned long timeMS )
	{
		Sleep( timeMS );
	}
	//--------------------------------------------------------------------
	int getCurrentThreadID()
	{
		return (int)GetCurrentThreadId();
	}
}