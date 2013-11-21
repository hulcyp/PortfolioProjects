#include <NormalizeListOfVectorsJob.h>


namespace Monky
{
	NormalizeListOfVectorsJob::NormalizeListOfVectorsJob( const std::string& eventToFire, std::vector< vec3f >& vectors, float priority )
		:	Job( eventToFire, priority )
		,	m_vectors( vectors )
	{}
	//------------------------------------------------------------------------------------
	void NormalizeListOfVectorsJob::execute()
	{
		for( int i = 0; i < (int)m_vectors.size(); ++i )
		{
			m_vectors[i].normalize();
		}
	}
}