#include "Camera.h"
#include <ErrorHandlingUtils.h>
#include <MathUtils.h>
#include <QuaternionLibrary.h>
#include <Matrix4Factory.h>
#include <glut.h>

namespace pdh
{
	Camera::Camera( const vec3f& pos )
		:	m_pos( pos )
		,	m_drag( 15.0f )
	{
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

	void Camera::updateSimulation( double deltaTime )
	{		
		float dt = static_cast<float>( deltaTime );

		mat4f yaw = convertToMatrix( makeQuaternionFromAxisAngle( degreesToRadians( m_rot.y ), vec3f( 0.0, 1.0, 0.0 ) ) );
		mat4f pitch = convertToMatrix( makeQuaternionFromAxisAngle( degreesToRadians( m_rot.x ),vec3f( 1.0, 0.0, 0.0 ) ) );

		mat4f rot = (pitch)*(yaw);
		
		vec3f tempVel = transformVector( m_vel, rot );

		m_pos += tempVel*dt;

		m_vel -= m_vel * m_drag * dt;
	
		m_dir = transformVector( vec3f( 0.0f, 0.0f, -1.0f ), rot, 0.0f );

		m_dir.normalize();

		mat4f itrans(	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						-m_pos.x, -m_pos.y, -m_pos.z, 1.0f );
		rot.transpose();
		m_matrix = itrans*rot;

	}

}