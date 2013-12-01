#include "SmokeParticleSystem.h"
#include <VectorUtils.h>

namespace pdh
{
	SmokeParticleSystem::SmokeParticleSystem(	size_t minNumParticles,
												size_t maxNumParticles,
												float minLifespan,
												float maxLifespan,
												float minLaunchSpeed,
												float maxLaunchSpeed,
												float emissionRate )
												:	ExplosionParticleSystem(	minNumParticles,
																				maxNumParticles,
																				minLifespan,
																				maxLifespan,
																				minLaunchSpeed,
																				maxLaunchSpeed,
																				emissionRate )
	{}

	SmokeParticleSystem::~SmokeParticleSystem()
	{}

	State< vec3, float > SmokeParticleSystem::simulationForces(
		State< vec3, float > state, psIter iter, float t )
	{
		vec3 f( 2*state[0].z*state[0].y, state[1].y, -2*state[0].x*state[0].y );
		return State< vec3, float >( state[1], ( f + m_force )/ (*iter)->getMass() );
	}

	void SmokeParticleSystem::emitParticles()
	{
		size_t numParticles = randInRange( m_minNumParticles, m_maxNumParticles );
		for( size_t i = 0; i < numParticles; ++i )
		{
			vec3 ranVec = randVec3InRange( m_minLaunchSpeed, m_maxLaunchSpeed );
			float lifespan = randInRange( m_minLifespan, m_maxLifespan );
			m_particles.push_back( std::make_shared< Particle >( vec3(), ranVec, 1.0f, lifespan ) );
		}
	}
}