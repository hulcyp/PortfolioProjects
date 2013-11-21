#include "FunctionJob.h"

namespace Monky
{
	FunctionJob::FunctionJob( std::function< void() > funcPtr,const std::string& eventToFire )
		:	Job( eventToFire )
		,	m_funcPtr( funcPtr )
	{}
	//---------------------------------------------------
	FunctionJob::~FunctionJob()
	{}
	//---------------------------------------------------
	void FunctionJob::execute()
	{
		m_funcPtr();
	}
}