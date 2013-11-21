#pragma once
#include "MonkyNew.h"
#include "MemoryManager.h"
#ifdef MONKY_NEW
//#define new new( __FILE__, __LINE__ )
#define new( tag ) new( __FILE__, __LINE__, tag )
#endif
#include "Assertion.h"
#include "ConsoleInterface.h"
#include "CommonTypeDefines.h"
#include "GDOInterface.h"
#include "ProfileSystemInterface.h"
#include "EventSystemInterface.h"
#include "StdLibraryIncludes.h"
#include "Logging.h"

#define ARRAY_CLEANUP( array, size )	\
	for( int i = 0; i < size; ++i )		\
	{									\
		SAFE_DELETE( array[i] );		\
	}