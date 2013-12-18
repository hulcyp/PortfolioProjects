#include "GraphicDebugObject.h"
#include "Mesh.h"
#include "CommonEngineIncludes.h"
#include "Renderer.h"
#include "Material.h"

namespace Monky
{
	GraphicDebugObject::GraphicDebugObject( const vec3f& pos, const mat3f& rotation, float showUntilTIme )
		:	m_pos( pos )
		,	m_rotation( rotation )
		,	m_showUntilTime( showUntilTIme )
		,	m_isDead( false )
	{}

	GraphicDebugObject::~GraphicDebugObject()
	{
		SAFE_DELETE( m_mesh );
	}

	void GraphicDebugObject::render( Renderer* renderer, float currentTime )
	{
		Material* material = Material::getMaterial( m_mesh->getMaterial( 0 ) );
		matStackf& stack = renderer->getModelStack();
		stack.push();
		stack.loadIdentity();
		stack.applyMatrix( m_rotation );
		stack.translate( m_pos );
		material->updateUniform( "uColor", vec3f( 1, 1, 1 ) );

		
		bool wireFrame = renderer->getWireFrameState();

		glDisable( GL_DEPTH_TEST );
		material->updateUniform( "uAlpha", 0.25f );
		renderer->setWireFrame( true );
		renderer->renderMesh( m_mesh );
		renderer->setLineWidth( 3.0f );
		glEnable( GL_DEPTH_TEST );
		material->updateUniform( "uAlpha", 1.0f );
		renderer->renderMesh( m_mesh );
		
		renderer->setWireFrame( false );		
		renderer->renderMesh( m_mesh );
		
		renderer->setLineWidth( 1.0f );
		renderer->setWireFrame( wireFrame );

		if( m_showUntilTime <= currentTime || m_showUntilTime == 0.0f )
		{
			m_isDead = true;
		}
		stack.pop();
	}
}