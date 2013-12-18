#include "Mesh.h"
#include <MonkyException.h>
#include "Renderer.h"
#include <MathFuncs.h>
#include "CommonEngineIncludes.h"
#include "BoundingSphere.h"
#include "GLBuffer.h"
#include "BufferLayout.h"
#include "VtxBufferAccessor.h"
#include "BufferAccessor.h"
#include "GLBuffer.h"
#include "Material.h"

namespace Monky
{
	Mesh::TriangleBatch::~TriangleBatch()
	{
		SAFE_DELETE( vboAccessor );
		SAFE_DELETE( iboAccessor );
		SAFE_DELETE( vboBuffer );
		SAFE_DELETE( iboBuffer );
	}

	Mesh::Mesh( TriangleBatch* triBatch )
		:	m_meshType( STATIC_MESH )
	{
		m_triangleBatches.push_back( triBatch );
		m_bSphere = new BoundingSphere();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::Mesh( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWinding )
		:	m_meshType( STATIC_MESH )
	{
		TriangleBatch* triBatch = buildTriangleBatch( vboBuffer, iboBuffer, vtxAccessor, material );
		if( rvsWinding )
		{
			reverseWinding( triBatch );
		}
		m_triangleBatches.push_back( triBatch );

		m_bSphere = new BoundingSphere();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::Mesh( std::vector< Mesh::Vertex >& vertices, const std::string& material, int mode, int bufferType  )
		:	m_debugMaterial( material )
		,	m_meshType( STATIC_MESH )

	{
		if( mode == GL_TRIANGLES )
			generateTangentSpaceVectors( vertices );
	
		TriangleBatch* triBatch = buildTriangleBatch( vertices, material, mode, bufferType );
		
		triBatch->vboBuffer->sendToOpenGL();
		m_triangleBatches.push_back( triBatch );

		m_bSphere = new BoundingSphere( vertices );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::Mesh( std::vector< Mesh::Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode, int bufferType  )
		:	m_debugMaterial( material )
		,	m_meshType( STATIC_MESH )
	{
		if( mode == GL_TRIANGLES )
			generateTangentSpaceVectors( vertices, indices );

		TriangleBatch* triBatch = buildTriangleBatch( vertices, indices, material, mode, bufferType );
		triBatch->vboBuffer->sendToOpenGL();
		triBatch->iboBuffer->sendToOpenGL();
		m_triangleBatches.push_back( triBatch );
		
		m_bSphere = new BoundingSphere( vertices );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::~Mesh()
	{
		VECTOR_CLEANUP( m_triangleBatches );
		SAFE_DELETE( m_bSphere );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::setIndices( std::vector< unsigned int >& indices, int triBatch, int mode, int bufferType )
	{
		BufferLayout* layout = new BufferLayout( 1, BufferLayout::INT );
		m_triangleBatches[triBatch]->iboBuffer = new GLBuffer( (char*)indices.data(), indices.size(), layout, GL_ELEMENT_ARRAY_BUFFER, mode, bufferType );
		m_triangleBatches[triBatch]->iboAccessor = new BufferAccessor( m_triangleBatches[triBatch]->iboBuffer );
		m_triangleBatches[triBatch]->iboBuffer->sendToOpenGL();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::setMaterial( const std::string& material, size_t i ) 
	{
		TriangleBatch* triBatch = m_triangleBatches[i];
		triBatch->material = material; 
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const std::string& Mesh::getMaterial( size_t i )
	{
		TriangleBatch* triBatch = getTriangleBatch( i );
		return triBatch->material;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const unsigned int Mesh::getVBOID( size_t i ) 
	{ 
		TriangleBatch* triBatch = getTriangleBatch( i );
		if( triBatch->vboBuffer != nullptr )
		{
			return triBatch->vboBuffer->getBufferID(); 
		}
		else
		{
			return 0;
		}
	}	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const unsigned int Mesh::getIBOID( size_t i ) 
	{ 
		TriangleBatch* triBatch = getTriangleBatch( i );
		if( triBatch->iboBuffer != nullptr )
		{
			return triBatch->iboBuffer->getBufferID();
		}
		else
		{
			return 0;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const int Mesh::getNumVertices( size_t i ) 
	{
		TriangleBatch* triBatch = getTriangleBatch( i );
		if( triBatch->vboBuffer != nullptr )
		{
			return triBatch->vboBuffer->count(); 
		}
		else
		{
			return 0;
		} 
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const int Mesh::getNumIndices( size_t i ) 
	{ 
		TriangleBatch* triBatch = getTriangleBatch( i );
		if( triBatch->iboBuffer != nullptr )
		{
			return triBatch->iboBuffer->count();
		}
		else
		{
			return 0;
		} 
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const BufferLayout* Mesh::getBufferLayout( size_t i ) 
	{ 
		TriangleBatch* triBatch = getTriangleBatch( i );
		if( triBatch->vboBuffer != nullptr )
		{
			return triBatch->vboBuffer->getBufferLayout(); 
		}
		else
		{
			return nullptr;
		} 
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const GLBuffer* Mesh::getVBOBuffer( size_t i ) 
	{ 
		TriangleBatch* triBatch = getTriangleBatch( i );
		if( triBatch->vboBuffer != nullptr )
		{
			return triBatch->vboBuffer; 
		}
		else
		{
			return nullptr;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const int Mesh::getRenderMode( size_t i ) 
	{ 
		TriangleBatch* triBatch = getTriangleBatch( i );
		if( triBatch->vboBuffer != nullptr )
		{
			return triBatch->vboBuffer->getGeometryType(); 
		}
		else
		{
			return 0;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::TriangleBatch* Mesh::getTriangleBatch( size_t i )
	{
		assertion( i < m_triangleBatches.size(), "Triangle batch index: %d is out of bounds", i );
		return m_triangleBatches[i];
	}	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::pushTriangleBatch( TriangleBatch* triBatch )
	{
		m_triangleBatches.push_back( triBatch );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::pushTriangleBatch( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWindinow )
	{
		TriangleBatch* triBatch = buildTriangleBatch( vboBuffer, iboBuffer, vtxAccessor, material );
		if( rvsWindinow )
		{
			reverseWinding( triBatch );
		}
		m_triangleBatches.push_back( triBatch );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::reverseWinding( TriangleBatch* triBatch )
	{
		for( size_t i = 0; i < triBatch->iboBuffer->count(); i += 3 )
		{
			int temp = triBatch->iboAccessor->get<int>( 0, i );
			triBatch->iboAccessor->get<int>( 0, i ) = triBatch->iboAccessor->get<int>( 0, i + 1 );
			triBatch->iboAccessor->get<int>( 0, i + 1 ) = temp;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mesh::addLight( const Material::Light* light, int index )
	{
		for( size_t i = 0; i < m_triangleBatches.size(); ++i )
		{
			Material::getMaterial( m_triangleBatches[i]->material )->addUniform( ("uLights[" + toString(index) + "]").c_str(), light );
		}
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::TriangleBatch* Mesh::buildTriangleBatch( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material )
	{
		TriangleBatch* triBatch = new TriangleBatch();
		triBatch->vboBuffer = vboBuffer;
		triBatch->iboBuffer = iboBuffer;
		triBatch->vboAccessor = vtxAccessor;
		triBatch->iboAccessor = new BufferAccessor( iboBuffer );
		triBatch->material = material;
		return triBatch;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::TriangleBatch* Mesh::buildTriangleBatch( std::vector< Mesh::Vertex >& vertices, const std::string& material, int mode, int bufferType )
	{
		BufferLayout* layout = new BufferLayout( 6, 
		BufferLayout::FLOAT3, BufferLayout::FLOAT3, 
		BufferLayout::FLOAT4, BufferLayout::FLOAT2, 
		BufferLayout::FLOAT3, BufferLayout::FLOAT3 );

		size_t size = layout->stride() * vertices.size();
		char* bufferData = new char[ size ];
		std::memcpy( bufferData, (char*)vertices.data(), size );

		GLBuffer* vboBuffer = new GLBuffer( bufferData, vertices.size(), layout, GL_ARRAY_BUFFER, mode, bufferType );
		VtxBufferAccessor* vboAccessor = new VtxBufferAccessor( vboBuffer, VtxBufferAccessor::POS_NORMAL_COLOR_UV_TAN_BI );
		
		TriangleBatch* triBatch = new TriangleBatch();
		triBatch->vboBuffer = vboBuffer;
		triBatch->vboAccessor = vboAccessor;
		triBatch->material = material;
		return triBatch;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mesh::TriangleBatch* Mesh::buildTriangleBatch( std::vector< Mesh::Vertex >& vertices, std::vector< unsigned int >& indices, const std::string& material, int mode, int bufferType )
	{
		TriangleBatch* triBatch = buildTriangleBatch( vertices, material, mode, bufferType );
		BufferLayout* layout = new BufferLayout( 1, BufferLayout::INT );

		size_t size = layout->stride() * indices.size();
		char* bufferData = new char[ size ];
		std::memcpy( bufferData, (char*)indices.data(), size );

		triBatch->iboBuffer = new GLBuffer( bufferData, indices.size(), layout, GL_ELEMENT_ARRAY_BUFFER, mode, bufferType );
		triBatch->iboAccessor = new BufferAccessor( triBatch->iboBuffer );
		return triBatch;
	}

}