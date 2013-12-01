#pragma once
#include "ExplosionParticleSystem.h"

namespace pdh
{
	class FountainParticleSystem : public ExplosionParticleSystem
	{
	public:
		FountainParticleSystem(	size_t minNumParticles,
								size_t maxNumParticles,
								float minLifespan,
								float maxLifespan,
								float minLaunchSpeed,
								float maxLaunchSpeed,
								float minLaunchAngle,
								float maxLaunchAngle,
								float emissionRate = 0 );
		virtual ~FountainParticleSystem();

		virtual State< vec3, float > simulationForces( 
			State< vec3, float > state, psIter iter, float t );

		virtual void emitParticles();

	protected:
		float	m_minLaunchAngle;
		float	m_maxLaunchAngle;
		vec3	m_g;
	};
}