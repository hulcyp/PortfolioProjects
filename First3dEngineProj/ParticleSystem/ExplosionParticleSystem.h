#pragma once
#include "ParticleSystem.h"

namespace pdh
{

	class ExplosionParticleSystem : public particleSystem3f
	{
	public:
		ExplosionParticleSystem(	size_t minNumParticles,
									size_t maxNumParticles,
									float minLifespan,
									float maxLifespan,
									float minLaunchSpeed,
									float maxLaunchSpeed,
									float emissionRate = 0 );
		virtual ~ExplosionParticleSystem();

		virtual State< vec3, float > simulationForces( 
			State< vec3, float > state, psIter iter, float t );
		virtual void updateSimulation( float deltaTime );

		virtual void emitParticles();

	protected:
		size_t	m_minNumParticles;
		size_t	m_maxNumParticles;
		float	m_minLifespan;
		float	m_maxLifespan;
		float	m_minLaunchSpeed;
		float	m_maxLaunchSpeed;
		float	m_emissionRate;		// If zero only emit once
		float	m_currentEmissionTime;
	};
}