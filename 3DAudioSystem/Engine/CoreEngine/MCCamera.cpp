#include "GameApp.h"
#include "MCCamera.h"
#include <MathFuncs.h>
#include <MonkyException.h>


namespace Monky
{
	MCCamera::MCCamera( const std::string& id, const vec3f& pos, const vec2i& mousPos, const mat4f& projMatrix, const mat4f& viewMatrix )
		:	Camera( id, pos, PH_PERSPECTIVE, projMatrix, viewMatrix )
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

	MCCamera& MCCamera::operator=( const MCCamera& rhs )
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
			m_impulse = rhs.m_impulse;
			m_boost = rhs.m_boost;
			m_rot = rhs.m_rot;
			m_vel = rhs.m_vel;
			m_drag = rhs.m_drag;
			m_mouseCenterPos = rhs.m_mouseCenterPos;
			m_mouseInputUpdated = rhs.m_mouseInputUpdated;
			for( int i = 0; i < 256; ++i )
			{
				m_keyboard[i] = rhs.m_keyboard[i];
			}
		}
		return *this;
	}

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

		GameApp::warpPointer( m_mouseCenterPos.x, m_mouseCenterPos.y );

		m_impulse = vec3f( 0.0, 0.0, 0.0 );

		if( m_keyboard[ MONKY_w ] )
		{
			m_impulse.z -= 1.0;	
		}
		if( m_keyboard[ MONKY_s ] )
		{
			m_impulse.z += 1.0;
		}

		if( m_keyboard[ MONKY_a ] )
		{
			m_impulse.x -= 1.0;
		}
		if( m_keyboard[ MONKY_d ] )
		{
			m_impulse.x += 1.0;
		}
		if( m_keyboard[ MONKY_e ] )
		{
			m_impulse.y += 1.0;
		}
		if( m_keyboard[ MONKY_q ] )
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

		m_up = transformVector( vec3f( 0.0f, 1.0f, 0.0f ), getLocalRotation() );
		m_up.normalize();

		const vec3f& pos = m_frames[ m_currentFrame ].position;

		mat4f itrans(	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						-pos.x, -pos.y, -pos.z, 1.0f );
		
		m_viewMatrix = itrans*getLocalRotation().getTranspose();
	}
}
