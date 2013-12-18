#include "EventSystem.h"
#include "ConsoleInterface.h"

namespace Monky
{
	EventSystem::EventSystem()
	{}
	EventSystem::~EventSystem()
	{
		for( auto iter = m_subscribers.begin(); iter != m_subscribers.end(); ++iter )
		{
			std::vector< EventSubscriberBase* >& subscribers = iter->second;
			VECTOR_CLEANUP( subscribers );
		}
		for( auto iter = m_standaloneFuncSubscribers.begin(); iter != m_standaloneFuncSubscribers.end(); ++iter )
		{
			iter->second.clear();
		}
	}
	//-----------------------------------------------------------------
	bool EventSystem::fireEvent( const std::string& evtName )
	{
		bool evtFired = false;
		NamedProperties params;
		auto subscribersToEvt = m_subscribers.find( evtName );
		if( subscribersToEvt != m_subscribers.end() )
		{
			for( size_t i = 0; i < subscribersToEvt->second.size(); ++i )
			{
				subscribersToEvt->second[i]->execute( params );
			}
			evtFired = true;
		}
		auto funcSubscribersToEvt = m_standaloneFuncSubscribers.find( evtName );
		if( funcSubscribersToEvt != m_standaloneFuncSubscribers.end() )
		{
			for( size_t i = 0; i < funcSubscribersToEvt->second.size(); ++i )
			{
				funcSubscribersToEvt->second[i]( params );
			}
			evtFired = true;
		}
		return evtFired;
	}
	//-----------------------------------------------------------------
	bool EventSystem::fireEvent( const std::string& evtName, NamedProperties& params )
	{
		bool evtFired = false;
		auto subscribersToEvt = m_subscribers.find( evtName );
		if( subscribersToEvt != m_subscribers.end() )
		{
			for( size_t i = 0; i < subscribersToEvt->second.size(); ++i )
			{
				subscribersToEvt->second[i]->execute( params );
			}
			evtFired = true;
		}
		auto funcSubscribersToEvt = m_standaloneFuncSubscribers.find( evtName );
		if( funcSubscribersToEvt != m_standaloneFuncSubscribers.end() )
		{
			for( size_t i = 0; i < funcSubscribersToEvt->second.size(); ++i )
			{
				funcSubscribersToEvt->second[i]( params );
			}
			evtFired = true;
		}
		return evtFired;
	}
	//-----------------------------------------------------------------
	void EventSystem::registerFuncForEvent( const std::string& eventName, eventFunc func )
	{
		m_standaloneFuncSubscribers[ eventName ].push_back( func );
	}
	//-----------------------------------------------------------------
	void EventSystem::unregisterFuncForEvent( const std::string& eventName, eventFunc func )
	{
		auto subscribersForEvent = m_standaloneFuncSubscribers.find( eventName );
		if( subscribersForEvent != m_standaloneFuncSubscribers.end() )
		{
			for( size_t i = 0; i < subscribersForEvent->second.size(); ++i )
			{
				if( &subscribersForEvent->second[i] == &func )
				{
					size_t size = subscribersForEvent->second.size();
					eventFunc tempToDelete = subscribersForEvent->second[i];
					subscribersForEvent->second[i] = subscribersForEvent->second[ size - 1 ];
					subscribersForEvent->second[ size - 1 ] = tempToDelete;
					subscribersForEvent->second.pop_back();
				}
			}
		}
	}
}