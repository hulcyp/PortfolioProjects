#include "ParameqDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	ParameqDSP::ParameqDSP()
		:	DSP( MONKY_DSP_TYPE_PARAMEQ )
	{}
	//------------------------------------------------------
	ParameqDSP::ParameqDSP( XMLParser& parser, XMLNode* parameqNode )
		:	DSP( MONKY_DSP_TYPE_PARAMEQ )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( parameqNode, "", "type,freqCenter,bandwidth,freqGain" );
		float freqCenter = parser.getXMLAttributeAsFloat( parameqNode, "freqCenter", NO_VALUE_SPECIFIED );
		float bandwidth = parser.getXMLAttributeAsFloat( parameqNode, "bandwidth", NO_VALUE_SPECIFIED );
		float freqGain = parser.getXMLAttributeAsFloat( parameqNode, "freqGain", NO_VALUE_SPECIFIED );

		if( freqCenter != NO_VALUE_SPECIFIED )
			setFreqCenter( freqCenter );
		if( bandwidth != NO_VALUE_SPECIFIED )
			setBandwidth( bandwidth );
		if( freqGain != NO_VALUE_SPECIFIED )
			setFreqGain( freqGain );
	}
	//------------------------------------------------------
	void ParameqDSP::setFreqCenter( float freqCenter )
	{
		m_dspImpl->setParameter( FMOD_DSP_PARAMEQ_CENTER, freqCenter );
	}
	//------------------------------------------------------
	float ParameqDSP::getFreqCenter() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_PARAMEQ_CENTER );
	}
	//------------------------------------------------------
	void ParameqDSP::setBandwidth( float bandwidth )
	{
		m_dspImpl->setParameter( FMOD_DSP_PARAMEQ_BANDWIDTH, bandwidth );
	}
	//------------------------------------------------------
	float ParameqDSP::getBandwidth() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_PARAMEQ_BANDWIDTH );
	}
	//------------------------------------------------------
	void ParameqDSP::setFreqGain( float freqGain )
	{
		m_dspImpl->setParameter( FMOD_DSP_PARAMEQ_GAIN, freqGain );
	}
	//------------------------------------------------------
	float ParameqDSP::getFreqGain() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_PARAMEQ_GAIN );
	}
}