
/*************************
* author: Preston Hulcy
* last updated: 9-21-12
*************************/
#ifdef _WIN32
#include <glew.h>
#endif
#include "Texture.h"
#include <Gl/GL.h>
#include "MonkyException.h"
#include "CommonEngineIncludes.h"
#include "FileManager.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"


#define REQUIRES assert
#define PROMISES assert

namespace Monky
{

	std::map< std::string, Texture* > Texture::sm_texturePool;


	Texture::Texture() : m_width(0), m_height(0), m_filePath(""), m_texels(nullptr), m_channels(0)
	{}

	Texture::Texture( const std::string& texturePath ) : m_width(0), m_height(0), m_texels(nullptr), m_channels(0)
	{
		REQUIRES( !texturePath.empty() );
	
		m_filePath = texturePath;
		LoadFromFile( m_filePath );
	}

	Texture::Texture( ID textureID )
		:	m_textureID( textureID )
	{}

	Texture::~Texture()
	{
		if( m_filePath != "" )
		{
			stbi_image_free( m_texels );
		}
		else
		{		
			SAFE_ARRAY_DELETE( m_texels );
		}

		if( m_textureID != 0 )
		{
			glDeleteTextures( 1, &m_textureID );
		}		
	}

	bool Texture::IsLoaded() const
	{
		return !( m_texels == nullptr );
	}


	std::string Texture::GetFilePath() const
	{
		return m_filePath;
	}

	Texture::Size Texture::GetWidth() const
	{
		REQUIRES( IsLoaded() );
		return m_width;
	}

	Texture::Size Texture::GetHeight() const
	{
		REQUIRES( IsLoaded() );
		return m_height;
	}

	Texture::ID Texture::GetTextureID() const
	{
		return m_textureID;
	}

	bool Texture::HasAlpha() const
	{
		REQUIRES( IsLoaded() );
	
		return ( m_channels == 4 || m_channels == 2);

	}
	

	void Texture::LoadFromMemory( Size width, Size height, Color* pRGBATexels, int channels )
	{
		REQUIRES( !IsLoaded() );
		REQUIRES( width > 0 );
		REQUIRES( height > 0 );
		REQUIRES( pRGBATexels );

		m_width = width;
		m_height = height;
		m_channels = channels;
		m_texels = new Color[m_width*m_height*m_channels];

		memcpy( m_texels, pRGBATexels, sizeof(Color)*(m_height*m_width*m_channels) );

		LoadToOpenGL();

		PROMISES( IsLoaded() );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Texture::LoadFromFile( const std::string& texturePath )
	{
		REQUIRES( !IsLoaded() );
		REQUIRES( !texturePath.empty() );
	
		m_filePath = texturePath;
		int bufferSize;
		const unsigned char* textureBuffer = reinterpret_cast< const unsigned char* >( getFileManager().readDataFromFile( texturePath, bufferSize ) );

		if( textureBuffer != nullptr )
			m_texels = stbi_load_from_memory( textureBuffer, bufferSize, &m_width, &m_height, &m_channels, 0 );

		SAFE_DELETE( textureBuffer );
		LoadToOpenGL();

		PROMISES( IsLoaded() );
		PROMISES( GetFilePath() == texturePath );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Texture::LoadSkyboxFromFile ( const std::string& texturePath )
	{
		REQUIRES( !IsLoaded() );
		REQUIRES( !texturePath.empty() );
	
		m_filePath = texturePath;
		m_texels = stbi_load( m_filePath.c_str(), &m_width, &m_height, &m_channels, 0 );
	
		
		LoadSkyboxToOpenGL();

		PROMISES( IsLoaded() );
		PROMISES( GetFilePath() == texturePath );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Texture::IsPowerOfTwo(Texture::Size num) const
	{
		return ( num != 0 && (( num & ( num - 1)) == 0));
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Texture::LoadToOpenGL()
	{
		REQUIRES( IsLoaded() );

		GLuint idTexture = 0;
				
		glGenTextures( 1, &idTexture );
		assert( idTexture );
		
		glBindTexture( GL_TEXTURE_2D, idTexture );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		if( m_channels == 4 )
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texels );
		}
		else
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_texels );
		}				

		m_textureID = idTexture;

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Texture::LoadSkyboxToOpenGL()
	{
		REQUIRES( IsLoaded() );

		GLuint idTexture = 0;
				
		glGenTextures( 1, &idTexture );
		assert( idTexture );
		
		glBindTexture( GL_TEXTURE_2D, idTexture );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		if( m_channels == 4 )
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texels );
		}
		else
		{
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_texels );
		}				

		m_textureID = idTexture;

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Texture::BindTexture( int slotNum )
	{
		glActiveTexture( slotNum );
		glBindTexture( GL_TEXTURE_2D, m_textureID );
		if( HasAlpha() )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		glEnable( GL_TEXTURE_2D );
	}


	//
	// Static Member Functions ////////////////////////////////////////////
	//

	void Texture::ApplyTexture( Texture* pTexture )
	{
		REQUIRES( pTexture == 0 || pTexture->IsLoaded() );

		glEnable( GL_TEXTURE_2D );
		glDisable( GL_BLEND );
		GLuint id = pTexture->GetTextureID();

		glBindTexture( GL_TEXTURE_2D, id );

		if( pTexture->HasAlpha() )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}	

	}
	
	Texture* Texture::CreateOrGetTexture( const std::string& textureName, const std::string& texturePath )
	{
		REQUIRES( !texturePath.empty() );
		REQUIRES( strlen( texturePath.c_str() ) > 0 );
	
		auto iter = sm_texturePool.find( textureName );
		Texture* texture = nullptr;//sm_texturePool[ textureName ];

		if( iter == sm_texturePool.end() )
		{
			texture = new Texture( texturePath );
			sm_texturePool.insert( std::pair< std::string, Texture* >( textureName, texture ) );
		}
		else
			texture = iter->second;

		PROMISES( texture );
		PROMISES( texture->IsLoaded() );
		PROMISES( texture->GetFilePath() == texturePath );	
		return texture;
	}

	Texture* Texture::CreateOrGetTexture( const std::string& textureName, ID textureID )
	{
		
		auto iter = sm_texturePool.find( textureName );
		Texture* texture = nullptr;//sm_texturePool[ textureName ];
		//for( auto iter = sm_texturePool.begin(); iter != sm_texturePool.end(); ++iter )
		//{
		//	if( iter->second->m_textureID == textureID )
		//	{
		//		texture = iter->second;
		//		break;
		//	}
		//}
		if( iter == sm_texturePool.end() )
		{
			texture = new Texture( textureID );
			sm_texturePool.insert( std::pair< std::string, Texture* >( textureName, texture ) );
		}
		else
			texture = iter->second;
		return texture;
	}

	Texture* Texture::GetTexture( const std::string& textureName )
	{
		auto iter = sm_texturePool.find( textureName );
		if( iter != sm_texturePool.end() )
			return iter->second;
		else
			return nullptr;
	}

	std::vector< Texture* > Texture::CreateOrGetSkybox( std::string texturePaths[6] )
	{
		std::vector< Texture* > skybox(6);

		for( int i = 0; i < 6; ++i )
		{
			Texture*& result = sm_texturePool[texturePaths[i]];

			if( !result )
			{
				result = new Texture();
				result->LoadSkyboxFromFile( texturePaths[i] );
			}

			skybox[i] = result;
		}

		return skybox;
	}


	void Texture::DestroyTexture( Texture*& pTexture )
	{
		REQUIRES( pTexture );
	
		sm_texturePool.erase( pTexture->GetFilePath() );
		pTexture = nullptr;
	
		PROMISES( pTexture == nullptr );
	}

	void Texture::CleanupTextures()
	{
		MAP_CLEANUP( sm_texturePool );
	}
}
