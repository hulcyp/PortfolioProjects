#include "GLBuffer.h"
#include "CommonEngineIncludes.h"
#include "BufferLayout.h"
#include "glHelper.h"

namespace Monky
{
	GLBuffer::GLBuffer( char* data, size_t count, const BufferLayout* layout, int bufferType, int geoType, int drawBufferType )
		:	Buffer( data, count, layout )
		,	m_geometryType( geoType )
		,	m_drawBufferType( drawBufferType )
		,	m_bufferType( bufferType )
	{}

	GLBuffer::GLBuffer( const BufferLayout* layout, int bufferType, int geoType, int drawBufferType )
		:	Buffer( layout )
		,	m_geometryType( geoType )
		,	m_drawBufferType( drawBufferType )
		,	m_bufferType( bufferType )
	{}

	GLBuffer::~GLBuffer()
	{
		if( m_bufferID )
		{
			glDeleteBuffers( 1, &m_bufferID );
			m_bufferID = 0;
		}
	}

	int GLBuffer::getGeometryType() const
	{
		return m_geometryType;
	}

	int GLBuffer::getDrawBufferType() const
	{
		return m_drawBufferType;
	}

	const unsigned int GLBuffer::getBufferID() const
	{
		return m_bufferID;
	}

	void GLBuffer::sendToOpenGL()
	{
		glGenBuffers( 1, &m_bufferID );
		glBindBuffer( m_bufferType, m_bufferID );
		glBufferData( m_bufferType, m_size, m_buffer, m_drawBufferType );
		glBindBuffer( m_bufferType, 0 );
		glAssertError;
	}
	
	void GLBuffer::enableAttributeArrays() const
	{
		for( size_t i = 0; i < m_bufferLayout->numAttributes(); ++i )
		{
			glEnableVertexAttribArray( i );
			glVertexAttribPointer( i, m_bufferLayout->numOfDataTypeForAttrib( i ), m_bufferLayout->getGLType( i ), GL_FALSE, m_bufferLayout->stride(),
				reinterpret_cast< GLvoid* >( m_bufferLayout->attributeOffset( i ) ) );
			
		}
	}

	void GLBuffer::disableAttributeArrays() const
	{
		for( size_t i = 0; i < m_bufferLayout->numAttributes(); ++i )
		{
			glDisableVertexAttribArray( i );
		}
	}

	void GLBuffer::bind()
	{
		glBindBuffer( m_bufferType, m_bufferID );
		glAssertError;
	}

	void GLBuffer::unbind()
	{
		glBindBuffer( m_bufferType, 0 );
		glAssertError;
	}
}