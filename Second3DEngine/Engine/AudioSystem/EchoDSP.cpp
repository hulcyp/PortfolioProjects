#include "EchoDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	EchoDSP::EchoDSP()
		:	DSP( MONKY_DSP_TYPE_ECHO )
	{}
	//---------------------------------------
	EchoDSP::EchoDSP( XMLParser& parser, XMLNode* echoNode )
		:	DSP( MONKY_DSP_TYPE_ECHO )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( echoNode, "", "type,delayMS,decayRatio,maxNumChannels,dryMixVolume,wetMixVolume" );
		int delayMS = parser.getXMLAttributeAsInt( echoNode, "delayMS", (int)NO_VALUE_SPECIFIED );
		float decayRatio = parser.getXMLAttributeAsFloat( echoNode, "decayRatio", NO_VALUE_SPECIFIED );
		int maxChannels = parser.getXMLAttributeAsInt( echoNode, "maxNumChannels", (int)NO_VALUE_SPECIFIED );
		float dryMixVol = parser.getXMLAttributeAsFloat( echoNode, "dryMixVolume", NO_VALUE_SPECIFIED );
		float wetMixVol = parser.getXMLAttributeAsFloat( echoNode, "wetMixVolume", NO_VALUE_SPECIFIED );

		if( delayMS != (int)NO_VALUE_SPECIFIED )
			setDelayMS( delayMS );
		if( decayRatio != NO_VALUE_SPECIFIED )
			setDecayRatio( decayRatio );
		if( maxChannels != (int)NO_VALUE_SPECIFIED )
			setMaxNumChannels( maxChannels );
		if( dryMixVol != NO_VALUE_SPECIFIED )
			setDryMixVolume( dryMixVol );
		if( wetMixVol != NO_VALUE_SPECIFIED )
			setWetMixVolume( wetMixVol );
	}
	//---------------------------------------
	void EchoDSP::setDelayMS( int delayMS )
	{
		m_dspImpl->setParameter( FMOD_DSP_ECHO_DELAY, (float)delayMS );
	}
	//---------------------------------------
	int EchoDSP::getDelayMS() const
	{
		return (int)m_dspImpl->getParameter( FMOD_DSP_ECHO_DELAY );
	}
	//---------------------------------------
	void EchoDSP::setDecayRatio( float decay )
	{
		m_dspImpl->setParameter( FMOD_DSP_ECHO_DECAYRATIO, decay );
	}
	//---------------------------------------
	float EchoDSP::getDecayRatio() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_ECHO_DECAYRATIO );
	}
	//---------------------------------------
	void EchoDSP::setMaxNumChannels( int maxChannels )
	{
		m_dspImpl->setParameter( FMOD_DSP_ECHO_MAXCHANNELS, (float)maxChannels );
	}
	//---------------------------------------
	int EchoDSP::getMaxNumChannels() const
	{
		return (int)m_dspImpl->getParameter( FMOD_DSP_ECHO_MAXCHANNELS );
	}
	//---------------------------------------
	void EchoDSP::setDryMixVolume( float volume )
	{
		m_dspImpl->setParameter( FMOD_DSP_ECHO_DRYMIX, volume );
	}
	//---------------------------------------
	float EchoDSP::getDryMixVolume() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_ECHO_DRYMIX );
	}
	//---------------------------------------
	void EchoDSP::setWetMixVolume( float volume )
	{
		m_dspImpl->setParameter( FMOD_DSP_ECHO_WETMIX, volume );
	}
	//---------------------------------------
	float EchoDSP::getWetMixVolume() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_ECHO_WETMIX );
	}
}