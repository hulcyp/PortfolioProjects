#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 6/27/2013
//	Description:
//
//---------------------------------------------
#include <fmod.hpp>
#include "DSPImpl.h"

namespace Monky
{
	class AudioSystemImpl;

	class CustomDSPImpl : public DSPImpl
	{
	public:
		friend AudioSystemImpl;
		
		CustomDSPImpl( const char* dspName );
		~CustomDSPImpl();

		virtual FMOD_RESULT create( FMOD_DSP_STATE* dsp_state );
		virtual FMOD_RESULT release( FMOD_DSP_STATE* dsp_state );
		virtual FMOD_RESULT reset( FMOD_DSP_STATE* dsp_state );
		virtual FMOD_RESULT read( FMOD_DSP_STATE* dsp_state, float* inBuffer, float* outBuffer, unsigned int length, int inChannels, int outChannels );

	private:
		FMOD_DSP_DESCRIPTION m_dspDesc;
	};

	FMOD_RESULT F_CALLBACK dspCreate( FMOD_DSP_STATE* dsp_state );
	FMOD_RESULT F_CALLBACK dspRelease( FMOD_DSP_STATE* dsp_state );
	FMOD_RESULT F_CALLBACK dspReset( FMOD_DSP_STATE* dsp_state );
	FMOD_RESULT F_CALLBACK dspRead( FMOD_DSP_STATE * dsp_state, float * inBuffer, float* outBuffer, unsigned int length, int inChannels, int outChannels );
	
}