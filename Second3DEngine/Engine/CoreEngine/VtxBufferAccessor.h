#pragma once
#include "BufferAccessor.h"
#include "Buffer.h"
#include "BufferLayout.h"
#include "StdLibraryIncludes.h"

namespace Monky
{
	class VtxBufferAccessor : public BufferAccessor
	{
	public:

		enum AttributeType
		{
			POSITION = 0,
			NORMAL,
			COLOR,
			UV,
			TANGENT,
			BITANGENT,
			COUNT
		};

		enum DefaultMapping
		{
			POS_NORMAL,
			POS_NORMAL_COLOR,
			POS_NORMAL_COLOR_UV,
			POS_NORMAL_UV,
			POS_NORMAL_UV_TAN_BI,
			POS_NORMAL_COLOR_UV_TAN_BI
		};

		VtxBufferAccessor( Buffer* buffer, DefaultMapping mapping );
		VtxBufferAccessor( Buffer* buffer, std::map< AttributeType, int >& attribMapping );

		template< typename T >
		T& get( AttributeType type, int i ) { return *(T*)( m_buffer->getBufferLayout()->attributeOffset( m_attributeMapping[type] ) + m_buffer->at( i ) ); }
		bool hasAttribute( AttributeType type );

		float* getPosition( int i );
		bool hasPosition();

		float* getNormal( int i );
		bool hasNormal();

		float* getColor( int i );
		bool hasColor();

		float* getUV( int i );
		bool hasUVs();

		float* getTangent( int i );
		bool hasTangent();

		float* getBitangent( int i );
		bool hasBitangent();

	protected:
		void buildMapping( DefaultMapping mapping );
	private:
		std::map< AttributeType, int > m_attributeMapping;	
	};
}