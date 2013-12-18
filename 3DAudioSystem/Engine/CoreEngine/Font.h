#pragma once
#include "CommonEngineIncludes.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Colors.h"
#include "Mesh.h"
#include "XMLParser.h"

namespace Monky
{
	typedef unsigned char ucsType;

	class Renderer;
	class Material;
	class Texture;

	class Font 
	{
	public:
		friend Renderer;

		Font( const std::string& materialName, const std::string& shaderName, const std::string& xmlFile, const std::string& bitmapFile );
		~Font();

		
		void renderText( const std::string& text, const mat4f& transformation, float fontHeight, const Color4f& color = color::WHITE, const vec2f& posRel = BOTLEFT, bool drawWithShadow = true, bool fixedWidth = false );
		void render3DText( const std::string& text, const vec3f& pos, float fontHeight, vec3f rasterDir, vec3f up, const Color4f& color = color::WHITE, const vec2f& posRel = TOPLEFT, bool fixedWidth = false );
		void renderBillboardText( const std::string& text, const vec3f& pos, float fontHeight, vec3f cameraK, const Color4f& color = color::WHITE, const vec2f& posRel = TOPLEFT, bool fixedWidth = false );
		float getLength( const std::string& text, float fontHeight, bool fixedWidth = false );
		float getLengthOfLongestLine( const std::string& text, float fontHeight, bool fixedWidth = false );
		int getNumOfPrintableCharacters( const std::string& text );
		int getFontHeight() { return m_pixelHeight; }
		const std::string& getFontName() const { return m_fontName; }


		void setMaterial( const std::string& material ) { m_material = material; }
		std::string& getMaterial() { return m_material; }

		static Font* createOrGetFont( const std::string& fontName, const std::string& filePath, const std::string& shaderName, const std::string& xmlFile, const std::string& bitmapFile );
		static void createFont( const std::string& fontName, const std::string& filePath, const std::string& shaderName, const std::string& xmlFile, const std::string& bitmapFile );
		static Font* getFont( const std::string& fontName );

		static vec2f getRelativePosFromStr( const std::string& posRelStr );
		

		///////////////////////////////////////////////////////////
		// Used to indicate where the text is drawn relative too
		//////////////////////////////////////////////////////////
		static const vec2f CENTER;
		static const vec2f TOPLEFT;
		static const vec2f BOTLEFT;
		static const vec2f TOPRIGHT;
		static const vec2f BOTRIGHT;
		static const vec2f BOTCENTER;

		//---------------------------------------------------------
		// Used for parsing color changes in strings of text
		//---------------------------------------------------------
		//static const std::string RED;
		//static const std::string BLUE;
		//static const std::string GREEN;

	protected:
		static void cleanupFonts();

		struct Glyph
		{
			Glyph()
			{}
			Glyph( ucsType ucsIndex, unsigned char sheet, const vec2f& tcMin, const vec2f& tcMax, float a, float b, float c )
				:	ucsIndex( ucsIndex )
				,	sheet( sheet )
				,	texCoordMin( tcMin )
				,	texCoordMax( tcMax )
				,	A( a )
				,	B( b )
				,	C( c )
			{}

			ucsType ucsIndex;
			unsigned char sheet;
			vec2f texCoordMin;
			vec2f texCoordMax;
			float A;
			float B;
			float C;
		};

		// Add option to render numberrs with fixed spacing.
		Glyph loadGlyph( XMLParser& parser, XMLNode* node );
		void loadFont( const char* xmlFile );
		Mesh* generateVertices( const std::string& text, vec3f pos, float fontHeight, 
			vec3f rasterDir, vec3f upDir, const Color4f& color, const vec2f& posRel, bool fixedWidth );

	
	private:
		std::map< ucsType, Glyph > m_glyphs;
		std::vector< Texture* > m_textures;
		std::string m_fontName;
		std::string m_fontLocale;
		int m_pixelHeight;
		std::string m_material;

		static std::map< std::string, Font* > sm_fonts;
		static std::map< std::string, vec2f > sm_posRel;
	};
}