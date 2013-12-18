#include "NormalizeDSP.h"
#include "DSPImpl.h"

namespace Monky
{	
	NormalizeDSP::NormalizeDSP()
		:	DSP( MONKY_DSP_TYPE_NORMALIZE )
	{}
	//----------------------------------------------------
	NormalizeDSP::NormalizeDSP( XMLParser& parser, XMLNode* normNode )
		:	DSP( MONKY_DSP_TYPE_NORMALIZE )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( normNode, "", "type,fadeTimeMS,threshold,maxAmplification" );
		float fadeTimeMS = parser.getXMLAttributeAsFloat( normNode, "fadeTimeMS", NO_VALUE_SPECIFIED );
		float threshold = parser.getXMLAttributeAsFloat( normNode, "threshold", NO_VALUE_SPECIFIED );
		float maxAmp = parser.getXMLAttributeAsFloat( normNode, "maxAmplification", NO_VALUE_SPECIFIED );

		if( fadeTimeMS != NO_VALUE_SPECIFIED )
			setFadeTimeMS( fadeTimeMS );
		if( threshold != NO_VALUE_SPECIFIED )
			setThreshold( threshold );
		if( maxAmp != NO_VALUE_SPECIFIED )
			setMaxAmp( maxAmp );
	}
	//----------------------------------------------------
	void NormalizeDSP::setFadeTimeMS( float timeMS )
	{
		m_dspImpl->setParameter( FMOD_DSP_NORMALIZE_FADETIME, timeMS );
	}
	//----------------------------------------------------
	float NormalizeDSP::getFadeTimeMS() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_NORMALIZE_FADETIME );
	}
	//----------------------------------------------------
	void NormalizeDSP::setThreshold( float threshold )
	{
		m_dspImpl->setParameter( FMOD_DSP_NORMALIZE_THRESHHOLD, threshold );
	}
	//----------------------------------------------------
	float NormalizeDSP::getThreshold() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_NORMALIZE_THRESHHOLD );
	}
	//----------------------------------------------------
	void NormalizeDSP::setMaxAmp( float maxAmp )
	{
		m_dspImpl->setParameter( FMOD_DSP_NORMALIZE_MAXAMP, maxAmp );
	}
	//----------------------------------------------------
	float NormalizeDSP::getMaxAmp() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_NORMALIZE_MAXAMP );
	}
}