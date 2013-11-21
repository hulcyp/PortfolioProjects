#pragma once
#include <Windows.h>
#include <WinBase.h>

namespace Monky
{
	typedef CRITICAL_SECTION MONKY_CRITICAL_SECTION;
	typedef HANDLE MONKY_HANDLE;

	MONKY_HANDLE beginThread( unsigned int( __stdcall *functionPtr )( void* ), void* argList );	
	void initializeCriticalSection( MONKY_CRITICAL_SECTION &criticalSection );
	void enterCriticalSection( MONKY_CRITICAL_SECTION &criticalSection );
	void leaveCriticalSection( MONKY_CRITICAL_SECTION &criticalSection );
	void sleep( unsigned long timeMS );
	int getCurrentThreadID();
}