#pragma once
#include <vector>
#include "CommonEngineIncludes.h"

namespace pdh
{
	class Mesh;
	class Renderer;
	class BoundingSphere;

	class Model
	{
	public:
		Model( Mesh* mesh );
		~Model();
		void addMesh( Mesh* mesh );
		void renderModel( Renderer* renderer );

		void applyDebugMaterial( const std::string& materialName );
		void renderDebugModel( Renderer* renderer );
		void renderBoundingSphere( Renderer* renderer );

		BoundingSphere* getBoundingSphere() { return m_bSphere; }

	private:
		std::vector< Mesh* > m_meshes;
		BoundingSphere* m_bSphere;
	};
}