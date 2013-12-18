#pragma once
#include <Camera.h>
#include <EventSystemInterface.h>
#include <map>

namespace pdh
{
	class RTSCamera : public Camera
	{
	public:
		RTSCamera( const std::string& id, const vec3f& pos, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );
		
		virtual void updateInput();
		virtual void update( double deltaTime );

		void onSpecialKeyDown( NamedProperties& params );
		void onSpecialKeyUp( NamedProperties& params );

		void onMouseButtonDown( NamedProperties& params );
		void onMouseButtonUp( NamedProperties& params );
		void onMouseMove( NamedProperties& params );

	private:
		vec2f m_impulse;
		vec2f m_vel;
		float m_drag;
		bool m_isDragging;
		vec2i m_lastDragPos;

		std::map< int, bool > m_activeSpecialKeys;

		const float KEYBOARD_IMPULSE_POWER;
	};
}