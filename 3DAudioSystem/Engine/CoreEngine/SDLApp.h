#pragma once
#ifdef _WIN32
#include <glew.h>
#endif
#include <Gl/GL.h>
#include <SDL.h>
#include "InputSystem.h"
#include "Singleton.h"
#include "Clock.h"

namespace Monky
{

	class SDLApp : public Singleton< SDLApp >
	{
	public:
		SDLApp( int width, int height, float fov, const char* windowTitle );
		virtual ~SDLApp();

		virtual void initialize(){}
		virtual void cleanup(){}

		virtual void runApp();
		virtual void updateFrame(){}
	#pragma warning( disable:4100 )
		virtual void onMouseMove( int mouseX, int mouseY ){}
		virtual bool onKeyDown( MonkyKey keyCode, int x, int y ){ return false; }
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y ){ return false; }
		virtual bool onMouseButton( int keyCode, MonkyMouseButtonState state, int x, int y ){ return false; }
	#pragma warning( default:4100 )
		
		void exitProgram( int code );

		const char* getWindowTitle() { return m_windowTitle; }

		static void warpPointer( int x, int y );
		static void showCursor( bool show );
		static int getModifiers();

	protected:
		void processSystemEvents();
		void handleKeyDownEvent( SDL_keysym* keysym );
		void handleKeyUpEvent( SDL_keysym* keysym );

		int m_screenWidth;
		int m_screenHeight;

		Clock m_applicationClock;
		float m_nextTimeFrameIsOwed;
		bool m_frameOwed;

	private:
		const char* m_windowTitle;
		SDL_Event m_evtStruct;

		class QuitProgram : public std::exception
		{};
	};

}