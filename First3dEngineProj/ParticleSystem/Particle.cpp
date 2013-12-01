#include "Particle.h"
#include <glut.h>
#include <Renderer.h>

namespace pdh
{
	Particle::Particle()
	{}
	Particle::Particle( vec3 pos, vec3 vel, float mass, float lifespan )
		:	m_currentState( pos, vel )
		,	m_mass( mass )
		,	m_age( 0.0f )
		,	m_lifespan( lifespan )
		,	m_isDead( false )
	{
		buildVBO();
	}

	void Particle::render( Renderer* renderer )
	{
		glPushMatrix();
		glTranslated(	m_currentState[0].x, 
						m_currentState[0].y, 
						m_currentState[0].z );
		glutWireSphere( 0.1, 10, 10 );
		glPopMatrix();
	}

	void Particle::updateState( State< vec3, float > newState )
	{ 
		m_currentState = newState;
	}

	void Particle::updateLifspan( float deltaTime )
	{
		m_age += deltaTime;
		if( m_lifespan > 0 && m_age >= m_lifespan )
		{
			m_isDead = true;
		}
	}

	void Particle::buildVBO()
	{										
	}
}
