#pragma once
#include <glew.h>
#include <Gl/GL.h>
#include "Buffer.h"


namespace Monky
{
	class GLBuffer : public Buffer
	{
	public:
		GLBuffer( char* data, size_t count, const BufferLayout* layout, int bufferType, int geoType = GL_TRIANGLES, int drawBufferType = GL_STATIC_DRAW );
		GLBuffer( const BufferLayout* layout, int bufferType, int geoType = GL_TRIANGLES, int drawBufferType = GL_STATIC_DRAW );
		virtual ~GLBuffer();

		int getGeometryType() const;
		int getDrawBufferType() const;
		const unsigned int getBufferID() const;

		void sendToOpenGL();
		void enableAttributeArrays() const;
		void disableAttributeArrays() const;
		void bind();
		void unbind();

	protected:
		int m_geometryType;
		int m_drawBufferType;
		unsigned int m_bufferID;
		int m_bufferType;
	};
}