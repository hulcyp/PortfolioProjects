#include "RingParticleSystem.h"

namespace pdh
{
	RingParticleSystem::RingParticleSystem( size_t minNumParticles, size_t maxNumParticles,
											float lifespan, float launchSpeed, float emissionRate )
											:	m_minNumParticles( minNumParticles )
											,	m_maxNumParticles( maxNumParticles )
											,	m_lifespan( lifespan )
											,	m_launchSpeed( launchSpeed )
											,	m_emissionRate( emissionRate )
											,	m_currentEmissionTime( 0.0f )
											,	m_g( 0.0f, -9.81f, 0.0f )
											,	m_gravityWellLoc( 0.0f, 10.0f, 0.0f )
											,	m_gravityWellMag( 1000.0f )
	{
		emitParticles();
	}

	RingParticleSystem::~RingParticleSystem()
	{}

	State< vec3, float > RingParticleSystem::simulationForces( 
		State< vec3, float > state, psIter iter, float t )
	{
		vec3 gravityWellDir = m_gravityWellLoc - state[0];
		float dist = gravityWellDir.norm();
		gravityWellDir.normalize();

		return State< vec3, float >( state[1], m_g + gravityWellDir*m_gravityWellMag*(1/(dist*dist)) );
	}

	void RingParticleSystem::updateSimulation( float deltaTime )
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

	void RingParticleSystem::emitParticles()
	{
		size_t numParticles = randInRange( m_minNumParticles, m_maxNumParticles );
		float deltaAngle = numParticles / 360.0f;
		float angle = 0.0f;
		for( size_t i = 0; i < numParticles; ++i, angle += deltaAngle )
		{
			vec3 vel( cos( angle )*10.0f, m_launchSpeed, sin( angle )*10.0f );
			m_particles.push_back( std::make_shared< Particle >( vec3(), vel, 1.0f, m_lifespan ) );
		}
	}
}