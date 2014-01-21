#pragma once
#include <memory>
#include <vector>
#include <Colors.h>
#include <string>
#include "Mesh.h"
#include "CircularBuffer.h"

namespace pdh
{
	class Font;
	class Renderer;

	class ConsoleOutputWindow
	{
	public:
		ConsoleOutputWindow( int width , int height, size_t bufferSize, Font* font, int fontHeight );

		void render( Renderer* renderer );
		void addString( const std::string& text, Color4f color = color::WHITE );
		void moveUp();
		void moveDown();
		void clearBuffer();


	private:
		int m_consoleWidth;
		int m_consoleHeight;
		const int m_fontHeight;
		Font* m_font;
		Mesh* m_meshBG;
		Mesh* m_meshBorder;
		CircularBuffer< std::pair< Color4f, std::string > > m_buffer;
		int m_startRenderIndex;
		int m_endRenderIndex;
		int m_maxLinesToRender;
	};
}