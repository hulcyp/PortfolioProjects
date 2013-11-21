#pragma once

#ifdef new
#undef new
#endif

#define MONKY_NEW

void* operator new( size_t bytes );
void* operator new( size_t bytes, const char* fileName, int lineNum );
void* operator new( size_t bytes, void* addr );
void* operator new( size_t bytes, const char* fileName, int lineNum, short tag );
void* operator new[]( size_t bytes );
void* operator new[]( size_t bytes, const char* fileName, int lineNum );
void* operator new[]( size_t bytes, const char* fileName, int lineNum, short tag );


void operator delete( void* addr );
void operator delete( void* addr, const char* fileName, int lineNum );
void operator delete( void* addr, const char* fileName, int lineNum, short tag );
void operator delete( void* addr, void* placeAddr );
void operator delete[]( void* addr );
void operator delete[]( void* addr, const char* fileName, int lineNum );
void operator delete( void* addr, const char* fileName, int lineNum, short tag );