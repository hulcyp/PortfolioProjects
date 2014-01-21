#include "EventSystemInterface.h"

namespace pdh
{	
	EventSystem& getEventSystem()
	{
		static EventSystem eventSystem;
		return eventSystem;
	}
	//-------------------------------------------------------------------
	void fireEvent( const std::string& evtName )
	{
		getEventSystem().fireEvent( evtName );
	}
	//-------------------------------------------------------------------
	void fireEvent( const std::string& evtName, NamedProperties& params )
	{
		getEventSystem().fireEvent( evtName, params );
	}
}