#pragma once

#include"CommonEngineIncludes.h"
#include "Node.h"

namespace pdh
{
	class Camera : public Node
	{
	public:
		Camera( const std::string& id, const vec3f& pos, const mat4f& projMatrix, const mat4f& viewMatrix = mat4f::IDENTITY );
		virtual ~Camera();

		
		virtual bool onKeyDown( unsigned char keyCode );
		virtual bool onKeyUp( unsigned char keyCode );
		virtual void onMouseMove( int x, int y ){}

		virtual void updateInput(){}
		virtual void update( double deltaTime );

		vec3f getPosition() const { return getWorldPosition(); }
		vec3f getDirection() const { return m_dir; }
		virtual const mat4f& getViewMatrix();
		virtual const mat4f& getProjectionMatrix() const;

	protected:
		vec3f m_dir;
		mat4f m_viewMatrix;
		mat4f m_projectionMatrix;
		
		bool m_keyboard[ 256 ];
		
	};
}