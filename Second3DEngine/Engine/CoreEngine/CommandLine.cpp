#include "CommandLine.h"
#include "Mesh.h"
#include "Renderer.h"
#include "CommonEngineIncludes.h"
#include "StringUtils.h"
#include "MeshFactory.h"
#include "Camera.h"
#include "MonkyKeys.h"
#include "GameApp.h"

namespace Monky
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

		m_commandLineParams.set( "fontName", m_font->getMaterial() );
		m_commandLineParams.set( "fontHeight", (float)m_fontHeight );
		m_commandLineParams.set( "color", color::WHITE );
		m_commandLineParams.set( "posRel", std::string( "BOTLEFT" ) );

		m_cursorParams.set( "text", std::string( "|" ) );
		m_cursorParams.set( "fontName", m_font->getMaterial() );
		m_cursorParams.set( "fontHeight", (float)m_fontHeight );
		m_cursorParams.set( "color", color::WHITE );
		m_cursorParams.set( "posRel", std::string( "BOTCENTER" ) );
	}

	CommandLine::~CommandLine()
	{
		SAFE_DELETE( m_bgMesh );
		SAFE_DELETE( m_borderMesh );
		SAFE_DELETE( m_selectionMesh );
	}

	void CommandLine::render( Camera* camera )
	{
		matStackf modelStack;

		modelStack.translate( 0.0f, 1.0f, 0.0f );
		NamedProperties params;
		params.set( "modelMatrix", modelStack.top() );
		params.set( "camera", camera );
		params.set( "mesh", m_bgMesh );
		fireEvent( "renderMesh", params );
		params.set( "mesh", m_borderMesh );
		fireEvent( "renderMesh", params );

		modelStack.loadIdentity();
		m_commandLineParams.set( "modelMatrix", modelStack.top() );
		m_commandLineParams.set( "text", std::string( ">" ) );
		m_commandLineParams.set( "pos", vec3f() );
		m_commandLineParams.set( "camera", camera );
		fireEvent( "renderText", m_commandLineParams );
		
		modelStack.translate( vec3f( m_arrowLength, 0.0f, 0.0f ) );
		m_commandLineParams.set( "text", m_lineBuffer );
		m_commandLineParams.set( "modelMatrix", modelStack.top() );
		fireEvent( "renderText", m_commandLineParams ); 

		if( m_displayCursor )
		{
			float renderCursorPos = m_font->getLength( m_lineBuffer.substr( 0, m_cursorPos ), (float)m_fontHeight ) + m_arrowLength;
			modelStack.loadIdentity();
			modelStack.translate( vec3f( renderCursorPos, 0.0f, 0.0f ) );
			m_cursorParams.set( "modelMatrix", modelStack.top() );
			m_cursorParams.set( "camera", camera );
			fireEvent( "renderText", m_cursorParams );
		}

		buildSelectionMesh();
		if( m_selectionMesh )
		{
			modelStack.loadIdentity();
			params.set( "modelMatrix", modelStack.top() );
			params.set( "mesh", m_selectionMesh );
			fireEvent( "renderMesh", params );
			SAFE_DELETE( m_selectionMesh );
		}

		if( m_blinkTime > m_blinkRate )
		{
			m_displayCursor = !m_displayCursor;
			m_blinkTime = 0.0f;
		}
		m_blinkTime += m_deltaTime;
	}

	bool CommandLine::onKeyDown( MonkyKey keyCode )
	{
		bool used = false;
		
		std::string selection;
		std::string sub1;
		std::string sub2;
		
		switch( keyCode )
		{		
		// ctrl + C
		case MONKY_c:
			if( ( GameApp::getModifiers() & MONKY_MOD_CTRL ) != 0 )
			{
				getSelectionString( selection );
				m_clipboard = selection;
				used = true;
			}
			break;
		// ctrl + V
		case MONKY_v:
			if( ( GameApp::getModifiers() & MONKY_MOD_CTRL ) != 0 )
			{
				splitLineBuffer( sub1, sub2 );
				m_lineBuffer = sub1 + m_clipboard + sub2;
				m_cursorPos = sub1.length() + m_clipboard.length();
				m_prevCursPos = -1;
				used = true;
			}
			break;
		// ctrl + X
		case MONKY_x:
			if( ( GameApp::getModifiers() & MONKY_MOD_CTRL ) != 0 )
			{
				getSelectionString( selection );
				m_clipboard = selection;
				splitLineBuffer( sub1, sub2 );
				m_lineBuffer = sub1 + sub2;
				m_cursorPos = sub1.length();
				m_prevCursPos = -1;
				used = true;
			}
			break;
		//enter
		case MONKY_KP_ENTER:
		case MONKY_RETURN:
			//consolePrintf( m_lineBuffer.c_str() );
			parseInput( m_lineBuffer );
			m_history.pushFront( m_lineBuffer );
			m_lineBuffer.clear();
			used = true;
			m_cursorPos = 0;
			break;
		//backspace
		case MONKY_BACKSPACE:
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
		case MONKY_DELETE:
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
		case MONKY_LEFT:
			if( m_cursorPos > 0 )
			{
				if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) != 0 ) && m_prevCursPos == -1)
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) == 0 ) )
				{
					m_prevCursPos = -1;
				}
				--m_cursorPos;
			}
			used = true;
			break;
		case MONKY_RIGHT:
			if( m_cursorPos < (int)m_lineBuffer.size() )
			{
				if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) != 0 ) && m_prevCursPos == -1 )
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) == 0 ) )
				{
					m_prevCursPos = -1;
				}
				++m_cursorPos;
			}
			used = true;
			break;
		case MONKY_HOME:
			if( m_cursorPos > 0 )
			{
				if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) != 0 ) && m_prevCursPos == -1)
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) == 0 ) )
				{
					m_prevCursPos = -1;
				}
				m_cursorPos = 0;
			}
			used = true;
			break;
		case MONKY_END:
			if( m_cursorPos < (int)m_lineBuffer.size() )
			{
				if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) != 0 ) && m_prevCursPos == -1 )
				{
					m_prevCursPos = m_cursorPos;
				}
				else if( ( ( GameApp::getModifiers() & MONKY_MOD_SHIFT ) == 0 ) )
				{
					m_prevCursPos = -1;
				}
				m_cursorPos = m_lineBuffer.size();
			}
			used = true;
			break;
		case MONKY_UP:
			if( m_history.size() > 0 )
			{
				m_lineBuffer = m_history[ m_historyIndex ];
				m_historyIndex = MathFuncs<int>::wrap( m_historyIndex + 1, 0, (int)m_history.size() - 1 );
				m_cursorPos = m_lineBuffer.size();
			}
			used = true;
			break;
		case MONKY_DOWN:
			if( m_history.size() > 0 )
			{
				m_lineBuffer = m_history[ m_historyIndex ];
				if( m_historyIndex > 0 )
					m_historyIndex = MathFuncs<int>::wrap( m_historyIndex - 1, 0, (int)m_history.size() - 1 );
				m_cursorPos = m_lineBuffer.size();
			}
		}

		if( !used )
		{
			if( MONKY_KP0 <= keyCode && keyCode <= MONKY_KP_EQUALS )
				keyCode = getKeyFromKeypadCode( keyCode );
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
				sub1 += (char)keyCode;
				m_lineBuffer = sub1 + sub2;
				++m_cursorPos;
				used = true;
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

			std::vector< Mesh::Vertex > vertices( 6 );
			std::vector< unsigned int > indices( 6 );

			vertices[0] = ( Mesh::Vertex(	left, (float)m_fontHeight, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.5f, 0.5f, 1.0f, 0.5f,
											0.0f, 0.0f ) );
			vertices[1] = ( Mesh::Vertex(	right, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.5f, 0.5f, 1.0f, 0.5f,
											0.0f, 0.0f ) );
			vertices[2] = ( Mesh::Vertex(	left, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.5f, 0.5f, 1.0f, 0.5f,
											0.0f, 0.0f ) );
			vertices[3] = ( Mesh::Vertex(	right, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.5f, 0.5f, 1.0f, 0.5f,
											0.0f, 0.0f ) );
			vertices[4] = ( Mesh::Vertex(	left, (float)m_fontHeight, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.5f, 0.5f, 1.0f, 0.5f,
											0.0f, 0.0f ) );
			vertices[5] = ( Mesh::Vertex(	right, (float)m_fontHeight, 0.0f,
											0.0f, 0.0f, 0.0f,
											0.5f, 0.5f, 1.0f, 0.5f,
											0.0f, 0.0f ) );

			indices[0] = ( 0 );
			indices[1] = ( 1 );
			indices[2] = ( 2 );
			indices[3] = ( 3 );
			indices[4] = ( 4 );
			indices[5] = ( 5 );

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
	//----------------------------------------------------------------------
	MonkyKey CommandLine::getKeyFromKeypadCode( MonkyKey keyCode )
	{
		MonkyKey modifiedKeyCode = MONKY_0;
		switch( keyCode )
		{
		case MONKY_KP0:
			modifiedKeyCode = MONKY_0;
			break;
		case MONKY_KP1:
			modifiedKeyCode = MONKY_1;
			break;
		case MONKY_KP2:
			modifiedKeyCode = MONKY_2;
			break;
		case MONKY_KP3:
			modifiedKeyCode = MONKY_3;
			break;
		case MONKY_KP4:
			modifiedKeyCode = MONKY_4;
			break;
		case MONKY_KP5:
			modifiedKeyCode = MONKY_5;
			break;
		case MONKY_KP6:
			modifiedKeyCode = MONKY_6;
			break;
		case MONKY_KP7:
			modifiedKeyCode = MONKY_7;
			break;
		case MONKY_KP8:
			modifiedKeyCode = MONKY_8;
			break;
		case MONKY_KP9:
			modifiedKeyCode = MONKY_9;
			break;
		case MONKY_KP_PERIOD:
			modifiedKeyCode = MONKY_PERIOD;
			break;
		case MONKY_KP_DIVIDE:
			modifiedKeyCode = MONKY_SLASH;
			break;
		case MONKY_KP_MULTIPLY:
			modifiedKeyCode = MONKY_ASTERISK;
			break;
		case MONKY_KP_MINUS:
			modifiedKeyCode = MONKY_MINUS;
			break;
		case MONKY_KP_PLUS:
			modifiedKeyCode = MONKY_PLUS;
			break;
		case MONKY_KP_EQUALS:
			modifiedKeyCode = MONKY_EQUALS;
			break;
		}
		return modifiedKeyCode;
	}
}