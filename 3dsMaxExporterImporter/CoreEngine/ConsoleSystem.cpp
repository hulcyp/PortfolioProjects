#include "ConsoleSystem.h"
#include "ConsoleOutputWindow.h"
#include "Renderer.h"
#include "Font.h"
#include "CommandLine.h"
#include "Camera.h"

namespace pdh
{
	ConsoleSystem::ConsoleSystem( int width, int height, int bufferSize, Renderer* renderer )
		:	m_width( width )
		,	m_height( height )
		,	m_renderer( renderer )
		,	m_render( false )
		,	m_whatToRender( BOTH )
	{		
		Renderer::createMaterial( "simpleMaterial", "simpleShader" );
		m_font = m_renderer->getFont( "consolas_24" );
		m_consoleOutput = new ConsoleOutputWindow( width, height - 24, bufferSize, m_font, 24 );		

		m_commandLine = new CommandLine( width, height, 24, m_font );

		initializeCommandList();
		m_flatCamera = new Camera( "ConsoleCamera", vec3f(), mat4f::ortho( 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f ) );
	}


	void ConsoleSystem::printToConsole( const std::string& text, Color4f color )
	{
		m_consoleOutput->addString( text, color );
	}

	void ConsoleSystem::renderConsole()
	{
		if( m_render )
		{
			m_renderer->pushCamera( m_flatCamera );
			m_renderer->setProjectionMode( Renderer::PH_ORTHOGRAPHIC );
			//m_renderer->pushProjectionMatrix( m_flatCamera->getProjectionMatrix() );
			if( m_whatToRender == BOTH )
			{
				m_consoleOutput->render( m_renderer );
				m_commandLine->render( m_renderer );
			}
			else if( m_whatToRender == COMMAND )
			{
				m_commandLine->render( m_renderer );
			}
			//m_renderer->popProjectionMatrix();
			m_renderer->popCamera();
		}
	}

	bool ConsoleSystem::onKeyDown( unsigned char keyCode )
	{	
		bool used = false;
		if( keyCode == '`' )
		{
			m_render = !m_render;
			m_whatToRender = BOTH;
			used = m_render;
		}
		else if( keyCode == (char)13 && !m_render )
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
		}
		else
		{
			used =  false;
		}
		return used;
	}

	bool ConsoleSystem::onSpecialKeyDown( int keyCode )
	{
		bool used = false;
		if( m_render )
		{
			switch( keyCode )
			{
			case GLUT_KEY_PAGE_UP:
				{		
					scrollUp();
					used = true;
					break;
				}		
			case GLUT_KEY_PAGE_DOWN:
				{
					scrollDown();
					used = true;
					break;
				}
			default:
				{
					used = m_commandLine->onSpecialKeyDown( keyCode );
					break;
				}
			}
		}
		return used;
	}

	bool ConsoleSystem::onMouseButton( int keyCode, int state )
	{
		bool used = false;
		if( m_render )
		{
			if( keyCode == 3 )
			{
				if( state == GLUT_UP )
					used =  false;
				else
				{
					scrollUp();
					used = true;
				}
			}
			else if( keyCode == 4 )
			{
				if( state == GLUT_UP )
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
		m_commandLine->registerCommand( "exit", "Exit the program", []( const std::string& ){ exit( 0 ); } );
		m_commandLine->registerCommand( "quit", "Quit the program", []( const std::string& ){ exit( 0 ); } );
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