#include "CommandLine.h"
#include "Font.h"
#include "Mesh.h"
#include "Renderer.h"
#include <glut.h>
#include "CommonEngineIncludes.h"
#include "StringUtils.h"
#include "MeshFactory.h"

namespace pdh
{
	CommandLine::CommandLine( int width, int height, int fontHeight, Font* font )
		:	m_screenWidth( width )
		,	m_screenHeight( height )
		,	m_fontHeight( fontHeight )
		,	m_font( font )
		,	m_cursorPos( 0 )
		,	m_prevCursPos( -1 )
		,	m_history( 10 )
		,	m_historyIndex( 0 )
		,	m_blinkRate( 0.25f )
		,	m_blinkTime( 0.0f )
		,	m_deltaTime( 1/60.0f )
		,	m_displayCursor( true )
		,	m_selectionMesh( nullptr )
	{		
		m_arrowLength = m_font->getLength( ">", (float)m_fontHeight );
		m_bgMesh = MeshFactory::generate2DOrthoRectangle( (float)m_screenWidth, (float)m_fontHeight, "SimpleColorMaterial", color::BLACK );
		m_borderMesh = MeshFactory::generate2DOrthoRectangleBorder( (float)m_screenWidth, (float)m_fontHeight - 1.0f, "SimpleColorMaterial", color::GREEN );
	}

	void CommandLine::render( Renderer* renderer )
	{
		matStackf& modelStack = renderer->getModelStack();
		renderer->setProjectionMode( Renderer::PH_ORTHOGRAPHIC );
		modelStack.loadIdentity();

		modelStack.translate( 0.0f, 1.0f, 0.0f );
		renderer->renderMesh( m_bgMesh );
		renderer->renderMesh( m_borderMesh );
		modelStack.loadIdentity();

		m_font->renderText( ">", vec3f( 0.0f, 0.0f, 0.0f ), (float)m_fontHeight, color::WHITE, Font::BOTLEFT );
		m_font->renderText( m_lineBuffer, 
			vec3f( m_arrowLength, 0.0f, 0.0f ), (float)m_fontHeight, color::WHITE, Font::BOTLEFT );
		
		if( m_displayCursor )
		{
			float renderCursorPos = m_font->getLength( m_lineBuffer.substr( 0, m_cursorPos ), (float)m_fontHeight ) + m_arrowLength;
			m_font->renderText( "|", vec3f( renderCursorPos, 0.0f, 0.0f ), (float)m_fontHeight, color::WHITE, Font::BOTCENTER );
		}

		buildSelectionMesh();
		if( m_selectionMesh )
		{
			renderer->renderMesh( m_selectionMesh );
			SAFE_DELETE( m_selectionMesh );
		}

		renderer->setProjectionMode( Renderer::PH_PERSPECTIVE );

		if( m_blinkTime > m_blinkRate )
		{
			m_displayCursor = !m_displayCursor;
			m_blinkTime = 0.0f;
		}
		m_blinkTime += m_deltaTime;
	}

	bool CommandLine::onKeyDown( unsigned char keyCode )
	{
		bool used = false;
		
		if( 32 <= keyCode && keyCode < 127 )
		{			
			
			std::string sub1;
			std::string sub2;
			if( m_prevCursPos != -1 )
			{
				splitLineBuffer( sub1, sub2 );
				m_cursorPos = sub1.length();
				m_prevCursPos = -1;
			}
			else
			{
				sub1 = m_lineBuffer.substr(	0, m_cursorPos );
				sub2 = m_lineBuffer.substr( m_cursorPos, std::string::npos );
			}
			sub1 += keyCode;
			m_lineBuffer = sub1 + sub2;
			++m_cursorPos;
			used = true;
		}

		std::string selection;
		std::string sub1;
		std::string sub2;
		
		switch( keyCode )
		{		
		// ctrl + C
		case (char)3:
			getSelectionString( selection );
			m_clipboard = selection;
			used = false;
			break;
		// ctrl + V
		case (char)22:
			splitLineBuffer( sub1, sub2 );
			m_lineBuffer = sub1 + m_clipboard + sub2;
			m_cursorPos = sub1.length() + m_clipboard.length();
			m_prevCursPos = -1;
			used = false;
			break;
		// ctrl + X
		case (char)24:
			getSelectionString( selection );
			m_clipboard = selection;
			splitLineBuffer( sub1, sub2 );
			m_lineBuffer = sub1 + sub2;
			m_cursorPos = sub1.length();
			m_prevCursPos = -1;
			used = false;
			break;
		//enter
		case (char)13:
			//consolePrintf( m_lineBuffer.c_str() );
			parseInput( m_lineBuffer );
			m_history.pushFront( m_lineBuffer );
			m_lineBuffer.clear();
			used = true;
			m_cursorPos = 0;
			break;
		//backspace
		case (char)8:
			if( m_lineBuffer.size() > 0 )
			{
				if( m_cursorPos == m_lineBuffer.size() && m_prevCursPos == -1 )
				{
					m_lineBuffer.pop_back();
				}
				else if( m_prevCursPos != -1 )
				{
					std::string sub1;
					std::string sub2;
					splitLineBuffer( sub1, sub2 );
					m_lineBuffer = sub1 + sub2;
					m_cursorPos = sub1.length() + 1;
					m_prevCursPos = -1;
				}
				else
				{
					std::string sub1 = m_lineBuffer.substr( 0, m_cursorPos );
					sub1.pop_back();
					m_lineBuffer = sub1 + m_lineBuffer.substr( m_cursorPos, std::string::npos );
				}
				--m_cursorPos;
			}
			used = true;
			break;
		//delete
		case (char)127:
			if( m_cursorPos < (int)m_lineBuffer.size() && m_prevCursPos == -1 )
			{
				std::string sub1 = m_lineBuffer.substr( 0, m_cursorPos + 1 );
				sub1.pop_back();
				m_lineBuffer = sub1 + m_lineBuffer.substr( m_cursorPos + 1, std::string::npos );
			}
			else if( m_prevCursPos != -1 )
			{
				std::string sub1;
				std::string sub2;
				splitLineBuffer( sub1, sub2 );
				m_lineBuffer = sub1 + sub2;
				m_cursorPos = sub1.length();				
				m_prevCursPos = -1;
			}
			used = true;
			break;
		}

		return used;
	}

	bool CommandLine::onSpecialKeyDown( int keyCode )
	{
		bool used = false;
		switch( keyCode )
		{
		case GLUT_KEY_LEFT:
			if( m_cursorPos > 0 )
			{
				if( glutGetModifiers() == GLUT_ACTIVE_SHIFT && m_prevCursPos == -1)
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
				{
					m_prevCursPos = -1;
				}
				--m_cursorPos;
			}
			used = true;
			break;
		case GLUT_KEY_RIGHT:
			if( m_cursorPos < (int)m_lineBuffer.size() )
			{
				if( glutGetModifiers() == GLUT_ACTIVE_SHIFT && m_prevCursPos == -1 )
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
				{
					m_prevCursPos = -1;
				}
				++m_cursorPos;
			}
			used = true;
			break;
		case GLUT_KEY_HOME:
			if( m_cursorPos > 0 )
			{
				if( glutGetModifiers() == GLUT_ACTIVE_SHIFT && m_prevCursPos == -1)
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
				{
					m_prevCursPos = -1;
				}
				m_cursorPos = 0;
			}
			used = true;
			break;
		case GLUT_KEY_END:
			if( m_cursorPos < (int)m_lineBuffer.size() )
			{
				if( glutGetModifiers() == GLUT_ACTIVE_SHIFT && m_prevCursPos == -1 )
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( glutGetModifiers() != GLUT_ACTIVE_SHIFT )
				{
					m_prevCursPos = -1;
				}
				m_cursorPos = m_lineBuffer.size();
			}
			used = true;
			break;
		case GLUT_KEY_UP:
			if( m_history.size() > 0 )
			{
				m_lineBuffer = m_history[ m_historyIndex ];
				m_historyIndex = MathFuncs<int>::wrap( m_historyIndex + 1, 0, (int)m_history.size() - 1 );
				m_cursorPos = m_lineBuffer.size();
			}
			used = true;
			break;
		case GLUT_KEY_DOWN:
			if( m_history.size() > 0 )
			{
				m_lineBuffer = m_history[ m_historyIndex ];
				if( m_historyIndex > 0 )
					m_historyIndex = MathFuncs<int>::wrap( m_historyIndex - 1, 0, (int)m_history.size() - 1 );
				m_cursorPos = m_lineBuffer.size();
			}
		}
		return used;
	}

	void CommandLine::registerCommand( const std::string& cmdString, const std::string& helpText, command cmd )
	{
		std::string commandString = cmdString;
		stringToLowerCase( commandString );
		m_commands[ commandString ] = std::pair< command, std::string >( cmd, helpText );
	}
	void CommandLine::executeCommand( const std::string& cmd, const std::string& args )
	{
		if( m_commands.find( cmd ) != m_commands.end() )
		{
			m_commands[ cmd ].first( args );
			consolePrintColorf( (cmd + " " + args).c_str(), color::WHITE );
		}
		else
		{
			consolePrintColorf( "Invalid command: %s", color::RED, cmd.c_str() );
		}
	}
	void CommandLine::executeCommand( const std::string& inputLine )
	{
		parseInput( inputLine );
	}

	void CommandLine::parseInput( const std::string& input )
	{
		size_t firstSpace = input.find_first_of( " " );
		std::string command = input.substr( 0, firstSpace );
		stringToLowerCase( command );
		if( firstSpace == std::string::npos )
		{
			if( command == "help" )
			{
				printHelp();
			}
			else
			{
				executeCommand( command, "" );
			}
		}
		else
		{
			executeCommand( command, input.substr( firstSpace + 1, std::string::npos ) );
		}
	}

	void CommandLine::buildSelectionMesh()
	{
		if( m_prevCursPos != -1 )
		{			
			float currentPosLength = m_font->getLength( m_lineBuffer.substr( 0, m_cursorPos  ), (float)m_fontHeight );
			float prevPosLength = m_font->getLength( m_lineBuffer.substr( 0, m_prevCursPos ), (float)m_fontHeight );

			float left = ( m_cursorPos > m_prevCursPos ? prevPosLength : currentPosLength ) + m_arrowLength;
			float right = ( m_cursorPos > m_prevCursPos ? currentPosLength : prevPosLength ) + m_arrowLength;

			std::vector< Mesh::Vertex > vertices;
			std::vector< unsigned int > indices;

			vertices.push_back( Mesh::Vertex(	left, (float)m_fontHeight, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.5f, 0.5f, 1.0f, 0.5f,
												0.0f, 0.0f ) );
			vertices.push_back( Mesh::Vertex(	right, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.5f, 0.5f, 1.0f, 0.5f,
												0.0f, 0.0f ) );
			vertices.push_back( Mesh::Vertex(	left, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.5f, 0.5f, 1.0f, 0.5f,
												0.0f, 0.0f ) );
			vertices.push_back( Mesh::Vertex(	right, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.5f, 0.5f, 1.0f, 0.5f,
												0.0f, 0.0f ) );
			vertices.push_back( Mesh::Vertex(	left, (float)m_fontHeight, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.5f, 0.5f, 1.0f, 0.5f,
												0.0f, 0.0f ) );
			vertices.push_back( Mesh::Vertex(	right, (float)m_fontHeight, 0.0f,
												0.0f, 0.0f, 0.0f,
												0.5f, 0.5f, 1.0f, 0.5f,
												0.0f, 0.0f ) );

			indices.push_back( 0 );
			indices.push_back( 1 );
			indices.push_back( 2 );
			indices.push_back( 3 );
			indices.push_back( 4 );
			indices.push_back( 5 );

			m_selectionMesh = new Mesh( vertices, indices, "simpleMaterial" );
			
		}
	}


	void CommandLine::printHelp()
	{
		for( auto iter = m_commands.begin(); iter != m_commands.end(); ++iter )
		{
			consolePrintColorf( (iter->first + " - " + iter->second.second).c_str(), color::GREEN );
		}
		consolePrintColorf( "help - Print the list of commands", color::GREEN );
	}

	void CommandLine::splitLineBuffer( std::string& sub1, std::string& sub2 )
	{
		if( m_prevCursPos != -1 )
		{
			int leftCursPos = ( m_cursorPos > m_prevCursPos ? m_prevCursPos : m_cursorPos );
			int rightCursPos = ( m_cursorPos > m_prevCursPos ? m_cursorPos : m_prevCursPos );
			sub1 = m_lineBuffer.substr( 0, leftCursPos );
			sub2 = m_lineBuffer.substr( rightCursPos, std::string::npos );
		}
		else
		{
			sub1 = m_lineBuffer.substr(	0, m_cursorPos );
			sub2 = m_lineBuffer.substr( m_cursorPos, std::string::npos );
		}
	}

	void CommandLine::getSelectionString( std::string& selection )
	{
		int leftCursPos = ( m_cursorPos > m_prevCursPos ? m_prevCursPos : m_cursorPos );
		int rightCursPos = ( m_cursorPos > m_prevCursPos ? m_cursorPos : m_prevCursPos );
		selection = m_lineBuffer.substr( leftCursPos, rightCursPos - leftCursPos );	
	}
}