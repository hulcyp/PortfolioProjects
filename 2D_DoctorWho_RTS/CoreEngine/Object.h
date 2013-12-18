#pragma once
#include "CommonEngineIncludes.h"
#include "Node.h"

namespace pdh
{
	class Renderer;
	class Mesh;

	class Object : public Node
	{
	public:
		Object( const std::string& id, Mesh* mesh = nullptr );
		vec3f getPosition() { return getWorldPosition(); }
		
		void render( Renderer* renderer );
		void renderDebug( Renderer* renderer );
		
		Mesh* getMesh() { return m_mesh; }
		void setMesh( Mesh* mesh ) { m_mesh = mesh; }

		virtual BoundingSphere* getBoundingSphere();


	private:
		Mesh* m_mesh;
	};
}