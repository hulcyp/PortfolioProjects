#pragma once

namespace pdh
{
	class Assertion
	{
	public:
		Assertion( bool condition, const char* file, int line, const char* format, ... );
		~Assertion();

		static const int MAX_MESSAGE_SIZE;
		static const char* ms_DebugMsg;
		static const char* ms_ErrorLocation;
		
	};
}

// __LINE__, __FILE__ , __VA_ARGS__