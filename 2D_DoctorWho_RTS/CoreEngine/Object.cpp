#include "Object.h"
#include "Renderer.h"
#include "BoundingSphere.h"
#include "CommonEngineIncludes.h"

namespace pdh
{
	Object::Object( const std::string& id, Mesh* mesh )
		:	Node( id )
		,	m_mesh( mesh )
	{}
	//-------------------------------------------------------------------
	BoundingSphere* Object::getBoundingSphere()
	{
		if( m_mesh != nullptr )
		{
			return m_mesh->getBoundingSphere();
		}
		else
		{
			return nullptr;
		}
	}
	//-------------------------------------------------------------------
	void Object::render( Renderer* renderer )
	{
		matStackf& modelStack = renderer->getModelStack();
		modelStack.push();
		modelStack.loadIdentity();
		modelStack.applyMatrix( getWorldTransformation() );


		if( m_mesh )
		{
			renderer->renderMesh( m_mesh );
		}
		modelStack.pop();
	}
	//-------------------------------------------------------------------
	void Object::renderDebug( Renderer* renderer )
	{
		matStackf& modelStack = renderer->getModelStack();
		modelStack.push();
		modelStack.loadIdentity();
		modelStack.applyMatrix( getWorldTransformation() );

		if( m_mesh )
		{
			renderer->renderDebugMesh( m_mesh );
			renderer->renderAxis( 5.0f );
		}
		modelStack.pop();
	}
}