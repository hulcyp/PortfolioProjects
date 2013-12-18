#pragma once
#include "CommonEngineIncludes.h"
#include "SDLApp.h"
#include "Actor.h"
#include "ActorManager.h"
#include "Clock.h"

namespace Monky
{
	class Font;
	class ConsoleSystem;
	class GDOManager;
	class Camera;

	class GameApp : public SDLApp
	{
	public:
		GameApp( double deltaTime, int width, int height, float fov, const char* windowTitle, bool shouldInitConsole = true );
		virtual ~GameApp();
				
		virtual void initialize();
		virtual void cleanup();
		
		virtual void updateFrame();
		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y );

		virtual void onMouseMove( int mouseX, int mouseY );
		virtual bool onMouseButton( int keyCode, MonkyMouseButtonState state, int mouseX, int mouseY );

		void executeStartUpCmds( const std::string& filePath );
				
		void initializeCamera( Camera* camera );

		void spawn( Actor* actor );
	
	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

		virtual void renderHUD();
		virtual void renderDebugHUD();
		void renderConsole();

		void updateFPS();

	protected:
		const double DELTA_TIME;
		bool m_displayConsole;
		ConsoleSystem * m_console;
		bool m_wireFrame;
		Font* m_font;
		ActorManager m_actorManager;
		GDOManager* m_GDOManager;

		Renderer* m_renderer;
		Camera* m_camera;
		Clock m_frameClock;
		
		NamedProperties m_mainFontParams;
		NamedProperties m_memoryVisualizerParams;

		float m_currentFPS;
		float m_previousTime;
		int m_frameCount;

		bool m_freeMouse;

		bool m_renderAxis;
		bool m_renderDebugHUD;

		Camera* m_debugCamera;
	};
}