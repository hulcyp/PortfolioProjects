#pragma once
#include "ExplosionParticleSystem.h"

namespace pdh
{
	class FireworksParticleSystem : public ExplosionParticleSystem
	{
	public:
		FireworksParticleSystem(	size_t minNumParticles,
									size_t maxNumParticles,
									float minLifespan,
									float maxLifespan,
									float minLaunchSpeed,
									float maxLaunchSpeed,
									float emissionRate = 0 );
		virtual ~FireworksParticleSystem();

		virtual State< vec3, float > simulationForces( 
			State< vec3, float > state, psIter iter, float t );

		virtual void emitParticles();
		
	protected:
		vec3 m_g;
	};
}