#pragma once
#ifdef _WIN32
#include <glew.h>
#endif
#include <Gl/GL.h>
#include <string>
#include <Singleton.h>
#include <MonkyKeys.h>
#include <MonkyMouse.h>

namespace Monky
{
	class Renderer;

	class GlutApp : public Singleton< GlutApp >
	{
	public:
		GlutApp( int width, int height, float fov, const char* windowTitle );
		virtual ~GlutApp();

		void runApp();
		
		virtual void updateFrame();
#pragma warning( disable:4100 )
		virtual void onKeyDown( unsigned char keyCode, int x, int y );
		virtual void onKeyUp( unsigned char keyCode, int x, int y );
		virtual void onSpecialKeyDown( int keyCode, int x, int y );
		virtual void onSpecialKeyUp( int keyCode, int x, int y );
		virtual void onMouseMove( int mouseX, int mouseY );
		virtual void onMouseButton( int keyCode, int state, int mouseX, int mouseY );

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y ){ return false; }
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y ){ return false; }
		virtual bool onMouseButton( int keyCode, MonkyMouseButtonState state, int x, int y ){ return false; }
#pragma warning( default:4100 )

		virtual void idle();
		
		const char* getWindowTitle() { return m_windowTitle; }
		
		///////////////////////////////////////////////////////////////////
		// Static member functions
		//////////////////////////////////////////////////////////////////
		static void glutUdpateFrame(){ getInstance()->updateFrame(); };
		static void glutOnKeyDown( unsigned char keyCode, int x, int y )
		{ getInstance()->onKeyDown( keyCode, x, y ); }
		
		static void glutOnKeyUp( unsigned char keyCode, int x, int y ) 
		{ getInstance()->onKeyUp( keyCode, x, y ); }

		static void glutOnSpecialKeyDown( int keyCode, int x, int y )
		{ getInstance()->onSpecialKeyDown( keyCode, x, y ); }

		static void glutOnSpecialKeyUp( int keyCode, int x, int y )
		{ getInstance()->onSpecialKeyUp( keyCode, x, y ); }

		static void glutOnMouseMove( int mouseX, int mouseY )
		{ getInstance()->onMouseMove( mouseX, mouseY ); }

		static void glutOnMouseButton( int keyCode, int state, int mouseX, int mouseY )
		{ getInstance()->onMouseButton( keyCode, state, mouseX, mouseY ); }

		static void glutIdleFunction()
		{ getInstance()->idle(); }

		static const char* sGetWindowTitle() { return getInstance()->getWindowTitle(); }

		
		static void warpPointer( int x, int y );
		static void showCursor( bool show );
		static int getModifiers();

	protected:
		int m_screenWidth;
		int m_screenHeight;
		float m_currentFPS;

	protected:
		MonkyKey getSpecialKeyMappingFromGlut( int keyCode ) const;
		int getMouseKeyCode( int keyCode ) const;
		MonkyMouseButtonState getMouseKeySate( int state ) const;

	private:
		int m_frameCount;
		int m_previousTime;
		const char* m_windowTitle;

		MonkyKey m_glutSpecialKeyMappings[108];
	};

}