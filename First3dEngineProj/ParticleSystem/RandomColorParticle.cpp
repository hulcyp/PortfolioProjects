#include "RandomColorParticle.h"
#include <VectorUtils.h>

namespace pdh
{
	RandomColorParticle::RandomColorParticle( vec3 pos, vec3 vel, float mass, float lifespan ) 
		:	Particle( pos, vel, mass, lifespan )
		,	m_color( randVec3InRange( 0.0f, 1.0f ) )
	{}

	void RandomColorParticle::render( Renderer* renderer )
	{
		glPushMatrix();
		glPushAttrib( GL_CURRENT_BIT );
		glColor3f( m_color.x, m_color.y, m_color.z );
		Particle::render( renderer );
		glPopAttrib();
		glPopMatrix();
	}
}