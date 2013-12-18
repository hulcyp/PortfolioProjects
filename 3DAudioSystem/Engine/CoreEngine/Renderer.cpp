#ifdef _WIN32
#include <glew.h>
#endif
#include <Gl/GL.h>
#include <fstream>
#include "Renderer.h"
#include "MathFuncs.h"
#include "Mesh.h"
#include "Font.h"
#include "tinyxml2.h"
#include "MonkyException.h"
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

namespace Monky
{
	Renderer::Renderer( int width, int height, float fov, float zNear, float zFar )
		:	m_screenWidth( width )
		,	m_screenHeight( height )
		,	m_renderWireFrame( false )
		,	m_lineWidth( 1.0f )
		,	m_backFaceCullingEnabled( true )
	{
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glFrontFace( GL_CW );
		glLineWidth( m_lineWidth );

		m_cameraStack.push( new Camera( "RendererCamera", vec3f(), Camera::PH_PERSPECTIVE, mat4f::perspective( fov, (float)width / height, zNear, zFar ) ) );

		registerForEvent( "renderMesh",  *this, &Renderer::renderMeshEvent );
		registerForEvent( "renderDebugMesh", *this, &Renderer::renderDebugMeshEvent );
		registerForEvent( "renderText", *this, &Renderer::renderTextEvent );
		registerForEvent( "renderAxis", *this, &Renderer::renderAxisEvent );
	}

	Renderer::~Renderer()
	{
		//Only delete the initial camera created by the renderer
		while( m_cameraStack.size() != 1 )
			m_cameraStack.pop();
		SAFE_DELETE( m_cameraStack.top() );
		m_cameraStack.pop();
		cleanupRendereringData();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadRendereringDataFiles()
	{
		loadShaders( "ShaderPrograms.xml", "shaders/" );
		loadTextures( "Textures.xml", "textures/" );
		loadMaterials( "Materials.xml", "" );
		loadFonts( "Fonts.xml", "fonts/" );
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

			bool wireFrame = false;
			bool prevWireFrame = m_renderWireFrame;
			if( params.get( "wireFrame", wireFrame ) == NamedProperties::SUCCESS )
				setWireFrame( wireFrame );

			float lineWidth = 0.0f;
			float prevLineWidth = m_lineWidth;
			if( params.get( "lineWidth", lineWidth ) == NamedProperties::SUCCESS )
				setLineWidth( lineWidth );

			bool enableBFCulling = true;
			bool prevBFCulling = m_backFaceCullingEnabled;
			if( params.get( "enableBackFaceCulling", enableBFCulling ) == NamedProperties::SUCCESS )
				setBackFaceCulling( enableBFCulling );

			renderMesh( mesh );

			setWireFrame( prevWireFrame );
			setLineWidth( prevLineWidth );
			setBackFaceCulling( prevBFCulling );

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
				Material::useMaterial( mesh->getMaterial( i ), m_lightingEnabled );
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
				}
				else
				{
					updateMaterialUniforms();
					glDrawArrays( mesh->getRenderMode( i ), i, mesh->getNumVertices( i ) );
					glAssertError;
				}
				mesh->getVBOBuffer( i )->disableAttributeArrays();
				glBindBuffer( GL_ARRAY_BUFFER, 0 );
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );				
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderDebugMeshEvent( NamedProperties& params )
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

			bool wireFrame = false;
			bool prevWireFrame = m_renderWireFrame;
			if( params.get( "wireFrame", wireFrame ) == NamedProperties::SUCCESS )
				setWireFrame( wireFrame );

			renderDebugMesh( mesh );

			setWireFrame( prevWireFrame );

			if( hasModelStack )
				m_modelStack.pop();
			if( hasCamera )
				popCamera();			
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderDebugMesh( Mesh* mesh )
	{
		if( mesh )
		{
			Material::useMaterial( mesh->getDebugMaterial(), m_lightingEnabled );
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
		float fontHeight = 12.0f;
		Color4f color;
		std::string posRel;
		bool fixedWidth = false;
		bool shadow = false;
		mat4f modelMatrix;

		params.get( "fontName", fontName );
		params.get( "text", text );
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
			renderText( fontName, text, m_modelStack.top(), fontHeight );
		}
		else if( params.get( "posRel", posRel ) != NamedProperties::SUCCESS )
		{
			renderText( fontName, text, m_modelStack.top(), fontHeight, color );
		}
		else if( params.get( "shadow", shadow ) != NamedProperties::SUCCESS )
		{
			renderText( fontName, text, m_modelStack.top(), fontHeight, color, Font::getRelativePosFromStr( posRel ) );
		}
		else if( params.get( "fixedWidth", fixedWidth ) != NamedProperties::SUCCESS )
		{
			renderText( fontName, text, m_modelStack.top(), fontHeight, color, Font::getRelativePosFromStr( posRel ), shadow );
		}
		else
		{
			renderText( fontName, text, m_modelStack.top(), fontHeight, color, Font::getRelativePosFromStr( posRel ), shadow, fixedWidth );
		}

		if( hasCamera )
			popCamera();

		if( hasModelMat )
			m_modelStack.pop();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderText( const std::string& fontName, const std::string& text, const mat4f& transformation, float fontHeight, const Color4f& color, const vec2f& posRel, bool drawWithShadow, bool fixedWidth )
	{
		Font* font = Font::getFont( fontName );
		if( font )
		{
			font->renderText( text, transformation, fontHeight, color, posRel, drawWithShadow, fixedWidth );
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
				Material* fboMaterial = Material::getMaterial( m_fboQueue[i]->getMaterial() );
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
	void Renderer::setProjectionMode( Camera::ProjectionMode mode )
	{
		if( mode == Camera::PH_ORTHOGRAPHIC )
		{
			glPushAttrib( GL_DEPTH_TEST );
			glDisable( GL_DEPTH_TEST );
		}
		else if( mode == Camera::PH_PERSPECTIVE && m_projectionMode == Camera::PH_ORTHOGRAPHIC )
		{
			glPopAttrib();
		}
		m_projectionMode = mode;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::setViewport( int x, int y, int width, int height )
	{
		glViewport( x, y, width, height );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::pushCamera( Camera* camera )
	{
		if( camera != nullptr )
		{
			m_cameraStack.push( camera );
			setProjectionMode( camera->getProjectionMode() );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::popCamera()
	{
		if( m_cameraStack.size() > 1 )
			m_cameraStack.pop();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Utility functions
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderAxisEvent( NamedProperties& params )
	{
		float length = 0.0f;
		params.get( "length", length );
		renderAxis( length );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::renderAxis( float length )
	{
		Mesh* mesh = MeshFactory::generateAxis( length );
		renderMesh( mesh );
		delete mesh;
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
		m_lineWidth = lineWidth;
	}
	//--------------------------------------------------------------------------------------------
	void Renderer::setBackFaceCulling( bool enabled )
	{
		m_backFaceCullingEnabled = enabled;
		if( m_backFaceCullingEnabled )			
			glEnable( GL_CULL_FACE );
		else
			glDisable( GL_CULL_FACE );
	}
	//--------------------------------------------------------------------------------------------
	bool Renderer::getBackFaceCulling() const
	{
		return m_backFaceCullingEnabled;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Protected members
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::cleanupRendereringData()
	{
		Font::cleanupFonts();
		Material::cleanupMaterials();
		ShaderProgram::cleanupShaderPrograms();
		Shader::cleanupShaders();
		Texture::CleanupTextures();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::updateMaterialUniforms()
	{
		Material* currentMaterial = Material::getActiveMaterial();
		if( currentMaterial != nullptr )
		{
			currentMaterial->updateUniform( "uProjectionMatrix", m_cameraStack.top()->getProjectionMatrix() );
			currentMaterial->updateUniform( "uViewMatrix", m_cameraStack.top()->getViewMatrix() );
			currentMaterial->updateUniform( "uMVPMatrix", m_modelStack.top() * m_cameraStack.top()->getViewMatrix() * m_cameraStack.top()->getProjectionMatrix() );
			currentMaterial->updateUniform( "uModelMatrix", m_modelStack.top() );
			currentMaterial->updateUniforms();
		}
		//else
		//{
		//	glUseProgram( 0 );
		//}

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadShaders( const std::string& fileName, const std::string& filePath )
	{
		std::string file = filePath + fileName;
		XMLParser parser( file.c_str() );
		XMLDocument& doc = parser.getDocument();

		if( !doc.Error() )
		{
			XMLNode* node = doc.FirstChildElement( "ShaderPrograms" );			
			XMLNode* shaderProgram;
			for( shaderProgram = node->FirstChildElement( "ShaderProgram" ); shaderProgram != 0; shaderProgram = shaderProgram->NextSiblingElement( "ShaderProgram" ) )
			{	
				std::string shaderName = shaderProgram->Attribute( "name" );
				const char* vertexShader = shaderProgram ->Attribute( "vertex" );
				const char* fragShader = shaderProgram->Attribute( "frag" );
				ShaderProgram::createShaderProgram( shaderName, filePath + std::string( vertexShader ), filePath + std::string( fragShader ) );
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadMaterials( const std::string& fileName, const std::string& filePath )
	{
		std::string file = filePath + fileName;
		XMLParser parser( file.c_str() );
		XMLDocument& doc = parser.getDocument();
		
		if( !doc.Error() )
		{
			XMLNode* root = doc.FirstChildElement( "Materials" );
			XMLNode* materialElement;
			for( materialElement = root->FirstChildElement( "Material" ); materialElement != 0; materialElement = materialElement->NextSiblingElement( "Material" ) )
			{
				parser.validateXMLChildElements( materialElement, "Uniform", "Occlusion" );
				parser.validateXMLAttributes( materialElement, "name,shaderProgram", "" );
				std::string materialName = materialElement->Attribute( "name" );
				std::string shaderProgramName = materialElement->Attribute( "shaderProgram" );
				Material::createMaterial( materialName, shaderProgramName );
				Material* material = Material::getMaterial( materialName );
				
				XMLNode* occlusion = materialElement->FirstChildElement( "Occlusion" );
				if( occlusion )
				{
					parser.validateXMLAttributes( occlusion, "", "direct,reverb,type" );
					std::string strType = parser.getXMLAttributeAsString( occlusion, "type", "" ); 
					if( !strType.empty() )
					{
						Material::MaterialType type = Material::getTypeFromString( strType );
						material->setMaterialType( type );
					}
					else
					{
						float direct = parser.getXMLAttributeAsFloat( occlusion, "direct", 0.0f );
						float reverb = parser.getXMLAttributeAsFloat( occlusion, "reverb", 0.0f );
						material->setDirectOcclusion( direct );
						material->setReverbOcclusion( reverb );
					}
				}
				
				tinyxml2::XMLElement* uniform;
				for( uniform = materialElement->FirstChildElement( "Uniform" ); uniform != 0; uniform = uniform->NextSiblingElement( "Uniform" ) )
				{
					parser.validateXMLAttributes( uniform, "name,type", "" );
					addUniform( uniform, material );
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::loadFonts( const std::string& fileName, const std::string& filePath )
	{
		std::string file = filePath + fileName;
		XMLParser parser( file.c_str() );
		XMLDocument& doc = parser.getDocument();
		XMLNode* root = doc.FirstChildElement( "Fonts" );
		
		if( !doc.Error() )
		{
			XMLNode* font;
			for( font = root->FirstChildElement( "Font" ); font != 0; font = font->NextSiblingElement( "Font" ) )
			{	
				std::string fontName = font->Attribute( "name" );
				const char* xmlFile = font ->Attribute( "xmlFile" );
				const char* bmpFile = font->Attribute( "bmpFile" );
				const char* shadername = font->Attribute( "shaderProgram" );
				Font::createFont( fontName, filePath, shadername, xmlFile, bmpFile );
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Renderer::addUniform( XMLNode* element, Material* material )
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
					material->addTexture( name, Texture::CreateOrGetTexture( name, textureID ) );	
				}
				else
				{
					material->addTexture( name, Texture::GetTexture( texture ) );
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
	void Renderer::loadTextures( const std::string& fileName, const std::string& filePath )
	{
		std::string file = filePath + fileName;
		XMLParser parser( file.c_str() );
		XMLDocument& doc = parser.getDocument();

		if( !doc.Error() )
		{
			XMLNode* root = doc.FirstChildElement( "Textures" );
			XMLNode* texture;
			for( texture = root->FirstChildElement( "Texture" ); texture != 0; texture = texture->NextSiblingElement( "Texture" ) )
			{	
				std::string textureName = texture->Attribute( "name" );
				const char* textureFile = texture->GetText();
				Texture::CreateOrGetTexture( textureName, filePath + std::string( textureFile ) );
			}
		}
	}
}