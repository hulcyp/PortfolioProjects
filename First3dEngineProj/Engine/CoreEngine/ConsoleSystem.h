#pragma once
#include <memory>
#include <string>
#include <Colors.h>
#include "CommandLine.h"

namespace pdh
{
	class Font;
	class ConsoleOutputWindow;
	class Renderer;

	class ConsoleSystem
	{
	public:

		enum Console
		{
			BOTH,
			COMMAND
		};

		ConsoleSystem( int width, int height, int bufferSize, Renderer* renderer );

		void printToConsole( const std::string& text, Color4f color = color::WHITE );
		void renderConsole( Console toRender = BOTH );
		bool onKeyDown( unsigned char keyCode );
		bool onMouseButton( int keyCode, int state );
		bool onSpecialKeyDown( int keyCode );
		void executeCommand( const std::string& cmd, const std::string& args );
		void registerCommand( const std::string& cmd, command cmdFunc, const std::string& helpText );

		bool rendering() { return m_render; }

	protected:
		void initializeCommandList();
		void scrollUp();
		void scrollDown();

	private:
		int m_width;
		int m_height;
		ConsoleOutputWindow* m_consoleOutput;
		CommandLine* m_commandLine;
		Font* m_font;
		Renderer* m_renderer;
		bool m_render;
	};
}