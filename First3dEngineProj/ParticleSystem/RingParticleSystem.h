#pragma once
#include "ParticleSystem.h"

namespace pdh
{
	class RingParticleSystem : public particleSystem3f
	{
	public:
		RingParticleSystem( size_t minNumParticles, size_t maxNumParticles,
							float lifespan, float launchSpeed,
							float emissionRate = 0 );
		virtual ~RingParticleSystem();

		virtual State< vec3, float > simulationForces( 
			State< vec3, float > state, psIter iter, float t );
		virtual void updateSimulation( float deltaTime );

		virtual void emitParticles();

	protected:
		size_t m_minNumParticles;
		size_t m_maxNumParticles;
		float m_lifespan;
		float m_launchSpeed;
		float m_emissionRate; // If sero only emit once
		float m_currentEmissionTime;
		vec3 m_g;
		vec3 m_gravityWellLoc;
		float m_gravityWellMag;


	};
}