#pragma once
#include "CommonEngineIncludes.h"
#include "Node.h"

namespace Monky
{
	class Renderer;
	class Mesh;
	class ASGeometry;

	class Object : public Node
	{
	public:
		Object( const std::string& id, Mesh* mesh = nullptr );
		virtual ~Object(){}
		vec3f getPosition() { return getWorldPosition(); }
		
		virtual void render();
		virtual void renderDebug();
		
		Mesh* getMesh() { return m_mesh; }
		void setMesh( Mesh* mesh ) { m_mesh = mesh; }

		virtual void setPosition( const vec3f& pos, int frame = 0 );
		virtual void setRotation( const mat3f& rot, int frame = 0 );
		virtual void setScale( const vec3f& scale, int frame = 0 );


		virtual BoundingSphere* getBoundingSphere();

	private:
		Mesh* m_mesh;
		std::string m_occlusionGeometryName;
	};
}