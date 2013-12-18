#include "CustomDSPImpl.h"
#include "StdLibraryIncludes.h"

namespace Monky
{
	CustomDSPImpl::CustomDSPImpl( const char* dspName )
	{
		std::memset( &m_dspDesc, 0, sizeof( FMOD_DSP_DESCRIPTION ) );
		std::strcpy( m_dspDesc.name, dspName );
		m_dspDesc.channels = 0;
		m_dspDesc.create = dspCreate;
		m_dspDesc.release = dspRelease;
		m_dspDesc.reset = dspReset;
		m_dspDesc.read = dspRead;
		m_dspDesc.userdata = (void*)this;
	}
	//----------------------------------------------------------------------
	CustomDSPImpl::~CustomDSPImpl()
	{}
	//----------------------------------------------------------------------
	FMOD_RESULT CustomDSPImpl::create( FMOD_DSP_STATE* dsp_state )
	{
		return FMOD_OK;
	}
	//----------------------------------------------------------------------
	FMOD_RESULT CustomDSPImpl::release( FMOD_DSP_STATE* dsp_state )
	{
		return FMOD_OK;
	}
	//----------------------------------------------------------------------
	FMOD_RESULT CustomDSPImpl::reset( FMOD_DSP_STATE* dsp_state )
	{
		return FMOD_OK;
	}
	//----------------------------------------------------------------------
	FMOD_RESULT CustomDSPImpl::read( FMOD_DSP_STATE* dsp_state, float* inBuffer, float* outBuffer, unsigned int length, int inChannels, int outChannels )
	{
		for( int bufferCount = 0; bufferCount < (int)length; ++bufferCount )
		{
			for( int channelCount = 0; channelCount < outChannels; ++channelCount )
			{
				outBuffer[ (bufferCount * outChannels ) + channelCount ] = inBuffer[ (bufferCount * inChannels ) + channelCount ];
			}
		}
		return FMOD_OK;
	}
	//----------------------------------------------------------------------
	// Static functions to call into the specific object
	//----------------------------------------------------------------------
	FMOD_RESULT F_CALLBACK dspCreate( FMOD_DSP_STATE* dsp_state )
	{
		CustomDSPImpl* customDSP = nullptr;

		FMOD::DSP* thisDSP = (FMOD::DSP*)dsp_state->instance;

		thisDSP->getUserData( (void**)&customDSP );
		
		return customDSP->create( dsp_state );
	}
	//----------------------------------------------------------------------
	FMOD_RESULT F_CALLBACK dspRelease( FMOD_DSP_STATE* dsp_state )
	{
		CustomDSPImpl* customDSP = nullptr;

		FMOD::DSP* thisDSP = (FMOD::DSP*)dsp_state->instance;

		thisDSP->getUserData( (void**)&customDSP );		

		return customDSP->release( dsp_state );
	}
	//----------------------------------------------------------------------
	FMOD_RESULT F_CALLBACK dspReset( FMOD_DSP_STATE* dsp_state )
	{
		CustomDSPImpl* customDSP = nullptr;

		FMOD::DSP* thisDSP = (FMOD::DSP*)dsp_state->instance;

		thisDSP->getUserData( (void**)&customDSP );		

		return customDSP->reset( dsp_state );
	}
	//----------------------------------------------------------------------
	FMOD_RESULT F_CALLBACK dspRead( FMOD_DSP_STATE * dsp_state, float * inBuffer, float* outBuffer, unsigned int length, int inChannels, int outChannels )
	{
		CustomDSPImpl* customDSP = nullptr;
		
		FMOD::DSP* thisDSP = (FMOD::DSP*)dsp_state->instance;

		thisDSP->getUserData( (void**)&customDSP );
		

		return customDSP->read( dsp_state, inBuffer, outBuffer, length, inChannels, outChannels );
	}
}