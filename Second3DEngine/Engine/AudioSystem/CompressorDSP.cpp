#include "CompressorDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	CompressorDSP::CompressorDSP()
		:	DSP( MONKY_DSP_TYPE_COMPRESSOR )
	{}
	//------------------------------------------------------
	CompressorDSP::CompressorDSP( XMLParser& parser, XMLNode* compressorNode )
		:	DSP( MONKY_DSP_TYPE_COMPRESSOR )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( compressorNode, "", "type,compressorThreshold,gainAttackMS,gainReleaseMS,gainMakeup" );
		float compressorThreshold = parser.getXMLAttributeAsFloat( compressorNode, "compressorThreshold", NO_VALUE_SPECIFIED );
		float gainAttack = parser.getXMLAttributeAsFloat( compressorNode, "gainAttackMS", NO_VALUE_SPECIFIED );
		float gainRelease = parser.getXMLAttributeAsFloat( compressorNode, "gainReleaseMS", NO_VALUE_SPECIFIED );
		float gainMakeup = parser.getXMLAttributeAsFloat( compressorNode, "gainMakeup", NO_VALUE_SPECIFIED );

		if( compressorThreshold != NO_VALUE_SPECIFIED )
			setCompressorThreshold( compressorThreshold );
		if( gainAttack != NO_VALUE_SPECIFIED )
			setGainReductionAttackTimeMS( gainAttack );
		if( gainRelease != NO_VALUE_SPECIFIED )
			setGainReductionReleaseTimeMS( gainRelease );
		if( gainMakeup != NO_VALUE_SPECIFIED )
			setGainMakeup( gainMakeup );
	}
	//------------------------------------------------------
	void CompressorDSP::setCompressorThreshold( float dbLevel )
	{
		m_dspImpl->setParameter( FMOD_DSP_COMPRESSOR_THRESHOLD, dbLevel );
	}
	//------------------------------------------------------
	float CompressorDSP::getCompressorThreshold() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_COMPRESSOR_THRESHOLD );
	}
	//------------------------------------------------------
	void CompressorDSP::setGainReductionAttackTimeMS( float timeMS )
	{
		m_dspImpl->setParameter( FMOD_DSP_COMPRESSOR_ATTACK, timeMS );
	}
	//------------------------------------------------------
	float CompressorDSP::getGainReductionAttackTimeMS() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_COMPRESSOR_ATTACK );
	}
	//------------------------------------------------------
	void CompressorDSP::setGainReductionReleaseTimeMS( float timeMS )
	{
		m_dspImpl->setParameter( FMOD_DSP_COMPRESSOR_RELEASE, timeMS );
	}
	//------------------------------------------------------
	float CompressorDSP::getGainReductionReleaseTimeMS( float timeMS )
	{
		return m_dspImpl->getParameter( FMOD_DSP_COMPRESSOR_RELEASE );
	}
	//------------------------------------------------------
	void CompressorDSP::setGainMakeup( float gainMakeup )
	{
		m_dspImpl->setParameter( FMOD_DSP_COMPRESSOR_GAINMAKEUP, gainMakeup );
	}
	//------------------------------------------------------
	float CompressorDSP::getGainMakeup() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_COMPRESSOR_GAINMAKEUP );
	}
}