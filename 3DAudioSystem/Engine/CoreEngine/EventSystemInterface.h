#pragma once
#include "EventSystem.h"

namespace Monky
{
	EventSystem& getEventSystem();
	bool fireEvent( const std::string& evtName );
	bool fireEvent( const std::string& evtName, NamedProperties& params );
	

	template< typename TObj, typename TMemFunc >
	void registerForEvent( const std::string& evtName, TObj& obj, TMemFunc func );

	template< typename TObj, typename TMemFunc >
	void unregisterForEvent( const std::string& evtName, TObj& obj, TMemFunc func );

	template< typename TObj >
	void unregisterForEvent( const std::string& evtName, TObj& obj );

	template< typename TObj >
	void unregisterForEvent( TObj& obj );

	void registerForEvent( const std::string& evtName, eventFunc func );
	void unregisterForEvent( const std::string& evtName, eventFunc func );

	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	template< typename TObj, typename TMemFunc >
	void registerForEvent( const std::string& evtName, TObj& obj, TMemFunc func )
	{
		getEventSystem().registerObjForEvent( evtName, obj, func );
	}
	//--------------------------------------------------------------------------
	template< typename TObj, typename TMemFunc >
	void unregisterForEvent( const std::string& evtName, TObj& obj, TMemFunc func )
	{
		getEventSystem().unregisterObjForEvent( evtName, obj, func );
	}
	//--------------------------------------------------------------------------
	template< typename TObj >
	void unregisterForEvent( const std::string& evtName, TObj& obj )
	{
		getEventSystem().unregisterObjForAll( evtName, obj );
	}
	//--------------------------------------------------------------------------
	template< typename TObj >
	void unregisterForEvent( TObj& obj )
	{
		getEventSystem().unregisterObjForAll( obj );
	}
}
