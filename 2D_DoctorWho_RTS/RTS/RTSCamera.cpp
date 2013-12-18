#include "RTSCamera.h"
#include <glut.h>

namespace pdh
{
	RTSCamera::RTSCamera( const std::string& id, const vec3f& pos, const mat4f& projMatrix, const mat4f& viewMatrix )
		:	Camera( id, pos, projMatrix, viewMatrix )
		,	m_drag( 15.0f )
		,	KEYBOARD_IMPULSE_POWER( 5.0f )
		,	m_isDragging( false )
	{
		registerForEvent( "onSpecialKeyDown", *this, &RTSCamera::onSpecialKeyDown );
		registerForEvent( "onSpecialKeyUp", *this, &RTSCamera::onSpecialKeyUp );
		//registerForEvent( "onMouseButtonDown", *this, &RTSCamera::onMouseButtonDown );
		//registerForEvent( "onMouseButtonUp", *this, &RTSCamera::onMouseButtonUp );
		//registerForEvent( "onMouseMove", *this, &RTSCamera::onMouseMove );
	}
	//-------------------------------------------------
	void RTSCamera::updateInput()
	{
		m_impulse = vec2f( 0.0, 0.0 );

		if( m_activeSpecialKeys[ GLUT_KEY_LEFT ] )
		{
			m_impulse.x -= 1.0f;
		}		
		if( m_activeSpecialKeys[ GLUT_KEY_RIGHT ] )
		{
			m_impulse.x += 1.0f;
		}
		if( m_activeSpecialKeys[ GLUT_KEY_UP ] )
		{
			m_impulse.y += 1.0f;
		}
		if( m_activeSpecialKeys[ GLUT_KEY_DOWN ] )
		{
			m_impulse.y -= 1.0f;
		}

		m_impulse *= KEYBOARD_IMPULSE_POWER;
		m_vel += m_impulse;
	}
	//-------------------------------------------------
	void RTSCamera::update( double deltaTime )
	{

		float dt = static_cast<float>( deltaTime );
		

		vec3f pos = getPosition();
		pos += m_vel * dt;

		m_vel -= m_vel * m_drag * dt;

		m_viewMatrix[12] = -pos.x;
		m_viewMatrix[13] = -pos.y;
		m_viewMatrix[14] = -pos.z;

		setPosition( pos );
		m_impulse = vec2f( 0.0f, 0.0f );
	}
	//-------------------------------------------------
	void RTSCamera::onSpecialKeyDown( NamedProperties& params )
	{
		int keyCode;
		if( params.get( "keyCode", keyCode ) == NamedProperties::SUCCESS )
		{
			m_activeSpecialKeys[ keyCode ] = true;
			switch( keyCode ) 
			{
			case GLUT_KEY_LEFT:
				m_impulse.x -= 1.0f;
				break;
			case GLUT_KEY_RIGHT:
				m_impulse.x += 1.0f;
				break;
			case GLUT_KEY_UP:
				m_impulse.y += 1.0f;
				break;
			case GLUT_KEY_DOWN:
				m_impulse.y -= 1.0f;
				break;
			}
		}
	}
	//-------------------------------------------------
	void RTSCamera::onSpecialKeyUp( NamedProperties& params )
	{
		int keyCode;
		if( params.get( "keyCode", keyCode ) == NamedProperties::SUCCESS )
		{
			m_activeSpecialKeys[ keyCode ] = false;
		}
	}
	//-------------------------------------------------
	void RTSCamera::onMouseButtonDown( NamedProperties& params )
	{
		int keyCode;
		vec2i mouseCoords;
		
		if( params.get( "keyCode", keyCode ) == NamedProperties::SUCCESS )
		{
			if( keyCode == GLUT_MIDDLE_BUTTON )
			{
				if( params.get( "screenCoords", mouseCoords ) == NamedProperties::SUCCESS )
					m_lastDragPos = mouseCoords;
								
				m_isDragging = true;
			}
		}
	}
	//-------------------------------------------------
	void RTSCamera::onMouseButtonUp( NamedProperties& params )
	{
		int keyCode;
		if( params.get( "keyCode", keyCode ) == NamedProperties::SUCCESS )
		{
			if( keyCode == GLUT_MIDDLE_BUTTON )
				m_isDragging = false;
		}
	}
	//-------------------------------------------------
	void RTSCamera::onMouseMove( NamedProperties& params )
	{
		vec2i mouseCoords;
		if( m_isDragging )
		{
			if( params.get( "mouseCoords", mouseCoords ) == NamedProperties::SUCCESS )
			{
				vec2i dirMouseSpace = mouseCoords - m_lastDragPos;
				vec2f dir = vec2f( (float)dirMouseSpace.x, (float)dirMouseSpace.y );
				dir.normalize();
				setPosition( getPosition() - dir );
			}
		}
	}
}