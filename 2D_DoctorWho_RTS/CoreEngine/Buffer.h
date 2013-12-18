#pragma once

namespace pdh
{
	class BufferLayout;

	class Buffer
	{
	public:
		Buffer( const BufferLayout* layout );
		Buffer( const BufferLayout* layout, size_t count );
		Buffer( char* data, size_t count, const BufferLayout* layout );
		virtual ~Buffer();

		void setData( char* data, size_t count );
		char* rawData();
		size_t size();
		size_t count();
		size_t stride();
		char* at( size_t i );
		const BufferLayout* getBufferLayout();

	protected:
		char* m_buffer;
		size_t m_size;
		const BufferLayout* m_bufferLayout;
	};
}