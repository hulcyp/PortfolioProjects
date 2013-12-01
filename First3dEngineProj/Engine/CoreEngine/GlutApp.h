#pragma once
#ifdef _WIN32
#include <glew.h>
#endif
#include <glut.h>
#include <string>
#include <Singleton.h>
#include "Camera.h"

namespace pdh
{
	class Renderer;

	class GlutApp : public Singleton< GlutApp >
	{
	public:
		GlutApp( int width, int height, float fov, const char* windowTitle );
		virtual ~GlutApp();

		void startApp();
		
		virtual void updateFrame(){}
		virtual bool onKeyDown( unsigned char keyCode, int x, int y ){ return false; }
		virtual bool onKeyUp( unsigned char keyCode, int x, int y ){ return false; }
		virtual bool onSpecialKeyDown( int keyCode, int x, int y ){ return false; }
		virtual bool onMouseMove( int mouseX, int mouseY ){ return false; }
		virtual bool onMouseButton( int keyCode, int state, int mouseX, int mouseY ){ return false; }
		virtual void idle();
		
		void initializeCamera( Camera* camera );	
		
		
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

		static void glutOnMouseMove( int mouseX, int mouseY )
		{ getInstance()->onMouseMove( mouseX, mouseY ); }

		static void glutOnMouseButton( int keyCode, int state, int mouseX, int mouseY )
		{ getInstance()->onMouseButton( keyCode, state, mouseX, mouseY ); }

		static void glutIdleFunction()
		{ getInstance()->idle(); }

	protected:
		int m_screenWidth;
		int m_screenHeight;
		Camera* m_camera;
		float m_currentFPS;
		Renderer* m_renderer;

	private:
		int m_frameCount;
		int m_previousTime;
	};
}