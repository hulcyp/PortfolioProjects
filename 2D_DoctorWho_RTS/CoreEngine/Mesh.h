#pragma once
#include <glew.h>
#include <glut.h>
#include <vector>
#include "Colors.h"
#include <Vector2.h>
#include <Vector3.h>
#include <Material.h>

namespace pdh
{
	typedef Vector3<float> vec3f;
	typedef Vector2<float> vec2f;

	class BoundingSphere;
	class VtxBufferAccessor;
	class GLBuffer;
	class BufferAccessor;
	class BufferLayout;

	class Mesh
	{
	public:

		enum MeshType
		{
			STATIC_MESH,
			SKELETAL_MESH
		};

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

		struct TriangleBatch
		{
			TriangleBatch()
				:	vboAccessor( nullptr )
				,	iboAccessor( nullptr )
				,	vboBuffer( nullptr )
				,	iboBuffer( nullptr )
			{}
			TriangleBatch( VtxBufferAccessor* vtxAccessor, BufferAccessor* iboAccessor,
				GLBuffer* vbo, GLBuffer* ibo, const std::string& mat )
				:	vboAccessor( vtxAccessor )
				,	iboAccessor( iboAccessor )
				,	vboBuffer( vbo )
				,	iboBuffer( ibo )
				,	material( mat )
			{}
			~TriangleBatch();
			VtxBufferAccessor*	vboAccessor;
			BufferAccessor*		iboAccessor;
			GLBuffer*			vboBuffer;
			GLBuffer*			iboBuffer;
			std::string			material;
		};

		Mesh( TriangleBatch* triBatch );
		Mesh( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWinding = false );

		Mesh( std::vector< Vertex >& vertices, const std::string& material, int mode = GL_TRIANGLES, int bufferType = GL_STATIC_DRAW );	
		Mesh( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode = GL_TRIANGLES, int bufferType = GL_STATIC_DRAW );
		~Mesh();

		void setIndices( std::vector< unsigned int >& indices, int triBatch = 0, int mode = GL_TRIANGLES, int bufferType = GL_STATIC_DRAW );
		void setMaterial( const std::string& material, size_t i = 0 );
		
		const std::string& getMaterial( size_t i );
		const unsigned int getVBOID( size_t i );
		const unsigned int getIBOID( size_t i );
		const int getNumVertices( size_t i );
		const int getNumIndices( size_t i );
		const BufferLayout* getBufferLayout( size_t i );
		const GLBuffer* getVBOBuffer( size_t i );
		const int getRenderMode( size_t i );
		
		BoundingSphere* getBoundingSphere() { return m_bSphere; }

		void setDebugMaterial( const std::string& debugMaterial ) { m_debugMaterial = debugMaterial; }
		const std::string& getDebugMaterial() { return m_debugMaterial; }
		
		TriangleBatch* getTriangleBatch( size_t i );
		size_t getNumTriBatches() { return m_triangleBatches.size(); }
		void pushTriangleBatch( TriangleBatch* triBatch );
		void pushTriangleBatch( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWinding = false );

		void reverseWinding( TriangleBatch* triBatch );
		MeshType getMeshType() const { return m_meshType; }

		virtual void updateMaterial( const matStackf& modelStack, int index ){};

		void addLight( const Material::Light* light, int i );
		
	protected:
		void generateTangentSpaceVectors( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices );
		void generateTangentSpaceVectors( std::vector< Vertex >& vertices );
		void computeTangentSpaceMatrix( Vertex& p0, Vertex& p1, Vertex& p2 );
		TriangleBatch* buildTriangleBatch( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material );
		TriangleBatch* buildTriangleBatch( std::vector< Vertex >& vertices, const std::string& material, int mode = GL_TRIANGLES, int bufferType = GL_STATIC_DRAW );
		TriangleBatch* buildTriangleBatch( std::vector< Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode = GL_TRIANGLES, int bufferType = GL_STATIC_DRAW );

	protected:
		BoundingSphere* m_bSphere;
	
		std::string m_debugMaterial;
		std::vector< TriangleBatch* > m_triangleBatches;
		MeshType m_meshType;
	};
}