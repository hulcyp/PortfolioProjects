#include "ConsoleOutputWindow.h"
#include "Renderer.h"
#include <MathFuncs.h>
#include "MeshFactory.h"
#include "Camera.h"

namespace Monky
{
	ConsoleOutputWindow::ConsoleOutputWindow( int width , int height, size_t bufferSize, Font* font, int fontHeight )
		:	m_consoleWidth( width )
		,	m_consoleHeight( height )
		,	m_font( font )
		,	m_buffer( bufferSize )
		,	m_startRenderIndex( 0 )
		,	m_endRenderIndex( 0 )
		,	m_fontHeight( fontHeight )
	{
		m_maxLinesToRender = m_consoleHeight / m_fontHeight + 1;

		m_meshBG = MeshFactory::generate2DOrthoRectangle( (float)m_consoleWidth, (float)m_consoleHeight, "simpleMaterial", color::FADEDBLACK );
		m_meshBorder = MeshFactory::generate2DOrthoRectangleBorder( (float)m_consoleWidth, (float)m_consoleHeight, "simpleMaterial", color::GREEN );

		m_outputParams.set( "fontName", m_font->getMaterial() );
		m_outputParams.set( "fontHeight", (float)m_fontHeight );
		m_outputParams.set( "posRel", std::string( "BOTLEFT" ) );
		m_outputParams.set( "pos", vec3f( 0.0f, (float)m_fontHeight ) );

		matStackf modelStack;
		modelStack.translate( 0.0f, (float)m_fontHeight, 0.0f );
		m_meshBorderParams.set( "modelMatrix", modelStack.top() );		
	}

	ConsoleOutputWindow::~ConsoleOutputWindow()
	{
		SAFE_DELETE( m_meshBG );
		SAFE_DELETE( m_meshBorder );
	}

	void ConsoleOutputWindow::render( Camera* camera )
	{
		mat4f modelMatrix( mat4f::IDENTITY );
		m_meshBorderParams.set( "camera", camera );
		m_meshBorderParams.set( "mesh", m_meshBG );
		fireEvent( "renderMesh", m_meshBorderParams );
		m_meshBorderParams.set( "mesh", m_meshBorder );
		fireEvent( "renderMesh", m_meshBorderParams );

		vec3f drawPos( 0.0f, 0.0f, 0.0f );
		std::string outputString;
		for( int i = m_endRenderIndex - 1; i >= m_startRenderIndex; --i )
		{
			if( m_buffer[i].second != "" )
			{
				char color[10];
				sprintf( color, "%x", m_buffer[i].first.getColorAsLong() );
				outputString += "<#"  + std::string( color ) + ">" + m_buffer[i].second + "\n";
			}
			drawPos.y += (float)m_fontHeight;
		}
		m_outputParams.set( "text", outputString );
		m_outputParams.set( "color", color::WHITE );
		modelMatrix.translate( drawPos );
		m_outputParams.set( "modelMatrix", modelMatrix );
		m_outputParams.set( "camera", camera );
		fireEvent( "renderText", m_outputParams );
	}

	void ConsoleOutputWindow::addString( const std::string& text, Color4f color )
	{		
		m_buffer.pushFront( std::pair< Color4f, std::string >( color, text ) );
		m_startRenderIndex = 0;
		m_endRenderIndex = MathFuncs<int>::clamp( m_startRenderIndex + m_maxLinesToRender, 0, (int)m_buffer.size() );
	}

	void ConsoleOutputWindow::moveUp()
	{		
		m_endRenderIndex = MathFuncs<int>::clamp( m_endRenderIndex + 1, 0, (int)m_buffer.size() );
		if( (int)m_buffer.size() - m_maxLinesToRender < 0 )
		{
			m_startRenderIndex = 0;
		}
		else
		{
			m_startRenderIndex = MathFuncs<int>::clamp( m_startRenderIndex + 1, 0, (int)m_buffer.size() - m_maxLinesToRender + 1 );
		}
	}

	void ConsoleOutputWindow::moveDown()
	{
		m_endRenderIndex = MathFuncs<int>::clamp( m_endRenderIndex - 1, std::min( m_maxLinesToRender, (int)m_buffer.size() ), (int)m_buffer.size() );
		m_startRenderIndex = MathFuncs<int>::clamp( m_startRenderIndex - 1, 0, std::max( (int)m_buffer.size(), (int)m_buffer.size() - m_maxLinesToRender ) );
	}

	void ConsoleOutputWindow::clearBuffer()
	{
		m_buffer.clear();
	}	
}