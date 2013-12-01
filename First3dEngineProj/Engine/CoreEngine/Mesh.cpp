#include "Mesh.h"
#include <ErrorHandlingUtils.h>
#include "Renderer.h"
#include <MathUtils.h>
#include "CommonEngineIncludes.h"
#include "BoundingSphere.h"

namespace pdh
{
	Mesh::Mesh( std::vector< Mesh::Vertex >& vertices, const std::string& material, int mode, int bufferType  )
		:	m_idVBO( 0 )
		,	m_idIBO( 0 )
		,	m_material( material )
		,	m_renderMode( mode )
		,	m_debugMaterial( material )
	{
		if( mode == GL_TRIANGLES )
			generateTangentSpaceVectors( vertices );

		glGenBuffers( 1, &m_idVBO );
		glBindBuffer( GL_ARRAY_BUFFER, m_idVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( vertices[0] ) * vertices.size(),
			vertices.data(), bufferType );
		m_vertices = vertices;
		glBindBuffer( GL_ARRAY_BUFFER, 0 );

		m_bSphere = new BoundingSphere( m_vertices );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::Mesh( std::vector< Mesh::Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode, int bufferType  )
		:	m_idVBO( 0 )
		,	m_idIBO( 0 )
		,	m_material( material )
		,	m_renderMode( mode )
		,	m_debugMaterial( material )
	{
		if( mode == GL_TRIANGLES )
			generateTangentSpaceVectors( vertices, indices );

		glGenBuffers( 1, &m_idVBO );
		glBindBuffer( GL_ARRAY_BUFFER, m_idVBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( vertices[0] ) * vertices.size(),
			vertices.data(), bufferType );
		m_vertices = vertices;
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		setIndices( indices, bufferType );
		
		m_bSphere = new BoundingSphere( m_vertices );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::~Mesh()
	{
		if( m_idVBO )
		{
			glDeleteBuffers( 1, &m_idVBO );
			m_idVBO = 0;
		}
		if( m_idIBO )
		{
			glDeleteBuffers( 1, &m_idIBO );
			m_idIBO = 0;
		}
		m_vertices.clear();
		m_indices.clear();
		SAFE_DELETE( m_bSphere );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::setIndices( std::vector< unsigned int >& indices, int bufferType )
	{
		glGenBuffers( 1, &m_idIBO );
		glBindBuffer( GL_ARRAY_BUFFER, m_idVBO );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_idIBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices[0] ) * indices.size(),
			indices.data(), bufferType );
		m_indices = indices;
	}

	////////////////////////////////////////////////////////////
	// Protected member functions
	///////////////////////////////////////////////////////////
	void Mesh::generateTangentSpaceVectors( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices )
	{
		for( size_t i = 0; i < indices.size(); i += 3 )
		{
			computeTangentSpaceMatrix( vertices[ indices[i] ], vertices[ indices[i + 1] ], vertices[ indices[i + 2] ] );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::generateTangentSpaceVectors( std::vector< Vertex >& vertices )
	{
		for( size_t i = 0; i < vertices.size(); i += 3 )
		{
			computeTangentSpaceMatrix( vertices[i], vertices[i + 1], vertices[i + 2] );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::computeTangentSpaceMatrix( Vertex& p0, Vertex& p1, Vertex& p2 )
	{
		vec3f v1;
		vec3f v2;
		vec2f uv1;
		vec2f uv2;
		float det;

		v1.x = p1.x - p0.x;
		v1.y = p1.y - p0.y;
		v1.z = p1.z - p0.z;

		v2.x = p2.x - p0.x;
		v2.y = p2.y - p0.y;
		v2.z = p2.z - p0.z;

		uv1.x = p1.u - p0.u;
		uv1.y = p1.v - p0.v;

		uv2.x = p2.u - p0.u;
		uv2.y = p2.v - p0.v;

		det = uv1.x * uv2.y - uv2.x * uv1.y;

		p0.tx = ( v1.x * uv2.y - v2.x * uv1.y ) / det;
		p0.ty = ( v1.y * uv2.y - v2.y * uv1.y ) / det;
		p0.tz = ( v1.z * uv2.y - v2.z * uv1.y ) / det;

		p0.bx = ( -v1.x * uv2.x + v2.x * uv1.x ) / det;
		p0.by = ( -v1.y * uv2.x + v2.y * uv1.x ) / det;
		p0.bz = ( -v1.z * uv2.x + v2.z * uv1.x ) / det;

		p1.tx = p0.tx;
		p1.ty = p0.ty;
		p1.tz = p0.tz;

		p1.bx = p0.bx;
		p1.by = p0.by;
		p1.bz = p0.bz;

		p2.tx = p0.tx;
		p2.ty = p0.ty;
		p2.tz = p0.tz;

		p2.bx = p0.bx;
		p2.by = p0.by;
		p2.bz = p0.bz;

	}

	
	////////////////////////////////////////////////////////////
	// Static member functions
	///////////////////////////////////////////////////////////
	Mesh* Mesh::generateCube( float size, const std::string& materialName, Color4f color )
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Algorithm obtianed from: http://stackoverflow.com/questions/7946770/calculating-a-sphere-in-opengl

	Mesh* Mesh::generateSphere( float radius, int rings, int sectors, const std::string& materialName, Color4f color )
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
                float const y = sin( -PI*.5f + PI * r * R );
                float const x = cos(2*PI * s * S) * sin( PI * r * R );
                float const z = sin(2*PI * s * S) * sin( PI * r * R );

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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh* Mesh::generateXYPlane( float size, const std::string& materialName )
	{
		std::vector< Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Vertex( vec3f( 0.0f, size, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 0.0f, 0.0f ) ) );
		
		vertices.push_back( Vertex( vec3f( size, size, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Vertex( vec3f( size, 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
									color::WHITE,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Vertex( vec3f( 0.0f, 0.0f, 0.0f ),
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh* Mesh::generateAxis( float length, const std::string& materialName )
	{
		std::vector< Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Vertex( vec3f(), 
									vec3f(),
									color::RED,
									vec2f() ) );
		vertices.push_back( Vertex( vec3f( length, 0.0f, 0.0f ),
									vec3f(),
									color::RED,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f(),
									vec3f(),
									color::GREEN,
									vec2f() ) );
		vertices.push_back( Vertex( vec3f( 0.0f, length, 0.0f ),
									vec3f(),
									color::GREEN,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f(),
									vec3f(),
									color::BLUE,
									vec2f() ) );
		vertices.push_back( Vertex( vec3f( 0.0f, 0.0f, length ),
									vec3f(),
									color::BLUE,
									vec2f() ) );
		
		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh* Mesh::generatePointMesh( float radius, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Vertex( vec3f( radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f( -radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f( -radius, radius, -radius ),
									vec3f(),
									color,
									vec2f() ) ) ;

		vertices.push_back( Vertex( vec3f( radius, -radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f( -radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );
		
		vertices.push_back( Vertex( vec3f( radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f( radius, -radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f( -radius, radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f( radius, radius, -radius ),
									vec3f(),
									color,
									vec2f() ) );

		vertices.push_back( Vertex( vec3f( -radius, -radius, radius ),
									vec3f(),
									color,
									vec2f() ) );

		for( size_t i = 0; i < vertices.size(); ++i )
		{
			indices.push_back( i );
		}

		return new Mesh( vertices, indices, materialName, GL_LINES );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh* Mesh::generateArrow( const vec3f& dir, float length, const std::string& materialName, Color4f color )
	{
		std::vector< Mesh::Vertex > vertices;
		std::vector< unsigned int > indices;

		//float lengthDivFour = length * 0.25f;
		//float xArrowHeadDist = length - lengthDivFour;

		vertices.push_back( Vertex( vec3f(),
									vec3f(),
									color::GREEN,
									vec2f() ) );

		vertices.push_back( Vertex(	dir*length,
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh* Mesh::generate2DOrthoRectangle( float width, float height, const std::string& materialName, Color4f color )
	{
		std::vector< Vertex > vertices;
		std::vector< unsigned int > indices;

		vertices.push_back( Vertex( vec3f( 0.0f, height ),
									vec3f(),
									color,
									vec2f( 0.0f, 1.0f ) ) );

		vertices.push_back( Vertex( vec3f( width, height ),
									vec3f(),
									color,
									vec2f( 1.0f, 1.0f ) ) );

		vertices.push_back( Vertex( vec3f( width, 0.0f ),
									vec3f(),
									color,
									vec2f( 1.0f, 0.0f ) ) );
		
		vertices.push_back( Vertex( vec3f( 0.0f, 0.0f ),
									vec3f( 0.0f, 0.0f, 1.0f ),
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
}