#include "BufferLayout.h"
#include "CommonEngineIncludes.h"

#include "glHelper.h"

namespace Monky
{
	BufferLayout::BufferLayout()
		:	m_stride( 0 )
	{}

	BufferLayout::BufferLayout( int numAttribs, ... )
		:	m_stride( 0 )
	{
		va_list args;
		va_start( args, numAttribs );

		for( int i = 0; i < numAttribs; ++i )
		{
			DataType type = va_arg( args, DataType );
			m_layout.push_back( type );
		}
		va_end( args );
		for( size_t i = 0; i < m_layout.size(); ++i )
		{
			m_stride += dataTypeSize( m_layout[i] );
		}
	}

	void BufferLayout::pushAttribute( DataType dt )
	{
		m_layout.push_back( dt );
		m_stride += dataTypeSize( dt );
	}

	size_t BufferLayout::numAttributes() const
	{
		return m_layout.size();
	}

	size_t BufferLayout::stride() const
	{
		return m_stride;
	}

	int BufferLayout::dataTypeSize( DataType dt ) const
	{
		int size = 0;
		switch( dt )
		{
		case INT:
			size = sizeof( int );
			break;
		case INT2:
			size = sizeof( int ) * 2;
			break;
		case INT3:
			size = sizeof( int ) * 3;
			break;
		case INT4:
			size = sizeof( int ) * 4;
			break;
		case BYTE:
			size = sizeof( char );
			break;
		case FLOAT1:
			size = sizeof( float );
			break;
		case FLOAT2:
			size = sizeof( float ) * 2;
			break;
		case FLOAT3:
			size = sizeof( float ) * 3;
			break;
		case FLOAT4:
			size = sizeof( float ) * 4;
			break;
		}
		return size;
	}

	int BufferLayout::attributeSize( size_t i ) const
	{
		assertion( i < m_layout.size(), "Layout index: %d is out of bounds", i );
		return dataTypeSize( m_layout[i] );
	}

	int BufferLayout::attributeOffset( size_t index ) const
	{
		assertion( index < m_layout.size(), "Layout index: %d is out of bounds", index );
		int offset = 0;
		for( size_t j = 0; j < index; ++j )
		{
			offset += attributeSize( j );
		}
		return offset;
	}

	int BufferLayout::numOfDataType( DataType dt ) const
	{
		int size = 0;
		switch( dt )
		{
		case INT:
			size = 1;
			break;
		case INT2:
			size = 2;
			break;
		case INT3:
			size = 3;
			break;
		case INT4:
			size = 4;
			break;
		case BYTE:
			size = 1;
			break;
		case FLOAT1:
			size = 1;
			break;
		case FLOAT2:
			size = 2;
			break;
		case FLOAT3:
			size = 3;
			break;
		case FLOAT4:
			size = 4;
			break;
		}
		return size;
	}

	int BufferLayout::numOfDataTypeForAttrib( size_t i ) const
	{
		assertion( i < m_layout.size(), "Layout index: %d is out of bounds", i );
		return numOfDataType( m_layout[i] );
	}

	int BufferLayout::getGLType( int i ) const
	{
		assertion( 0 <= i && i < (int)m_layout.size(), "Bufffer layout index: %d is out of bounds", i );
		int glType = GL_INT;

		switch( m_layout[i] )
		{
		case INT:
		case INT2:
		case INT3:
		case INT4:
			glType = GL_INT;
			break;
		case BYTE:
			glType = GL_BYTE;
			break;
		case FLOAT1:
		case FLOAT2:
		case FLOAT3:
		case FLOAT4:
			glType = GL_FLOAT;
			break;
		}

		return glType;
	}
}