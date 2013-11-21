#include "GlutApp.h"
#include "Renderer.h"
#include <glut.h>

namespace Monky
{
#pragma warning( disable:4355 )
	GlutApp::GlutApp( int width, int height, float fov, const char* windowTitle )
		:	Singleton( this )
		,	m_screenWidth( width )
		,	m_screenHeight( height )
		,	m_currentFPS( 0.0f )
		,	m_frameCount( 0 )
		,	m_previousTime( 0 )
		,	m_windowTitle( windowTitle )
	{
		int argc = 0;
		char** argv = nullptr;

		glutInit( &argc, argv );
		glutInitWindowSize( m_screenWidth, m_screenHeight );
		glutInitWindowPosition( (int)( ( glutGet( GLUT_SCREEN_WIDTH ) - m_screenWidth ) * .5 ),
								(int)( ( glutGet( GLUT_SCREEN_HEIGHT ) - m_screenHeight ) * .5 ) );
		glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA );
		glutCreateWindow( windowTitle );

		

#ifdef _WIN32
		glewInit();
#endif
		
		glutDisplayFunc( glutUdpateFrame );
		glutKeyboardFunc( glutOnKeyDown );
		glutKeyboardUpFunc( glutOnKeyUp );
		glutSpecialFunc( glutOnSpecialKeyDown );
		glutSpecialUpFunc( glutOnSpecialKeyUp );
		glutPassiveMotionFunc( glutOnMouseMove );
		glutMotionFunc( glutOnMouseMove );
		glutMouseFunc( glutOnMouseButton );
		glutIdleFunc( glutIdleFunction );
		glutSetCursor( GLUT_CURSOR_NONE );

		m_glutSpecialKeyMappings[ GLUT_KEY_F1 ] = MONKY_F1;
		m_glutSpecialKeyMappings[ GLUT_KEY_F2 ] = MONKY_F2;
		m_glutSpecialKeyMappings[ GLUT_KEY_F3 ] = MONKY_F3;
		m_glutSpecialKeyMappings[ GLUT_KEY_F4 ] = MONKY_F4;
		m_glutSpecialKeyMappings[ GLUT_KEY_F5 ] = MONKY_F5;
		m_glutSpecialKeyMappings[ GLUT_KEY_F6 ] = MONKY_F6;
		m_glutSpecialKeyMappings[ GLUT_KEY_F7 ] = MONKY_F7;
		m_glutSpecialKeyMappings[ GLUT_KEY_F8 ] = MONKY_F8;
		m_glutSpecialKeyMappings[ GLUT_KEY_F9 ] = MONKY_F9;
		m_glutSpecialKeyMappings[ GLUT_KEY_F10 ] = MONKY_F10;
		m_glutSpecialKeyMappings[ GLUT_KEY_F11 ] = MONKY_F11;
		m_glutSpecialKeyMappings[ GLUT_KEY_F12 ] = MONKY_F12;

		m_glutSpecialKeyMappings[ GLUT_KEY_LEFT ] = MONKY_LEFT;
		m_glutSpecialKeyMappings[ GLUT_KEY_UP ] = MONKY_UP;
		m_glutSpecialKeyMappings[ GLUT_KEY_RIGHT ] = MONKY_RIGHT;
		m_glutSpecialKeyMappings[ GLUT_KEY_DOWN ] = MONKY_DOWN;
		m_glutSpecialKeyMappings[ GLUT_KEY_PAGE_UP ] = MONKY_PAGEUP;
		m_glutSpecialKeyMappings[ GLUT_KEY_PAGE_DOWN ] = MONKY_PAGEDOWN;
		m_glutSpecialKeyMappings[ GLUT_KEY_HOME ] = MONKY_HOME;
		m_glutSpecialKeyMappings[ GLUT_KEY_END ] = MONKY_END;
		m_glutSpecialKeyMappings[ GLUT_KEY_INSERT ] = MONKY_INSERT;

	}
#pragma warning( default:4355 )

	GlutApp::~GlutApp()
	{
	}

	void GlutApp::runApp()
	{
		glutMainLoop();
	}

	void GlutApp::updateFrame()
	{
		glutSwapBuffers();
		glutPostRedisplay();
	}

	void GlutApp::onKeyDown( unsigned char keyCode, int x, int y )
	{ 
		MonkyKey key = (MonkyKey)keyCode; 
		onKeyDown( key, x, y );
	}
	void GlutApp::onKeyUp( unsigned char keyCode, int x, int y )
	{
		MonkyKey key = (MonkyKey)keyCode; 
		onKeyUp( key, x, y );
	}
	void GlutApp::onSpecialKeyDown( int keyCode, int x, int y )
	{
		MonkyKey key = getSpecialKeyMappingFromGlut( keyCode ); 
		onKeyDown( key, x, y );
	}
	void GlutApp::onSpecialKeyUp( int keyCode, int x, int y )
	{
		MonkyKey key = getSpecialKeyMappingFromGlut( keyCode );
		onKeyUp( key, x, y );
	}
	void GlutApp::onMouseMove( int mouseX, int mouseY )
	{}
	void GlutApp::onMouseButton( int keyCode, int state, int mouseX, int mouseY )
	{
		onMouseButton( getMouseKeyCode( keyCode ), getMouseKeySate( state ), mouseX, mouseY );
	}

	void GlutApp::idle()
	{
		m_frameCount++;

		int currentTime = glutGet( GLUT_ELAPSED_TIME );
		int timeInterval = currentTime - m_previousTime;

		if( timeInterval > 1000 )
		{
			m_currentFPS = (float)( m_frameCount / ( timeInterval / 1000 ) );
			m_previousTime = currentTime;
			m_frameCount = 0;
		}
	}	
	
	//------------------------------------------------
	// Utility functions for current Framework
	//------------------------------------------------
	void GlutApp::warpPointer( int x, int y )
	{
		glutWarpPointer( x, y );
	}
	//------------------------------------------------
	void GlutApp::showCursor( bool show )
	{
		if( show )
			glutSetCursor( GLUT_CURSOR_LEFT_ARROW );
		else
			glutSetCursor( GLUT_CURSOR_NONE );
	}
	//------------------------------------------------
	int GlutApp::getModifiers()
	{
		return glutGetModifiers();
	}
	//------------------------------------------------
	MonkyKey GlutApp::getSpecialKeyMappingFromGlut( int keyCode ) const
	{
		return m_glutSpecialKeyMappings[ keyCode ];
	}
	//------------------------------------------------
	int GlutApp::getMouseKeyCode( int keyCode ) const
	{
		switch( keyCode )
		{
		case GLUT_LEFT_BUTTON:
			return MONKY_BUTTON_LEFT;
		case GLUT_RIGHT_BUTTON:
			return MONKY_BUTTON_RIGHT;
		case GLUT_MIDDLE_BUTTON:
			return MONKY_BUTTON_MIDDLE;
		case 3:
			return MONKY_BUTTON_WHEELUP;
		case 4:
			return MONKY_BUTTON_WHEELDOWN;
		default:
			return 0;
		}
	}
	//------------------------------------------------
	MonkyMouseButtonState GlutApp::getMouseKeySate( int state ) const
	{
		switch( state )
		{
		case GLUT_DOWN:
			return MONKY_MOUSE_BUTTON_DOWN;
		case GLUT_UP:
			return MONKY_MOUSE_BUTTON_UP;
		default:
			return MONKY_MOUSE_BUTTON_DOWN;
		}
	}

}