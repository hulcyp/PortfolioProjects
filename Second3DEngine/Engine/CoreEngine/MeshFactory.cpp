#include "MeshFactory.h"
#include "Mesh.h"
#include <GLBuffer.h>
#include <BufferLayout.h>
#include <VtxBufferAccessor.h>

namespace Monky
{
	Mesh* MeshFactory::generateCube( float size, const std::string& materialName, Color4f color )
	{
		float hSize = size;
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;
		
		// +X axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, size ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, 0.0f ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, 0.0f ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, size ),
											vec3f( 1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		// -X axis face
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, 0.0f ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, size ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, size ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, 0.0f ),
											vec3f( -1.0f, 0.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 4 );
		indices.push_back( 6 );
		indices.push_back( 7 );
		indices.push_back( 6 );
		indices.push_back( 4 );
		indices.push_back( 5 );
				
		// +Z axis face
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, size ),
											vec3f( 0.0f, 0.0f, 1.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 8 );
		indices.push_back( 10 );
		indices.push_back( 11 );
		indices.push_back( 10 );
		indices.push_back( 8 );
		indices.push_back( 9 );

		// -Z axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, 0.0f ),
											vec3f( 0.0f, 0.0f, -1.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 12 );
		indices.push_back( 14 );
		indices.push_back( 15 );
		indices.push_back( 14 );
		indices.push_back( 12 );
		indices.push_back( 13 );

		// +Y axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, 0.0f ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, size, size ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, size ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, size, 0.0f ),
											vec3f( 0.0f, 1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 16 );
		indices.push_back( 18 );
		indices.push_back( 19 );
		indices.push_back( 18 );
		indices.push_back( 16 );
		indices.push_back( 17 );

		// -Y axis face
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, size ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( size, 0.0f, 0.0f ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 0.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, 0.0f ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 1.0f, 1.0f ) ) );
		vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, 0.0f, size ),
											vec3f( 0.0f, -1.0f, 0.0f ),
											color,
											vec2f( 0.0f, 1.0f ) ) );
		indices.push_back( 20 );
		indices.push_back( 22 );
		indices.push_back( 23 );
		indices.push_back( 22 );
		indices.push_back( 20 );
		indices.push_back( 21 );

		for( size_t i = 0; i < vertices.size(); ++i )
		{
			vertices[i].x -= size * .5f;
			vertices[i].y -= size * .5f;
			vertices[i].z -= size * .5f;
		}
		

		Mesh* mesh = new Mesh( vertices, indices, materialName, GL_TRIANGLES );
		return mesh;
	}
	//-------------------------------------------------------------------------------------------------------------------------
	// Algorithm obtianed from: http://stackoverflow.com/questions/7946770/calculating-a-sphere-in-opengl

	Mesh* MeshFactory::generateSphere( float radius, int rings, int sectors, const std::string& materialName, Color4f color )
	 {	
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

        float const R = 1.f/(float)(rings-1);
        float const S = 1.f/(float)(sectors-1);
        int r, s;

        for(r = 0; r < rings; r++) 
		{
			for(s = 0; s < sectors; s++) 
			{
                float const y = sin( -MathFuncs<float>::PI*.5f + MathFuncs<float>::PI * r * R );
                float const x = cos(2*MathFuncs<float>::PI * s * S) * sin( MathFuncs<float>::PI * r * R );
                float const z = sin(2*MathFuncs<float>::PI * s * S) * sin( MathFuncs<float>::PI * r * R );

				vec2f uv( s*S, r*R );
				vec3f verts( x * radius, y * radius, z * radius );
				vec3f norm( x, y, z );
				vertices.push_back( Mesh::Vertex( verts, norm, color, uv ) );
			}
		}

        for(r = 0; r < rings; r++) 
		{
			for(s = 0; s < sectors; s++) 
			{
				indices.push_back( r * sectors + s );
				indices.push_back( r * sectors + (s+1) );
                indices.push_back( (r+1) * sectors + (s+1) );
                indices.push_back( (r+1) * sectors + s );
			}
		}

		Mesh* mesh = new Mesh( vertices, indices, materialName, GL_QUADS );
		return mesh;
    }
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateXYPlane( float size, const std::string& materialName )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, size, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 0.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( size, size, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( size, 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 0.0f, 1.0f ) ) );

		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		
		Mesh* mesh = new Mesh( vertices, indices, materialName );
		return mesh;

	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAxis( float length, const std::string& materialName )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f(), 
									vec3f(),
									color::RED,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( length, 0.0f, 0.0f ),
									vec3f(),
									color::RED,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color::GREEN,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, length, 0.0f ),
									vec3f(),
									color::GREEN,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color::BLUE,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, length ),
									vec3f(),
									color::BLUE,
									vec2f() ) );
		
		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );

	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAxis( float length, Color4f color, const std::string& materialName )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f(), 
									vec3f(),
									color,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( length, 0.0f, 0.0f ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, length, 0.0f ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color,
									vec2f() ) );
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, length ),
									vec3f(),
									color,
									vec2f() ) );
		
		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generatePointMesh( float radius, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, radius, -radius ),
									vec3f(),
									color,
									vec2f() ) ) ;

		vertices.push_back( Mesh::Vertex( vec3f( radius, -radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( radius, radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex( vec3f( -radius, -radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateArrow( const vec3f& dir, float length, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		//float lengthDivFour = length * 0.25f;
		//float xArrowHeadDist = length - lengthDivFour;

		vertices.push_back( Mesh::Vertex( vec3f(),
									vec3f(),
									color::GREEN,
									vec2f() ) );

		vertices.push_back( Mesh::Vertex(	dir*length,
									vec3f(),
									color::RED,
									vec2f() ) );

		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, lengthDivFour, -lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, lengthDivFour, lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, -lengthDivFour, -lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//vertices.push_back( Vertex(	vec3f( xArrowHeadDist, -lengthDivFour, lengthDivFour ),
		//							vec3f(),
		//							color::RED,
		//							vec2f() ) );
		//

		
		indices.push_back( 0 );
		indices.push_back( 1 );
		//indices.push_back( 1 );
		//indices.push_back( 2 );
		//indices.push_back( 1 );
		//indices.push_back( 3 );
		//indices.push_back( 1 );
		//indices.push_back( 4 );
		//indices.push_back( 1 );
		//indices.push_back( 5 );

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generate2DOrthoRectangle( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, height ),
									vec3f(),
									color,
									vec2f( 0.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, height ),
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f ),
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generate2DOrthoRectangleBorder( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, height ),
									vec3f(),
									color,
									vec2f( 0.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, height ),
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f ),
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 1 );
		indices.push_back( 1 );
		indices.push_back( 2 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		indices.push_back( 3 );
		indices.push_back( 0 );

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAAPlaneXY( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, height ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, height ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 1.0f, 0.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 1.0f, 1.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAAPlaneXZ( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, height ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f, height ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( width, 0.0f, 0.0f ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, 0.0f ),
									vec3f( 0.0f, 1.0f, 0.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 3 );
		indices.push_back( 2 );
		
		indices.push_back( 2 );
		indices.push_back( 1 );
		indices.push_back( 0 );

		return new Mesh( vertices, indices, materialName );
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Mesh* MeshFactory::generateAAPlaneYZ( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, height ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 0.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, width, height ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, width, 0.0f ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Mesh::Vertex( vec3f( 0.0f, 0.0f, 0.0f ),
									vec3f( 1.0f, 0.0f, 0.0f ),
									color,
									vec2f( 0.0f, 0.0f ) ) );
		
		
		indices.push_back( 0 );
		indices.push_back( 2 );
		indices.push_back( 3 );
		
		indices.push_back( 2 );
		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName );	
	}

	Mesh* MeshFactory::generateLine( const vec3f& start, const vec3f& end, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Mesh::Vertex( start, vec3f(), color, vec2f() ) );
		vertices.push_back( Mesh::Vertex( end, vec3f(), color, vec2f() ) );

		indices.push_back( 0 );
		indices.push_back( 1 );

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	//----------------------------------------------------------------------------------------------
	/* Generates along the +x-axis */
	Mesh* MeshFactory::generateCone( float length, float angleFromCenterRadians, int numSectors, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		float zyRadius = length * std::tan( angleFromCenterRadians );
		float angleDelta = 2.0f * MathFuncs<float>::PI / numSectors;
		float startX = 0.0f;
		float endX = startX + length;

		float currentAngle = 0.0f;
		for( int s = 0; s <= numSectors*3; ++s )
		{
			float y = zyRadius * std::sin( currentAngle );
			float z = zyRadius * std::cos( currentAngle );

			vertices.push_back( Mesh::Vertex(	vec3f( endX, y, z ),
												vec3f(),
												color,
												vec2f() ) );
			
			currentAngle += angleDelta;
			y = zyRadius * std::sin( currentAngle );
			z = zyRadius * std::cos( currentAngle );

			vertices.push_back( Mesh::Vertex(	vec3f( endX, y, z ),
												vec3f(),
												color,
												vec2f() ) );
			
			vertices.push_back( Mesh::Vertex(	vec3f( startX, 0.0f, 0.0f ),
												vec3f(),
												color,
												vec2f() ) );
			
			indices.push_back( s + 2 );
			indices.push_back( s + 1 );
			indices.push_back( s );
			s += 2;
		}
		return new Mesh( vertices, indices, materialName );
	}
}