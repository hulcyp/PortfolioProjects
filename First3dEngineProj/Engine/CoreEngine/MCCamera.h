#ifndef PDH_CAMERA
#define PDH_CAMERA

#include <Camera.h>

namespace pdh
{

	class MCCamera : public Camera
	{
	public:
		MCCamera( const vec3f& pos, const vec2i& mousePos );
		~MCCamera();

		virtual void onMouseMove( int x, int y );
		virtual void updateInput();
		
		void setBoost( bool boost ){ m_boost = boost; }

	private:
		const float MOUSE_YAW_POWER;
		const float MOUSE_PITCH_POWER;
		const float KEYBOARD_IMPULSE_POWER;
		const float BOOST_FACTOR;
		vec3f m_impulse;
		bool m_boost;
		
		vec2i m_mouseCenterPos;
		bool m_mouseInputUpdated;
	};
}

#endif