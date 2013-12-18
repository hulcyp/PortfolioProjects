#include "ConsoleSystem.h"
#include "ConsoleOutputWindow.h"
#include "Renderer.h"
#include "Font.h"
#include "CommandLine.h"
#include "Camera.h"
#include "MonkyKeys.h"
#include "MonkyMouse.h"
#include "GameApp.h"

namespace Monky
{
	ConsoleSystem::ConsoleSystem( int width, int height, int bufferSize )
		:	m_width( width )
		,	m_height( height )
		,	m_render( false )
		,	m_whatToRender( BOTH )
	{		
		Material::createMaterial( "simpleMaterial", "simpleShader" );
		m_font = Font::getFont( "consolas_24" );
		m_consoleOutput = new ConsoleOutputWindow( width, height - 24, bufferSize, m_font, 24 );		

		m_commandLine = new CommandLine( width, height, 24, m_font );

		initializeCommandList();
		m_flatCamera = new Camera( "ConsoleCamera", vec3f(), Camera::PH_ORTHOGRAPHIC, mat4f::ortho( 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f ) );
	}

	ConsoleSystem::~ConsoleSystem()
	{
		SAFE_DELETE( m_flatCamera );
		SAFE_DELETE( m_consoleOutput );
		SAFE_DELETE( m_commandLine );
	}

	void ConsoleSystem::printToConsole( const std::string& text, Color4f color )
	{
		m_consoleOutput->addString( text, color );
	}

	void ConsoleSystem::renderConsole()
	{
		if( m_render )
		{
			if( m_whatToRender == BOTH )
			{
				m_consoleOutput->render( m_flatCamera );
				m_commandLine->render( m_flatCamera );
			}
			else if( m_whatToRender == COMMAND )
			{
				m_commandLine->render( m_flatCamera );
			}
		}
	}

	bool ConsoleSystem::onKeyDown( MonkyKey keyCode )
	{	
		bool used = false;
		if( keyCode == MONKY_BACKQUOTE )
		{
			m_render = !m_render;
			m_whatToRender = BOTH;
			used = m_render;
		}
		else if( (char)keyCode == (char)13 && !m_render )
		{
			m_render = !m_render;
			m_whatToRender = COMMAND;
			used = m_render;
		}
		else if( m_render )
		{
			used = m_commandLine->onKeyDown( keyCode );
			if( keyCode == (char)13 && !used )
			{
				m_render = false;
				used = true;
			}
			if( !used )
			{
				switch( keyCode )
				{
				case MONKY_PAGEUP:
					{		
						scrollUp();
						used = true;
						break;
					}		
				case MONKY_PAGEDOWN:
					{
						scrollDown();
						used = true;
						break;
					}
				}
			}
		}
		else
		{
			used =  false;
		}
		return used;
	}

	bool ConsoleSystem::onMouseButton( int keyCode, int state )
	{
		bool used = false;
		if( m_render )
		{
			if( keyCode == MONKY_BUTTON_WHEELUP)
			{
				if( state == MONKY_MOUSE_BUTTON_UP )
					used =  false;
				else
				{
					scrollUp();
					used = true;
				}
			}
			else if( keyCode == MONKY_BUTTON_WHEELDOWN )
			{
				if( state == MONKY_MOUSE_BUTTON_UP )
					used = false;
				else
				{
					scrollDown();
					used = true;
				}
			}
		}
		return used;
	}

	void ConsoleSystem::executeCommand( const std::string& cmd, const std::string& args )
	{
		m_commandLine->executeCommand( cmd, args );
	}
	void ConsoleSystem::executeCommand( const std::string& inputLine )
	{
		m_commandLine->executeCommand( inputLine );
	}
	void ConsoleSystem::registerCommand( const std::string& cmd, const std::string& helpText, command cmdFunc )
	{
		m_commandLine->registerCommand( cmd, helpText, cmdFunc );
	}
	///////////////////////////////////////////
	// Protected member functions
	//////////////////////////////////////////
	void ConsoleSystem::initializeCommandList()
	{
		m_commandLine->registerCommand( "exit", "Exit the program", []( const std::string& ){ GameApp::getInstance()->exitProgram( 0 ); } );
		m_commandLine->registerCommand( "quit", "Quit the program", []( const std::string& ){ GameApp::getInstance()->exitProgram( 0 ); } );
		m_commandLine->registerCommand( "dalek", "Self explanatory", 
			[]( const std::string& ) 
			{
				for( int i = 0; i < 10; ++i )
				{
					consolePrintColorf( "EXTERMINATE!!!", color::RED );
				}
			} );
	}
	void ConsoleSystem::scrollUp()
	{
		m_consoleOutput->moveUp();
	}
	void ConsoleSystem::scrollDown()
	{
		m_consoleOutput->moveDown();
	}
}