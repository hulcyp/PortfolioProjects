#pragma once
#include <CommonEngineIncludes.h>
#include <vector>
#include <fmod.hpp>

namespace Monky
{
	class ASGeometryImpl;
	class Buffer;
	class VtxBufferAccessor;
	class BufferAccessor;

	class ASGeometryImpl
	{
	public:
		ASGeometryImpl( Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
			Buffer* indices, BufferAccessor* idxAccessor, 
			float directOcclusion, float reverbOcclusion );
		~ASGeometryImpl();

		void setPosition( const vec3f& pos );
		vec3f getPosition() const;

		void setOrientation( const mat3f& orientation );
		mat3f getOrientation() const;

		void setScale( const vec3f& scale );
		vec3f getScale() const;
		
	private:
		struct Polygon
		{
			int numVertices;
			int indicesOffset;
			float directOcclusion;
			float reverbOcclusion;
			FMOD_VECTOR normal;
			int polygonIndex;
		};

		int m_numVertices;
		std::vector< FMOD_VECTOR > m_vertices;
		int m_numPolygon;
		std::vector< Polygon > m_polygons;
		int m_numIndices;
		std::vector< int > m_indices;
		FMOD::Geometry* m_geometry;
	};
}