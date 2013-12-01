#ifdef _WIN32
#include <glew.h>
#endif
#include <glut.h>
#include <fstream>
#include "Renderer.h"
#include "MathUtils.h"
#include "Mesh.h"
#include "Font.h"
#include "tinyxml2.h"
#include "ErrorHandlingUtils.h"
#include <Matrix4Factory.h>
#include "Material.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "StringUtils.h"
#include "Camera.h"
#include <algorithm>
#include "FrameBufferObject.h"

namespace pdh
{
	Renderer::Renderer( int width, int height, float fov, float zNear, float zFar )
		:	m_screenWidth( width )
		,	m_screenHeight( height )
		,	m_orthoEnabled( false )
		,	m_renderWireFrame( false )
	{
		loadShaders( "../data/shaders/ShaderPrograms.xml" );
		loadTextures( "../data/textures/Textures.xml" );
		loadMaterials( "../data/materials/Materials.xml" );
		loadFonts( "../data/fonts/Fonts.xml" );

		m_perspectiveProjectionMatrix = mat4f::perspective( fov ,static_cast<float>( width ) / height,  zNear, zFar );
		//m_orthographicProjectionMatrix = mat4f::ortho( (float)width, 0.0f, (float)height, 0.0f, zNear, zFar );
		m_orthographicProjectionMatrix = mat4f::ortho( 0.0f, (float)width, (float)height, 0.0f, 1.0f, -1.0f );

		m_positionLocation	= 0;
		m_normalLocation	= 1;
		m_colorLocation		= 2;
		m_texCoordLocation	= 3;
		m_tangentLoction	= 4;
		m_bitangentLocation = 5;
	}

	Renderer::~Renderer()
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::createShaderProgram( const std::string& shaderName, const char* vertexFile, const char* fragFile )
	{
		Shader*& vertex = m_shaders[ vertexFile ];
		if( !vertex )
		{
			vertex = new Shader( "../data/shaders/" + std::string( vertexFile ), GL_VERTEX_SHADER );
		}

		Shader*& frag = m_shaders[ fragFile ];
		if( !frag )
		{
			frag = new Shader( "../data/shaders/" + std::string( fragFile ), GL_FRAGMENT_SHADER );
		}		

		ShaderProgram* program = new ShaderProgram( shaderName, vertex, frag );

		m_programs[ shaderName ] = program;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Material* Renderer::createMaterial( const std::string& materialName, const std::string& shaderProgramName )
	{
		Material*& newMaterial = m_materials[ materialName ];
		if( !newMaterial )
		{
			newMaterial = new Material( m_programs[ shaderProgramName ] );
			newMaterial->addUniform( "uProjectionMatrix", m_perspectiveProjectionMatrix );
			newMaterial->addUniform( "uViewMatrix", mat4f::identity() );
			newMaterial->addUniform( "uModelMatrix", mat4f::identity() );
			newMaterial->addUniform( "uMVPMatrix", mat4f::identity() );
			newMaterial->addUniform( "time", 0.0f );
		}		
		return newMaterial;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::useMaterial( const std::string& materialName )
	{
		if( materialName != "" && m_materials.find( materialName ) != m_materials.end() )
		{
			m_currentMaterial = m_materials[ materialName ];
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Font* Renderer::createFont( const std::string& fontName, const std::string& shaderName, const char* xmlFile, const char* bitmapFile )
	{
		Font*& newFont = m_fonts[ fontName ];
		if( !newFont )
		{
			newFont = new Font( fontName, shaderName, "../data/fonts/" + std::string( xmlFile ), "../data/fonts/" + std::string( bitmapFile ), this );
		}
		return newFont;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Texture* Renderer::createTexture( const std::string& textureName, const std::string&  fileName )
	{
		Texture*& newTexture = m_textures[ textureName ];
		if( !newTexture )
		{
			newTexture = new Texture( "../data/textures/" + fileName );
		}
		return newTexture;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Texture* Renderer::createTexture( const std::string& textureName, unsigned int textureID )
	{
		Texture* newTexture = m_textures[ textureName ];
		if( !newTexture )
		{
			newTexture = new Texture( textureID );
		}
		return newTexture;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Material* Renderer::getMaterial( const std::string& materialName )
	{
		Material* material = nullptr;
		if( materialName != "" && m_materials.find( materialName ) != m_materials.end() )
		{
			material = m_materials[ materialName ];
		}
		return material;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ShaderProgram* Renderer::getShaderProgram( const std::string programName )
	{
		ShaderProgram* shaderProgram = nullptr;
		if( programName != "" && m_programs.find( programName ) != m_programs.end() )
		{
			shaderProgram = m_programs[ programName ];
		}
		return shaderProgram;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Font* Renderer::getFont( const std::string& fontName )
	{
		Font* font = nullptr;
		if( fontName != "" && m_fonts.find( fontName ) != m_fonts.end() )
		{
			font = m_fonts[ fontName ];
		}
		return font;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Texture* Renderer::getTexture( const std::string& textureName )
	{
		Texture* texture;
		if( !textureName.empty() && m_textures.find( textureName ) != m_textures.end() )
		{
			texture = m_textures[ textureName ];
		}
		return texture;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderMesh( Mesh* mesh )
	{
		if( mesh )
		{
			useMaterial( mesh->getMaterial() );
			glBindBuffer( GL_ARRAY_BUFFER, mesh->getVBOID() );
			if( mesh->getIBOID() )
			{
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh->getIBOID() );
				enableRenderStates();
				glDrawElements( mesh->getRenderMode(), mesh->getNumIndices(), GL_UNSIGNED_INT, 0 );
			}
			else
			{
				enableRenderStates();
				glDrawArrays( mesh->getRenderMode(), 0, mesh->getNumVertices() );
			}
			disableRenderStates();
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderDebugMesh( Mesh* mesh )
	{
		if( mesh )
		{
			useMaterial( mesh->getDebugMaterial() );
			glBindBuffer( GL_ARRAY_BUFFER, mesh->getVBOID() );
			if( mesh->getIBOID() )
			{
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh->getIBOID() );
				enableRenderStates();
				glDrawElements( mesh->getRenderMode(), mesh->getNumIndices(), GL_UNSIGNED_INT, 0 );
			}
			else
			{
				enableRenderStates();
				glDrawArrays( mesh->getRenderMode(), 0, mesh->getNumVertices() );
			}
			disableRenderStates();
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderVertices( std::vector< Mesh::Vertex > vertices, const std::string& material, int mode )
	{
		if( vertices.size() > 0 )
		{
			glEnableVertexAttribArray( m_positionLocation );
			glEnableVertexAttribArray( m_normalLocation );
			glEnableVertexAttribArray( m_colorLocation );
			glEnableVertexAttribArray( m_texCoordLocation );
			glEnableVertexAttribArray( m_tangentLoction );
			glEnableVertexAttribArray( m_bitangentLocation );

			glVertexAttribPointer( m_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof( vertices[0] ),
				&vertices[0].x );
			glVertexAttribPointer( m_normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof( vertices[0] ),
				&vertices[0].xn );
			glVertexAttribPointer( m_colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof( vertices[0] ),
				&vertices[0].r );
			glVertexAttribPointer( m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof( vertices[0] ),
				&vertices[0].u );
			glVertexAttribPointer( m_tangentLoction, 3, GL_FLOAT, GL_FALSE, sizeof( vertices[0] ),
				&vertices[0].tx );
			glVertexAttribPointer( m_bitangentLocation, 3, GL_FLOAT, GL_FALSE,sizeof( vertices[0] ),
				&vertices[0].bx );
			
			useMaterial( material );
			if( m_currentMaterial )
			{
				if( m_orthoEnabled )
				{
					m_currentMaterial->updateUniform( "uProjectionMatrix", m_orthographicProjectionMatrix );
					m_currentMaterial->updateUniform( "uViewMatrix", mat4f::identity() );
					m_currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_orthographicProjectionMatrix );
				}
				else
				{
					m_currentMaterial->updateUniform( "uProjectionMatrix", m_perspectiveProjectionMatrix );
					m_currentMaterial->updateUniform( "uViewMatrix", m_viewMatrix );
					m_currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_viewMatrix * m_perspectiveProjectionMatrix );
				}				
				m_currentMaterial->updateUniform( "uModelMatrix", m_modelStack.top() );				
				m_currentMaterial->activateProgram();
			}
			glDrawArrays( mode, 0, vertices.size() );
			disableRenderStates();	
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::pushFBO( int width, int height, const std::string& material )
	{
		m_fboQueue.push_back( new FrameBufferObject( width, height, static_cast<float>( width ) / height, material, this ) );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::popFBO()
	{
		m_fboQueue.pop_back();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::bindFBOQueue()
	{
		m_fboQueue[0]->bindFBO();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderFBOQueue()
	{
		m_fboQueue[0]->unBindFBO();
		m_modelStack.push();
		m_modelStack.loadIdentity();
		enableOrthoRendering();
		for( size_t i = 1; i < m_fboQueue.size(); ++i )
		{
			for( int j = i - 1; j >= 0; --j )
			{
				Material* fboMaterial = getMaterial( m_fboQueue[i]->getMaterial() );
				std::stringstream ss;
				ss << "uBufferPassTextureColor" << j;
				std::string uniformName;
				ss >> uniformName;				
				fboMaterial->addTexture( uniformName.c_str(), m_fboQueue[j]->getColorTexture() );
			}
			m_fboQueue[i]->bindFBO();
			m_fboQueue[i - 1]->render( this );
			m_fboQueue[i]->unBindFBO();
		}
		m_fboQueue.back()->render( this );
		disableOrthoRendering();
		m_modelStack.pop();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::enableOrthoRendering()
	{
		m_orthoEnabled = true;
		glPushAttrib( GL_DEPTH_TEST );
		glDisable( GL_DEPTH_TEST );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::disableOrthoRendering()
	{
		glPopAttrib();
		m_orthoEnabled = false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::setViewport( int x, int y, int width, int height )
	{
		glViewport( x, y, width, height );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Utility functions
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderAxis( float length )
	{
		Mesh* mesh = Mesh::generateAxis( length );
		renderMesh( mesh );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::toggleWireFrame()
	{
		m_renderWireFrame = !m_renderWireFrame;		
		if( m_renderWireFrame )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		else
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::setWireFrame( bool renderWireFrame )
	{
		m_renderWireFrame = renderWireFrame;
		if( m_renderWireFrame )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		else
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Renderer::getWireFrameState()
	{
		return m_renderWireFrame;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::setLineWidth( float lineWidth )
	{
		glLineWidth( lineWidth );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::pushCamera( Camera* camera )
	{
		if( camera != nullptr )
		{
			m_cameraStack.push( camera );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::popCamera()
	{
		m_cameraStack.pop();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::updateTimeOnMaterials( float time )
	{
		std::for_each( m_materials.begin(), m_materials.end(),
			[&]( std::pair< std::string, Material* > material )
		{
			material.second->updateUniform( "time", time );
		} );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Protected members
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::enableRenderStates()
	{

		glEnableVertexAttribArray( m_positionLocation );
		glEnableVertexAttribArray( m_normalLocation );
		glEnableVertexAttribArray( m_colorLocation );
		glEnableVertexAttribArray( m_texCoordLocation );
		glEnableVertexAttribArray( m_tangentLoction );
		glEnableVertexAttribArray( m_bitangentLocation );

		glVertexAttribPointer( m_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof( Mesh::Vertex ),
			reinterpret_cast< GLvoid* >( offsetof( Mesh::Vertex, x ) ) );
		glVertexAttribPointer( m_normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof( Mesh::Vertex ),
			reinterpret_cast< GLvoid* >( offsetof( Mesh::Vertex, xn ) ) );
		glVertexAttribPointer( m_colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof( Mesh::Vertex ),
			reinterpret_cast< GLvoid* >( offsetof( Mesh::Vertex, r ) ) );
		glVertexAttribPointer( m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof( Mesh::Vertex ),
			reinterpret_cast< GLvoid* >( offsetof( Mesh::Vertex, u ) ) );
		glVertexAttribPointer( m_tangentLoction, 3, GL_FLOAT, GL_FALSE, sizeof( Mesh::Vertex ),
			reinterpret_cast< GLvoid* >( offsetof( Mesh::Vertex, tx ) ) );
		glVertexAttribPointer( m_bitangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof( Mesh::Vertex ),
			reinterpret_cast< GLvoid* >( offsetof( Mesh::Vertex, bx ) ) );

		if( m_cameraStack.top() != nullptr )
		{
			m_viewMatrix = m_cameraStack.top()->getViewMatrix();
		}

		if( m_currentMaterial != nullptr )
		{
			if( m_currentMaterial )
			{
				if( m_orthoEnabled )
				{
					m_currentMaterial->updateUniform( "uProjectionMatrix", m_orthographicProjectionMatrix );
					m_currentMaterial->updateUniform( "uViewMatrix", mat4f::identity() );
					m_currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_orthographicProjectionMatrix );
				}
				else
				{
					m_currentMaterial->updateUniform( "uProjectionMatrix", m_perspectiveProjectionMatrix );
					m_currentMaterial->updateUniform( "uViewMatrix", m_viewMatrix );
					m_currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_viewMatrix * m_perspectiveProjectionMatrix );
				}
				m_currentMaterial->activateProgram();
			}
			m_currentMaterial->updateUniform( "uModelMatrix", m_modelStack.top() );
			m_currentMaterial->activateProgram();
		}
		else
		{
			glUseProgram( 0 );
		}

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::disableRenderStates()
	{
		glDisableVertexAttribArray( m_positionLocation );
		glDisableVertexAttribArray( m_normalLocation );
		glDisableVertexAttribArray( m_colorLocation );
		glDisableVertexAttribArray( m_texCoordLocation );
		glDisableVertexAttribArray( m_tangentLoction );
		glDisableVertexAttribArray( m_bitangentLocation );
		glDisable( GL_TEXTURE_2D );
		glUseProgram( 0 );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadShaders( const char* file )
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile( file );

		if( !doc.Error() )
		{
			tinyxml2::XMLElement* shaderProgram;
			for( shaderProgram = doc.FirstChildElement( "ShaderProgram" ); shaderProgram != 0; shaderProgram = shaderProgram->NextSiblingElement( "ShaderProgram" ) )
			{	
				std::string shaderName = shaderProgram->Attribute( "name" );
				const char* vertexShader = shaderProgram ->Attribute( "vertex" );
				const char* fragShader = shaderProgram->Attribute( "frag" );
				createShaderProgram( shaderName, vertexShader, fragShader );
			}
		}
		else
		{
			reportError( "Failed to load shader file", "File could not be opened" );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadMaterials( const char* file )
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile( file );

		if( !doc.Error() )
		{
			tinyxml2::XMLElement* materialElement;
			for( materialElement = doc.FirstChildElement( "Material" ); materialElement != 0; materialElement = materialElement->NextSiblingElement( "Material" ) )
			{
				std::string materialName = materialElement->Attribute( "name" );
				std::string shaderProgramName = materialElement->Attribute( "shaderProgram" );
				createMaterial( materialName, shaderProgramName );
				Material* material = getMaterial( materialName );
				tinyxml2::XMLElement* uniform;
				for( uniform = materialElement->FirstChildElement( "Uniform" ); uniform != 0; uniform = uniform->NextSiblingElement( "Uniform" ) )
				{
					addUniform( uniform, material );
				}
			}
		}
		else
		{
			reportError( "Failed to load Material file", "File could not be opened" );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadFonts( const char* file )
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile( file );

		if( !doc.Error() )
		{
			tinyxml2::XMLElement* font;
			for( font = doc.FirstChildElement( "Font" ); font != 0; font = font->NextSiblingElement( "Font" ) )
			{	
				std::string fontName = font->Attribute( "name" );
				const char* xmlFile = font ->Attribute( "xmlFile" );
				const char* bmpFile = font->Attribute( "bmpFile" );
				const char* shadername = font->Attribute( "shaderProgram" );
				createFont( fontName, shadername, xmlFile, bmpFile );
			}
		}
		else
		{
			reportError( "Failed to load shader file", "File could not be opened" );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::addUniform( tinyxml2::XMLElement* element, Material* material )
	{
		const char* name = element->Attribute( "name" );
		std::string type = element->Attribute( "type" );

		if( type == "texture" )
		{
			const char* texture = element->GetText();
			unsigned int textureID = 0;
			if( texture )
			{
				if( stringToType( texture, &textureID ) )
				{
					material->addTexture( name, createTexture( name, textureID ) );	
				}
				else
				{
					material->addTexture( name, getTexture( texture ) );
				}
			}
		}
		else if( type == "int" )
		{
			int uniform;
			if( stringToType( element->GetText(), &uniform ) )
			{
				material->addUniform( name,  uniform );
			}
			else
			{
				material->addUniform( name, 0 );
			}
		}
		else if( type == "unsigned int" )
		{
			unsigned int uniform;
			if( stringToType( element->GetText(), &uniform ) )
			{
				material->addUniform( name,  uniform );
			}
			else
			{
				material->addUniform( name, 0 );
			}
		}
		else if( type == "float" )
		{
			float uniform;
			const char* text = element->GetText();
			if( text )
			{
				if( stringToType( text, &uniform ) )
				{
					material->addUniform( name,  uniform );
				}
				else
				{
					material->addUniform( name, 0 );
				}
			}
		}
		else if( type == "vec2i" )
		{
			const char* text = element->GetText();
			vec2i uniform;
			if( text )
			{
				uniform = parseStringToVec2< int >( text );
			}
			material->addUniform( name,  uniform );
		}
		else if( type == "vec2ui" )
		{
			const char* text = element->GetText();
			vec2ui uniform;
			if( text )
			{
				uniform = parseStringToVec2< unsigned int >( text );
			}
			material->addUniform( name,  uniform );
		}
		else if( type == "vec2f" )
		{
			const char* text = element->GetText();
			vec2f uniform;
			if( text )
			{
				uniform = parseStringToVec2< float >( text );
			}
			material->addUniform( name, uniform );
		}
		else if( type == "vec3i" )
		{
			const char* text = element->GetText();
			vec3i uniform;
			if( text )
			{
				uniform = parseStringToVec3< int >( text );
			}
			material->addUniform( name,  uniform );
		}
		else if( type == "vec3ui" )
		{
			const char* text = element->GetText();
			vec3ui uniform;
			if( text )
			{
				uniform = parseStringToVec3< unsigned int >( text );
			}
			material->addUniform( name,  uniform );
		}
		else if( type == "vec3f" )
		{
			const char* text = element->GetText();
			vec3f uniform;
			if( text )
			{
				uniform = parseStringToVec3< float >( text );
			}
			material->addUniform( name,  uniform );	
			
		}
		else if( type == "mat4f" )
		{
			material->addUniform( name, mat4f() );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadTextures( const char* file )
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile( file );

		if( !doc.Error() )
		{
			tinyxml2::XMLElement* texture;
			for( texture = doc.FirstChildElement( "Texture" ); texture != 0; texture = texture->NextSiblingElement( "Texture" ) )
			{	
				std::string textureName = texture->Attribute( "name" );
				const char* textureFile = texture->GetText();
				createTexture( textureName, textureFile );
			}
		}
		else
		{
			reportError( "Failed to load texture file", "File could not be opened" );
		}
	}
}