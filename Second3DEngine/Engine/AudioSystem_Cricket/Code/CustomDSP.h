#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 6/27/2013
//	Description:
//
//---------------------------------------------
#include "DSP.h"

namespace Monky
{
	class CustomDSPImpl;

	class CustomDSP : public DSP
	{
	public:
		CustomDSP( const char* dspName );
		virtual ~CustomDSP();

		virtual void initialize();

	protected:
		const char* m_dspName;
	};
}