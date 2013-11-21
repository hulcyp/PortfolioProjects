#pragma once
#include "EventSubscriberBase.h"

namespace Monky
{
	template< typename T >
	class EventSubscriber : public EventSubscriberBase
	{
	public:
		typedef void( T::*MemFuncPtr )( NamedProperties& );

		EventSubscriber( T* obj, MemFuncPtr func )
			:	m_object( obj )
			,	m_memberFunc( func )
		{}

		virtual void execute( NamedProperties& params ) const
		{
			(m_object->*m_memberFunc )( params );
		}

		virtual const void* getObjPtr() const { return reinterpret_cast< void* >( m_object ); }
		const MemFuncPtr getObjFuncPtr() const { return m_memberFunc; }

	private:
		T* m_object;
		MemFuncPtr m_memberFunc;
	};
}