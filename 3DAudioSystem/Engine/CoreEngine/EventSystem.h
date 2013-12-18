#pragma once
#include "EventSubscriber.h"
#include "StdLibraryIncludes.h"
#include "NamedProperties.h"

namespace Monky
{
	typedef std::function< void( NamedProperties& ) > eventFunc;
	class EventSystem
	{
	public:
		EventSystem();
		~EventSystem();

		bool fireEvent( const std::string& evtName );
		bool fireEvent( const std::string& evtName, NamedProperties& params );

		template< typename TObj, typename TMemFunc >
		void registerObjForEvent( const std::string& eventName, TObj& obj, TMemFunc func );
		
		template< typename TObj, typename TMemFunc >
		void unregisterObjForEvent( const std::string& eventName, TObj& obj, TMemFunc func );

		template< typename TObj >
		void unregisterObjForAll( TObj& obj );

		template< typename TObj >
		void unregisterObjForAll( const std::string& eventName, TObj& obj );	

		void registerFuncForEvent( const std::string& eventName, eventFunc func );
		void unregisterFuncForEvent( const std::string& eventName, eventFunc func );
		
	private:
		std::map< std::string, std::vector< EventSubscriberBase* > > m_subscribers;
		std::map< std::string, std::vector< eventFunc > > m_standaloneFuncSubscribers;
	};

	template< typename TObj, typename TMemFunc >
	void EventSystem::registerObjForEvent( const std::string& eventName, TObj& obj, TMemFunc func )
	{
		EventSubscriberBase* newSub = new EventSubscriber< TObj >( &obj, func );
		m_subscribers[ eventName ].push_back( newSub );
	}
	//---------------------------------------------------------------------------------------
	template< typename TObj, typename TMemFunc >
	void EventSystem::unregisterObjForEvent( const std::string& eventName, TObj& obj, TMemFunc func )
	{
		auto subscribersForEvt = m_subscribers.find( eventName );
		if( subscribersForEvt != m_subscribers.end() )
		{			
			for( size_t i = 0; i < subscribersForEvt->second.size(); ++i )
			{
				if( subscribersForEvt->second[i]->getObjPtr() == &obj )
				{
					if( func == reinterpret_cast< EventSubscriber< TObj >* >( subscribersForEvt->second[i] )->getObjFuncPtr() )
					{
						size_t size = subscribersForEvt->second.size();
						EventSubscriberBase* tempToDelete = subscribersForEvt->second[i];
						subscribersForEvt->second[i] = subscribersForEvt->second[ size - 1 ];
						subscribersForEvt->second[ size - 1 ] = tempToDelete;
						SAFE_DELETE( tempToDelete );
						subscribersForEvt->second.pop_back();						
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------
	template< typename TObj >
	void EventSystem::unregisterObjForAll( TObj& obj )
	{		
		for( auto subscribersForEvt = m_subscribers.begin(); subscribersForEvt != m_subscribers.end(); ++subscribersForEvt )
		{
			if( subscribersForEvt != m_subscribers.end() )
			{			
				for( size_t i = 0; i < subscribersForEvt->second.size(); ++i )
				{
					if( subscribersForEvt->second[i]->getObjPtr() == &obj )
					{
						size_t size = subscribersForEvt->second.size();
						EventSubscriberBase* tempToDelete = subscribersForEvt->second[i];
						subscribersForEvt->second[i] = subscribersForEvt->second[ size - 1 ];
						subscribersForEvt->second[ size - 1 ] = tempToDelete;
						SAFE_DELETE( tempToDelete );
						subscribersForEvt->second.pop_back();
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------
	template< typename TObj >
	void EventSystem::unregisterObjForAll( const std::string& eventName, TObj& obj )
	{
		auto subscribersForEvt = m_subscribers.find( eventName );
		if( subscribersForEvt != m_subscribers.end() )
		{			
			for( size_t i = 0; i < subscribersForEvt->second.size(); ++i )
			{
				if( subscribersForEvt->second[i]->getObjPtr() == &obj )
				{
					size_t size = subscribersForEvt->second.size();
					EventSubscriberBase* tempToDelete = subscribersForEvt->second[i];
					subscribersForEvt->second[i] = subscribersForEvt->second[ size - 1 ];
					subscribersForEvt->second[ size - 1 ] = tempToDelete;
					SAFE_DELETE( tempToDelete );
					subscribersForEvt->second.pop_back();
				}
			}
		}
	}
}