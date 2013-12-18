#pragma once
#include "Buffer.h"

namespace Monky
{	
	class BufferAccessor
	{
	public:
		BufferAccessor( Buffer* buffer )
			:	m_buffer( buffer )
		{}
		virtual ~BufferAccessor()
		{
			m_buffer = nullptr;
		}

		template< typename T>
		T& get( int attribIndex, int i ) { return *(T*)( m_buffer->getBufferLayout()->attributeOffset( attribIndex ) + m_buffer->at( i ) ); }

		Buffer* getBuffer() { return m_buffer; }
		
	protected:
		Buffer* m_buffer;
	};
}