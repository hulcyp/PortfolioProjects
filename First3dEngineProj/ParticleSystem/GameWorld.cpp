#include "GameWorld.h"
#include <Renderer.h>

namespace pdh
{
	GameWorld::GameWorld( double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GlutApp( width, height, fov, windowTitle )
		,	DELTA_TIME( deltaTime )
	{}

	GameWorld::~GameWorld()
	{}

	void GameWorld::updateFrame()
	{
		updateSimulation();
		updateDisplay();
	}

	void GameWorld::onKeyDown( unsigned char keyCode, int x, int y )
	{
		if( m_camera )
		{
			m_camera->onKeyDown( keyCode );
		}
	}

	void GameWorld::onKeyUp( unsigned char keyCode, int x, int y )
	{
		if( m_camera )
		{
			m_camera->onKeyUp( keyCode );
		}
	}

	void GameWorld::onSpecialKeyDown( int keyCode, int x, int y )
	{
		switch( keyCode )
		{
		case GLUT_KEY_F4:
			exit( 0 );
			break;
		}
	}

	void GameWorld::onMouseMove( int mouseX, int mouseY )
	{
		if( m_camera )
		{
			m_camera->onMouseMove( mouseX, mouseY );
		}
	}

	//////////////////////////////////////////
	// Protected member functions
	/////////////////////////////////////////
	void GameWorld::updateSimulation()
	{}

	void GameWorld::updateDisplay()
	{
		glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		if( m_camera )
		{
			m_camera->updateInput();
			m_camera->updateSimulation( DELTA_TIME );
		}
	}
}