#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 9/9/2013
//	Description:
//
//---------------------------------------------

namespace Monky
{
	class Packet
	{
	public:
		Packet( int packetSize );
		~Packet();

		char* GetBuffer() const;
		int GetBufferSize() const;
		void ClearData();
		bool AddBufferOfData( const char* buffer, int size );
		bool GetBufferOfData( char* buffer, int size );

		template< typename T >
		bool AddData( const T& data )
		{
			bool succeeded = false;
			int sizeOfData = sizeof( T );

			if( m_nextAddrToAddData + sizeOfData < m_buffer + m_bufferSize )
			{
				*reinterpret_cast<T*>( m_nextAddrToAddData ) = data;
				m_nextAddrToAddData += sizeof( T );
				succeeded = true;
			}			
			return succeeded;
		}
		//----------------------------------------------------------------------
		template< typename T >
		bool GetNextChunkOfData( T& outData )
		{
			bool succeeded = false;
			int sizeOfData = sizeof( T );

			if( m_nextAddrToAddData + sizeOfData <= m_buffer + m_bufferSize )
			{
				outData = *reinterpret_cast< T* >( m_nextAddrToAddData );
				m_nextAddrToAddData += sizeOfData;
				succeeded = true;
			}
			return succeeded;
		}

	private:
		char* m_buffer;
		char* m_nextAddrToAddData;
		int m_bufferSize;
	};
}