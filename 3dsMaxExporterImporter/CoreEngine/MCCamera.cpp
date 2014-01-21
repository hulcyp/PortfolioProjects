#include "GlutApp.h"
#include "MCCamera.h"
#include <MathFuncs.h>
#include <ErrorHandlingUtils.h>


namespace pdh
{
	MCCamera::MCCamera( const std::string& id, const vec3f& pos, const vec2i& mousPos, const mat4f& projMatrix, const mat4f& viewMatrix )
		:	Camera( id, pos, projMatrix, viewMatrix )
		,	MOUSE_PITCH_POWER( 0.06f )
		,	KEYBOARD_IMPULSE_POWER( 7.0f )
		,	BOOST_FACTOR( 20.0f )
		,	MOUSE_YAW_POWER( 0.06f )
		,	m_impulse( 0.0f, 0.0f, 0.0f )
		,	m_mouseCenterPos( mousPos )
		,	m_boost( false )
		,	m_mouseInputUpdated( false )
		,	m_drag( 15.0f )
	{}

	MCCamera::~MCCamera()
	{}

	void MCCamera::onMouseMove( int x, int y )
	{
		if( !m_mouseInputUpdated )
		{
			float mouseDx = static_cast<float>( x - m_mouseCenterPos.x);
			float mouseDy = static_cast<float>( y - m_mouseCenterPos.y );

			m_rot.y -= mouseDx * MOUSE_YAW_POWER;

			m_rot.x -= mouseDy * MOUSE_PITCH_POWER;

			m_rot.x = MathFuncs<float>::clamp( m_rot.x, -90.0f, 90.0f );
		
			m_mouseInputUpdated = true;
		}
	}

	void MCCamera::updateInput()
	{

		glutWarpPointer( m_mouseCenterPos.x, m_mouseCenterPos.y );

		m_impulse = vec3f( 0.0, 0.0, 0.0 );

		if( m_keyboard[ 'W' ] )
		{
			m_impulse.z -= 1.0;	
		}
		if( m_keyboard[ 'S' ] )
		{
			m_impulse.z += 1.0;
		}

		if( m_keyboard[ 'A' ] )
		{
			m_impulse.x -= 1.0;
		}
		if( m_keyboard[ 'D' ] )
		{
			m_impulse.x += 1.0;
		}
		if( m_keyboard[ 'E' ] )
		{
			m_impulse.y += 1.0;
		}
		if( m_keyboard[ 'Q' ] )
		{
			m_impulse.y -= 1.0;
		}
		
		m_impulse.normalize();
		
		if( m_boost )
		{
			m_impulse *= BOOST_FACTOR;
		}

		m_impulse *= KEYBOARD_IMPULSE_POWER;
		m_vel += m_impulse;

		m_mouseInputUpdated = false;
	}
	
	void MCCamera::update( double deltaTime )
	{		
		float dt = static_cast<float>( deltaTime );

		mat3f yaw = mat3f::createMatrixFromQuaternion( quatf::makeQuaternionFromAxisAngle( MathFuncs<float>::degreesToRadians( -m_rot.y ), vec3f( 0.0, 1.0, 0.0 ) ) );
		mat3f pitch = mat3f::createMatrixFromQuaternion( quatf::makeQuaternionFromAxisAngle( MathFuncs<float>::degreesToRadians( -m_rot.x ),vec3f( 1.0, 0.0, 0.0 ) ) );

		setRotation( (pitch)*(yaw) );
		
		vec3f tempVel = transformVector( m_vel, getLocalRotation() );

		m_frames[ m_currentFrame ].position += tempVel*dt;

		m_vel -= m_vel * m_drag * dt;
	
		m_dir = transformVector( vec3f( 0.0f, 0.0f, -1.0f ), getLocalRotation() );

		m_dir.normalize();

		const vec3f& pos = m_frames[ m_currentFrame ].position;

		mat4f itrans(	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						-pos.x, -pos.y, -pos.z, 1.0f );
		
		m_viewMatrix = itrans*getLocalRotation().getTranspose();
	}
}
