#include "LoadFileJob.h"
#include <fstream>
#include <EventSystemInterface.h>
#include <ConsoleInterface.h>
#include <FileManager.h>

namespace Monky
{
	LoadFileJob::LoadFileJob( const std::string& filePath, const std::string& eventToFire, float priority )
		:	Job( eventToFire, priority )
		,	m_filePath( filePath )
	{}
	//---------------------------------------------------------------------
	void LoadFileJob::execute()
	{
		Monky::enterCriticalSection( g_fileManagerCS );
		m_buffer = getFileManager().readDataFromFile( m_filePath, m_bufferSize );
		Monky::enterCriticalSection( g_fileManagerCS );

		if( m_bufferSize == -1 )
		{
			consolePrintColorf( "Failed to load file: %s", color::RED, m_filePath.c_str() );
			return;
		}
	}
	//---------------------------------------------------------------------
	void LoadFileJob::fireCompletedEvent()
	{		
		NamedProperties params;
		params.set( "byteBuffer", m_buffer );
		params.set( "bufferSizes", m_bufferSize );
		fireEvent( m_eventToFire, params );
	}
}