#include "FountainParticleSystem.h"
#include "RandomColorParticle.h"

namespace pdh
{
	FountainParticleSystem::FountainParticleSystem(	size_t minNumParticles,
													size_t maxNumParticles,
													float minLifespan,
													float maxLifespan,
													float minLaunchSpeed,
													float maxLaunchSpeed,
													float minLaunchAngle,
													float maxLaunchAngle,
													float emissionRate )
													:	ExplosionParticleSystem(	minNumParticles, 
																					maxNumParticles,
																					minLifespan,
																					maxLifespan,
																					minLaunchSpeed,
																					maxLaunchSpeed,
																					emissionRate )
													,	m_minLaunchAngle( minLaunchAngle )
													,	m_maxLaunchAngle( maxLaunchAngle )
													,	m_g( 0.0f, -9.81f, 0.0f )
	{}

	FountainParticleSystem::~FountainParticleSystem()
	{}

	State< vec3, float > FountainParticleSystem::simulationForces( 
			State< vec3, float > state, psIter iter, float t )
	{
		return State< vec3, float >( state[1], m_g );
	}
			
	void FountainParticleSystem::emitParticles()
	{
		size_t numParticles = randInRange( m_minNumParticles, m_maxNumParticles );
		
		for( size_t i = 0; i < numParticles; ++i )
		{
			float angle1 = randInRange( m_minLaunchAngle, m_maxLaunchAngle );
			float angle2 = randInRange( m_minLaunchAngle, m_maxLaunchAngle );
			vec3 ranVec( sin( angle1 ), 5*std::abs(cos( angle2 )), cos( angle1 ) );
			ranVec.normalize();
			float mag = randInRange( m_minLaunchSpeed, m_maxLaunchSpeed );
			float lifespan = randInRange( m_minLifespan, m_maxLifespan );
			m_particles.push_back( std::make_shared< RandomColorParticle >( vec3(), ranVec * mag, 1.0f, lifespan ) );
		}
	}
}