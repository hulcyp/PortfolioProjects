#include "GameWorld.h"
#include "Renderer.h"
#include "CommonEngineIncludes.h"
#include "ConsoleSystem.h"
#include "Font.h"
#include "StringUtils.h"
#include "GDOManager.h"
#include "DebugStatistics.h"
#include "EventSystemInterface.h"
#include "XMLParser.h"

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
		glutSetCursor( GLUT_CURSOR_LEFT_ARROW );
		m_debugCamera = new Camera( "DebugCamera", vec3f(), mat4f::ortho( 0.0f, (float)width, (float)height, 0.0f, 1.0f, -1.0f ) );
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
		
		if( m_console->onKeyDown( keyCode ) )
		{
			used = true;
		}
		else 
		{	
			if( m_camera )
			{
				m_camera->onKeyDown( keyCode );
			}
			NamedProperties params;
			params.set( "keyCode", keyCode );
			fireEvent( "onKeyDown", params );
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
		NamedProperties params;
		params.set( "keyCode", keyCode );
		fireEvent( "onKeyUp", params );
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
			break;
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
		case GLUT_KEY_F10:			
			exit( 0 );
			break;
		}
		if( !used )
		{
			used = m_console->onSpecialKeyDown( keyCode );
		}
		if( !used )
		{
			NamedProperties params;
			params.set( "keyCode", keyCode );
			fireEvent( "onSpecialKeyDown", params );
		}
		return used;
	}

	bool GameWorld::onSpecialKeyUp( int keyCode, int x, int y )
	{
		NamedProperties params;
		params.set( "keyCode", keyCode );
		fireEvent( "onSpecialKeyUp", params );
		return true;
	}

	bool GameWorld::onMouseMove( int mouseX, int mouseY )
	{
		if( m_camera )
		{
			m_camera->onMouseMove( mouseX, mouseY );
		}
		NamedProperties params;
		params.set( "mouseCoords", vec2i( mouseX, mouseY ) );
		fireEvent( "onMouseMove", params );
		return true;
	}

	bool GameWorld::onMouseButton( int keyCode, int state, int mouseX, int mouseY )
	{
		bool used = false;
		if( m_console->onMouseButton( keyCode, state ) )
		{
			used = true;
		}
		else
		{
			if( state == GLUT_DOWN )
			{
				vec2i screenCoords( mouseX, mouseY );
				NamedProperties params;
				params.set( "screenCoords", screenCoords );
				params.set( "keyCode", keyCode );
				fireEvent( "onMouseButtonDown", params );
			}
			else if( state == GLUT_UP )
			{
				vec2i screenCoords( mouseX, mouseY );
				NamedProperties params;
				params.set( "screenCoords", screenCoords );
				params.set( "keyCode", keyCode );
				fireEvent( "onMouseButtonUp", params );
			}
		}

		return used;
	}

	void GameWorld::executeStartUpCmds( const std::string& filePath )
	{
		XMLParser parser( filePath.c_str() );
		const XMLNode* root = parser.getDocument().FirstChildElement( "StartupCommands" );
		parser.validateXMLChildElements( root, "Command", "" );
		for( const XMLNode* command = root->FirstChildElement( "Command" ); command != nullptr; command = command->NextSiblingElement( "Command" ) )
		{
			m_console->executeCommand( parser.getXMLElementPCDataAsString( command ) );
		}
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
		m_renderer->pushCamera( m_camera );
		if( m_camera && !m_freeMouse )
		{
			m_camera->updateInput();
			m_camera->update( DELTA_TIME );
		}
		m_renderer->setWireFrame( m_wireFrame );
		m_actorManager.renderActors( m_renderer );
		m_GDOManager->renderGDOs( m_renderer, m_time );
	}

	void GameWorld::idle()
	{
		GlutApp::idle();
		updateSimulation();
		glutPostRedisplay();
		DebugStatistics::clearStats();
		//m_renderer->popCamera();
	}

	void GameWorld::renderHUD()
	{
		
	}
	void GameWorld::renderDebugHUD()
	{
		if( m_renderDebugHUD )
		{
			matStackf& modelStack = m_renderer->getModelStack();
			m_renderer->setProjectionMode( Renderer::PH_ORTHOGRAPHIC );
			modelStack.push();
			modelStack.loadIdentity();
			m_font->renderText( "FPS: " + toString( m_currentFPS ), vec3f( (float)m_screenWidth, (float)m_screenHeight - 12.0f ), 12.0f, color::GREEN, Font::TOPRIGHT );		
			m_font->renderText( "Material Swaps: " + toString( DebugStatistics::MATERIAL_SWAPS ), vec3f( (float)m_screenWidth, (float)( m_screenHeight - 24.0f ) ), 12.0f, color::WHITE, Font::TOPRIGHT );
			m_font->renderText( "Render Calls: " + toString( DebugStatistics::RENDER_CALLS ), vec3f( (float)m_screenWidth, (float)( m_screenHeight - 36.0f ) ), 12.0f, color::WHITE, Font::TOPRIGHT );
			modelStack.pop();			
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