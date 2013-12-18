#pragma once
#include "NamedProperties.h"

namespace pdh
{
	class EventSubscriberBase
	{
	public:
		virtual void execute( NamedProperties& params ) const = 0;

		virtual const void* getObjPtr() const = 0;
	};
}