#pragma once

#include"CommonEngineIncludes.h"

namespace pdh
{
	class Camera
	{
	public:
		Camera( const vec3f& pos );
		virtual ~Camera();

		
		virtual bool onKeyDown( unsigned char keyCode );
		virtual bool onKeyUp( unsigned char keyCode );
		virtual void onMouseMove( int x, int y ){}

		virtual void updateInput(){}
		virtual void updateSimulation( double deltaTime );

		vec3f getPosition() { return m_pos; }
		vec3f getDirection() { return m_dir; }
		mat4f getViewMatrix() { return m_matrix; }

	protected:
		vec3f m_pos;
		vec3f m_rot;
		vec3f m_dir;
		mat4f m_matrix;
		vec3f m_vel;
		float m_drag;
		
		bool m_keyboard[ 256 ];
		
	};
}