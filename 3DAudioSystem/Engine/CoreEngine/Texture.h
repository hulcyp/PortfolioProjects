#pragma once

/*************************
* author: Preston Hulcy
* last updated: 9-21-12
*************************/

#include "StdLibraryIncludes.h"
#include "CommonTypeDefines.h"

namespace Monky
{
	class Renderer;

	class Texture
	{
	public:

		friend Renderer;

		typedef int Size;					// Used for the width and height
		typedef unsigned char Color;		// RGBA color
		typedef unsigned int ID;			// Texture id

		//
		// Member Functions ///////////////////////////////////////////////
		//
		Texture();

		explicit Texture( const std::string& texturePath );
		explicit Texture( ID textureID );

		~Texture();

		bool IsLoaded() const;

		std::string GetFilePath() const;
			
		Size GetWidth() const;
		Size GetHeight() const;
		ID GetTextureID() const;

		Color* GetTexels() const { return m_texels; }
		
		void SetTextureID( const ID &texID )
		{
			m_textureID = texID;
		}		
		
		bool HasAlpha() const;		
		void LoadFromMemory( Size width, Size height, Color* pRGBATexels, int channels );	
		void LoadFromFile( const std::string& texturePath );
		void LoadSkyboxFromFile ( const std::string& texturePath );
		void BindTexture( int slotNum );
		
		//
		// Static Member Functions ////////////////////////////////////////////
		//
		
		static void ApplyTexture( Texture* pTexture );
		
		static Texture* CreateOrGetTexture( const std::string& textureName, const std::string& path );
		static Texture* CreateOrGetTexture( const std::string& textureName, ID textureID );
		static Texture* GetTexture( const std::string& textureName );

		static std::vector< Texture* > CreateOrGetSkybox( std::string texturePaths[6] );
		
		static void DestroyTexture( Texture*& pTexture );
		

	private:		
		static void CleanupTextures();
		void LoadToOpenGL();
		void LoadSkyboxToOpenGL();
		
	private:

		// Do not implement! Prevents copying.
		Texture( const Texture& );
		void operator=( const Texture& );

		bool IsPowerOfTwo(Size num) const;

		static std::map< std::string, Texture* > sm_texturePool;
		Size m_height;
		Size m_width;
		int m_channels;
		std::string m_filePath; 
		Color* m_texels;
		ID m_textureID;

	};


}
