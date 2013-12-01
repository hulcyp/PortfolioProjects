#pragma once
#include <glew.h>
#include <glut.h>
#include <vector>
#include "Colors.h"
#include <Vector2.h>
#include <Vector3.h>

namespace pdh
{
	typedef Vector3<float> vec3f;
	typedef Vector2<float> vec2f;

	class BoundingSphere;

	class Mesh
	{
	public:

		struct Vertex
		{
			Vertex(){}

			Vertex( float x, float y, float z,
					float xn, float yn, float zn,
					float r, float g, float b, float a,
					float u, float v, float xt = 0.0f, float yt = 0.0f,
					float zt = 0.0f, float xb = 0.0f, float yb = 0.0f, float zb = 0.0f )
					:	x( x )
					,	y( y )
					,	z( z )
					,	xn( xn )
					,	yn( yn )
					,	zn( zn )
					,	r( r )
					,	g( g )
					,	b( b )
					,	a( a )
					,	u( u )
					,	v( v )
					,	tx( xt )
					,	ty( yt )
					,	tz( zt )
					,	bx( xb )
					,	by( yb )
					,	bz( zb )
			{}

			Vertex( vec3f pos, vec3f norm, Color4f color, vec2f uv,
				vec3f tan = vec3f(), vec3f bit = vec3f() )
				:	x( pos.x )
				,	y( pos.y )
				,	z( pos.z )
				,	xn( norm.x )
				,	yn( norm.y )
				,	zn( norm.z )
				,	r( color.r )
				,	g( color.g )
				,	b( color.b )
				,	a( color.a )
				,	u( uv.x )
				,	v( uv.y )
				,	tx( tan.x )
				,	ty( tan.y )
				,	tz( tan.z )
				,	bx( bit.x )
				,	by( bit.y )
				,	bz( bit.z )
			{}
			float x, y, z;
			float xn, yn, zn;
			float r, g, b, a;
			float u, v;
			float tx, ty, tz;
			float bx, by, bz;
		};

		Mesh( std::vector< Vertex >& vertices, const std::string& material, int mode = GL_TRIANGLES, int bufferType = GL_STATIC_DRAW );	
		Mesh( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode = GL_TRIANGLES, int bufferType = GL_STATIC_DRAW );
		~Mesh();

		void setIndices( std::vector< unsigned int >& indices, int bufferType = GL_STATIC_DRAW );
		void setMaterial( const std::string& material ) { m_material = material; }
		void setRenderMode( int renderMode ) { m_renderMode = renderMode; }
		const std::string& getMaterial() { return m_material; }
		const unsigned int getVBOID() { return m_idVBO; }
		const unsigned int getIBOID() { return m_idIBO; }
		const int getNumVertices() { return m_vertices.size(); }
		const int getNumIndices() { return m_indices.size(); }
		const int getRenderMode() { return m_renderMode; }
		BoundingSphere* getBoundingSphere() { return m_bSphere; }

		void setDebugMaterial( const std::string& debugMaterial ) { m_debugMaterial = debugMaterial; }
		const std::string& getDebugMaterial() { return m_debugMaterial; }
			
		
	protected:
		void generateTangentSpaceVectors( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices );
		void generateTangentSpaceVectors( std::vector< Vertex >& vertices );
		void computeTangentSpaceMatrix( Vertex& p0, Vertex& p1, Vertex& p2 );
		
		/////////////////////////////////////////
		// Static geometry builder functions
		////////////////////////////////////////
	public:
		static Mesh* generateCube( float size, const std::string& materialName = "simpleMaterial", Color4f color  = color::WHITE );
		static Mesh* generateSphere( float radius, int rings, int sectors, const std::string& materialName = "simpleMaterial", Color4f color = color::WHITE );
		static Mesh* generateXYPlane( float size, const std::string& materialName = "simpleMaterial" );
		static Mesh* generateAxis( float length, const std::string& materialName = "simpleMaterial" );
		static Mesh* generatePointMesh( float radius, const std::string& materialName = "simpleMaterial", Color4f color = color::WHITE );
		static Mesh* generateArrow( const vec3f& dir, float length, const std::string& materialName = "simpleMaterial", Color4f color = color::WHITE );
		static Mesh* generate2DOrthoRectangle( float width, float height, const std::string& materialName = "simpleMaterial", Color4f color = color::WHITE );
		

	private:
		unsigned int m_idVBO;
		unsigned int m_idIBO;
		std::vector< Mesh::Vertex > m_vertices;
		std::vector< unsigned int > m_indices;
		BoundingSphere* m_bSphere;

		int m_renderMode;
		std::string m_material;
		std::string m_debugMaterial;
	};
}