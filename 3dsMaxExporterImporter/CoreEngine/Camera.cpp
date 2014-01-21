#include "Camera.h"
#include <ErrorHandlingUtils.h>
#include <MathFuncs.h>
#include <glut.h>

namespace pdh
{
	Camera::Camera( const std::string& id, const vec3f& pos, const mat4f& projMatrix, const mat4f& viewMatrix  )
		:	Node( id )
		,	m_projectionMatrix( projMatrix )
		,	m_viewMatrix( viewMatrix )
	{
		setPosition( pos );
		for( int i = 0; i < 256; ++i )
		{
			m_keyboard[i] = false;
		}
	}

	Camera::~Camera()
	{}

	bool Camera::onKeyDown( unsigned char keyCode )
	{	
		keyCode = toupper(  keyCode );
		ASSERT( keyCode >= 0 && keyCode < 256 );
		m_keyboard[keyCode] = true;	
		return true;
	}

	bool Camera::onKeyUp( unsigned char keyCode )
	{
		keyCode = toupper( keyCode );
		m_keyboard[keyCode] = false;
		return true;
	}

	void Camera::update( double deltaTime )
	{	
		Node::update( deltaTime );
		m_dir = transformVector( vec3f( 0.0f, 0.0f, -1.0f ), getLocalRotation() );
	}

	const mat4f& Camera::getViewMatrix()
	{
		return m_viewMatrix;
	}

	const mat4f& Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

}