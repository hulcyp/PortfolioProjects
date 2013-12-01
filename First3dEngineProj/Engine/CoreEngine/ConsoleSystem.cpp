#include "ConsoleSystem.h"
#include "ConsoleOutputWindow.h"
#include "Renderer.h"
#include "Font.h"
#include "CommandLine.h"
#include "DefaultConsoleCommands.h"

namespace pdh
{
	ConsoleSystem::ConsoleSystem( int width, int height, int bufferSize, Renderer* renderer )
		:	m_width( width )
		,	m_height( height )
		,	m_renderer( renderer )
		,	m_render( false )
	{		
		m_renderer->createMaterial( "simpleMaterial", "simpleShader" );
		m_font = m_renderer->getFont( "consolas_24" );
		m_consoleOutput = new ConsoleOutputWindow( width, height - 24, bufferSize, m_font, 24 );		

		m_commandLine = new CommandLine( width, height, 24, m_font );

		initializeCommandList();
	}


	void ConsoleSystem::printToConsole( const std::string& text, Color4f color )
	{
		m_consoleOutput->addString( text, color );
	}

	void ConsoleSystem::renderConsole( Console toRender )
	{
		if( m_render )
		{
			if( toRender == BOTH )
			{
				m_consoleOutput->render( m_renderer );
				m_commandLine->render( m_renderer );
			}
		}
	}

	bool ConsoleSystem::onKeyDown( unsigned char keyCode )
	{	
		if( keyCode == '`' )
		{
			m_render = !m_render;
			return true;
		}
		if( m_render )
		{
			return m_commandLine->onKeyDown( keyCode );
		}
		else
		{
			return false;
		}
	}

	bool ConsoleSystem::onSpecialKeyDown( int keyCode )
	{
		bool used = false;
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
		return used;
	}

	bool ConsoleSystem::onMouseButton( int keyCode, int state )
	{
		bool used = false;
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

		return used;
	}

	void ConsoleSystem::executeCommand( const std::string& cmd, const std::string& args )
	{
		m_commandLine->executeCommand( cmd, args );
	}

	void ConsoleSystem::registerCommand( const std::string& cmd, command cmdFunc, const std::string& helpText )
	{
		m_commandLine->registerCommand( cmd, cmdFunc, helpText );
	}
	///////////////////////////////////////////
	// Protected member functions
	//////////////////////////////////////////
	void ConsoleSystem::initializeCommandList()
	{
		m_commandLine->registerCommand( "exit", exitCmd, "Exit the program" );
		m_commandLine->registerCommand( "print", printCmd, "Print text to the console" );
		m_commandLine->registerCommand( "dalek", dalekCmd, "Self explanatory" );
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