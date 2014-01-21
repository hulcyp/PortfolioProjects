#pragma once
#include "stdafx.h"
#include <fstream>

#define INT 0
#define INT2 1
#define INT3 2
#define INT4 3
#define BYTE 4
#define FLOAT1 5
#define FLOAT2 6
#define FLOAT3 7
#define FLOAT4 8


class SceneData
{
public:
	struct Vertex
	{
		Vertex( Point3 p, Point3 n, Point3 c, float a, Point2 tex, Point3 tan, Point3 bitan );
		
		float pos[3];
		float normal[3];
		float color[4];
		float uv[2];
		float tangent[3];
		float bitangent[3];

		bool operator<( const Vertex& other ) const;
		bool operator>( const Vertex& other ) const;
		bool operator==( const Vertex& other ) const;
	};

	struct SkelVertex
	{
		SkelVertex( Point3 p, Point3 n, Point3 c, float a, Point2 tex, Point3 tan, Point3 bitan );

		float pos[3];
		float normal[3];
		float color[4];
		float uv[2];
		float tangent[3];
		float bitangent[3];
		float boneIndices[4];
		float weights[3];

		bool operator<( const SkelVertex& other ) const;
		bool operator>( const SkelVertex& other ) const;
		bool operator==( const SkelVertex& other ) const;
	};

	struct Object
	{
		Object( int i, int pId )
			:	id( i )
			,	parentID( pId )
		{}
		int id;
		int parentID;
		std::vector< Matrix3 > localTM;
	};

	struct Mesh : public Object
	{
		Mesh( int i, int parentID )
			:	Object( i, parentID )
		{}

		enum MeshType
		{
			STATIC = 0,
			SKELETAL
		};

		int* layout;
		int numAttribs;
		MeshType meshType;
	};
	
	//---------------------------------
	// Static Mesh
	//---------------------------------
	struct TriangleBatch
	{
		TriangleBatch()
			:	mat( nullptr )
		{}

		IGameMaterial* mat;
		std::vector< int > indices;
		std::vector< Vertex > vertices;
	};

	struct StaticMesh : public Mesh
	{
		StaticMesh( int i, int parentID )
			:	Mesh( i, parentID )
		{}
		std::vector< TriangleBatch* > triBatches;
	};

	//---------------------------------
	// Skeletal Mesh
	//---------------------------------
	struct SkelTriBatch
	{
		SkelTriBatch()
			:	mat( nullptr )
		{}

		IGameMaterial* mat;
		std::vector< int > indices;
		std::vector< SkelVertex > vertices;
	};

	struct Bone
	{
		std::vector< Matrix3 > worldTMFrames;
		const char* boneName;

		bool operator<( const Bone& other ) const;
		bool operator>( const Bone& other ) const;
		bool operator==( const Bone& other ) const;
	};

	struct SkelMesh : public Mesh
	{
		SkelMesh( int i, int parentID )
			:	Mesh( i, parentID )
		{}
		std::vector< SkelTriBatch* > triBatches;
		std::vector< Bone* > bones;
	};


	struct Camera : public Object
	{
		Camera( int i, int parentID )
			:	Object( i, parentID )
		{}
	};
	

	SceneData( const wchar_t* fileName );

	void addMesh( Mesh* );
	void addCamera( Camera* );
	void addMaterial( IGameMaterial* material );
	void exportData( FILE* fh );
	int getFrameRate() { return FRAMES_PER_SECOND; }

	void log( const char* fmt, ... );
	void flush();


private:
	void exportMaterialData( FILE* fh );
	void exportMeshData( FILE* fh );
	void exportStaticMeshData( StaticMesh* mesh, FILE* fh );
	void exportSkelMeshData( SkelMesh* mesh, FILE* fh );
	void exportCameraData( FILE* fh );
	void exportTextureMaps( FILE* fh, IGameMaterial* material );
	const wchar_t* extractFileName( const wchar_t* filePath, int& fileNameLen );
	int getMaterialIndex( IGameMaterial* material );

	std::vector< Mesh* > m_meshes;
	std::vector< Camera* > m_cameras;
	std::vector< IGameMaterial* > m_materials;

	int m_version;

	int m_numAttribs;
	int* m_layout;

	const int FRAMES_PER_SECOND;

	FILE* m_logfh;
};