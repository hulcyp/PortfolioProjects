#pragma once
#include "CommonEngineIncludes.h"

/*
Version Number: int
Number of materials: int
	Material#1:
		str length: int
		Name:  wchar_t* 
		Num of Texture files: int
		Texture type ID: int
		str length: int
		Texture File Name: wchar_t* 		
		...
	...
Frames per second: int
Number of Meshes: int
	Mesh#1:
		node id: int
		parent id: int
		Mesh Type: 0(Static) 1(Skel)
		Buffer Layout:
			Num Attributes: int
			Attribute#1 Type: int
			Attribute#2 Type: int
			...
		Number of TriangleBatches: int
		TriangleBatch:
			Material ID: int 
			Number of indices: int
			Number of vertices: int
			Index data: int*
			Vertex data: float*
		number of frames: int
		frame#1:
			translation: vec3f
			rotation: quaternion
			scale: vec3f
		frame#2:
			translation: vec3f
			rotation: quaternion
			scale: vec3f
		...
		**if skeletal**
		number of bones: int
		number of frames: int
		worldToBone transform:
			translation: vec3f
			rotation: quaternion
			scale: vec3f
		frame#1:
			boneToWorld:
				translation: vec3f
				rotation: quaternion
				scale: vec3f
		...
	...
Number of Cameras: int
	Camera#1:
		node id: int
		parent id: int
		number of frames: int
		frame#1:
			translation: vec3f
		...
*/

namespace Monky
{

#define ID_AM 0
#define ID_DI 1
#define ID_SP 2
#define ID_SI 5
#define ID_BU 8


	class Actor;
	class Material;
	class Scene;

	class P3DImporter
	{
	public:

		struct Object
		{
			Actor* actor;
			int id;
			int parentID;
		};

		P3DImporter( const std::string& file );

		void importData( Scene* scene );

	protected:
		void importMaterials();
		void importMeshes( Scene* scene );
		void importCameras( Scene* scene );
		void addTextureToMaterial( Material* material, int texType, const std::string& texFile );
		std::string changeFileExt( const std::string& texFile, const std::string& newExt );

	private:
		FILE* m_fh;
		std::vector< std::string > m_materials;
		// A mapping from mesh id from file to the index it is stored in the scene->actors vector
		std::map< int, int > m_parentIDToActorIndex;
		int m_version;
		std::string m_fileName;
		int m_framesPerSecond;
	};
}