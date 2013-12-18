#pragma once
#include <functional>

namespace Monky
{
	class Thread
	{
	public:
		Thread( std::function< void( void* ) > callee, void* args );
		~Thread();

	private:
		class ThreadImplementation* m_pImpl;
	};
}