#pragma once
#include <Vector3.h>
#include "CommonEngineIncludes.h"

namespace pdh
{
	class Model;
	class Renderer;
	class Camera;
	class BoundingSphere;

	class Actor
	{
	public:
		Actor( const std::string& id, const vec3f& pos = vec3f() );

		void setPosition( const vec3f pos ) { m_pos = pos; }
		void setRotation( const mat4f rot ) { m_rotation = rot; }
		vec3f getPosition() { return m_pos; }
		mat4f getRotation() { return m_rotation; }
		const std::string& getID() { return m_id; }
		Model* getModel() { return m_model; }
		BoundingSphere* getBoundingSphere();

		void setModel( Model* model ) { m_model = model; }
		void render( Renderer* renderer );

		void renderDebug( Renderer* renderer );
		
	private:
		vec3f m_pos;
		mat4f m_rotation;
		Model* m_model;
		std::string m_id;
	};
}