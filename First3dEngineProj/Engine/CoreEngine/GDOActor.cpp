#include "GDOActor.h"
#include "Actor.h"
#include "Renderer.h"
#include "Material.h"
#include "Model.h"

namespace pdh
{
	GDOActor::GDOActor( Actor* actor, float showUntilTime, const std::string& materialName, Color4f color )
		:	GraphicDebugObject( actor->getPosition(), actor->getRotation(), showUntilTime )
		,	m_actor( actor )
		,	m_color( color )
		,	m_materialName( materialName )
	{
		m_actor->getModel()->applyDebugMaterial( m_materialName );
	}

	void GDOActor::render( Renderer* renderer, float currentTime )
	{
		Material* material = renderer->getMaterial( m_materialName );

		glDisable( GL_DEPTH_TEST );
		material->updateUniform( "uAlpha", 0.25f );
		
		bool wireFrame = renderer->getWireFrameState();

		renderer->setWireFrame( true );
		renderer->setLineWidth( 3.0f );
		m_actor->renderDebug( renderer );

		glEnable( GL_DEPTH_TEST );
		m_actor->renderDebug( renderer );
		
		renderer->setWireFrame( false );
		material->updateUniform( "uColor", vec3f( m_color.r, m_color.g, m_color.b ) );
		m_actor->renderDebug( renderer );
		renderer->setLineWidth( 1.0f );
		renderer->setWireFrame( wireFrame );

		material->updateUniform( "uAlpha", 1.0f );

		if( m_showUntilTime <= currentTime || m_showUntilTime == 0.0f )
		{
			m_isDead = true;
		}
	}
}