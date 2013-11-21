#include "MonkyNew.h"
#include "MemoryManager.h"

#ifdef new
#undef new
#endif

#ifdef MONKY_NEW

void* operator new( size_t bytes )
{
	return Monky::allocMemoryFromPool( bytes, "", USED_BLOCK_NO_LINE_NUM );
}
//--------------------------------------------------------------
void* operator new( size_t bytes, const char* fileName, int lineNum )
{
	return Monky::allocMemoryFromPool( bytes, fileName, lineNum );
}
//--------------------------------------------------------------
void* operator new( size_t bytes, const char* fileName, int lineNum, short tag )
{
	return Monky::allocMemoryFromPool( bytes, fileName, lineNum, tag );
}
//--------------------------------------------------------------
void* operator new[]( size_t bytes )
{
	return Monky::allocMemoryFromPool( bytes, "", USED_BLOCK_NO_LINE_NUM );
}
//--------------------------------------------------------------
void* operator new[]( size_t bytes, const char* fileName, int lineNum )
{
	return Monky::allocMemoryFromPool( bytes, fileName, lineNum );
}
//--------------------------------------------------------------
void* operator new[]( size_t bytes, const char* fileName, int lineNum, short tag )
{
	return Monky::allocMemoryFromPool( bytes, fileName, lineNum, tag );
}
//--------------------------------------------------------------
void operator delete( void* addr )
{
	Monky::freeMemoryFromPool( addr );
}
//--------------------------------------------------------------
void operator delete( void* addr, const char* fileName, int lineNum )
{
	Monky::freeMemoryFromPool( addr );
}
//--------------------------------------------------------------
void operator delete( void* addr, const char* fileName, int lineNum, short tag )
{
	Monky::freeMemoryFromPool( addr );
}
//--------------------------------------------------------------
void operator delete[]( void* addr )
{
	Monky::freeMemoryFromPool( addr );
}
//--------------------------------------------------------------
void operator delete[]( void* addr, const char* fileName, int lineNum )
{
	Monky::freeMemoryFromPool( addr );
}
//--------------------------------------------------------------
void operator delete[]( void* addr, const char* fileName, int lineNum, short tag )
{
	Monky::freeMemoryFromPool( addr );
}

#endif
