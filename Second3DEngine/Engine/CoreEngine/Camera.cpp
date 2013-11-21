#include "Camera.h"
#include <MonkyException.h>
#include <MathFuncs.h>

namespace Monky
{
	Camera::Camera( const std::string& id, const vec3f& pos, ProjectionMode projMode, const mat4f& projMatrix, const mat4f& viewMatrix  )
		:	Node( id )
		,	InputListener( false )
		,	m_projectionMatrix( projMatrix )
		,	m_viewMatrix( viewMatrix )
		,	m_projectionMode( projMode )
	{
		setPosition( pos );
		for( int i = 0; i < 256; ++i )
		{
			m_keyboard[i] = false;
		}
	}
	//-------------------------------------------
	Camera& Camera::operator=( const Camera& rhs )
	{
		if( this != &rhs )
		{
			m_id = rhs.m_id;
			m_projectionMatrix = rhs.m_projectionMatrix;
			m_viewMatrix = rhs.m_viewMatrix;
			m_projectionMode = rhs.m_projectionMode;
			setPosition( rhs.getLocalPosition() );
			m_dir = rhs.m_dir;
			m_up = rhs.m_up;
			for( int i = 0; i < 256; ++i )
			{
				m_keyboard[i] = rhs.m_keyboard[i];
			}
		}
		return *this;
	}
	//-------------------------------------------
	Camera::~Camera()
	{}
	//-------------------------------------------
	bool Camera::onKeyDown( MonkyKey keyCode, int x, int y )
	{	
		bool used = false;
		if( keyCode >= 0 && keyCode < 256 )
		{
			m_keyboard[keyCode] = true;	
			used = true;
		}
		return used;
	}
	//-------------------------------------------
	bool Camera::onKeyUp( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		if( keyCode >= 0 && keyCode < 256 )
		{
			m_keyboard[keyCode] = false;	
			used = true;
		}
		return used;
	}
	//-------------------------------------------
	void Camera::update( double deltaTime )
	{	
		Node::update( deltaTime );
		m_dir = transformVector( vec3f( 0.0f, 0.0f, -1.0f ), getLocalRotation() );
	}
	//-------------------------------------------
	const mat4f& Camera::getViewMatrix()
	{
		return m_viewMatrix;
	}
	//-------------------------------------------
	const mat4f& Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

}