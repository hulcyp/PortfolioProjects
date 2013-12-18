#pragma once
#include "Job.h"
#include "StdLibraryIncludes.h"

namespace Monky
{
	class FunctionJob : public Job
	{
	public:
		FunctionJob( std::function< void() > funcPtr,const std::string& eventToFire );
		virtual ~FunctionJob();

		virtual void execute();

	private:
		std::function< void() > m_funcPtr;
	};
}