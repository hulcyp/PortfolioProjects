#include "stdafx.h"
#include "MaxExporter.h"
#include <sstream>

template< typename T >
std::string toString( T i )
{
	std::stringstream out;
	out << i;
	return out.str();
}

MaxExporter::MaxExporter() :
	m_igame( 0 ),
	m_fh( 0 )
{
}

MaxExporter::~MaxExporter()
{
	deinit();
}

void MaxExporter::deinit()
{
	if ( m_fh != 0 )
	{
		fclose( m_fh );
		m_fh = 0;
	}


	if ( m_igame != 0 )
	{
		m_igame->ReleaseIGame();
		m_igame = 0;
	}
}

int MaxExporter::DoExport( const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressprompts, DWORD options )
{
	try
	{
		const bool exportselected = 0 != (options & SCENE_EXPORT_SELECTED); // export only selected objects

		// initialize IGame
		m_igame = GetIGameInterface();
		if ( !m_igame )
			throw std::exception( "Failed to initialize IGame interface" );
		IGameConversionManager* cm = GetConversionManager();
		cm->SetCoordSystem( IGameConversionManager::IGAME_OGL );
		m_igame->InitialiseIGame( exportselected );


		// open file for writing
		_wfopen_s( &m_fh, name, L"wb" );
		m_sceneData = new SceneData( name );


		if( m_fh == nullptr )
		{
			perror( "The following error occurred" );
			throw std::exception( "Failed to open file" );
		}


		// TODO: export materials, meshes, ...
		collectNodes();
		loadObjects();
		
		m_sceneData->exportData( m_fh );

		// release initialized stuff
		deinit();
	}
	catch ( std::exception& e )
	{
		MessageBoxA( i->GetMAXHWnd(), e.what(), ("Export Error"), MB_OK|MB_ICONERROR );
		deinit();
	}
	return TRUE;
}

void MaxExporter::ShowAbout( HWND hwnd )
{
}

int MaxExporter::ExtCount()
{
	return 1;
}

const TCHAR* MaxExporter::Ext( int /*n*/ )
{
	return _T("p3d");
}

const TCHAR* MaxExporter::LongDesc()
{
	return _T("PDH Engine 3DS Max Exporter");
}

const TCHAR* MaxExporter::ShortDesc()
{
	return _T("PDH 3DS Max Exporter");
}

const TCHAR* MaxExporter::AuthorName()
{
	return _T("Preston Hulcy");
}

const TCHAR* MaxExporter::CopyrightMessage()
{
	return _T("Copyright (C) Preston Hulcy 2013");
}

const TCHAR* MaxExporter::OtherMessage1()
{
	return _T("");
}

const TCHAR* MaxExporter::OtherMessage2()
{
	return _T("");
}

unsigned int MaxExporter::Version()
{
	return 1;
}

BOOL MaxExporter::SupportsOptions( int ext, DWORD options )
{
	return TRUE;
}

void MaxExporter::collectNodes()
{
	int num_toplevel_nodes = m_igame->GetTopLevelNodeCount();
	
	for( int i = 0; i < num_toplevel_nodes; ++i )
	{
		IGameNode* node = m_igame->GetTopLevelNode( i );
		if( node != nullptr )
		{
			m_sceneNodes.push_back( node );
			collectChildNodes( node );
		}
	}
}

void MaxExporter::collectChildNodes( IGameNode* node )
{
	if( node == nullptr || node->GetChildCount() <= 0 )
		return;
	
	for( int i = 0; i < node->GetChildCount(); ++i )
	{
		IGameNode* child = node->GetNodeChild( i );
		if( child != nullptr )
		{
			m_sceneNodes.push_back( child );
			collectChildNodes( child );
		}
	}
}


void MaxExporter::loadObjects()
{
	int objId = 0;
	for( int i = 0; i < m_sceneNodes.size(); ++i )
	{
		switch( m_sceneNodes[i]->GetIGameObject()->GetIGameType() )
		{
		case IGameObject::IGAME_MESH:
			loadMesh( m_sceneNodes[i], i, getParentIndex( m_sceneNodes[i] ) );
			break;
		case IGameObject::IGAME_CAMERA:
			loadCamera( m_sceneNodes[i], new SceneData::Camera( i, getParentIndex( m_sceneNodes[i] ) ) );
			break;
		}
	}
}

void MaxExporter::loadMesh( IGameNode* node, int index, int parentIndex )
{
	
	IGameMesh* mesh = reinterpret_cast< IGameMesh* >( node->GetIGameObject() );
	int numModifiers = mesh->GetNumModifiers();
	m_sceneData->log( "Number of modifiers: %d\n", numModifiers );
	bool isSkel = false;

	for( int i = 0; i < numModifiers; ++i )
	{
		if( mesh->GetIGameModifier( i )->IsSkin() )
		{
			m_sceneData->log( "LOADING SKELETAL MESH!!!\n" );
			loadSkelMesh( node, reinterpret_cast< IGameSkin* >( mesh->GetIGameModifier( i ) ), new SceneData::SkelMesh( index, parentIndex ) );
			isSkel = true;
			break;
		}
	}
	if( !isSkel )
		loadStaticMesh( node, new SceneData::StaticMesh( index, parentIndex ) );
}

void MaxExporter::loadStaticMesh( IGameNode* node, SceneData::StaticMesh* meshData )
{
	IGameMesh* mesh = reinterpret_cast< IGameMesh* >( node->GetIGameObject() );
	if( mesh == nullptr )
		throw std::exception( "ERROR: Tried to load NULL mesh" );

	mesh->SetCreateOptimizedNormalList();
	mesh->InitializeData();

	loadMeshAnimationsData( node, meshData );
	Matrix3 worldTM = node->GetWorldTM().ExtractMatrix3();
	Matrix3 invWorldTM = Inverse( worldTM );

	int numFaces = mesh->GetNumberOfFaces();
	
	std::map< IGameMaterial*, std::vector< FaceEx* > > faceMap;
	
	for( int i = 0; i < numFaces; ++i )
	{
		FaceEx* face = mesh->GetFace( i );
		faceMap[ mesh->GetMaterialFromFace( face ) ].push_back( face );	
	}
	

	std::map< SceneData::Vertex, int > vmap;

	for( auto faceIter = faceMap.begin(); faceIter != faceMap.end(); ++faceIter )
	{		
		TriBatch* triBatch = new TriBatch();
		triBatch->mat = faceIter->first;
		m_sceneData->addMaterial( triBatch->mat );

		for( int i = 0; i < faceIter->second.size(); ++i )
		{
			FaceEx* face = faceIter->second[i];
			for( int x = 0; x < 3; ++x )
			{
				int tanBitanIndex = mesh->GetFaceVertexTangentBinormal( face->meshFaceIndex, x );


				Vertex v(	invWorldTM.PointTransform( mesh->GetVertex( face->vert[x] ) ),			
							invWorldTM.VectorTransform( mesh->GetNormal( face->norm[x] ) ), 
							mesh->GetColorVertex( face->color[x] ),
							mesh->GetAlphaVertex( face->alpha[x] ),
							mesh->GetTexVertex( face->texCoord[x] ),
							invWorldTM.VectorTransform( mesh->GetTangent( tanBitanIndex ) ),
							invWorldTM.VectorTransform( mesh->GetBinormal( tanBitanIndex ) ) );




				int new_index;
				if( vmap.find( v ) == vmap.end() )
				{
					// new vertex found
					new_index = (int)vmap.size();
					vmap[v] = new_index;
					triBatch->vertices.push_back( v );
				}
				else
				{
					new_index = vmap[v];
				}
				triBatch->indices.push_back( new_index );
			}
		}
		meshData->triBatches.push_back( triBatch );
	}

	meshData->meshType = SceneData::Mesh::STATIC;
	meshData->numAttribs = 6;
	meshData->layout = new int[ meshData->numAttribs ];
	meshData->layout[0] = FLOAT3;
	meshData->layout[1] = FLOAT3;
	meshData->layout[2] = FLOAT4;
	meshData->layout[3] = FLOAT2;
	meshData->layout[4] = FLOAT3;
	meshData->layout[5] = FLOAT3;

	m_sceneData->addMesh( meshData );
}

void MaxExporter::loadSkelMesh( IGameNode* node, IGameSkin* skin, SceneData::SkelMesh* meshData )
{
	IGameMesh* mesh = reinterpret_cast< IGameMesh* >( node->GetIGameObject() );
	//mesh = skin->GetInitialPose();
	if( mesh == nullptr )
		throw std::exception( "ERROR: Tried to load NULL mesh" );

	mesh->SetCreateOptimizedNormalList();
	mesh->InitializeData();

	loadMeshAnimationsData( node, meshData );
	Matrix3 worldTM = node->GetWorldTM().ExtractMatrix3();
	Matrix3 invWorldTM = Inverse( worldTM );
	
	std::map< int, int > boneMap;

	int numBones = skin->GetTotalBoneCount();
	for( int i = 0; i < numBones; ++i )
	{
		IGameNode* boneNode = skin->GetIGameBone( i, true );
		SceneData::Bone* bone = new SceneData::Bone();
				
		char* boneName;
		
		int strLength = (int)wcslen( boneNode->GetName() );
		boneName = new char[ strLength + 1 ];
		int length = (int)wcstombs( boneName, boneNode->GetName(), strLength );
		if( length == strLength )
			boneName[ strLength ] = '\0';
		
		bone->boneName = boneName;

		m_sceneData->log( "Bone name: %s, Index: %d\n", boneName, i );
		meshData->bones.push_back( bone );
		boneMap[ boneNode->GetNodeID() ] = i;
	}

	TimeValue t = m_igame->GetSceneStartTime();
	for( ; t < m_igame->GetSceneEndTime(); t +=  4800 / m_sceneData->getFrameRate() )
	{
		for( int i = 0; i < numBones; ++i )
		{
			IGameNode* boneNode = skin->GetIGameBone( i, true );			
			meshData->bones[i]->worldTMFrames.push_back( boneNode->GetWorldTM( t ).ExtractMatrix3() );
		}
	}
	
	
	int numFaces = mesh->GetNumberOfFaces();
	
	std::map< IGameMaterial*, std::vector< FaceEx* > > faceMap;
	
	for( int i = 0; i < numFaces; ++i )
	{
		FaceEx* face = mesh->GetFace( i );
		faceMap[ mesh->GetMaterialFromFace( face ) ].push_back( face );	
	}
	
	m_sceneData->log( "Num of materials: %d", faceMap.size() );
	std::map< SceneData::SkelVertex, int > vmap;

	for( auto faceIter = faceMap.begin(); faceIter != faceMap.end(); ++faceIter )
	{		
		SceneData::SkelTriBatch* triBatch = new SceneData::SkelTriBatch();
		triBatch->mat = faceIter->first;
		if( triBatch->mat != nullptr )
			m_sceneData->addMaterial( triBatch->mat );

		for( int i = 0; i < faceIter->second.size(); ++i )
		{
			FaceEx* face = faceIter->second[i];
			for( int x = 0; x < 3; ++x )
			{
				int tanBitanIndex = mesh->GetFaceVertexTangentBinormal( face->meshFaceIndex, x );
			
				SceneData::SkelVertex v(	mesh->GetVertex( face->vert[x] ),			
											mesh->GetNormal( face->norm[x] ), 
											mesh->GetColorVertex( face->color[x] ),
											mesh->GetAlphaVertex( face->alpha[x] ),
											mesh->GetTexVertex( face->texCoord[x] ),
											mesh->GetTangent( tanBitanIndex ),
											mesh->GetBinormal( tanBitanIndex ) );

				//----------------------------
				// Load bone data
				//--------------------------- 
				int numBones = skin->GetNumberOfBones( face->vert[x] );
				//m_sceneData->log( "Number of bones: %d\n", numBones );
				m_sceneData->flush();
				std::vector< std::pair< int, float > > bones;
				for( int b = 0; b < numBones; ++b )
				{
					bones.push_back( std::pair< int, float >( b, skin->GetWeight( face->vert[x], b ) ) );
				}

				std::sort( bones.begin(), bones.end(), 
					[&] ( std::pair< int, float >& a, std::pair< int, float >& b ) -> bool
						{
							return	a.second > b.second;
						} );
				
				if( bones.size() > 4 )
					bones.resize(4);
				float totalWeight = 0.0f;
				for( int w = 0; w < bones.size(); ++w )
					totalWeight += bones[w].second;
				for( int n = 0; n < bones.size(); ++n )
					bones[n].second /= totalWeight;

				m_sceneData->log( "Bone indices:\n" );
				m_sceneData->flush();
				for( int b = 0; b < 4 && b < bones.size(); ++b )
				{
					IGameNode* boneNode = skin->GetIGameBone( face->vert[x], bones[b].first );
					if( boneNode == nullptr )
						continue;
						
					v.boneIndices[b] = (float)boneMap[ boneNode->GetNodeID() ];	

					char* boneName;
					int strLength = (int)wcslen( boneNode->GetName() );
					boneName = new char[ strLength + 1 ];
					int length = (int)wcstombs( boneName, boneNode->GetName(), strLength );
					if( length == strLength )
						boneName[ strLength ] = '\0';
					m_sceneData->log( "Bone Name: %s, ", boneName );
					m_sceneData->flush();
					m_sceneData->log( "Bone Index: %f\n", v.boneIndices[b] );
					m_sceneData->flush();

					if( b < 3 )
					{
						v.weights[b] = bones[b].second;
						m_sceneData->log( "Bone weight: %f\n", v.weights[b] );
						m_sceneData->flush();
					}
				}
				
				//--------------------
				//-------------------
				int new_index;
				if( vmap.find( v ) == vmap.end() )
				{
					// new vertex found
					new_index = (int)vmap.size();
					vmap[v] = new_index;
					triBatch->vertices.push_back( v );
				}
				else
				{
					new_index = vmap[v];
				}
				triBatch->indices.push_back( new_index );
			}			
		}
		meshData->triBatches.push_back( triBatch );
	}
	
	meshData->meshType = SceneData::Mesh::SKELETAL;
	meshData->numAttribs = 8;
	meshData->layout = new int[ meshData->numAttribs ];
	meshData->layout[0] = FLOAT3;
	meshData->layout[1] = FLOAT3;
	meshData->layout[2] = FLOAT4;
	meshData->layout[3] = FLOAT2;
	meshData->layout[4] = FLOAT3;
	meshData->layout[5] = FLOAT3;
	meshData->layout[6] = FLOAT4;
	meshData->layout[7] = FLOAT3;

	m_sceneData->addMesh( meshData );
}

void MaxExporter::loadMeshAnimationsData( IGameNode* node, SceneData::Mesh* meshData )
{
	TimeValue t = m_igame->GetSceneStartTime();
	
	Matrix3 worldTM = node->GetWorldTM( t ).ExtractMatrix3();
	Matrix3 invWorldTM = Inverse( worldTM );
	IGameControl* controller = node->GetIGameControl();
	if( controller->IsAnimated( IGAME_POS ) || controller->IsAnimated( IGAME_ROT ) || controller->IsAnimated( IGAME_SCALE ) )
	{
		for( ; t < m_igame->GetSceneEndTime(); t += 4800 / m_sceneData->getFrameRate() )
		{
			Matrix3 worldTM = node->GetWorldTM( t ).ExtractMatrix3();
			if( node->GetNodeParent() != nullptr )
			{
				meshData->localTM.push_back( worldTM * Inverse( node->GetNodeParent()->GetWorldTM( t ).ExtractMatrix3() ) );
			}
			else
			{
				meshData->localTM.push_back( worldTM );
			}
		}
	}
	else
	{
		Matrix3 worldTM = node->GetWorldTM().ExtractMatrix3();
		if( node->GetNodeParent() != nullptr )
		{
			meshData->localTM.push_back( worldTM * Inverse( node->GetNodeParent()->GetWorldTM().ExtractMatrix3() ) );
		}
		else
		{
			meshData->localTM.push_back( worldTM );
		}
	}
}


void MaxExporter::loadCamera( IGameNode* node, SceneData::Camera* cameraData )
{
	IGameCamera* camera = reinterpret_cast< IGameCamera* >( node->GetIGameObject() );
	TimeValue t = m_igame->GetSceneStartTime();
	
	Matrix3 worldTM = node->GetWorldTM( t ).ExtractMatrix3();
	Matrix3 invWorldTM = Inverse( worldTM );

	IGameControl* controller = node->GetIGameControl();
	if( controller->IsAnimated( IGAME_POS ) || controller->IsAnimated( IGAME_ROT ) || controller->IsAnimated( IGAME_SCALE ) )
	{
		for( ; t < m_igame->GetSceneEndTime(); t += 4800 / m_sceneData->getFrameRate() )
		{
			Matrix3 worldTM = node->GetWorldTM( t ).ExtractMatrix3();
			if( node->GetNodeParent() != nullptr )
			{
				cameraData->localTM.push_back( worldTM * Inverse( node->GetNodeParent()->GetWorldTM( t ).ExtractMatrix3() ) );
			}
			else
			{
				cameraData->localTM.push_back( worldTM );
			}
		}
	}
	else
	{
		Matrix3 worldTM = node->GetWorldTM().ExtractMatrix3();
		if( node->GetNodeParent() != nullptr )
		{
			cameraData->localTM.push_back( worldTM * Inverse( node->GetNodeParent()->GetWorldTM().ExtractMatrix3() ) );
		}
		else
		{
			cameraData->localTM.push_back( worldTM );
		}
	}

	m_sceneData->addCamera( cameraData );
}


int MaxExporter::getParentIndex( IGameNode* node )
{
	IGameNode* parent = node->GetNodeParent();
	if(  parent != nullptr && node != parent )
	{
		for( int i = 0; m_sceneNodes.size(); ++i )
		{
			if( m_sceneNodes[i] == parent )
				return i;
		}
	}
	return -1;
}
