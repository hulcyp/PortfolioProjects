#pragma once
#include "NamedProperties.h"

namespace Monky
{
	class EventSubscriberBase
	{
	public:
		virtual void execute( NamedProperties& params ) const = 0;

		virtual const void* getObjPtr() const = 0;
	};
}