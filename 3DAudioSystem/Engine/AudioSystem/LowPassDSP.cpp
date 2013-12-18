#include "LowPassDSP.h"
#include "DSPImpl.h"

namespace Monky
{	
	LowPassDSP::LowPassDSP()
		:	DSP( MONKY_DSP_TYPE_LOWPASS )
	{}
	//---------------------------------------------
	LowPassDSP::LowPassDSP( XMLParser& parser, XMLNode* hpNode )
		:	DSP( MONKY_DSP_TYPE_LOWPASS )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( hpNode, "", "type,cutoff,resonance" );
		float cutoff = parser.getXMLAttributeAsFloat( hpNode, "cutoff", NO_VALUE_SPECIFIED );
		float resonance = parser.getXMLAttributeAsFloat( hpNode, "resonance", NO_VALUE_SPECIFIED );

		if( cutoff != NO_VALUE_SPECIFIED )
			setCutoff( cutoff );
		if( resonance != NO_VALUE_SPECIFIED )
			setResonance( resonance );
	}
	//-------------------------------------------------
	void LowPassDSP::setCutoff( float freq )
	{
		m_dspImpl->setParameter( FMOD_DSP_LOWPASS_CUTOFF, freq );
	}
	//-------------------------------------------------
	float LowPassDSP::getCutoff() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_LOWPASS_CUTOFF );
	}
	//-------------------------------------------------
	void LowPassDSP::setResonance( float resonance )
	{
		m_dspImpl->setParameter( FMOD_DSP_LOWPASS_RESONANCE, resonance );
	}
	//-------------------------------------------------
	float LowPassDSP::getResonance() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_LOWPASS_CUTOFF );
	}
}