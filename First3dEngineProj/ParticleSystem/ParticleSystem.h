#pragma once
#include "Particle.h"
#include <StateSpaceNumericalMethods.h>
#include <list>

namespace pdh
{
	typedef std::list< std::shared_ptr< Particle > >::iterator psIter;

	class Renderer;

	template< typename T, typename precision >
	class ParticleSystem
	{
	public:
		ParticleSystem();
		virtual ~ParticleSystem()
		{}

		virtual State< T, precision > simulationForces( State< T, precision > state, psIter iter, precision t ) = 0;

		virtual void updateSimulation( precision deltaTime );
		virtual void updateDisplay( Renderer* renderer );
		int getNumParticles() { return m_particles.size(); }

	protected:
		void destroyDeadParticles();

	protected:
		std::list< std::shared_ptr< Particle > > m_particles;
		float m_time;
	
	private:
		void rk4( psIter iter, precision t, precision dt )
		{
			precision h = dt;
			precision thalf = t + (precision)( h*.5 );
			precision tnext = t + h;
		
			State< T, precision > s = (*iter)->getState();
			State< T, precision > s1 = s;
		
			State< T, precision > f1 = simulationForces( s1, iter, t );
			State< T, precision > s2 = s + f1 * (precision)(h*.5);
		
			State< T, precision > f2 = simulationForces( s2, iter, thalf );
			State< T, precision > s3 = s + f2 * (precision)(h*.5);

			State< T, precision > f3 = simulationForces( s3, iter, thalf );
			State< T, precision > s4 = s + f3 * h;

			(*iter)->updateState( s + ( f1 + f2 * 2 + f3 * 2 + simulationForces( s4, iter, tnext ) ) * (h/6) );
		}

		void fwdEuler(  psIter iter, precision t, precision dt )
		{
			(*iter)->updateState( (*iter)->getState() + simulationForces( (*iter)->getState(), iter, t )*dt  );
		}
	};

	typedef ParticleSystem< vec3, float > particleSystem3f;
}