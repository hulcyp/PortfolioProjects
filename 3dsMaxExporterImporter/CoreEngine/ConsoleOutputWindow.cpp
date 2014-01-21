#include "ConsoleOutputWindow.h"
#include "Font.h"
#include "Renderer.h"
#include <MathFuncs.h>
#include "MeshFactory.h"

namespace pdh
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
	}

	void ConsoleOutputWindow::render( Renderer* renderer )
	{
		matStackf& modelStack = renderer->getModelStack();
		renderer->setProjectionMode( Renderer::PH_ORTHOGRAPHIC );
		modelStack.loadIdentity();
		modelStack.translate( 0.0f, (float)m_fontHeight, 0.0f );
		renderer->renderMesh( m_meshBG );
		renderer->renderMesh( m_meshBorder );

		vec3f drawPos( 0.0f, (float)m_fontHeight, 0.0f );
		for( int i = m_startRenderIndex; i < m_endRenderIndex; ++i )
		{
			if( m_buffer[i].second != "" )
			{
				m_font->renderText( m_buffer[i].second, drawPos, (float)m_fontHeight, m_buffer[i].first, Font::BOTLEFT );
			}
			drawPos.y += m_fontHeight;
		}

		modelStack.pop();
		//renderer->setProjectionMode( Renderer::PH_PERSPECTIVE );
		
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