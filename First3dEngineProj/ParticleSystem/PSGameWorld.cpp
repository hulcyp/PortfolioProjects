#include "PSGameWorld.h"
#include <StringUtils.h>
#include <Renderer.h>
#include <Font.h>

namespace pdh
{
	PSGameWorld::PSGameWorld( double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GameWorld( deltaTime, width, height, fov, windowTitle )
	{
		m_font = m_renderer->getFont( "mainFont_72" );

		m_explosionPS = std::make_shared< ExplosionParticleSystem >(	100,		// min num particles
																		500,		// max num particles
																		0.1f,		// min lifespan
																		2.0f,		// max lifespan
																		1.0f,		// min launch speed
																		15.0f,		// max launch speed
																		1.0f );		// emission rate

		m_fireworksPS = std::make_shared< FireworksParticleSystem >( 100,		// min num particles
																	 500,		// max num particles
																	 0.1f,		// min lifespan
																	 2.0f,		// max lifespan
																	 1.0f,		// min launch speed
																	 15.0f,		// max launch speed
																	 1.0f );	// emission rate

		m_fountainPS = std::make_shared< FountainParticleSystem >(	50,			// min num particles
																	100,		// max num particles
																	0.1f,		// min lifespan
																	2.0f,		// max lifespan
																	5.0f,		// min launch speed
																	20.0f,		// max launch speed
																	-1.0f,		// min angle
																	1.0f,		// max angle
																	0.1f );		// emission rate

		m_smokePS = std::make_shared< SmokeParticleSystem >(	50,			// min num particles
																100,		// max num particles
																0.1f,		// min lifespan
																2.0f,		// max lifespan
																5.0f,		// min launch speed
																10.0f,		// max launch speed
																0.05f );		// emission rate

		m_ringPS = std::make_shared< RingParticleSystem >(	50, 100, 2.0f, 10.0f, 0.1f );
	}

	PSGameWorld::~PSGameWorld()
	{}

	///////////////////////////////////////////////////
	// Protected members
	//////////////////////////////////////////////////
	void PSGameWorld::updateSimulation()
	{
		m_explosionPS->updateSimulation( (float)DELTA_TIME );
		m_fireworksPS->updateSimulation( (float)DELTA_TIME );
		m_fountainPS->updateSimulation( (float)DELTA_TIME );
		m_smokePS->updateWindForce( vec3( 0.0f, 0.0f, -20.0f ) );
		m_smokePS->updateSimulation( (float)DELTA_TIME );
		m_ringPS->updateSimulation( (float)DELTA_TIME );
	}

	void PSGameWorld::updateDisplay()
	{
		GameWorld::updateDisplay();
		glLoadMatrixf( m_camera->getViewMatrix().getRawMatrix() );

		m_renderer->renderAxis( 10.0f );
		m_numParticles = 0;

		glPushMatrix();
		glTranslatef( 0.0f, 0.0f, -40.0f );
		m_explosionPS->updateDisplay( m_renderer );
		m_numParticles += m_explosionPS->getNumParticles();
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef( 40.0f, 40.0f, 0.0f );
		m_fireworksPS->updateDisplay( m_renderer );
		m_numParticles += m_fireworksPS->getNumParticles();
		glPopMatrix();

		glPushMatrix();
		glTranslatef( -40.0f, 0.0f, 0.0f );
		m_fountainPS->updateDisplay( m_renderer );
		m_numParticles += m_fountainPS->getNumParticles();
		glPopMatrix();

		glPushMatrix();
		glTranslatef( 0.0f, 0.0f, 40.0f );
		m_smokePS->updateDisplay( m_renderer );
		m_numParticles += m_smokePS->getNumParticles();
		glPopMatrix();

		m_ringPS->updateDisplay( m_renderer );
		m_numParticles += m_ringPS->getNumParticles();

		renderHUD();

		glPopMatrix();

		glutSwapBuffers();
	}

	void PSGameWorld::renderHUD()
	{
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		glOrtho( m_screenWidth/2.0, -m_screenWidth/2.0, -m_screenHeight/2.0, m_screenHeight/2.0, -1.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
		glPushAttrib( GL_DEPTH_TEST );
		glDisable( GL_DEPTH_TEST );
	
		m_font->renderText( "FPS: " + toString( m_currentFPS ), vec3f( -m_screenWidth*.5f, m_screenHeight * .5f ), 12.0f, color::GREEN, Font::TOPRIGHT, false );
		m_font->renderText( "Number of particles: " + toString( m_numParticles ), vec3f( vec3f( -m_screenWidth*.5f, m_screenHeight * .5f - 12.0f ) ), 24.0f, color::GREEN, Font::TOPRIGHT );
	
		glPopAttrib();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
	}
}