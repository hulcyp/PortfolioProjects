#include "ChorusDSP.h"
#include "DSPImpl.h"
#include <MathFuncs.h>

namespace Monky
{
	ChorusDSP::ChorusDSP()
		:	DSP( MONKY_DSP_TYPE_CHORUS )
	{}
	//--------------------------------------------------
	ChorusDSP::ChorusDSP( XMLParser& parser, XMLNode* chorusNode )
		:	DSP( MONKY_DSP_TYPE_CHORUS )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLChildElements( chorusNode, "", "WetVolume" );
		parser.validateXMLAttributes( chorusNode, "", "type,dryVolume,delayMS,modulationRate,modulationDepth" );
		float dryVolume = parser.getXMLAttributeAsFloat( chorusNode, "dryVolume", NO_VALUE_SPECIFIED );
		float delay = parser.getXMLAttributeAsFloat( chorusNode, "delayMS", NO_VALUE_SPECIFIED );
		float modulationRate = parser.getXMLAttributeAsFloat( chorusNode, "modulationRate", NO_VALUE_SPECIFIED );
		float modulationDepth = parser.getXMLAttributeAsFloat( chorusNode, "modulationDepth", NO_VALUE_SPECIFIED );
				
		for( const XMLNode* wetNode = chorusNode->FirstChildElement( "WetVolume" ); wetNode != nullptr; wetNode = wetNode->NextSiblingElement( "WetVolume" ) )
		{
			parser.validateXMLAttributes( wetNode, "channel,volume", "" );
			int channel = parser.getXMLAttributeAsInt( wetNode, "channel", -1 );
			float volume = parser.getXMLAttributeAsFloat( wetNode, "volume", 0.0f );
			if( channel > 0 )
				setWetMixVolume( channel, volume );
		}

		if( dryVolume != NO_VALUE_SPECIFIED )
			setDryMixVolume( dryVolume );
		if( delay != NO_VALUE_SPECIFIED )
			setDelayMS( delay );
		if( modulationRate != NO_VALUE_SPECIFIED )
			setModulationRate( modulationRate );
		if( modulationDepth != NO_VALUE_SPECIFIED )
			setModulationDepth( modulationDepth );
	}
	//--------------------------------------------------
	void ChorusDSP::setDryMixVolume( float volume )
	{
		m_dspImpl->setParameter( FMOD_DSP_CHORUS_DRYMIX, volume );
	}
	//--------------------------------------------------
	float ChorusDSP::getDryMixVolume() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_CHORUS_DRYMIX );
	}
	//--------------------------------------------------
	void ChorusDSP::setWetMixVolume( int chorusTap, float volume )
	{
		int clampedChorusTap = MathFuncs<int>::clamp( chorusTap, 0, 2 );
		m_dspImpl->setParameter( FMOD_DSP_CHORUS_WETMIX1 + clampedChorusTap, volume );
	}
	//--------------------------------------------------
	float ChorusDSP::getWetMixVolume( int chorusTap ) const
	{
		int clampedChorusTap = MathFuncs<int>::clamp( chorusTap, 0, 2 );
		return m_dspImpl->getParameter( FMOD_DSP_CHORUS_WETMIX1 + clampedChorusTap );
	}
	//--------------------------------------------------
	void ChorusDSP::setDelayMS( float delayMS )
	{
		m_dspImpl->setParameter( FMOD_DSP_CHORUS_DELAY, delayMS );
	}
	//--------------------------------------------------
	float ChorusDSP::getDelayMS() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_CHORUS_DELAY );
	}
	//--------------------------------------------------
	void ChorusDSP::setModulationRate( float modRate )
	{
		m_dspImpl->setParameter( FMOD_DSP_CHORUS_RATE, modRate );
	}
	//--------------------------------------------------
	float ChorusDSP::getModulationRate() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_CHORUS_RATE );
	}
	//--------------------------------------------------
	void ChorusDSP::setModulationDepth( float modDepth )
	{
		m_dspImpl->setParameter( FMOD_DSP_CHORUS_DEPTH, modDepth );
	}
	//--------------------------------------------------
	float ChorusDSP::getModulationDepth() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_CHORUS_DEPTH );
	}
}