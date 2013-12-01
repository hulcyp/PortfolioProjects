#include "FrameBufferObject.h"
#include "Renderer.h"
#include "CommonEngineIncludes.h"
#include "Material.h"
#include "Mesh.h"

namespace pdh
{
	FrameBufferObject::FrameBufferObject( int width, int height, float aspectRatio, const std::string& materialName, Renderer* renderer )
		:	m_width( width )
		,	m_height( height )
		,	m_aspectRatio( aspectRatio )
		,	m_currentTextureIndex( 0 )
	{
		GLuint colorTexID;
		GLuint depthTexID;

		// Generate default color texture
		glGenTextures( 1, &colorTexID );
		glBindTexture( GL_TEXTURE_2D, colorTexID );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
		glBindTexture( GL_TEXTURE_2D, 0 );

		// Generate depth texture
		glGenTextures( 1, &depthTexID );
		glBindTexture( GL_TEXTURE_2D, depthTexID );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
		glBindTexture( GL_TEXTURE_2D, 0 );

		// Generate framebuffer
		glGenFramebuffers( 1, &m_FBOId );
		glBindFramebuffer( GL_FRAMEBUFFER, m_FBOId );
		
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexID, 0 );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexID, 0 );

		GLenum e = glCheckFramebufferStatus( GL_FRAMEBUFFER );
		if( e != GL_FRAMEBUFFER_COMPLETE )
			consolePrintColorf( "Error creating FBO", color::RED );

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		glBindTexture( GL_TEXTURE_2D, 0 );
		
		m_colorTexture = new Texture( colorTexID );
		m_depthTexture = new Texture( depthTexID );

		m_mesh = Mesh::generate2DOrthoRectangle( static_cast<float>( m_width ), static_cast<float>( m_height ), materialName );
		Material* material = renderer->getMaterial( materialName );
		material->addTexture( "uColorTexture", m_colorTexture );
		material->addTexture( "uDepthTexture", m_depthTexture );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FrameBufferObject::~FrameBufferObject()
	{
		glDeleteFramebuffers( 1, &m_FBOId );
		m_FBOId = 0;
		SAFE_DELETE( m_mesh );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FrameBufferObject::bindFBO()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_FBOId );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FrameBufferObject::unBindFBO()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FrameBufferObject::render( Renderer* renderer )
	{
		Material* material = renderer->getMaterial( m_mesh->getMaterial() );
		material->updateUniform( "uAspectRatio", m_aspectRatio );
		material->updateUniform( "uColorTexture", m_colorTexture );
		material->updateUniform( "uDepthTexture", m_depthTexture );
		renderer->renderMesh( m_mesh );
	}	
}