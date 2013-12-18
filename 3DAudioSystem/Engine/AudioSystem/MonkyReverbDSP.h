#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 6/27/2013
//	Description:
//
//---------------------------------------------
#include "CustomDSP.h"

namespace Monky
{
	class MonkyReverbDSP : public CustomDSP
	{
	public:
		MonkyReverbDSP();
		virtual ~MonkyReverbDSP();

		virtual void initialize();
	};
}