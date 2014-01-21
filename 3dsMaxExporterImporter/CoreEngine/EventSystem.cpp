#include "EventSystem.h"

namespace pdh
{
	EventSystem::EventSystem()
	{}
	//-----------------------------------------------------------------
	void EventSystem::fireEvent( const std::string& evtName )
	{
		auto subscribersToEvt = m_subscribers.find( evtName );
		if( subscribersToEvt != m_subscribers.end() )
		{
			NamedProperties params;
			for( size_t i = 0; i < subscribersToEvt->second.size(); ++i )
			{
				subscribersToEvt->second[i]->execute( params );
			}
		}
	}
	//-----------------------------------------------------------------
	void EventSystem::fireEvent( const std::string& evtName, NamedProperties& params )
	{
		auto subscribersToEvt = m_subscribers.find( evtName );
		if( subscribersToEvt != m_subscribers.end() )
		{
			for( size_t i = 0; i < subscribersToEvt->second.size(); ++i )
			{
				subscribersToEvt->second[i]->execute( params );
			}
		}
	}	
}