#include "Packet.h"
#include "CommonEngineIncludes.h"

namespace Monky
{
	Packet::Packet( int packetSize )
		:	m_bufferSize( packetSize )
		,	m_buffer( nullptr )
		,	m_nextAddrToAddData( nullptr )
	{
		if( m_bufferSize > 0 )
		{
			m_buffer = new char[ packetSize ];
			m_nextAddrToAddData = m_buffer;
		}
	}
	//----------------------------------------------------------------------
	Packet::~Packet()
	{
		SAFE_DELETE( m_buffer );
		m_nextAddrToAddData = nullptr;
		m_bufferSize = 0;
	}
	//----------------------------------------------------------------------
	char* Packet::GetBuffer() const
	{
		return m_buffer;
	}
	//----------------------------------------------------------------------
	int Packet::GetBufferSize() const
	{
		return m_bufferSize;
	}
	//----------------------------------------------------------------------
	void Packet::ClearData()
	{
		m_nextAddrToAddData = m_buffer;
	}
	//----------------------------------------------------------------------
	bool Packet::AddBufferOfData( const char* buffer, int size )
	{
		bool succeeded = false;

		if( m_nextAddrToAddData + size < m_buffer + m_bufferSize )
		{
			std::memcpy( m_nextAddrToAddData, buffer, size );
			succeeded = true;
			m_nextAddrToAddData += size;
		}
		return succeeded;
	}
	//----------------------------------------------------------------------
	bool Packet::GetBufferOfData( char* buffer, int size )
	{
		bool succeeded = false;

		if( m_nextAddrToAddData + size < m_buffer + m_bufferSize )
		{
			std::memcpy( buffer, m_nextAddrToAddData, size );
			succeeded = true;
			m_nextAddrToAddData += size;
		}
		return succeeded;
	}
}