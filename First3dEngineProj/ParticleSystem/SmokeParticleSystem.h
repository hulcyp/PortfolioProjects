#pragma once
#include "ExplosionParticleSystem.h"

namespace pdh
{
	class SmokeParticleSystem : public ExplosionParticleSystem
	{
	public:
		SmokeParticleSystem(	size_t minNumParticles,
								size_t maxNumParticles,
								float minLifespan,
								float maxLifespan,
								float minLaunchSpeed,
								float maxLaunchSpeed,
								float emissionRate = 0 );

		virtual ~SmokeParticleSystem();

		virtual State< vec3, float > simulationForces(
			State< vec3, float > state, psIter iter, float t );
		virtual void emitParticles();

		void updateWindForce( const vec3& force ) { m_force = force; }

	protected:
		vec3 m_force;

	};
}