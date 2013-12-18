#pragma once
#include<vector>

namespace pdh
{
	class BufferLayout
	{
	public:
		enum DataType
		{
			INT = 0,
			INT2,
			INT3,
			INT4,
			BYTE,
			FLOAT1,
			FLOAT2,
			FLOAT3,
			FLOAT4
		};

		BufferLayout();
		BufferLayout( int numAttribs, ... );

		void pushAttribute( DataType dt );
		size_t numAttributes() const;
		size_t stride() const;
		int dataTypeSize( DataType dt ) const;
		int attributeSize( size_t i ) const;
		int attributeOffset( size_t index ) const;
		int numOfDataType( DataType dt ) const;
		int numOfDataTypeForAttrib( size_t i ) const;

		int getGLType( int i ) const;

	private:
		size_t m_stride;
		std::vector< DataType > m_layout;
	};
}