#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 6/27/2013
//	Description:
//
//---------------------------------------------
#include "CustomDSPImpl.h"

namespace Monky
{
	class MonkyReverbDSPImpl : public CustomDSPImpl
	{
	public:
		MonkyReverbDSPImpl( const char* name );
		~MonkyReverbDSPImpl();

		virtual FMOD_RESULT read( FMOD_DSP_STATE* dsp_state, float* inBuffer, float* outBuffer, unsigned int length, int inChannels, int outChannels );
	};
}