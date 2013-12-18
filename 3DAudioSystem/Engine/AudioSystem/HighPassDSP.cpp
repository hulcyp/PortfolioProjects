#include "HighPassDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	HighPassDSP::HighPassDSP()
		:	DSP( MONKY_DSP_TYPE_HIGHPASS )
	{}
	//---------------------------------------------
	HighPassDSP::HighPassDSP( XMLParser& parser, XMLNode* hpNode )
		:	DSP( MONKY_DSP_TYPE_HIGHPASS )
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
	//---------------------------------------------
	void HighPassDSP::setCutoff( float cutoff )
	{
		m_dspImpl->setParameter( FMOD_DSP_HIGHPASS_CUTOFF, cutoff );
	}
	//---------------------------------------------
	float HighPassDSP::getCutoff() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_HIGHPASS_CUTOFF );
	}
	//---------------------------------------------
	void HighPassDSP::setResonance( float resonance )
	{
		m_dspImpl->setParameter( FMOD_DSP_HIGHPASS_RESONANCE, resonance );
	}
	//---------------------------------------------
	float HighPassDSP::getResonance() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_HIGHPASS_RESONANCE );
	}
}