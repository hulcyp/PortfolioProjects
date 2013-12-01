#include "ExplosionParticleSystem.h"
#include <MathUtils.h>

namespace pdh
{
	ExplosionParticleSystem::ExplosionParticleSystem(	size_t minNumParticles,
														size_t maxNumParticles,
														float minLifespan,
														float maxLifespan,
														float minLaunchSpeed,
														float maxLaunchSpeed,
														float emissionRate )
														:	m_minNumParticles( minNumParticles )
														,	m_maxNumParticles( maxNumParticles )
														,	m_minLifespan( minLifespan )
														,	m_maxLifespan( maxLifespan )
														,	m_minLaunchSpeed( minLaunchSpeed )
														,	m_maxLaunchSpeed( maxLaunchSpeed )
														,	m_emissionRate( emissionRate )
														,	m_currentEmissionTime( 0.0f )

	{}

	ExplosionParticleSystem::~ExplosionParticleSystem()
	{}

	State< vec3, float > ExplosionParticleSystem::simulationForces( 
		State< vec3, float > state, psIter iter, float t )
	{
		return State< vec3, float >( state[1], vec3() );
	}

	void ExplosionParticleSystem::updateSimulation( float deltaTime )
	{
		ParticleSystem::updateSimulation( deltaTime );
		if( m_emissionRate > 0 && m_currentEmissionTime >= m_emissionRate )
		{
			emitParticles();
			m_currentEmissionTime = 0;
		}
		else
		{
			m_currentEmissionTime += deltaTime;
		}
	}

	void ExplosionParticleSystem::emitParticles()
	{
		size_t numParticles = randInRange( m_minNumParticles, m_maxNumParticles );
		for( size_t i = 0; i < numParticles; ++i )
		{
			float angle1 = randInRange( 0.0f, 360.0f );
			float angle2 = randInRange( 0.0f, 360.0f );
			vec3 ranVec( cos( angle1 ), sin( angle1 ), sin( angle2 ) );
			float mag = randInRange( m_minLaunchSpeed, m_maxLaunchSpeed );
			float lifespan = randInRange( m_minLifespan, m_maxLifespan );
			m_particles.push_back( std::make_shared< Particle >( vec3(), ranVec * mag, 1.0f, lifespan ) );
		}
	}
}