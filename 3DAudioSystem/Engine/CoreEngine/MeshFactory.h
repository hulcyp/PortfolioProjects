#pragma once
#include "CommonEngineIncludes.h"

namespace Monky
{
	class Mesh;

	class MeshFactory
	{
	public:
		static Mesh* generateCube( float size, const std::string& materialName = "SimpleColorMaterial", Color4f color  = color::WHITE );
		static Mesh* generateSphere( float radius, int rings, int sectors, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE );
		static Mesh* generateXYPlane( float size, const std::string& materialName = "SimpleColorMaterial" );
		static Mesh* generateAxis( float length, const std::string& materialName = "SimpleColorMaterial" );
		static Mesh* generateAxis( float length, Color4f color, const std::string& materialName = "SimpleColorMaterial" );
		static Mesh* generatePointMesh( float radius, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE );
		static Mesh* generateArrow( const vec3f& dir, float length, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE );
		static Mesh* generate2DOrthoRectangle( float width, float height, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE );
		static Mesh* generate2DOrthoRectangleBorder( float width, float height, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE );
		static Mesh* generateAAPlaneXY( float width, float height, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE  ); 
		static Mesh* generateAAPlaneXZ( float width, float height, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE  );
		static Mesh* generateAAPlaneYZ( float width, float height, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE  );
		static Mesh* generateLine( const vec3f& start, const vec3f& end, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE );
		static Mesh* generateCone( float length, float angleFromCenterRadians, int sectors, const std::string& materialName = "SimpleColorMaterial", Color4f color = color::WHITE ); 
	};
}