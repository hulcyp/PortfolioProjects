#include "GameWorld.h"
#include "Renderer.h"
#include "CommonEngineIncludes.h"
#include "ConsoleSystem.h"
#include "Font.h"
#include "StringUtils.h"
#include "GDOManager.h"

namespace pdh
{
	GameWorld::GameWorld( double deltaTime, int width, int height, float fov, const char* windowTitle )
		:	GlutApp( width, height, fov, windowTitle )
		,	DELTA_TIME( deltaTime )
		,	m_time( 0.0f )
		,	m_displayConsole( false )
		,	m_wireFrame( false )
		,	m_renderAxis( false )
		,	m_renderDebugHUD( false )
		,	m_freeMouse( false )
	{
		m_console = initConsole( width, height, 500, m_renderer );
		m_GDOManager = initGDOManager( &m_actorManager );
		m_font = m_renderer->getFont( "mainFont_72" );
	}

	GameWorld::~GameWorld()
	{}

	void GameWorld::updateFrame()
	{
		updateDisplay();
	}

	bool GameWorld::onKeyDown( unsigned char keyCode, int x, int y )
	{
		bool used = false;
		switch( keyCode )
		{			
		case (char)27:
			exit( 0 );
			break;
		}
		if( m_console->onKeyDown( keyCode ) )
		{
			used = true;
		}
		else if( m_camera )
		{
			m_camera->onKeyDown( keyCode );
		}

		return used;
	}

	bool GameWorld::onKeyUp( unsigned char keyCode, int x, int y )
	{
		bool used = false;
		if( m_camera )
		{
			used = m_camera->onKeyUp( keyCode );
		}
		return used;
	}

	bool GameWorld::onSpecialKeyDown( int keyCode, int x, int y )
	{
		bool used = false;
		switch( keyCode )
		{
		case GLUT_KEY_F1:
			m_freeMouse = !m_freeMouse;
			if( m_freeMouse )
			{				
				glutSetCursor( GLUT_CURSOR_LEFT_ARROW );
			}
			else
			{
				glutSetCursor( GLUT_CURSOR_NONE );
			}
			used = true;
		case GLUT_KEY_F2:
			m_renderAxis = !m_renderAxis;
			used = true;
			break;
		case GLUT_KEY_F3:
			m_wireFrame = !m_wireFrame; 
			used = true;
			break;		
		case GLUT_KEY_F4:
			m_renderDebugHUD = !m_renderDebugHUD;
			used = true;
			break;
		}
		if( !used )
		{
			m_console->onSpecialKeyDown( keyCode );
			used = true;
		}
		return used;
	}

	bool GameWorld::onMouseMove( int mouseX, int mouseY )
	{
		if( m_camera )
		{
			m_camera->onMouseMove( mouseX, mouseY );
		}
		return true;
	}

	bool GameWorld::onMouseButton( int keyCode, int state, int mouseX, int mouseY )
	{
		return m_console->onMouseButton( keyCode, state );
	}


	//////////////////////////////////////////
	// Protected member functions
	/////////////////////////////////////////
	void GameWorld::updateSimulation()
	{
		m_time += static_cast< float >( DELTA_TIME );
		m_renderer->updateTimeOnMaterials( m_time );
	}

	void GameWorld::updateDisplay()
	{
		glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		matStackf& modelStack = m_renderer->getModelStack();
		modelStack.loadIdentity();

		if( m_camera && !m_freeMouse )
		{
			m_camera->updateInput();
			m_camera->updateSimulation( DELTA_TIME );
		}
		m_renderer->setWireFrame( m_wireFrame );
		m_actorManager.renderActors( m_renderer );
		m_GDOManager->renderGDOs( m_renderer, m_time );
		m_renderer->setWireFrame( false );
		renderHUD();	
	}

	void GameWorld::idle()
	{
		GlutApp::idle();
		updateSimulation();
		glutPostRedisplay();
	}

	void GameWorld::renderHUD()
	{
		
	}
	void GameWorld::renderDebugHUD()
	{
		if( m_renderDebugHUD )
		{
			matStackf& modelStack = m_renderer->getModelStack();
			m_renderer->enableOrthoRendering();
			modelStack.push();
			modelStack.loadIdentity();
			m_font->renderText( "FPS: " + toString( m_currentFPS ), vec3f( 0.0f, (float)m_screenHeight ), 12.0f, color::GREEN, Font::TOPRIGHT );		
			modelStack.pop();
			m_renderer->disableOrthoRendering();
		}
	}

	void GameWorld::renderConsole()
	{
		if( m_console )
		{
			m_console->renderConsole();
		}		
	}
}