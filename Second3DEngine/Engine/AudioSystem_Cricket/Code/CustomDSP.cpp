#include "CustomDSP.h"
#include "AudioSystem.h"

namespace Monky
{
	Monky::CustomDSP::CustomDSP( const char* dspName )
		:	DSP( MONKY_DSP_TYPE_CUSTOM )
		,	m_dspName( dspName )
	{}
	//----------------------------------------------------------------------
	Monky::CustomDSP::~CustomDSP()
	{}
	//----------------------------------------------------------------------
	void Monky::CustomDSP::initialize()
	{
	}
}


