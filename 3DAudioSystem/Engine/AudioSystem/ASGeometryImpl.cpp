#include "ASGeometryImpl.h"
#include <CommonEngineIncludes.h>
#include "AudioSystem.h"
#include "AudioSystemImpl.h"
#include "ASErrorHandling.h"
#include <Buffer.h>
#include <BufferAccessor.h>
#include <VtxBufferAccessor.h>

namespace Monky
{
	ASGeometryImpl::ASGeometryImpl( Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
			Buffer* indices, BufferAccessor* idxAccessor, 
			float directOcclusion, float reverbOcclusion )
	{
		if( vertices != nullptr && vtxAccessor != nullptr &&
			indices != nullptr && idxAccessor != nullptr )
		{
			int numVertsPerFace = 3;

			m_numVertices = vertices->count();
			m_numIndices = indices->count();
			m_vertices.resize( m_numVertices );
			m_indices.resize( m_numIndices );

			//For now assume all polys are triangles
			m_numPolygon = m_numIndices / numVertsPerFace;
			m_polygons.resize( m_numPolygon );

			for( int v = 0; v < m_numVertices; ++v )
			{
				m_vertices[v].x = vtxAccessor->getPosition( v )[0];
				m_vertices[v].y = vtxAccessor->getPosition( v )[1];
				m_vertices[v].z = vtxAccessor->getPosition( v )[2];
			}
			for( int i = 0; i < m_numIndices; ++i )
				m_indices[i] = idxAccessor->get<int>( 0 , i );

			int indicesOffset = 0;
			for( int poly = 0; poly < m_numPolygon; ++poly )
			{
				Polygon& polygon = m_polygons[ poly ];

				polygon.numVertices = numVertsPerFace;
				polygon.directOcclusion = directOcclusion;
				polygon.reverbOcclusion = reverbOcclusion;				
				polygon.indicesOffset = indicesOffset;

				//adapted from code taken from some fmod example code for occlusion
				float xN = 0.0f;
				float yN = 0.0f;
				float zN = 0.0f;

				int* indices = &m_indices[ indicesOffset ];
				for( int vertex = 0; vertex < polygon.numVertices - 2; ++vertex )
				{
					float xA = m_vertices[ indices[ vertex + 1 ] ].x - m_vertices[ indices[0] ].x;
					float yA = m_vertices[ indices[ vertex + 1 ] ].y - m_vertices[ indices[0] ].y;
					float zA = m_vertices[ indices[ vertex + 1 ] ].z - m_vertices[ indices[0] ].z;
					float xB = m_vertices[ indices[ vertex + 2 ] ].x - m_vertices[ indices[0] ].x;
					float yB = m_vertices[ indices[ vertex + 2 ] ].y - m_vertices[ indices[0] ].y;
					float zB = m_vertices[ indices[ vertex + 2 ] ].z - m_vertices[ indices[0] ].z;
					// cross product
					xN += yA * zB - zA * yB;
					yN += zA * xB - xA * zB;
					zN += xA * yB - yA * xB;
				}
				vec3f normal( xN, yN, zN );
				normal.normalize();
				polygon.normal.x = normal.x;
				polygon.normal.y = normal.y;
				polygon.normal.z = normal.z;
				
				indicesOffset += polygon.numVertices;
			}
			FMOD_RESULT result = AudioSystem::getInstance()->m_asImpl->m_system->createGeometry( m_numPolygon, m_numIndices, &m_geometry );
			asErrorCheck( result );

			for( int poly = 0; poly < m_numPolygon; ++poly )
			{
				Polygon& polygon = m_polygons[poly];
				FMOD_VECTOR vertices[16];
				
				for( int i = 0; i < polygon.numVertices; ++i )
				{
					vertices[i] = m_vertices[ m_indices[ polygon.indicesOffset + i ] ];
				}

				result = m_geometry->addPolygon( polygon.directOcclusion, polygon.reverbOcclusion, false,
					polygon.numVertices, vertices, &polygon.polygonIndex );
				
				asErrorCheck( result );
			}
		}
	}
	//----------------------------------------------------------------------
	ASGeometryImpl::~ASGeometryImpl()
	{
		m_geometry->release();
	}
	//----------------------------------------------------------------------
	void ASGeometryImpl::setPosition( const vec3f& pos )
	{
		FMOD_VECTOR position = { pos.x, pos.y, pos.z };
		m_geometry->setPosition( &position );
	}
	//----------------------------------------------------------------------
	vec3f ASGeometryImpl::getPosition() const
	{
		FMOD_VECTOR position;
		m_geometry->getPosition( &position );
		vec3f pos( position.x, position.y, position.z );
		return pos;
	}
	//----------------------------------------------------------------------
	void ASGeometryImpl::setOrientation( const mat3f& orientation )
	{
		FMOD_VECTOR forward = { orientation[0], orientation[1], orientation[2] };
		FMOD_VECTOR up = { orientation[3], orientation[4], orientation[5] };
		m_geometry->setRotation( &forward, &up );
	}
	//----------------------------------------------------------------------
	mat3f ASGeometryImpl::getOrientation() const
	{
		FMOD_VECTOR forward;
		FMOD_VECTOR up;
		m_geometry->getRotation( &forward, &up );

		return mat3f(	forward.x, forward.y, forward.z,
						up.x,	   up.y,	  up.z,
						0.0f,	   0.0f,	  1.0f );
	}
	//----------------------------------------------------------------------
	void ASGeometryImpl::setScale( const vec3f& scale )
	{
		FMOD_VECTOR scaleVec = { scale.x, scale.y, scale.z };
		m_geometry->setScale( &scaleVec );
	}
	//----------------------------------------------------------------------
	vec3f ASGeometryImpl::getScale() const
	{
		FMOD_VECTOR scale;
		m_geometry->getScale( &scale );
		return vec3f( scale.x, scale.y, scale.z );
	}
}