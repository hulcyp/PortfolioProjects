#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "Mesh.h"
#include "CircularBuffer.h"
#include <functional>

namespace pdh
{
	typedef std::function< void( const std::string& ) > command;	
	
	class Font;
	class Renderer;
	
	class CommandLine
	{	
	public:
		CommandLine( int width, int height, int fontHeight, Font* font );

		void render( Renderer* renderer );
		bool onKeyDown( unsigned char keyCode );
		bool onSpecialKeyDown( int keyCode );
		void registerCommand( const std::string& cmdString, const std::string& helpText, command cmd );
		void executeCommand( const std::string& cmd, const std::string& args );
		void executeCommand( const std::string& inputLine );

	protected:
		void parseInput( const std::string& input );
		void buildBGMeshes();
		void buildSelectionMesh();
		void printHelp();
		void splitLineBuffer( std::string& sub1, std::string& sub2 );
		void getSelectionString( std::string& selection );
		

	private:
		int m_screenWidth;
		int m_screenHeight;
		int m_fontHeight;
		float m_arrowLength;
		Font* m_font;
		int m_cursorPos;
		int m_prevCursPos;
		std::string m_lineBuffer;
		std::map< std::string, std::pair< command, std::string > > m_commands;
		Mesh* m_bgMesh;
		Mesh* m_borderMesh;
		Mesh* m_selectionMesh;
		CircularBuffer< std::string > m_history;
		int m_historyIndex;
		float m_blinkRate;
		float m_blinkTime;
		float m_deltaTime;
		bool m_displayCursor;
		std::string m_clipboard;
	};
	
}