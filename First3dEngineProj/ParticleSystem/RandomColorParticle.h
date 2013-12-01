#pragma once
#include "Particle.h"

namespace pdh
{
	class RandomColorParticle : public Particle
	{
	public:
		RandomColorParticle( vec3 pos, vec3 vel, float mass = 1.0f, float lifespan = 0 );
		virtual ~RandomColorParticle(){}

		virtual void render( Renderer* renderer );

	private:
		vec3 m_color;
	};
}
