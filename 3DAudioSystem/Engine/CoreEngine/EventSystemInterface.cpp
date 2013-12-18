#include "EventSystemInterface.h"

namespace Monky
{	
	EventSystem& getEventSystem()
	{
		static EventSystem eventSystem;
		return eventSystem;
	}
	//-------------------------------------------------------------------
	bool fireEvent( const std::string& evtName )
	{
		return getEventSystem().fireEvent( evtName );
	}
	//-------------------------------------------------------------------
	bool fireEvent( const std::string& evtName, NamedProperties& params )
	{
		return getEventSystem().fireEvent( evtName, params );
	}
	//-------------------------------------------------------------------
	void registerForEvent( const std::string& evtName, eventFunc func )
	{
		getEventSystem().registerFuncForEvent( evtName, func );
	}
	//-------------------------------------------------------------------
	void unregisterForEvent( const std::string& evtName, eventFunc func )
	{
		getEventSystem().unregisterFuncForEvent( evtName, func );
	}
}