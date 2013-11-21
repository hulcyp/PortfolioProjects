#include "GameApp.h"
#include "Renderer.h"
#include "CommonEngineIncludes.h"
#include "ConsoleSystem.h"
#include "StringUtils.h"
#include "GDOManager.h"
#include "EventSystemInterface.h"
#include "XMLParser.h"
#include "Time.h"
#include "MonkyKeys.h"
#include "InputSystem.h"
#include "JobManager.h"
#include "MemoryManager.h"

namespace Monky
{
	GameApp::GameApp( double deltaTime, int width, int height, float fov, const char* windowTitle, bool shouldInitConsole )
		:	SDLApp( width, height, fov, windowTitle )
		,	DELTA_TIME( deltaTime )
		,	m_displayConsole( false )
		,	m_wireFrame( false )
		,	m_renderAxis( false )
		,	m_renderDebugHUD( false )
		,	m_frameClock( nullptr )
		,	m_currentFPS( 0.0f )
		,	m_previousTime( 0.0f )
		,	m_frameCount( 0 )
		,	m_freeMouse( false )
		,	m_debugCamera( nullptr )
		,	m_console( nullptr )
		,	m_GDOManager( nullptr )
		,	m_font( nullptr )
		,	m_renderer( nullptr )
		,	m_camera( nullptr )
	{	

		if( windowTitle != nullptr )
		{
			m_renderer = new Renderer( m_screenWidth, m_screenHeight, fov, 0.1f, 100000.0f );
			m_renderer->loadRendereringDataFiles();
		}
		if( shouldInitConsole )
			m_console = initConsole( m_screenWidth, m_screenHeight, 500 );

		initialize();
	}
	//-------------------------------------------------------------------------------
	GameApp::~GameApp()
	{
		cleanup();
	}
	//-------------------------------------------------------------------------------
	void GameApp::initialize()
	{
		if( m_renderer != nullptr )
		{
			m_GDOManager = initGDOManager( &m_actorManager );
			m_font = Font::getFont( "mainFont_72" );
			showCursor( false );
			m_debugCamera = new Camera( "DebugCamera", vec3f(), Camera::PH_ORTHOGRAPHIC, mat4f::ortho( 0.0f, (float)m_screenWidth, (float)m_screenHeight, 0.0f, 1.0f, -1.0f ) );
		}

		executeStartUpCmds( "xml/StartupCmds.xml" );

		registerConsoleCommand( "event", "Fire event. Usage: [event name]",
			[&]( const std::string& args )
		{
			fireEvent( args );
		} );

		//getJobManager().setNumThreads( 4 );

		m_mainFontParams.set( "fontName", std::string( "mainFont_72" ) );
		m_mainFontParams.set( "fontHeight", 12.0f );
		m_mainFontParams.set( "color", color::GREEN );
		m_mainFontParams.set( "posRel", std::string( "TOPLEFT" ) );

		matStackf modelStack;
		modelStack.translate( 0.0f, (float)m_screenHeight - 108, 0.0f );
		m_memoryVisualizerParams.set( "modelMatrix", modelStack.top() );
	}
	//-------------------------------------------------------------------------------
	void GameApp::cleanup()
	{
		SAFE_DELETE( m_renderer );
		SAFE_DELETE( m_camera );	
		SAFE_DELETE( m_debugCamera );
		getJobManager().setNumThreads( 0 );
		cleanupGDOManager();
		cleanupConsole();
		m_GDOManager = nullptr;		
	}
	//-------------------------------------------------------------------------------
	void GameApp::updateFrame()
	{	
		processSystemEvents();		
		updateSimulation();
		updateDisplay();		
		getJobManager().update();
		getJobManager().fireEventsForMyCompletedJobs();
		updateMemoryManagerFrame();
		m_frameOwed = false;	
		updateFPS();	
		
		if( !m_frameOwed )
		{
			m_nextTimeFrameIsOwed = m_applicationClock.getElapsedSecondsFloat() + (float)DELTA_TIME;
		}
		ProfileSystem::getInstance()->clearFrameData();
	}
	//-------------------------------------------------------------------------------
	bool GameApp::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		
		if( m_console->onKeyDown( keyCode ) )
		{
			used = true;
		}
		else 
		{	
			InputSystem::getInstance()->onKeyDown( keyCode, x, y );
			switch( keyCode )
			{
			case MONKY_F1:
				m_freeMouse = !m_freeMouse;
				if( m_freeMouse )
				{				
					showCursor( true );
				}
				else
				{
					showCursor( false );
				}
				used = true;
				break;
			case MONKY_F2:
				m_renderAxis = !m_renderAxis;
				used = true;
				break;
			case MONKY_F3:
				m_wireFrame = !m_wireFrame; 
				used = true;
				break;		
			case MONKY_F4:
				m_renderDebugHUD = !m_renderDebugHUD;
				used = true;
				break;
			case MONKY_F10:
				exitProgram( 0 );
				break;
			}
		}
		return used;
	}
	//-------------------------------------------------------------------------------
	bool GameApp::onKeyUp( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		InputSystem::getInstance()->onKeyUp( keyCode, x, y );\
		return used;
	}
	//-------------------------------------------------------------------------------
	void GameApp::onMouseMove( int mouseX, int mouseY )
	{
		InputSystem::getInstance()->onMouseMove( mouseX, mouseY );
	}
	//-------------------------------------------------------------------------------
	bool GameApp::onMouseButton( int keyCode, MonkyMouseButtonState state, int mouseX, int mouseY )
	{
		bool used = false;
		if( m_console->onMouseButton( keyCode, state ) )
		{
			used = true;
		}
		else
		{
			InputSystem::getInstance()->onMouseButton( keyCode, state, mouseX, mouseY );
		}
		return used;
	}
	//-------------------------------------------------------------------------------
	void GameApp::executeStartUpCmds( const std::string& filePath )
	{
		XMLParser parser( filePath.c_str(), false );
		XMLDocument& doc = parser.getDocument();
		if( !doc.Error() )
		{
			const XMLNode* root = doc.FirstChildElement( "StartupCommands" );
			parser.validateXMLChildElements( root, "Command", "" );
			for( const XMLNode* command = root->FirstChildElement( "Command" ); command != nullptr; command = command->NextSiblingElement( "Command" ) )
			{
				std::string pcData = parser.getXMLElementPCDataAsString( command );
				if( pcData.size() > 0 )
					m_console->executeCommand( pcData );
			}
		}
	}
	//-------------------------------------------------------------------------------
	void GameApp::initializeCamera( Camera* camera )
	{
		if( camera != nullptr && m_renderer != nullptr )
		{
			m_camera = camera;
			m_renderer->pushCamera( m_camera );
			m_camera->registerForInput();
		}
	}
	//-------------------------------------------------------------------------------
	void GameApp::spawn( Actor* actor )
	{
		m_actorManager.addActor( actor );
	}

	//-------------------------------------------------------------------------------
	// Protected member functions
	//-------------------------------------------------------------------------------
	void GameApp::updateSimulation()
	{
		m_frameClock.advance( DELTA_TIME );
		Material::updateTimeOnMaterials( m_frameClock.getElapsedSecondsFloat() );
		m_actorManager.update( DELTA_TIME );
	}
	//-------------------------------------------------------------------------------
	void GameApp::updateDisplay()
	{
		PROFILE_SECTION( "GameApp" );
		glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		m_renderer->pushCamera( m_camera );
		if( m_camera && !m_freeMouse )
		{
			m_camera->updateInput();
			m_camera->update( DELTA_TIME );
		}
		m_renderer->setWireFrame( m_wireFrame );
		m_actorManager.renderActors();
		m_GDOManager->renderGDOs( m_renderer, m_frameClock.getElapsedSecondsFloat() );
		m_renderer->popCamera();
		ProfileSystem::getInstance()->clearFrameData();		
	}
	//-------------------------------------------------------------------------------
	void GameApp::renderHUD()
	{}
	//-------------------------------------------------------------------------------
	void GameApp::renderDebugHUD()
	{
		if( m_renderDebugHUD )
		{
			m_renderer->pushCamera( m_debugCamera );
			m_mainFontParams.set( "text", "FPS: " + floatToString( m_currentFPS, 0 ) );
			mat4f modelMatrix( mat4f::IDENTITY );
			modelMatrix.translate( vec3f( (float)m_screenWidth, (float) m_screenHeight ) );
			m_mainFontParams.set( "modelMatrix", modelMatrix );
			m_mainFontParams.set( "posRel", std::string( "TOPRIGHT" ) );
			fireEvent( "renderText", m_mainFontParams );
			
			m_mainFontParams.set( "posRel", std::string( "TOPLEFT" ) );
			modelMatrix.loadIdentity();
			modelMatrix.translate( vec3f( 0.0f, (float)m_screenHeight ) );
			m_mainFontParams.set( "modelMatrix", modelMatrix );
			renderMemoryUsageToHUD( m_mainFontParams );
			
			visualizeMemoryUsage( m_memoryVisualizerParams );
			ProfileSystem::getInstance()->renderData( vec3f( 50.0f, (float)m_screenHeight, 0.0f ), m_debugCamera );
			m_renderer->popCamera();
		}
	}
	//-------------------------------------------------------------------------------
	void GameApp::renderConsole()
	{
		if( m_console )
		{
			m_console->renderConsole();
		}		
	}
	//-------------------------------------------------------------------------------
	void GameApp::updateFPS()
	{
		++m_frameCount;
		
		float timeInterval = m_applicationClock.getElapsedSecondsFloat() - m_previousTime;

		if( timeInterval > 0.1f )
		{
			m_currentFPS = (float)( m_frameCount / timeInterval );
			m_previousTime = m_applicationClock.getElapsedSecondsFloat();
			m_frameCount = 0;
		}
	}
}