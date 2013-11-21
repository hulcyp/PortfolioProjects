#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 9/4/2013
//	Description:
//
//---------------------------------------------
#include <cstdarg>

namespace Monky
{
	void Log( const char* format, ... );
	void Log( const char* format, va_list args );

	void vLog( const char* format, ... );

	void Flush();
}