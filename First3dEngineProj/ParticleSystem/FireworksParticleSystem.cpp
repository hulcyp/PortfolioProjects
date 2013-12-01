#include "FireworksParticleSystem.h"
#include "RandomColorParticle.h"

namespace pdh
{
	FireworksParticleSystem::FireworksParticleSystem(	size_t minNumParticles,
														size_t maxNumParticles,
														float minLifespan,
														float maxLifespan,
														float minLaunchSpeed,
														float maxLaunchSpeed,
														float emissionRate )
														:	ExplosionParticleSystem( minNumParticles, maxNumParticles,
																minLifespan, maxLifespan,
																minLaunchSpeed, maxLaunchSpeed,
																emissionRate )
														,	m_g( vec3( 0.0f, -9.81f, 0.0f ) )
	{
	}

	FireworksParticleSystem::~FireworksParticleSystem()
	{}

	State< vec3, float > FireworksParticleSystem::simulationForces( 
			State< vec3, float > state, psIter iter, float t )
	{
		return State< vec3, float >( state[1], m_g );
	}

	void FireworksParticleSystem::emitParticles()
	{
		size_t numParticles = randInRange( m_minNumParticles, m_maxNumParticles );
		for( size_t i = 0; i < numParticles; ++i )
		{
			float angle1 = randInRange( 0.0f, 360.0f );
			float angle2 = randInRange( 0.0f, 360.0f );
			vec3 ranVec( cos( angle1 ), sin( angle1 ), sin( angle2 ) );
			float mag = randInRange( m_minLaunchSpeed, m_maxLaunchSpeed );
			float lifespan = randInRange( m_minLifespan, m_maxLifespan );
			m_particles.push_back( std::make_shared< RandomColorParticle >( vec3(), ranVec * mag, 1.0f, lifespan ) );
		}
	}

}