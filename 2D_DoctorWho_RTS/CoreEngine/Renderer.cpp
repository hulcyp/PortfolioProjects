#ifdef _WIN32
#include <glew.h>
#endif
#include <glut.h>
#include <fstream>
#include "Renderer.h"
#include "MathFuncs.h"
#include "Mesh.h"
#include "Font.h"
#include "tinyxml2.h"
#include "ErrorHandlingUtils.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "StringUtils.h"
#include "Camera.h"
#include <algorithm>
#include "FrameBufferObject.h"
#include "MeshFactory.h"
#include "glHelper.h"
#include "BufferLayout.h"
#include "GLBuffer.h"
#include "DebugStatistics.h"

namespace pdh
{
	std::map< std::string, Material* > Renderer::sm_materials;
	std::map< std::string, ShaderProgram* > Renderer::sm_programs;
	std::map< std::string, Shader* > Renderer::sm_shaders;
	std::map< std::string, Texture* > Renderer::sm_textures;


	Renderer::Renderer( int width, int height, float fov, float zNear, float zFar )
		:	m_screenWidth( width )
		,	m_screenHeight( height )
		//,	m_projectionMode( PH_PERSPECTIVE )
		,	m_renderWireFrame( false )
		,	m_currentMaterial( nullptr )
	{
		loadShaders( "../data/shaders/ShaderPrograms.xml" );
		loadTextures( "../data/textures/Textures.xml" );
		loadMaterials( "../data/materials/Materials.xml" );
		loadFonts( "../data/fonts/Fonts.xml" );

		//m_perspectiveProjStack.push( mat4f::perspective( fov ,static_cast<float>( width ) / height,  zNear, zFar ) );
		//m_orthographicProjStack.push( mat4f::ortho( 0.0f, (float)width, (float)height, 0.0f, 1.0f, -1.0f ) );
		m_cameraStack.push( new Camera( "RendererCamera", vec3f(), mat4f::perspective( fov, (float)width / height, zNear, zFar ) ) );

		registerForEvent( "renderMesh",  *this, &Renderer::renderMeshEvent );
		registerForEvent( "renderText", *this, &Renderer::renderTextEvent );
	}

	Renderer::~Renderer()
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::createShaderProgram( const std::string& shaderName, const char* vertexFile, const char* fragFile )
	{
		Shader*& vertex = sm_shaders[ vertexFile ];
		if( !vertex )
		{
			vertex = new Shader( "../data/shaders/" + std::string( vertexFile ), GL_VERTEX_SHADER );
		}

		Shader*& frag = sm_shaders[ fragFile ];
		if( !frag )
		{
			frag = new Shader( "../data/shaders/" + std::string( fragFile ), GL_FRAGMENT_SHADER );
		}		

		ShaderProgram* program = new ShaderProgram( shaderName, vertex, frag );

		sm_programs[ shaderName ] = program;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Material* Renderer::createMaterial( const std::string& materialName, const std::string& shaderProgramName )
	{
		Material*& newMaterial = sm_materials[ materialName ];
		if( !newMaterial )
		{
			newMaterial = new Material( sm_programs[ shaderProgramName ] );

			newMaterial->addUniform( "uProjectionMatrix", mat4f::identity() );
			newMaterial->addUniform( "uViewMatrix", mat4f::identity() );
			newMaterial->addUniform( "uModelMatrix", mat4f::identity() );
			newMaterial->addUniform( "uMVPMatrix", mat4f::identity() );
			newMaterial->addUniform( "time", 0.0f );
			newMaterial->addUniform( "uUnlit", 0 );
		}		
		return newMaterial;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::useMaterial( const std::string& materialName )
	{
		std::map< std::string, Material* >::iterator iter = sm_materials.find( materialName );
		if( materialName != "" && iter != sm_materials.end() )
		{
			Material* newMat = iter->second;
			if( m_currentMaterial == nullptr )
			{
				m_currentMaterial = newMat;
			}
			else if( newMat != m_currentMaterial )
			{
				m_currentMaterial->deactivateProgram();
				m_currentMaterial = newMat;
				DebugStatistics::MATERIAL_SWAPS++;
			}
			m_currentMaterial->updateUniform( "uUnlit", (int)(!m_lightingEnabled) );
			m_currentMaterial->activateProgram();
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
		Texture*& newTexture = sm_textures[ textureName ];
		if( !newTexture )
		{
			newTexture = new Texture( "../data/textures/" + fileName );
		}
		return newTexture;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Texture* Renderer::createTexture( const std::string& textureName, unsigned int textureID )
	{
		Texture* newTexture = sm_textures[ textureName ];
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
		if( materialName != "" && sm_materials.find( materialName ) != sm_materials.end() )
		{
			material = sm_materials[ materialName ];
		}
		return material;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ShaderProgram* Renderer::getShaderProgram( const std::string programName )
	{
		ShaderProgram* shaderProgram = nullptr;
		if( programName != "" && sm_programs.find( programName ) != sm_programs.end() )
		{
			shaderProgram = sm_programs[ programName ];
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
		Texture* texture = nullptr;
		if( !textureName.empty() && sm_textures.find( textureName ) != sm_textures.end() )
		{
			texture = sm_textures[ textureName ];
		}
		return texture;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderMeshEvent( NamedProperties& params )
	{
		Mesh* mesh;
		if( params.get( "mesh", mesh ) == NamedProperties::SUCCESS )
		{
			mat4f modelMatrix;
			bool hasModelStack = params.get( "modelMatrix", modelMatrix ) == NamedProperties::SUCCESS; 
			if( hasModelStack )
				m_modelStack.pushMatrix( modelMatrix );

			Camera* camera = nullptr;
			bool hasCamera = params.get( "camera", camera ) == NamedProperties::SUCCESS;
			if( hasCamera )
				pushCamera( camera );

			renderMesh( mesh );

			if( hasModelStack )
				m_modelStack.pop();
			if( hasCamera )
				popCamera();
			
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderMesh( Mesh* mesh )
	{
		if( mesh )
		{			
			for( size_t i = 0; i < mesh->getNumTriBatches(); ++i )
			{
				useMaterial( mesh->getMaterial( i ) );
				glBindBuffer( GL_ARRAY_BUFFER, mesh->getVBOID( i ) );
				glAssertError;
				if( mesh->getIBOID( i ) )
				{
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh->getIBOID( i ) );
					mesh->updateMaterial( m_modelStack.top(), i );
					updateMaterialUniforms();
					mesh->getVBOBuffer( i )->enableAttributeArrays();
					glDrawElements( mesh->getRenderMode( i ), mesh->getNumIndices( i ), GL_UNSIGNED_INT, 0 );
					glAssertError;
					DebugStatistics::RENDER_CALLS++;
				}
				else
				{
					updateMaterialUniforms();
					glDrawArrays( mesh->getRenderMode( i ), i, mesh->getNumVertices( i ) );
					glAssertError;
					DebugStatistics::RENDER_CALLS++;
				}
				mesh->getVBOBuffer( i )->disableAttributeArrays();
				glBindBuffer( GL_ARRAY_BUFFER, 0 );
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );				
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderDebugMesh( Mesh* mesh )
	{
		if( mesh )
		{
			useMaterial( mesh->getDebugMaterial() );
			for( size_t i = 0; i < mesh->getNumTriBatches(); ++i )
			{
				glBindBuffer( GL_ARRAY_BUFFER, mesh->getVBOID( i ) );
				glAssertError;
				if( mesh->getIBOID( i ) )
				{
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh->getIBOID( i ) );
					updateMaterialUniforms();
					glDrawElements( mesh->getRenderMode( i ), mesh->getNumIndices( i ), GL_UNSIGNED_INT, 0 );
					glAssertError;
				}
				else
				{
					updateMaterialUniforms();
					glDrawArrays( mesh->getRenderMode( i ), i, mesh->getNumVertices( i ) );
					glAssertError;
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderTextEvent( NamedProperties& params )
	{
		std::string fontName;
		std::string text;
		vec3f pos;
		float fontHeight = 12.0f;
		Color4f color;
		vec2f posRel;
		bool fixedWidth;
		mat4f modelMatrix;

		params.get( "fontName", fontName );
		params.get( "text", text );
		params.get( "pos", pos );
		params.get( "fontHeight", fontHeight );
		
		bool hasModelMat = params.get( "modelMatrix", modelMatrix ) == NamedProperties::SUCCESS;

		if( hasModelMat )
			m_modelStack.pushMatrix( modelMatrix );

		Camera* camera = nullptr;
		bool hasCamera = params.get( "camera", camera ) == NamedProperties::SUCCESS;
		if( hasCamera )
			pushCamera( camera );
		
		if( params.get( "color", color ) != NamedProperties::SUCCESS )
		{
			renderText( fontName, text, pos, fontHeight );
		}
		else if( params.get( "posRel", posRel ) != NamedProperties::SUCCESS )
		{
			renderText( fontName, text, pos, fontHeight, color );
		}
		else if( params.get( "fixedWidth", fixedWidth ) != NamedProperties::SUCCESS )
		{
			renderText( fontName, text, pos, fontHeight, color, posRel );
		}
		else
		{
			renderText( fontName, text, pos, fontHeight, color, posRel, fixedWidth );
		}

		if( hasCamera )
			popCamera();

		if( hasModelMat )
			m_modelStack.pop();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderText( const std::string& fontName, const std::string& text, vec3f pos, float fontHeight, const Color4f& color, const vec2f& posRel, bool fixedWidth )
	{
		Font* font = getFont( fontName );
		if( font )
		{
			font->renderText( text, pos, fontHeight, color, posRel, fixedWidth );
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
		//setProjectionMode( PH_ORTHOGRAPHIC );
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
		//setProjectionMode( PH_PERSPECTIVE );
		m_modelStack.pop();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const mat4f& Renderer::getViewMatrix() const
	{
		return m_cameraStack.top()->getViewMatrix();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::setProjectionMode( const ProjectionMode& mode )
	{
		if( mode == PH_ORTHOGRAPHIC )
		{
			glPushAttrib( GL_DEPTH_TEST );
			glDisable( GL_DEPTH_TEST );
		}
		else if( mode == PH_PERSPECTIVE && m_projectionMode == PH_ORTHOGRAPHIC )
		{
			glPopAttrib();
		}
		m_projectionMode = mode;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//void Renderer::pushProjectionMatrix( const mat4f& projMatrix )
	//{
	//	if( m_projectionMode == PH_ORTHOGRAPHIC )
	//	{
	//		m_orthographicProjStack.push( projMatrix );
	//	}
	//	else if( m_projectionMode == PH_PERSPECTIVE )
	//	{
	//		m_perspectiveProjStack.push( projMatrix );
	//	}
	//}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//void Renderer::popProjectionMatrix()
	//{
	//	if( m_projectionMode == PH_ORTHOGRAPHIC && m_orthographicProjStack.size() > 1 )
	//	{
	//		m_orthographicProjStack.pop();			
	//	}
	//	else if( m_projectionMode == PH_PERSPECTIVE && m_perspectiveProjStack.size() > 1 )
	//	{
	//		m_perspectiveProjStack.pop();
	//	}
	//}
	const mat4f& Renderer::getProjMatrix() const
	{
		return m_cameraStack.top()->getProjectionMatrix();
		//if( m_projectionMode == PH_ORTHOGRAPHIC )
		//{
		//	return m_orthographicProjStack.top();
		//}
		//else if( m_projectionMode == PH_PERSPECTIVE )
		//{
		//	return m_perspectiveProjStack.top();
		//}
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
		Mesh* mesh = MeshFactory::generateAxis( length );
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
		if( m_cameraStack.size() > 1 )
			m_cameraStack.pop();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::updateTimeOnMaterials( float time )
	{
		std::for_each( sm_materials.begin(), sm_materials.end(),
			[&]( std::pair< std::string, Material* > material )
		{
			material.second->updateUniform( "time", time );
		} );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Protected members
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::updateMaterialUniforms()
	{
		//if( m_cameraStack.top() != nullptr )
		//{
		//	m_viewMatrix = m_cameraStack.top()->getViewMatrix();
		//}

		if( m_currentMaterial != nullptr )
		{
				
			//if( m_projectionMode == PH_ORTHOGRAPHIC )
			//{
			//	m_currentMaterial->updateUniform( "uProjectionMatrix", m_orthographicProjStack.top() );
			//	m_currentMaterial->updateUniform( "uViewMatrix", m_viewMatrix );
			//	m_currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_viewMatrix * m_orthographicProjStack.top() );
			//}
			//else if( m_projectionMode == PH_PERSPECTIVE )
			//{
			//	m_currentMaterial->updateUniform( "uProjectionMatrix", m_perspectiveProjStack.top() );
			//	m_currentMaterial->updateUniform( "uViewMatrix", m_viewMatrix );
			//	m_currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_viewMatrix * m_perspectiveProjStack.top() );
			//}
			m_currentMaterial->updateUniform( "uProjectionMatrix", m_cameraStack.top()->getProjectionMatrix() );
			m_currentMaterial->updateUniform( "uViewMatrix", m_cameraStack.top()->getViewMatrix() );
			m_currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_cameraStack.top()->getViewMatrix() * m_cameraStack.top()->getProjectionMatrix() );
			m_currentMaterial->updateUniform( "uModelMatrix", m_modelStack.top() );
			m_currentMaterial->updateUniforms();
		}
		else
		{
			glUseProgram( 0 );
		}

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