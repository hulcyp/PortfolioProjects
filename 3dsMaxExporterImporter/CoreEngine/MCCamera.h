#ifndef PDH_CAMERA
#define PDH_CAMERA

#include <Camera.h>

namespace pdh
{

	class MCCamera : public Camera
	{
	public:
		MCCamera( const std::string& id, const vec3f& pos, const vec2i& mousePos, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );
		~MCCamera();

		virtual void onMouseMove( int x, int y );
		virtual void updateInput();
		virtual void update( double deltaTime );
		
		void setBoost( bool boost ){ m_boost = boost; }
		virtual const mat4f& getViewMatrix() { return m_viewMatrix; }

	private:
		const float MOUSE_YAW_POWER;
		const float MOUSE_PITCH_POWER;
		const float KEYBOARD_IMPULSE_POWER;
		const float BOOST_FACTOR;
		vec3f m_impulse;
		bool m_boost;
		vec3f m_rot;
		vec3f m_vel;
		float m_drag;
		
		vec2i m_mouseCenterPos;
		bool m_mouseInputUpdated;
	};
}

#endif