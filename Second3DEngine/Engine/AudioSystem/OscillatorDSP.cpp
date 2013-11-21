#include "OscillatorDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	OscillatorDSP::OscillatorDSP()
		:	DSP( MONKY_DSP_TYPE_OSCILLATOR )
	{}
	//-----------------------------------------------------
	OscillatorDSP::OscillatorDSP( XMLParser& parser, XMLNode* oscillatorNode )
		:	DSP( MONKY_DSP_TYPE_OSCILLATOR )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( oscillatorNode, "", "type,oscillatorType,freq" );
		std::string typeStr = parser.getXMLAttributeAsString( oscillatorNode, "oscillatorType", "SINE" );
		float freq = parser.getXMLAttributeAsFloat( oscillatorNode, "freq", NO_VALUE_SPECIFIED );

		setType( getOscillatorType( typeStr ) );
		if( freq != NO_VALUE_SPECIFIED )
			setFrequency( freq );
	}
	//-----------------------------------------------------
	void OscillatorDSP::setType( OscillatorType type )
	{
		m_dspImpl->setParameter( FMOD_DSP_OSCILLATOR_TYPE, (float)type ); 
	}
	//-----------------------------------------------------
	OscillatorDSP::OscillatorType OscillatorDSP::getType() const
	{
		return (OscillatorDSP::OscillatorType) static_cast<int>( m_dspImpl->getParameter( FMOD_DSP_OSCILLATOR_TYPE ) );
	}
	//-----------------------------------------------------
	void OscillatorDSP::setFrequency( float freq )
	{
		m_dspImpl->setParameter( FMOD_DSP_OSCILLATOR_RATE, freq );
	}
	//-----------------------------------------------------
	float OscillatorDSP::getFrequency() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_OSCILLATOR_RATE );
	}

	OscillatorDSP::OscillatorType OscillatorDSP::getOscillatorType( const std::string& type ) const
	{
		static std::map< std::string, OscillatorType > oscillatorTypeMap;
		if( oscillatorTypeMap.empty() )
		{
			oscillatorTypeMap[ "SINE" ] = SINE;
			oscillatorTypeMap[ "SQUARE" ] = SQUARE;
			oscillatorTypeMap[ "SAWUP" ] = SAWUP;
			oscillatorTypeMap[ "SAWDOWN" ] = SAWDOWN;
			oscillatorTypeMap[ "TRIANGLE" ] = TRIANGLE;
			oscillatorTypeMap[ "NOISE" ] = NOISE;
		}

		std::string upperStrType = type;
		stringToUpperCase( upperStrType );
		auto iter = oscillatorTypeMap.find( upperStrType );
		if( iter != oscillatorTypeMap.end() )
			return iter->second;
		else
			return SINE;
	}
}