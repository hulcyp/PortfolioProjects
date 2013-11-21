#include "Model.h"
#include "CommonEngineIncludes.h"
#include "Renderer.h"
#include "BoundingSphere.h"

namespace Monky
{
	Model::Model( Mesh* mesh )
	{
		m_meshes.push_back( mesh );
		m_bSphere = new BoundingSphere( *( mesh->getBoundingSphere() ) );
	}
	
	Model::~Model()
	{
		VECTOR_CLEANUP( m_meshes );
		SAFE_DELETE( m_bSphere );
	}

	void Model::addMesh( Mesh* mesh )
	{
		m_meshes.push_back( mesh );
		m_bSphere->mergeSpheres( *( mesh->getBoundingSphere() ) );
	}

	void Model::renderModel( Renderer* renderer )
	{
		for( size_t i = 0; i < m_meshes.size(); ++i )
		{
			renderer->renderMesh( m_meshes[i] );
		}
	}

	void Model::applyDebugMaterial( const std::string& materialName )
	{
		for( size_t i = 0; i < m_meshes.size(); ++i )
		{
			m_meshes[i]->setDebugMaterial( materialName );
		}
	}

	void Model::renderDebugModel( Renderer* renderer )
	{
		for( size_t i = 0; i < m_meshes.size(); ++i )
		{
			renderer->renderDebugMesh( m_meshes[i] );
		}
		//renderBoundingSphere( renderer );
	}

	void Model::renderBoundingSphere( Renderer* renderer )
	{
		m_bSphere->render( renderer );
	}
}