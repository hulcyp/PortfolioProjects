#include "Font.h"
#include <ErrorHandlingUtils.h>
#include <MathFuncs.h>
#include "Renderer.h"
#include "Material.h"
#include "ShaderProgram.h"
#include <Texture.h>

namespace pdh
{
	Font::Font( const std::string& materialName, const std::string& shaderName, const std::string& xmlFile, const std::string& bitmapFile, Renderer* renderer )
		:	m_renderer( renderer )
		,	m_material( materialName )
	{
		loadFont( xmlFile.c_str() );
		m_textures.push_back( new Texture ( bitmapFile.c_str() ) );
		Material* material = Renderer::createMaterial( m_material, shaderName );
		material->addUniform( "uFontHeight", 0.0f );
		material->addTexture( "uTexels", m_textures[0] );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Font::~Font()
	{}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::renderText( const std::string& text, vec3f pos, float fontHeight, const Color4f& color, const vec2f& relPos, bool fixedWidth )
	{
		
		Mesh* sMesh = generateVertices( text, pos + vec3f( -1.0f, -1.0f, 0.0f ), fontHeight, vec3f( 1.0f, 0.0f, 0.0f ), 
				vec3f( 0.0f, 1.0f, 0.0f ), color::FADEDBLACK, relPos, fixedWidth );
		
		Mesh* mesh = generateVertices( text, pos, fontHeight, vec3f( 1.0f, 0.0f, 0.0f ), 
				vec3f( 0.0f, 1.0f, 0.0f ), color, relPos, fixedWidth );

		Material* material = Renderer::getMaterial( mesh->getMaterial( 0 ) );
		material->updateUniform( "uFontHeight", fontHeight );
		matStackf& modelStack = m_renderer->getModelStack();
		modelStack.push();
		modelStack.loadIdentity();
		modelStack.translate( pos );
		m_renderer->setProjectionMode( Renderer::PH_ORTHOGRAPHIC );
		m_renderer->renderMesh( sMesh );
		m_renderer->renderMesh( mesh );

		modelStack.pop();
		
		SAFE_DELETE( sMesh );
		SAFE_DELETE( mesh );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::render3DText( const std::string& text, vec3f pos, float fontHeight, vec3f rasterDir, vec3f up, const Color4f& color, const vec2f& relPos, bool fixedWidth )
	{
		
		Mesh* mesh = generateVertices( text, pos, fontHeight, rasterDir, up, color, relPos, fixedWidth );

		matStackf& modelStack = m_renderer->getModelStack();
		modelStack.push();
		modelStack.loadIdentity();
		modelStack.translate( pos );
		
		m_renderer->renderMesh( mesh );
		
		modelStack.pop();

		SAFE_DELETE( mesh );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::renderBillboardText( const std::string& text, vec3f pos, float fontHeight, vec3f cameraK, const Color4f& color, const vec2f& relPos, bool fixedWidth )
	{
		vec3f kb( cameraK );
		kb.normalize();
		vec3f ib( vec3f( 0.0f, 1.0f, 0.0f ).cross( kb ) );
		ib.normalize();
		vec3f jb( kb.cross( ib ) );
		jb.normalize();

		render3DText( text, pos, fontHeight, ib, jb, color, relPos, fixedWidth );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float Font::getLength( const std::string& text, float fontHeight, bool fixedWidth )
	{
		float length = 0.0f;
		for( size_t i = 0; i < text.size(); ++i )
		{
			Glyph glyph = m_glyphs[ text[i] ];
			length += (glyph.A + glyph.B + glyph.C) * fontHeight;
		}
		return length;
	}	
	//////////////////////////////////////////////////////////
	// Protected member varaibles
	/////////////////////////////////////////////////////////
	Font::Glyph Font::loadGlyph( tinyxml2::XMLElement* node )
	{
		unsigned int ucsIndex = 0;
		unsigned int sheet = 0;
		if( tinyxml2::XML_NO_ERROR != node->QueryUnsignedAttribute( "ucsIndex", &ucsIndex ) )
			reportError( "Error loading font", "UCS Index invalid" );

		if( tinyxml2::XML_NO_ERROR != node->QueryUnsignedAttribute( "sheet", &sheet ) )
			reportError( "Error loading font", "Sheet number invalid" );
				
		std::string texCMin;
		std::vector< std::string > tokens;
		texCMin = node->Attribute( "texCoordMins" );
		if( texCMin == "" )
			reportError( "Error loading font", "Min Texture Coordinate invalid" );

		stringTokenizer( texCMin, tokens, "," );
		vec2f texCoordMin ( (float)atof( tokens[0].c_str() ), (float)atof( tokens[1].c_str() ) ) ;

		std::string texCMax;
		tokens.clear();
		texCMax = node->Attribute( "texCoordMaxs" );
		if( texCMax == "" )
			reportError( "Error loading font", "Max Texture Coordinate invalid" );

		stringTokenizer( texCMax, tokens, "," );
		vec2f texCoordMax( (float)atof( tokens[0].c_str() ), (float)atof( tokens[1].c_str() ) );
				
		float A;
		float B;
		float C;
		if( tinyxml2::XML_NO_ERROR != node->QueryFloatAttribute( "ttfA", &A ) )
			reportError( "Error loading font", "A value invalid" );
		if( tinyxml2::XML_NO_ERROR != node->QueryFloatAttribute( "ttfB", &B ) )
			reportError( "Error loading font", "B value invalid" );
		if( tinyxml2::XML_NO_ERROR != node->QueryFloatAttribute( "ttfC", &C ) )
			reportError( "Error loading font", "C value invalid" );

		return Glyph( ( unsigned char )ucsIndex, ( unsigned char )sheet, 
			texCoordMin, texCoordMax, A, B, C );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::loadFont( const char* xmlFile )
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile( xmlFile );
		
		if( !doc.Error() )
		{		
			tinyxml2::XMLElement* root = doc.FirstChildElement( "FontDefinition" );
			tinyxml2::XMLElement* fontInfo = root->FirstChildElement( "FontInfo" );
			m_fontName = fontInfo->Attribute( "name" );
			m_fontLocale = fontInfo->Attribute( "locale" );
			if( fontInfo->QueryIntAttribute( "cellPixelHeight", &m_pixelHeight ) )
				reportError( "Failed to load font", "Invalid pixel height" );

			tinyxml2::XMLElement* element; 
			for( element = root->FirstChildElement( "Glyph" ); element != 0; element = element->NextSiblingElement( "Glyph" ) )
			{
				Glyph g = loadGlyph( element );
				m_glyphs.insert( std::pair< unsigned char, Glyph >( g.ucsIndex, g ) );
			}
		}
		else
		{
			reportError( "Failed to load font", "File could not be opened" );
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh* Font::generateVertices( const std::string& text, vec3f pos, float fontHeight, 
		vec3f rasterDir, vec3f upDir, const Color4f& color, const vec2f& posRel, bool fixedWidth )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		rasterDir.normalize();
		upDir.normalize();
		vec3f rasterPos = rasterDir * ( getLength( text, fontHeight ) * .5f ) * posRel.x + upDir * ( (float)fontHeight * .5f ) * posRel.y;
		vec3f height = upDir * ( -(float)fontHeight ) * (float)MathFuncs<float>::getSign( posRel.y );
		unsigned int index = 0;
		for( size_t i = 0; i < text.size(); ++i )
		{
			Glyph glyph = m_glyphs[text[i]] ;
			vec2f texCoordMin = glyph.texCoordMin;
			vec2f texCoordMax = glyph.texCoordMax;
			float A = 0.0f;
			float B = glyph.B * fontHeight;
			float C = 0.0f;
			float fixedOffset = 0.0f;
			if( fixedWidth && ( text[i] >= '0' && text[i] <= '9' ) )
			{
				A = m_glyphs['0'].A;
				fixedOffset = ( m_glyphs['0'].B * fontHeight - B ) * 0.5f;
				C = m_glyphs['0'].C;
			}
			else
			{
				A = glyph.A;
				C = glyph.C;
			}

			A *= fontHeight;
			C *= fontHeight;

			rasterPos += rasterDir*( A + fixedOffset );
			vertices.push_back( Mesh::Vertex(	rasterPos.x, rasterPos.y, rasterPos.z,
													0.0f, 0.0f, 0.0f,
													color.r, color.g, color.b, color.a, 
													texCoordMin.x, texCoordMin.y ) );
			rasterPos += rasterDir*B;												  
			vertices.push_back( Mesh::Vertex(	rasterPos.x + height.x ,height.y + rasterPos.y, rasterPos.z + height.z, 
													0.0f, 0.0f, 0.0f,
													color.r, color.g, color.b, color.a, 
													texCoordMax.x, texCoordMax.y ) );
			rasterPos -= rasterDir*B;												  
			vertices.push_back( Mesh::Vertex(	rasterPos.x + height.x, height.y + rasterPos.y, rasterPos.z + height.z,
													0.0f, 0.0f, 0.0f,
													color.r, color.g, color.b, color.a,
													texCoordMin.x, texCoordMax.y ) );
			rasterPos += rasterDir*B;												  
			vertices.push_back( Mesh::Vertex(	rasterPos.x + height.x, height.y + rasterPos.y, rasterPos.z + height.z, 
													0.0f, 0.0f, 0.0f,
													color.r, color.g, color.b, color.a, 
													texCoordMax.x, texCoordMax.y ) );
			rasterPos -= rasterDir*B;												  
			vertices.push_back( Mesh::Vertex(	rasterPos.x, rasterPos.y, rasterPos.z, 
													0.0f, 0.0f, 0.0f,
													color.r, color.g, color.b, color.a, 
													texCoordMin.x, texCoordMin.y ) );
			rasterPos += rasterDir*B;												  
			vertices.push_back( Mesh::Vertex(	rasterPos.x, rasterPos.y, rasterPos.z, 
													0.0f, 0.0f, 0.0f,
													color.r, color.g, color.b, color.a, 
													texCoordMax.x, texCoordMin.y ) );
			rasterPos += rasterDir*( C + fixedOffset );

			indices.push_back( index++ );
			indices.push_back( index++ );
			indices.push_back( index++ );
			indices.push_back( index++ );
			indices.push_back( index++ );
			indices.push_back( index++ );
		}

		return new Mesh( vertices, indices, m_material );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const vec2f Font::CENTER( -1.0f, 1.0f );
	const vec2f Font::TOPLEFT( 0.0f, 0.0f );
	const vec2f Font::BOTLEFT( 0.0f, 2.0f );
	const vec2f Font::TOPRIGHT( -2.0f, 0.0f );
	const vec2f Font::BOTRIGHT( -2.0f, 2.0f );
	const vec2f Font::BOTCENTER( -1.0f, 2.0f );
}