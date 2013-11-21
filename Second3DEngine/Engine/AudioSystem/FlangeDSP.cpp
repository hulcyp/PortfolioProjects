#include "FlangeDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	FlangeDSP::FlangeDSP()
		:	DSP( MONKY_DSP_TYPE_FLANGE )
	{}
	//-----------------------------------------------------------------
	FlangeDSP::FlangeDSP( XMLParser& parser, XMLNode* flangeNode )
		:	DSP( MONKY_DSP_TYPE_FLANGE )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( flangeNode, "", "type,dryMixVolume,wetMixVolume,depth,rate" );
		float dryMix = parser.getXMLAttributeAsFloat( flangeNode, "dryMixVolume", NO_VALUE_SPECIFIED );
		float wetMix = parser.getXMLAttributeAsFloat( flangeNode, "wetMixVolume", NO_VALUE_SPECIFIED );
		float depth = parser.getXMLAttributeAsFloat( flangeNode, "depth", NO_VALUE_SPECIFIED );
		float rate = parser.getXMLAttributeAsFloat( flangeNode, "rate", NO_VALUE_SPECIFIED );

		if( dryMix != NO_VALUE_SPECIFIED )
			setDryMixVolume( dryMix );
		if( wetMix != NO_VALUE_SPECIFIED )
			setWetMixVolume( wetMix );
		if( depth != NO_VALUE_SPECIFIED )
			setDepth( depth );
		if( rate != NO_VALUE_SPECIFIED )
			setRate( rate );
	}
	//-----------------------------------------------------------------
	void FlangeDSP::setDryMixVolume( float volume )
	{
		m_dspImpl->setParameter( FMOD_DSP_FLANGE_DRYMIX, volume );
	}
	//-----------------------------------------------------------------
	float FlangeDSP::getDryMixVolume() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_FLANGE_WETMIX );
	}
	//-----------------------------------------------------------------
	void FlangeDSP::setWetMixVolume( float volume )
	{
		m_dspImpl->setParameter( FMOD_DSP_FLANGE_WETMIX, volume );
	}
	//-----------------------------------------------------------------
	float FlangeDSP::getWetMixVolume() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_FLANGE_WETMIX );
	}
	//-----------------------------------------------------------------
	void FlangeDSP::setDepth( float depth )
	{
		m_dspImpl->setParameter( FMOD_DSP_FLANGE_DEPTH, depth );
	}
	//-----------------------------------------------------------------
	float FlangeDSP::getDepth() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_FLANGE_DEPTH );
	}
	//-----------------------------------------------------------------
	void FlangeDSP::setRate( float rate )
	{
		m_dspImpl->setParameter( FMOD_DSP_FLANGE_RATE, rate );
	}
	//-----------------------------------------------------------------
	float FlangeDSP::getRate() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_FLANGE_RATE );
	}
}