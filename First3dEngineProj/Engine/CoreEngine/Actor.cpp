#include "Actor.h"
#include "Renderer.h"
#include "Model.h"
#include "Font.h"

namespace pdh
{
	Actor::Actor( const std::string& id, const vec3f& pos )
		:	m_pos( pos )
		,	m_model( nullptr )
		,	m_rotation( mat4f::identity() )
		,	m_id( id )
	{}

	BoundingSphere* Actor::getBoundingSphere()
	{
		if( m_model != nullptr )
		{
			return m_model->getBoundingSphere();
		}
		else
		{
			return nullptr;
		}
	}

	void Actor::render( Renderer* renderer )
	{
		matStackf& modelStack = renderer->getModelStack();
		modelStack.push();
		modelStack.loadIdentity();
		modelStack.applyMatrix( m_rotation );
		modelStack.translate( m_pos );
		if( m_model )
		{
			m_model->renderModel( renderer );
		}
		modelStack.pop();
	}

	void Actor::renderDebug( Renderer* renderer )
	{
		matStackf& modelStack = renderer->getModelStack();
		modelStack.push();
		modelStack.loadIdentity();
		modelStack.scale( 1.01f );
		modelStack.applyMatrix( m_rotation );
		modelStack.translate( m_pos );
		if( m_model )
		{
			m_model->renderDebugModel( renderer );
			renderer->renderAxis( 5.0f );
		}
		modelStack.pop();
	}
}