#include "stdafx.h"
#include "SceneData.h"
#include <decomp.h>
#include <string>
#include <cstdarg>

#define P3D_DEBUG

#ifdef P3D_DEBUG
	#define Log( fmt, ... )	\
		fprintf( m_logfh, fmt, __VA_ARGS__ );
#else
	#define Log( fmt, ... )
#endif

SceneData::SceneData( const wchar_t* fileName )
	:	m_numAttribs( 6 )
	,	m_version( 3 )
	,	FRAMES_PER_SECOND( 30 )
{
	int fileNameLen = 0;

#ifdef P3D_DEBUG
	_wfopen_s( &m_logfh, (std::wstring( L"B:\\" ) + extractFileName( fileName, fileNameLen ) + L"log.txt" ).c_str(), L"w" );
#endif

	m_layout = new int[ m_numAttribs ];
	m_layout[0] = FLOAT3;
	m_layout[1] = FLOAT3;
	m_layout[2] = FLOAT4;
	m_layout[3] = FLOAT2;
	m_layout[4] = FLOAT3;
	m_layout[5] = FLOAT3;

}

void SceneData::addMaterial( IGameMaterial* material )
{
	m_materials.push_back( material );
}

void SceneData::exportData( FILE* fh )
{

	fwrite( &m_version, sizeof( int ), 1, fh );
	Log( "Outputing version number: %d", m_version );
	exportMaterialData( fh );
	exportMeshData( fh );
	exportCameraData( fh );
	
#ifdef P3D_DEBUG
	fclose( m_logfh );
#endif
}

void SceneData::log( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	vfprintf( m_logfh, fmt, args );
	va_end( args );
}

void SceneData::flush()
{
	fflush( m_logfh );
}

void SceneData::addMesh( Mesh* mesh )
{
	m_meshes.push_back( mesh );
}

void SceneData::addCamera( Camera* camera )
{
	m_cameras.push_back( camera );
}

void SceneData::exportMaterialData( FILE* fh )
{
	int numMats = (int)m_materials.size();
	//
	fwrite( &numMats, sizeof( int ), 1, fh );
	//
	for( int c = 0; c < numMats; ++c )
	{
		IGameMaterial* mat = m_materials[c];
		const wchar_t* matName = mat->GetMaterialName();
		int matNameLen = (int)wcslen( matName ) + 1;
		
		char* matNameChar = new char[matNameLen];
		wcstombs( matNameChar, matName, (size_t)matNameLen );

		//
		fwrite( &matNameLen, sizeof( int ), 1, fh );
		fwrite( matName, sizeof( wchar_t ), matNameLen, fh );
		//

		exportTextureMaps( fh, mat );
	}
}


void SceneData::exportMeshData( FILE* fh )
{
	int numMeshes = (int)m_meshes.size();
	//
	fwrite( &FRAMES_PER_SECOND, sizeof( int ), 1, fh );
	fwrite( &numMeshes, sizeof( int ), 1, fh );
	//
	for( int i = 0; i < numMeshes; ++i )
	{
		switch( m_meshes[i]->meshType )
		{
		case Mesh::STATIC:
			exportStaticMeshData( reinterpret_cast< StaticMesh* >( m_meshes[i] ), fh );
			break;
		case Mesh::SKELETAL:
			exportSkelMeshData( reinterpret_cast< SkelMesh* >( m_meshes[i] ), fh );
			break;
		}		
	}
}

void SceneData::exportStaticMeshData( StaticMesh* mesh, FILE* fh )
{
	int numTriBatches = (int)mesh->triBatches.size();
				
	Log( "Parent ID: %d\n", mesh->parentID );
	fwrite( &mesh->id, sizeof( int ), 1, fh );
	fwrite( &mesh->parentID, sizeof( int ), 1, fh );
	fwrite( &mesh->meshType, sizeof( int ), 1, fh );
	
	fwrite( &mesh->numAttribs, sizeof( int ), 1, fh );
	fwrite( mesh->layout, sizeof( int ), m_numAttribs, fh );
	fwrite( &numTriBatches, sizeof( int ), 1, fh );
	//
	for( int j = 0; j < numTriBatches; ++j )
	{
		int matID = -1;
		int numIndices = (int)mesh->triBatches[j]->indices.size();
		int numVertices = (int)mesh->triBatches[j]->vertices.size();
			
		matID = getMaterialIndex( mesh->triBatches[j]->mat );
		fwrite( &matID, sizeof( int ), 1, fh );
		fwrite( &numIndices, sizeof( int ), 1, fh );
		fwrite( &numVertices, sizeof( int ), 1, fh );
		fwrite( mesh->triBatches[j]->indices.data(), sizeof( int ), numIndices, fh );
		fwrite( mesh->triBatches[j]->vertices.data(), sizeof( Vertex ), numVertices, fh );			
	}
		
	int numFrames = (int)mesh->localTM.size();
	fwrite( &numFrames, sizeof( int ), 1, fh );
	for( int f = 0; f < mesh->localTM.size(); ++f )
	{		
		AffineParts parts;
		decomp_affine( mesh->localTM[f], &parts );
		fwrite( &parts.t, sizeof( Point3 ), 1, fh );
		fwrite( &parts.q, sizeof( Quat ), 1, fh );
		fwrite( &parts.k, sizeof( Point3 ), 1, fh );
	}
}

void SceneData::exportSkelMeshData( SkelMesh* mesh, FILE* fh )
{
	int numTriBatches = (int)mesh->triBatches.size();
				
	Log( "Parent ID: %d\n", mesh->parentID );
	fwrite( &mesh->id, sizeof( int ), 1, fh );
	fwrite( &mesh->parentID, sizeof( int ), 1, fh );
	fwrite( &mesh->meshType, sizeof( int ), 1, fh );
	
	fwrite( &mesh->numAttribs, sizeof( int ), 1, fh );
	fwrite( mesh->layout, sizeof( int ), mesh->numAttribs, fh );
	fwrite( &numTriBatches, sizeof( int ), 1, fh );
	Log( "Num of Tri batches: %d", numTriBatches );
	//
	for( int j = 0; j < numTriBatches; ++j )
	{
		int matID = -1;
		int numIndices = (int)mesh->triBatches[j]->indices.size();
		int numVertices = (int)mesh->triBatches[j]->vertices.size();
		matID = getMaterialIndex( mesh->triBatches[j]->mat );

		Log( "Material ID: %d\n", matID );
		Log( "Num indices: %d\n", numIndices );
		Log( "Num vertices: %d\n", numVertices );
			
		fwrite( &matID, sizeof( int ), 1, fh );
		fwrite( &numIndices, sizeof( int ), 1, fh );
		fwrite( &numVertices, sizeof( int ), 1, fh );
		fwrite( mesh->triBatches[j]->indices.data(), sizeof( int ), numIndices, fh );
		fwrite( mesh->triBatches[j]->vertices.data(), sizeof( SkelVertex ), numVertices, fh );			
	}
		
	int numFrames = (int)mesh->localTM.size();
	fwrite( &numFrames, sizeof( int ), 1, fh );
	for( int f = 0; f < mesh->localTM.size(); ++f )
	{		
		AffineParts parts;
		decomp_affine( mesh->localTM[f], &parts );
		fwrite( &parts.t, sizeof( Point3 ), 1, fh );
		fwrite( &parts.q, sizeof( Quat ), 1, fh );
		fwrite( &parts.k, sizeof( Point3 ), 1, fh );
	}

	int numBones = (int)mesh->bones.size();
	int numBoneFrames = (int)mesh->bones[0]->worldTMFrames.size();
	
	fwrite( &numBones, sizeof( int ), 1, fh );
	fwrite( &numBoneFrames, sizeof( int ), 1, fh );

	for( int b = 0; b < numBones; ++b )
	{
		for( int bf = 0; bf < numBoneFrames; ++bf )
		{
			//fwrite( &mesh->bones[b]->worldTMFrames[bf], sizeof( Matrix3 ), 1, fh );
			AffineParts parts;
			decomp_affine( mesh->bones[b]->worldTMFrames[bf], &parts );
			fwrite( &parts.t, sizeof( Point3 ), 1, fh );
			fwrite( &parts.q, sizeof( Quat ), 1, fh );
			fwrite( &parts.k, sizeof( Point3 ), 1, fh );
		}
	}
}

void SceneData::exportCameraData( FILE* fh )
{
	int numCameras = (int)m_cameras.size();

	fwrite( &numCameras, sizeof( int ), 1, fh );

	for( int i = 0; i < numCameras; ++i )
	{
		Camera* camera = m_cameras[i];

		fwrite( &camera->id, sizeof( int ), 1, fh );
		fwrite( &camera->parentID, sizeof( int ), 1, fh );
		int numFrames = (int)camera->localTM.size();
		fwrite( &numFrames, sizeof( int ), 1, fh );

		for( int f = 0; f < numFrames; ++f )
		{
			AffineParts parts;
			decomp_affine( camera->localTM[f], &parts );
			fwrite( &parts.t, sizeof( Point3 ), 1, fh );
			fwrite( &parts.q, sizeof( Quat ), 1, fh );
			fwrite( &parts.k, sizeof( Point3 ), 1, fh );
			//fwrite( &camera->localTM[f], sizesof( Matrix3 ), 1, fh );
		}
	}
}


void SceneData::exportTextureMaps( FILE* fh, IGameMaterial* material )
{
	std::vector< IGameTextureMap* > texMaps;
	int numTexMaps = material->GetNumberOfTextureMaps();
	for( int i = 0; i < numTexMaps; ++i )
	{
		IGameTextureMap* texture = material->GetIGameTextureMap( i );
		if( texture == NULL || texture->GetBitmapFileName() == NULL )
			continue;

		int length = (int)wcslen( texture->GetBitmapFileName() );
		if( length == 0 )
			continue;

		switch( texture->GetStdMapSlot() )
		{
		case ID_AM:
		case ID_DI:
		case ID_SP:
		case ID_SI:
		case ID_BU:
			texMaps.push_back( texture );
			break;
		}
	}

	numTexMaps = (int)texMaps.size();
	//
	fwrite( &numTexMaps, sizeof( int ), 1, fh );	
	//

	for( int i = 0; i < numTexMaps; ++i )
	{
		IGameTextureMap* tex = texMaps[i];
		int slotNum = tex->GetStdMapSlot();
			
		int fileNameLen = 0;
		const wchar_t* fileName = extractFileName( tex->GetBitmapFileName(), fileNameLen );
		//
		fwrite( &slotNum, sizeof( int ), 1, fh );
		fwrite( &fileNameLen, sizeof( int ), 1, fh );
		fwrite( fileName, sizeof( wchar_t ), fileNameLen, fh );
		//
	}
}

const wchar_t* SceneData::extractFileName( const wchar_t* filePath, int& fileNameLen )
{
	if( filePath == nullptr )
		return L'';

	const wchar_t* lastPtr = wcsrchr( filePath, '\\' );
	
	if( lastPtr == nullptr )
		lastPtr = filePath;

	fileNameLen = (int)wcslen( lastPtr );
	if( fileNameLen == 0 )
		throw std::exception( "Invalid file name length" );
	return lastPtr + 1;
}

int SceneData::getMaterialIndex( IGameMaterial* material )
{
	if( material != nullptr )
	{
		for( int i = 0; i < m_materials.size(); ++i )
		{
			if( material->GetMaterialName() == m_materials[i]->GetMaterialName() )
				return i;
		}
	}
	return -1;
}

SceneData::Vertex::Vertex( Point3 p, Point3 n, Point3 c, float a, Point2 tex, Point3 tan, Point3 bitan )
{
	pos[0] = p.x;
	pos[1] = p.y;
	pos[2] = p.z;
	normal[0] = n.x;
	normal[1] = n.y;
	normal[2] = n.z;
	color[0] = c.x;
	color[1] = c.y;
	color[2] = c.z;
	color[3] = a;
	uv[0] = tex.x;
	uv[1] = 1.0f - tex.y;
	tangent[0] = tan.x;
	tangent[1] = tan.y;
	tangent[2] = tan.z;
	bitangent[0] = bitan.x;
	bitangent[1] = bitan.y;
	bitangent[2] = bitan.z;
}

bool SceneData::Vertex::operator<( const Vertex& other ) const
{
	return memcmp( this, &other, sizeof( Vertex ) ) < 0;
}
bool SceneData::Vertex::operator>( const Vertex& other ) const
{
	return memcmp( this, &other, sizeof( Vertex ) ) > 0;
}
bool SceneData::Vertex::operator==( const Vertex& other ) const
{
	return memcmp( this, &other, sizeof( Vertex ) ) == 0;
}


SceneData::SkelVertex::SkelVertex( Point3 p, Point3 n, Point3 c, float a, Point2 tex, Point3 tan, Point3 bitan )
{
	pos[0] = p.x;
	pos[1] = p.y;
	pos[2] = p.z;
	normal[0] = n.x;
	normal[1] = n.y;
	normal[2] = n.z;
	color[0] = c.x;
	color[1] = c.y;
	color[2] = c.z;
	color[3] = a;
	uv[0] = tex.x;
	uv[1] = 1.0f - tex.y;
	tangent[0] = tan.x;
	tangent[1] = tan.y;
	tangent[2] = tan.z;
	bitangent[0] = bitan.x;
	bitangent[1] = bitan.y;
	bitangent[2] = bitan.z;

	boneIndices[0] = -1.0f;
	boneIndices[1] = -1.0f;
	boneIndices[2] = -1.0f;
	boneIndices[3] = -1.0f;
	weights[0] = 0.0f;
	weights[1] = 0.0f;
	weights[2] = 0.0f;

}

bool SceneData::SkelVertex::operator<( const SkelVertex& other ) const
{
	return memcmp( this, &other, sizeof( SkelVertex ) ) < 0;
}
bool SceneData::SkelVertex::operator>( const SkelVertex& other ) const
{
	return memcmp( this, &other, sizeof( SkelVertex ) ) > 0;
}
bool SceneData::SkelVertex::operator==( const SkelVertex& other ) const
{
	return memcmp( this, &other, sizeof( SkelVertex ) ) == 0;
}


bool SceneData::Bone::operator<( const Bone& other ) const
{
	return memcmp( this, &other, sizeof( Bone ) ) < 0;
}
bool SceneData::Bone::operator>( const Bone& other ) const
{
	return memcmp( this, &other, sizeof( Bone ) ) > 0;
}
bool SceneData::Bone::operator==( const Bone& other ) const
{
	return memcmp( this, &other, sizeof( Bone ) ) == 0;
}

