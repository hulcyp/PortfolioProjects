#include "HashBufferJob.h"
#include <EventSystemInterface.h>

namespace Monky
{
	HashBufferJob::HashBufferJob( const char* buffer, int size, const std::string& eventToFire, float priority )
		:	Job( eventToFire, priority )
		,	m_buffer( buffer )
		,	m_size( size )
		,	m_hash( 0 )
	{}
	//------------------------------------------------------------------
	void HashBufferJob::execute()
	{
		for( int b = 0; b < m_size; ++b )
		{
			m_hash &= 0x07FFFFFF;
			m_hash *= 31;
			m_hash += m_buffer[b];
		}
	}
	//------------------------------------------------------------------
	void HashBufferJob::fireCompletedEvent()
	{
		NamedProperties params;
		params.set( "hash", m_hash );
		fireEvent( m_eventToFire, params );
	}
}