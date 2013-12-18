#include "P3DImporter.h"
#include "CommonEngineIncludes.h"
#include "Actor.h"
#include "Mesh.h"
#include "GLBuffer.h"
#include "VtxBufferAccessor.h"
#include "BufferLayout.h"
#include "Renderer.h"
#include "Material.h"
#include "Scene.h"
#include "Camera.h"
#include "P3DCamera.h"
#include "SkeletalMesh.h"

namespace pdh
{
	P3DImporter::P3DImporter( const std::string& file )
		:	m_version( 0 )
	{
		std::vector< std::string > tokens;
		stringTokenizer( file, tokens, "." );
		assertion( tokens[ tokens.size() - 1 ] == "p3d", "Invalid file format.\n%s", file.c_str() );
		
		m_fh = fopen( file.c_str(), "rb" );
		assertion( m_fh != nullptr, "File could not be opened.\n%s", file.c_str() );

		fread( &m_version, sizeof( int ), 1, m_fh );

		std::vector< std::string > pathTokens;
		stringTokenizer( file, pathTokens, "\\" );
		std::string fileName = pathTokens[ pathTokens.size() - 1 ];
		std::vector< std::string > fileNameAndPath;
		stringTokenizer( fileName, fileNameAndPath );
		m_fileName = fileNameAndPath[ fileNameAndPath.size() - 1 ];		
	}

	void P3DImporter::importData( Scene* scene )
	{
		importMaterials();
		importMeshes( scene );
		importCameras( scene );
	}

	void P3DImporter::importMaterials()
	{
		int numMats = 0;
		fread( &numMats, sizeof( int ), 1, m_fh );
		
		for( int i = 0; i < numMats; ++i )
		{
			int matStrLen = 0;
			fread( &matStrLen, sizeof( int ), 1, m_fh );
			
			wchar_t* materialName = new wchar_t[matStrLen];
			fread( materialName, sizeof( wchar_t ), matStrLen, m_fh );
			
			char* materialNameChar = new char[ matStrLen ];
			
			int length = wcstombs( materialNameChar, materialName, matStrLen );
			if( length == matStrLen )
				materialNameChar[ matStrLen - 1 ] = '\0';
			
						
			// create material
			std::string materialNameStr = materialNameChar + m_fileName;
			Material* material = Renderer::createMaterial( materialNameStr, "DiffuseShader" );
			material->addUniform( "uUseNormalMap", 0 );
			material->addUniform( "uUseEmissiveMap", 0 );
			m_materials.push_back( materialNameStr );

			int numTexs = 0;
			fread( &numTexs, sizeof( int ), 1, m_fh );

			for( int t = 0; t < numTexs; ++t )
			{
				int texType = -1;
				int texFileLen = 0;
				wchar_t* texFile;
				char* texFileChar;

				fread( &texType, sizeof( int ), 1, m_fh );
				fread( &texFileLen, sizeof( int ), 1, m_fh );
				texFile = new wchar_t[ texFileLen ];
				fread( texFile, sizeof( wchar_t ), texFileLen, m_fh );

				texFileChar = new char[ texFileLen ];
				int length = wcstombs( texFileChar, texFile, texFileLen );
				
				//assertion( length != texFileLen && length, "Error parsering texture file: %s", texFileChar );
				if( length > 0 )
				{
					std::string newFileName = changeFileExt( std::string( texFileChar ), ".png" );
					addTextureToMaterial( material, texType, newFileName );
				}
			}
		}
	}

	void P3DImporter::importMeshes( Scene* scene )
	{
		int numMeshes = 0;
		fread( &m_framesPerSecond, sizeof( int ), 1, m_fh );
		fread( &numMeshes, sizeof( int ), 1, m_fh );

		for( int i = 0; i < numMeshes; ++i )
		{
			int numAttribs = 0;
			int id = -1;
			int parentID = -1;
			int meshType = -1;

			fread( &id, sizeof( int ), 1, m_fh );
			fread( &parentID, sizeof( int ), 1, m_fh );
			fread( &meshType, sizeof( int ), 1, m_fh );
			fread( &numAttribs, sizeof( int ), 1, m_fh );
			BufferLayout* vtxLayout = new BufferLayout();
			BufferLayout* idxLayout = new BufferLayout( 1, BufferLayout::INT );
			
			for( int a = 0; a < numAttribs; ++a )
			{
				int type = 0;
				fread( &type, sizeof( int ), 1, m_fh );
				vtxLayout->pushAttribute( (BufferLayout::DataType )type );
			}

			
			int numTriBatches = 0;
			fread( &numTriBatches, sizeof( int ), 1, m_fh );
			

			Mesh* mesh = nullptr;				

			for( int t = 0; t < numTriBatches; ++t )
			{					
				int matID = -1;
				int numIndices = 0;
				int numVertices = 0;
					
				fread( &matID, sizeof( int ), 1, m_fh );
				fread( &numIndices, sizeof( int ), 1, m_fh );
				fread( &numVertices, sizeof( int ), 1, m_fh );

				int vtxBufferSize = vtxLayout->stride() * numVertices;
				int idxBufferSize = idxLayout->stride() * numIndices;
				char* vertices = new char[ vtxBufferSize ];
				char* indices = new char[ idxBufferSize ];

				fread( indices, sizeof( char ), idxBufferSize, m_fh );
				fread( vertices, sizeof( char ), vtxBufferSize, m_fh );

				GLBuffer* vtxBuffer = new GLBuffer( vertices, numVertices, vtxLayout, GL_ARRAY_BUFFER, GL_TRIANGLES, GL_STATIC_DRAW );
				GLBuffer* idxBuffer = new GLBuffer( indices, numIndices, idxLayout, GL_ELEMENT_ARRAY_BUFFER, GL_TRIANGLES, GL_STATIC_DRAW );

				
				std::string matName;
				if( matID == -1 )
					if( meshType == 1 )
					{
						matName = "SkinningMaterial";
						//Renderer::getMaterial( matName )->changeShaderProgram( "SkinningShader" );
					}
					else
						matName = "DiffuseMaterial";
				else
				{
					matName = m_materials[matID];
					if( meshType == 1 )
					{
						Renderer::getMaterial( matName )->changeShaderProgram( "SkinningShader" );
					}
				}

				if( mesh == nullptr )
				{
					if( meshType == 1 )
						mesh = new SkeletalMesh( vtxBuffer, idxBuffer, nullptr, matName, true );
					else
						mesh = new Mesh( vtxBuffer, idxBuffer, nullptr, matName, true );
				}
				else
					mesh->pushTriangleBatch( vtxBuffer, idxBuffer, nullptr, matName, true );

				vtxBuffer->sendToOpenGL();
				idxBuffer->sendToOpenGL();
			}
			
			
			// create the actor for the mesh
			Actor* actor = new Actor( m_fileName + toString( i ), mesh );

			int numFrames = 0;
			fread( &numFrames, sizeof( int ), 1, m_fh );
			actor->setFrameCount( numFrames );
			for( int f = 0; f < numFrames; ++f )
			{
				vec3f translation;
				quatf rotation;
				vec3f scale;

				fread( &translation, sizeof( vec3f ), 1, m_fh );
				fread( &rotation, sizeof( quatf ), 1, m_fh );
				fread( &scale, sizeof( vec3f ), 1, m_fh );

				actor->setFrame( Node::Frame( translation, mat3f::createMatrixFromQuaternion( rotation ), scale ), f );
			}
			actor->setFrameRate( 1 / static_cast<float>(m_framesPerSecond) );
			scene->actors.push_back( actor );
			m_parentIDToActorIndex[ id ] = scene->actors.size() - 1;
			
			if( parentID != -1 )
			{
				auto iter = m_parentIDToActorIndex.find( parentID );
				if( iter != m_parentIDToActorIndex.end() )
				{
					scene->actors[ iter->second ]->addChild( actor );
				}				
			}

			// If skeletal mesh
			if( meshType == 1 )
			{
				
				SkeletalMesh* skelMesh = reinterpret_cast< SkeletalMesh* >( mesh );
				skelMesh->setFrameRate( 1 / static_cast<float>(m_framesPerSecond) );
				int numBones = 0;
				int numFrames = 0;
				fread( &numBones, sizeof( int ), 1, m_fh );
				fread( &numFrames, sizeof( int ), 1, m_fh );
				
				std::vector< mat4f > bone;
				for( int b = 0; b < numBones; ++b )
				{
					bone.clear();
					for( int bf = 0; bf < numFrames; ++bf )
					{
						vec3f translation;
						quatf rotation;
						vec3f scale;
						
						fread( &translation, sizeof( vec3f ), 1, m_fh );
						fread( &rotation, sizeof( quatf ), 1, m_fh );
						fread( &scale, sizeof( vec3f ), 1, m_fh );

						mat4f boneFrameTM( mat4f::IDENTITY );
						boneFrameTM.scale( scale );
						boneFrameTM.rotate( rotation );
						boneFrameTM.translate( translation );
						
						bone.push_back( boneFrameTM );
					}
					skelMesh->addBone( bone );
				}
				skelMesh->play();
			}
		}
	}

	void P3DImporter::importCameras( Scene* scene )
	{
		int numCameras = 0;
		fread( &numCameras, sizeof( int ), 1, m_fh );

		for( int i = 0; i < numCameras; ++i )
		{
			int numAttribs = 0;
			int id = -1;
			int parentID = -1;

			fread( &id, sizeof( int ), 1, m_fh );
			fread( &parentID, sizeof( int ), 1, m_fh );
			
			int numFrames = 0;
			fread( &numFrames, sizeof( int ), 1, m_fh );
			Camera* camera = new P3DCamera( m_fileName + "P3DCamera" + toString( i ), vec3f(), mat4f::perspective( 45.0f, 1024.0f/768.0f, 0.1f, 100000.0f ) );
			camera->setFrameRate( 1 / static_cast<float>( m_framesPerSecond ) );
			camera->setFrameCount( numFrames );

			for( int f = 0; f < numFrames; ++f )
			{
				vec3f translation;
				quatf rotation;
				vec3f scale;

				fread( &translation, sizeof( vec3f ), 1, m_fh );
				fread( &rotation, sizeof( quatf ), 1, m_fh );
				fread( &scale, sizeof( vec3f ), 1, m_fh );

				mat3f rot = mat3f::createMatrixFromQuaternion( rotation );
				
				camera->setFrame( Node::Frame( translation, rot, vec3f::ONE ), f );
			}

			if( parentID != -1 )
			{
				auto iter = m_parentIDToActorIndex.find( parentID );
				if( iter != m_parentIDToActorIndex.end() )
				{
					scene->actors[ iter->second ]->addChild( camera );
				}				
			}

			scene->cameras.push_back( camera );
		}

	}

	void P3DImporter::addTextureToMaterial( Material* material, int texType, const std::string& texFile )
	{
		Texture* tex = Renderer::createTexture( texFile, texFile );

		switch( texType )
		{
		case ID_AM:
			break;
		case ID_DI:
			material->addTexture( "uDiffuseMap", tex );
			break;
		case ID_SP:
			//material->addTexture( "uSpecularMap", tex );
			break;
		case ID_SI:
			material->addTexture( "uEmissiveMap", tex );
			material->addUniform( "uUseEmissiveMap", 1 );
			break;
		case ID_BU:
			material->addTexture( "uNormalMap", tex );
			material->addUniform( "uUseNormalMap", 1 );
			break;
		}
	}

	std::string P3DImporter::changeFileExt( const std::string& texFile, const std::string& newExt )
	{
		std::vector< std::string > tokens;
		stringTokenizer( texFile, tokens, "." );
		assertion( tokens.size() == 2, "Invalid texture file name: %s", texFile.c_str() );

		return tokens[0] + newExt;
	}
}