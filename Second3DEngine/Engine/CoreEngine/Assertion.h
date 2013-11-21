#pragma once

namespace Monky
{
	class Assertion
	{
	public:
		Assertion( bool condition, const char* file, int line, const char* format, ... );
		~Assertion();

		static const int MAX_MESSAGE_SIZE;
		static const char* ms_DebugMsg;
		static const char* ms_ErrorLocation;
		static const char* ms_Title;
	};

#define MONKY_DEBUG
#ifdef MONKY_DEBUG
#define assertion( condition, format, ... )	\
	Assertion( condition, __FILE__, __LINE__, format, __VA_ARGS__ );
#else
#define assertion( condition, format, ... ) \
	assert( condition );
#endif
}