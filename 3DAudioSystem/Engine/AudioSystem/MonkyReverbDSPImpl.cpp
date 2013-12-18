#include "MonkyReverbDSPImpl.h"
#include "CommonEngineIncludes.h"

namespace Monky
{
	MonkyReverbDSPImpl::MonkyReverbDSPImpl( const char* name )
		:	CustomDSPImpl( name )
	{}
	//----------------------------------------------------------------------
	MonkyReverbDSPImpl::~MonkyReverbDSPImpl()
	{}
	//----------------------------------------------------------------------
	FMOD_RESULT MonkyReverbDSPImpl::read( FMOD_DSP_STATE* dsp_state, float* inBuffer, float* outBuffer, unsigned int length, int inChannels, int outChannels )
	{
		float alpha = 0.9f;
		int delay = 10;
		float decay = 1.0f;
		outBuffer[0] = ( inBuffer[0] );
		for( int bufferCount = 1; bufferCount < (int)length; ++bufferCount )
		{
			for( int channelCount = 0; channelCount < outChannels; ++channelCount )
			{
				int outIndex = bufferCount*outChannels + channelCount + delay;
				outBuffer[ bufferCount*outChannels + channelCount ] = inBuffer[ (bufferCount * inChannels ) + channelCount ];
				//outBuffer[ outIndex ] += inBuffer[ (bufferCount * inChannels ) + channelCount ] * decay;
				//outBuffer[ (bufferCount * outChannels ) + channelCount ] = ( inBuffer[ (bufferCount * inChannels ) + channelCount ] ) * alpha + ( 1.0f - alpha ) * outBuffer[ (bufferCount * outChannels ) + channelCount - 1 ];
			}
		}
		return FMOD_OK;
	}
}