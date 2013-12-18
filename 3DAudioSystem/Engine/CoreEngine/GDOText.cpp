#include "GDOText.h"
#include "Actor.h"
#include "Renderer.h"
#include "Material.h"
#include "Font.h"
#include "BoundingSphere.h"

namespace Monky
{
	GDOText::GDOText( Actor* actor, const std::string& type, float currentTime, Color4f color, float fontHeight )
		:	GraphicDebugObject( actor->getPosition(), mat3f::identity(), currentTime )
		,	m_actor( actor )
		,	m_type( type )
		,	m_color( color )
		,	m_fontHeight( fontHeight )
	{}

	void GDOText::render( Renderer* renderer, float currentTime )
	{
		Material* material = Material::getMaterial( "DebugMaterial" );
		Font* font = Font::getFont( "mainFont_72" );
		mat4f viewMatrix = renderer->getViewMatrix();
		
		std::string text = getStringBasedOnType();

		BoundingSphere* bSphere = m_actor->getBoundingSphere();
		vec3f renderPos;
		if( bSphere != nullptr )
		{
			renderPos = bSphere->getCenter() + vec3f( 0.0f, bSphere->getRadius(), 0.0f );
		}
		 
		renderPos += m_actor->getPosition();
		material->updateUniform( "uColor", vec3f( 1, 1, 1 ) );

		
		glDisable( GL_DEPTH_TEST );
		material->updateUniform( "uAlpha", 0.25f );
		font->renderBillboardText( text, renderPos, m_fontHeight, 
			vec3f( viewMatrix[2], viewMatrix[6], viewMatrix[10] ), m_color, Font::CENTER );

		glEnable( GL_DEPTH_TEST );
		material->updateUniform( "uAlpha", 1.0f );
		font->renderBillboardText( text, renderPos, m_fontHeight, 
			vec3f( viewMatrix[2], viewMatrix[6], viewMatrix[10] ), m_color, Font::CENTER );
		
		
		if( m_showUntilTime <= currentTime || m_showUntilTime == 0.0f )
		{
			m_isDead = true;
		}
	}

	std::string GDOText::getStringBasedOnType()
	{
		std::stringstream ss;
		std::string text;
		if( m_type == "pos" )
		{
			ss << m_actor->getPosition();
			ss >> text;
		}

		return text;
	}
}