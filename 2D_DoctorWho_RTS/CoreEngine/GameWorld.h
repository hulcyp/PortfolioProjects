#pragma once
#include "GlutApp.h"
#include <memory>
#include "Actor.h"
#include <vector>
#include "CommonEngineIncludes.h"
#include "ActorManager.h"

namespace pdh
{
	class Font;
	class ConsoleSystem;
	class GDOManager;

	class GameWorld : public GlutApp
	{
	public:
		GameWorld( double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~GameWorld();
		
		virtual void updateFrame();
		virtual bool onKeyDown( unsigned char keyCode, int x, int y );
		virtual bool onKeyUp( unsigned char keyCode, int x, int y );
		virtual bool onSpecialKeyDown( int keyCode, int x, int y );
		virtual bool onSpecialKeyUp( int keyCode, int x, int y );
		virtual bool onMouseMove( int mouseX, int mouseY );
		virtual bool onMouseButton( int keyCode, int state, int mouseX, int mouseY );

		void executeStartUpCmds( const std::string& filePath );
	
	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();
		virtual void idle();

		virtual void renderHUD();
		virtual void renderDebugHUD();
		void renderConsole();

	protected:
		const double DELTA_TIME;
		float m_time;
		bool m_displayConsole;
		ConsoleSystem * m_console;
		bool m_wireFrame;
		Font* m_font;
		ActorManager m_actorManager;
		GDOManager* m_GDOManager;

		bool m_freeMouse;

		bool m_renderAxis;
		bool m_renderDebugHUD;

		Camera* m_debugCamera;
	};
}