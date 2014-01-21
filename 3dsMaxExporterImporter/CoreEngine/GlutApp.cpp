#include "GlutApp.h"
#include "Renderer.h"

namespace pdh
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

		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glFrontFace( GL_CW );

#ifdef _WIN32
		glewInit();
#endif

		glMatrixMode( GL_PROJECTION );
		gluPerspective( fov,	static_cast< double >( m_screenWidth ) /
								m_screenWidth,
								0.1,
								100000.0 );

		m_renderer = new Renderer( m_screenWidth, m_screenHeight, fov, 0.1f, 100000.0f );

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
	}
#pragma warning( default:4355 )

	GlutApp::~GlutApp()
	{
		SAFE_DELETE( m_camera );
	}

	void GlutApp::startApp()
	{
		glutMainLoop();
	}

	void GlutApp::initializeCamera( Camera* camera )
	{
		m_camera = camera;
		m_renderer->pushCamera( m_camera );
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
}