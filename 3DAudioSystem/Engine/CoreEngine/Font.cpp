#include "Font.h"
#include <MonkyException.h>
#include <MathFuncs.h>
#include "Renderer.h"
#include "Material.h"
#include "ShaderProgram.h"
#include <Texture.h>

namespace Monky
{
	std::map< std::string, Font* > Font::sm_fonts;
	std::map< std::string, vec2f > Font::sm_posRel;

	Font::Font( const std::string& materialName, const std::string& shaderName, const std::string& xmlFile, const std::string& bitmapFile )
		:	m_material( materialName )
	{
		loadFont( xmlFile.c_str() );
		m_textures.push_back( Texture::CreateOrGetTexture( bitmapFile, bitmapFile ) );
		Material* material = Material::createOrGetMaterial( m_material, shaderName );
		material->addUniform( "uFontHeight", 0.0f );
		material->addTexture( "uTexels", m_textures[0] );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Font::~Font()
	{}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::renderText( const std::string& text, const mat4f& transformation, float fontHeight, const Color4f& color, const vec2f& relPos, bool drawWithShadow, bool fixedWidth )
	{
		
		Mesh* sMesh = nullptr;
		if( drawWithShadow )
		{
			sMesh = generateVertices( text, vec3f( -1.0f, -1.0f, -0.001f ), fontHeight, vec3f( 1.0f, 0.0f, 0.0f ), 
					vec3f( 0.0f, 1.0f, 0.0f ), color::FADEDBLACK, relPos, fixedWidth );
		}

		Mesh* mesh = generateVertices( text, vec3f(), fontHeight, vec3f( 1.0f, 0.0f, 0.0f ), 
				vec3f( 0.0f, 1.0f, 0.0f ), color, relPos, fixedWidth );

		Material* material = Material::getMaterial( mesh->getMaterial( 0 ) );
		material->updateUniform( "uFontHeight", fontHeight );
		//matStackf modelStack;
		//modelStack.translate( pos );
		NamedProperties params;
		params.set( "modelMatrix", transformation );
		params.set( "sMesh", sMesh );
		if( sMesh )
			fireEvent( "renderMesh", params );
		params.set( "mesh", mesh );
		fireEvent( "renderMesh", params );
			
		SAFE_DELETE( sMesh );
		SAFE_DELETE( mesh );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::render3DText( const std::string& text, const vec3f& pos, float fontHeight, vec3f rasterDir, vec3f up, const Color4f& color, const vec2f& relPos, bool fixedWidth )
	{
		
		Mesh* mesh = generateVertices( text, pos, fontHeight, rasterDir, up, color, relPos, fixedWidth );

		matStackf modelStack;
		modelStack.translate( pos );
		NamedProperties params;
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", mesh );
		fireEvent( "renderMesh", params );

		SAFE_DELETE( mesh );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::renderBillboardText( const std::string& text, const vec3f& pos, float fontHeight, vec3f cameraK, const Color4f& color, const vec2f& relPos, bool fixedWidth )
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
			if( text[i] == '<' && i + 1 < text.size() && text[i + 1] == '#' )
			{
				while( ++i < text.size() && text[i] != '>' );
			}
			else if( text[i] != '\n' )
			{
				Glyph glyph = m_glyphs[ text[i] ];
				length += (glyph.A + glyph.B + glyph.C) * fontHeight;
			}
		}
		return length;
	}
	//------------------------------------------------------------------------------------------------------
	float Font::getLengthOfLongestLine( const std::string& text, float fontHeight, bool fixedWidth )
	{
		float maxLineLength = 0.0f;
		float lineLength = 0.0f;
		for( size_t i = 0; i < text.size(); ++i )
		{
			if( text[i] == '<' && i + 1 < text.size() && text[i + 1] == '#' )
			{
				while( ++i < text.size() && text[i] != '>' );
			}
			else if( text[i] != '\n' )
			{
				Glyph glyph = m_glyphs[ text[i] ];
				lineLength += (glyph.A + glyph.B + glyph.C) * fontHeight;
			}
			else
			{
				if( maxLineLength < lineLength )
					maxLineLength = lineLength;
				lineLength = 0.0f;
			}
		}
		if( maxLineLength == 0.0f )
			maxLineLength = lineLength;
		return maxLineLength;
	}
	int Font::getNumOfPrintableCharacters( const std::string& text )
	{
		int numCharacters = 0;
		for( size_t i = 0; i < text.size(); ++i )
		{
			if( text[i] == '<' && i + 1 < text.size() && text[i + 1] == '#' )
			{
				while( ++i < text.size() && text[i] != '>' );
			}
			else if( text[i] != '\n' )
			{
				++numCharacters;
			}
		}
		return numCharacters;
	}
	//------------------------------------------------------------------------------------------------------
	Font* Font::createOrGetFont( const std::string& fontName, const std::string& filePath, const std::string& shaderName, const std::string& xmlFile, const std::string& bitmapFile )
	{
		auto iter = sm_fonts.find( fontName );
		Font* newFont = nullptr;//sm_fonts[ fontName ];
		if( iter == sm_fonts.end() )
		{
			newFont = new Font( fontName, shaderName, filePath + std::string( xmlFile ), filePath + std::string( bitmapFile ) );
			sm_fonts.insert( std::pair< std::string, Font* >( fontName, newFont ) );
		}
		else
			newFont = iter->second;
		return newFont;
	}
	//------------------------------------------------------------------------------------------------------
	void Font::createFont( const std::string& fontName, const std::string& filePath, const std::string& shaderName, const std::string& xmlFile, const std::string& bitmapFile )
	{
		createOrGetFont( fontName, filePath, shaderName, xmlFile, bitmapFile );
	}
	//------------------------------------------------------------------------------------------------------
	Font* Font::getFont( const std::string& fontName )
	{
		Font* font = nullptr;
		if( fontName != "" && sm_fonts.find( fontName ) != sm_fonts.end() )
		{
			font = sm_fonts[ fontName ];
		}
		return font;
	}
	vec2f Font::getRelativePosFromStr( const std::string& posRelStr )
	{
		if( sm_posRel.size() == 0 )
		{
			sm_posRel[ "CENTER" ] = CENTER;
			sm_posRel[ "TOPLEFT" ] = TOPLEFT;
			sm_posRel[ "BOTLEFT" ] = BOTLEFT;
			sm_posRel[ "TOPRIGHT" ] = TOPRIGHT;
			sm_posRel[ "BOTRIGHT" ] = BOTRIGHT;
			sm_posRel[ "BOTCENTER" ] = BOTCENTER;
		}
		auto iter = sm_posRel.find( posRelStr );
		if( iter != sm_posRel.end() )
			return iter->second;
		else
			return TOPLEFT;
	}
	//------------------------------------------------------------------------------------------------------
	// Protected static function
	//------------------------------------------------------------------------------------------------------
	void Font::cleanupFonts()
	{
		MAP_CLEANUP( sm_fonts );
	}
	//////////////////////////////////////////////////////////
	// Protected member functions
	/////////////////////////////////////////////////////////
	Font::Glyph Font::loadGlyph( XMLParser& parser, XMLNode* node )
	{
		unsigned int ucsIndex = 0;
		unsigned int sheet = 0;
		
		ucsIndex = (unsigned int)parser.getXMLAttributeAsInt( node, "ucsIndex", 0 );
		sheet = (unsigned int)parser.getXMLAttributeAsInt( node, "sheet", 0 );
				
		vec2f texCoordMin = parser.getXMLAttributeAsVec2( node, "texCoordMins", vec2f( -1.0f ) );
		vec2f texCoordMax = parser.getXMLAttributeAsVec2( node, "texCoordMaxs", vec2f( -1.0f ) );

		assertion( texCoordMin.x != -1.0f, "Min texture coordinate invalid" );
		assertion( texCoordMax.x != 1.0f, "Max texture coordinate invalid" );
				
		float A = parser.getXMLAttributeAsFloat( node, "ttfA", -1000.0f );
		float B = parser.getXMLAttributeAsFloat( node, "ttfB", -1000.0f);
		float C = parser.getXMLAttributeAsFloat( node, "ttfC", -1000.0f );
		assertion( A != -1000.0f, "Error loading A value for font: %s", m_fontName.c_str() );
		assertion( B != -1000.0f, "Error loading B value for font: %s", m_fontName.c_str() );
		assertion( C != -1000.0f, "Error loading C value for font: %s", m_fontName.c_str() );

		return Glyph( ( unsigned char )ucsIndex, ( unsigned char )sheet, 
			texCoordMin, texCoordMax, A, B, C );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Font::loadFont( const char* xmlFile )
	{
		XMLParser parser( xmlFile );
		XMLDocument& doc = parser.getDocument();		
		
		if( !doc.Error() )
		{		
			XMLNode* root = doc.FirstChildElement( "FontDefinition" );
			XMLNode* fontInfo = root->FirstChildElement( "FontInfo" );
			m_fontName = parser.getXMLAttributeAsString( fontInfo, "name", "" );
			m_fontLocale = parser.getXMLAttributeAsString( fontInfo, "local", "en" );
			m_pixelHeight = parser.getXMLAttributeAsInt( fontInfo, "cellPixelHeight", 1 );


			XMLNode* element; 
			for( element = root->FirstChildElement( "Glyph" ); element != 0; element = element->NextSiblingElement( "Glyph" ) )
			{
				Glyph g = loadGlyph( parser, element );
				m_glyphs.insert( std::pair< unsigned char, Glyph >( g.ucsIndex, g ) );
			}
		}
		else
		{
			MonkyException::simpleErrorMessageBox( "Failed to load font", "File could not be opened" );
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Verts are built relative to top left corner
	Mesh* Font::generateVertices( const std::string& text, vec3f pos, float fontHeight, 
		vec3f rasterDir, vec3f upDir, const Color4f& color, const vec2f& posRel, bool fixedWidth )
	{
		std::vector< Mesh::Vertex > vertices( getNumOfPrintableCharacters( text ) * 6 );
		std::vector< unsigned int > indices( vertices.size() );
		Color4f currentColor = color;

		rasterDir.normalize();
		upDir.normalize();
		vec3f rasterPos = rasterDir * ( getLengthOfLongestLine( text, fontHeight ) * .5f ) * posRel.x + upDir * ( (float)fontHeight * .5f ) * posRel.y;
		vec3f startOfLineRasterPos = rasterPos;
		vec3f height = upDir * ( -(float)fontHeight ) * (float)MathFuncs<float>::getSign( posRel.y );
		unsigned int index = 0;
		for( size_t i = 0; i < text.size(); ++i )
		{
			if( text[i] == '\n' )
			{
				startOfLineRasterPos += height;
				rasterPos = startOfLineRasterPos;
			}
			else if( text[i] == '<' )
			{
				if( i + 1 < text.size() && text[i + 1] == '#' )
				{
					++i;
					std::string color;
					while( i < text.size() && text[i] != '>' )
						color += text[i++];
					currentColor = parseStringToColor4f( color, color::WHITE );
				}
			}
			else
			{

				Glyph glyph = m_glyphs[text[i]];
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
				vertices[ index ] = ( Mesh::Vertex(	rasterPos.x, rasterPos.y, rasterPos.z,
														0.0f, 0.0f, 0.0f,
														currentColor.r, currentColor.g, currentColor.b, currentColor.a, 
														texCoordMin.x, texCoordMin.y ) );
				rasterPos += rasterDir*B;												  
				vertices[ index + 1 ] = ( Mesh::Vertex(	rasterPos.x + height.x ,height.y + rasterPos.y, rasterPos.z + height.z, 
														0.0f, 0.0f, 0.0f,
														currentColor.r, currentColor.g, currentColor.b, currentColor.a, 
														texCoordMax.x, texCoordMax.y ) );
				rasterPos -= rasterDir*B;												  
				vertices[ index + 2 ] = ( Mesh::Vertex(	rasterPos.x + height.x, height.y + rasterPos.y, rasterPos.z + height.z,
														0.0f, 0.0f, 0.0f,
														currentColor.r, currentColor.g, currentColor.b, currentColor.a,
														texCoordMin.x, texCoordMax.y ) );
				rasterPos += rasterDir*B;												  
				vertices[ index + 3 ] = ( Mesh::Vertex(	rasterPos.x + height.x, height.y + rasterPos.y, rasterPos.z + height.z, 
														0.0f, 0.0f, 0.0f,
														currentColor.r, currentColor.g, currentColor.b, currentColor.a, 
														texCoordMax.x, texCoordMax.y ) );
				rasterPos -= rasterDir*B;												  
				vertices[ index + 4 ] = ( Mesh::Vertex(	rasterPos.x, rasterPos.y, rasterPos.z, 
														0.0f, 0.0f, 0.0f,
														currentColor.r, currentColor.g, currentColor.b, currentColor.a, 
														texCoordMin.x, texCoordMin.y ) );
				rasterPos += rasterDir*B;												  
				vertices[ index + 5 ] = ( Mesh::Vertex(	rasterPos.x, rasterPos.y, rasterPos.z, 
														0.0f, 0.0f, 0.0f,
														currentColor.r, currentColor.g, currentColor.b, currentColor.a, 
														texCoordMax.x, texCoordMin.y ) );
				rasterPos += rasterDir*( C + fixedOffset );

				indices[ index ] = ( index++ );
				indices[ index ] = ( index++ );
				indices[ index ] = ( index++ );
				indices[ index ] = ( index++ );
				indices[ index ] = ( index++ );
				indices[ index ] = ( index++ );
			}
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