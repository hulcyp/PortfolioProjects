#pragma once
#include "Mesh.h"
#include "CommonEngineIncludes.h"

namespace pdh
{
	class Renderer;

	class BoundingSphere
	{
	public:
		BoundingSphere( const vec3f& pos = vec3f(), float radius = 0.0f ); 
		BoundingSphere( const std::vector< Mesh::Vertex >& vertices );

		void genereateBoundingSphere( const std::vector< Mesh::Vertex >& vertices );

		vec3f getCenter() const { return m_position; }
		float getRadius() const { return m_radius; }

		void mergeSpheres( const BoundingSphere& bSphere );

		void render( Renderer* renderer );
	
	private:
		vec3f m_position;
		float m_radius;
	};
}