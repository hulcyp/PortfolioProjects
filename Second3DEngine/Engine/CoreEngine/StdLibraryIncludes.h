#pragma once


#ifdef new
#undef new
#endif

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <functional>
#include <memory>
#include <stack>
#include <list>
#include <deque>
#include <queue>
#include <stdarg.h>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <crtdbg.h>
#include <stdio.h>
#include <cstring>
#include <string.h>

#define MONKY_NEW

#ifdef MONKY_NEW
//#define new new( __FILE__, __LINE__ )
#define new( tag ) new( __FILE__, __LINE__, tag )
#endif

#define SAFE_DELETE( ptr ) {	\
	if( ptr != nullptr )		\
	{							\
		delete ptr;				\
		ptr = nullptr;			\
	}							\
}

#define SAFE_ARRAY_DELETE( ptr ) {	\
	if( ptr != nullptr )			\
	{								\
		delete [] ptr;				\
		ptr = nullptr;				\
	}								\
}

// Cleans up a vector of ptrs
#define VECTOR_CLEANUP( vector ) {				\
	for( size_t i = 0; i < vector.size(); ++i )	\
	{											\
		SAFE_DELETE( vector[i] );				\
	}											\
	vector.clear();								\
}

// Cleans up a map of keys to ptrs
#define MAP_CLEANUP( map ) {									\
	for( auto iter = map.begin(); iter != map.end(); ++iter )	\
	{															\
		SAFE_DELETE( iter->second );							\
	}															\
	map.clear();												\
}

// Cleans up a list of ptrs
#define LIST_CLEANUP( list ) {									\
	for( auto iter = list.begin(); iter != list.end(); ++iter )	\
	{															\
		SAFE_DELETE( *iter );									\
	}															\
	list.clear();												\
}

// Cleans up a queue of  ptrs
#define QUEUE_CLEANUP( queue ) {								\
	while( !queue.empty() )										\
	{															\
		SAFE_DELETE( queue.front() );							\
		queue.pop();											\
	}															\

// Cleans up a queue of  ptrs
#define DEQUE_CLEANUP( deque ) {								\
	while( !deque.empty() )										\
	{															\
		SAFE_DELETE( deque.front() );							\
		deque.pop_front();										\
	}															\
}