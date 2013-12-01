#include "ParticleSystem.h"
#include <Vector3.h>
#include <algorithm>
#include <functional>

namespace pdh
{
	template< typename T, typename precision >
	ParticleSystem< T, precision >::ParticleSystem()
		:	m_time( 0.0 )
	{}

	template< typename T, typename precision >
	void ParticleSystem< T, precision >::updateSimulation( precision deltaTime )
	{
		for( psIter iter = m_particles.begin(); iter != m_particles.end(); ++iter )
		{
			fwdEuler( iter, m_time, deltaTime );
			(*iter)->updateLifspan( deltaTime );
		}
		destroyDeadParticles();
		m_time += deltaTime;
	}
	template< typename T, typename precision >
	void ParticleSystem< T, precision >::updateDisplay( Renderer* renderer  )
	{
		for( psIter iter = m_particles.begin(); iter != m_particles.end(); ++iter )
		{
			(*iter)->render( renderer );
		}
	}
	template< typename T, typename precision >
	void ParticleSystem< T, precision >::destroyDeadParticles()
	{
		m_particles.remove_if( std::mem_fn( &Particle::wantsToDie ) );
	}

	template class ParticleSystem< vec3, float >;
}