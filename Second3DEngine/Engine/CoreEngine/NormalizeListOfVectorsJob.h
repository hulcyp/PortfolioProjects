#pragma once
#include "CommonEngineIncludes.h"
#include "Job.h"

namespace Monky
{
	class NormalizeListOfVectorsJob : public Job
	{
	public:
		NormalizeListOfVectorsJob( const std::string& eventToFire, std::vector< vec3f >& vectors, float priority = 1.0f );
		
		virtual void execute();

	private:
		std::vector< vec3f >& m_vectors;
	};
}