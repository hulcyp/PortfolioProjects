#include "DSP.h"
#include "AudioSystem.h"

namespace Monky
{
	DSP::DSP( Monky_DSP_Type dspType )
		:	m_dspImpl( nullptr )
		,	m_dspType( dspType )
	{}
	//----------------------------------------------------------------------------------
	DSP::~DSP()
	{
	}
	//----------------------------------------------------------------------------------
	void DSP::initialize()
	{
	}
	//----------------------------------------------------------------------------------
	Monky_DSP_Type DSP::getDSPTypeFromString( const std::string& stringAsType )
	{
		static std::map< std::string, Monky_DSP_Type > dspTypes;
		if( dspTypes.empty() )
		{
			dspTypes[ "MONKY_DSP_TYPE_UNKNOWN" ]			= MONKY_DSP_TYPE_UNKNOWN;
			dspTypes[ "MONKY_DSP_TYPE_MIXER" ]				= MONKY_DSP_TYPE_MIXER;
			dspTypes[ "MONKY_DSP_TYPE_OSCILLATOR" ]			= MONKY_DSP_TYPE_OSCILLATOR;
			dspTypes[ "MONKY_DSP_TYPE_LOWPASS" ]			= MONKY_DSP_TYPE_LOWPASS;
			dspTypes[ "MONKY_DSP_TYPE_ITLOWPASS" ]			= MONKY_DSP_TYPE_ITLOWPASS;
			dspTypes[ "MONKY_DSP_TYPE_HIGHPASS" ]			= MONKY_DSP_TYPE_HIGHPASS;
			dspTypes[ "MONKY_DSP_TYPE_ECHO" ]				= MONKY_DSP_TYPE_ECHO;
			dspTypes[ "MONKY_DSP_TYPE_FLANGE" ]				= MONKY_DSP_TYPE_FLANGE;
			dspTypes[ "MONKY_DSP_TYPE_DISTORTION" ]			= MONKY_DSP_TYPE_DISTORTION;
			dspTypes[ "MONKY_DSP_TYPE_NORMALIZE" ]			= MONKY_DSP_TYPE_NORMALIZE;
			dspTypes[ "MONKY_DSP_TYPE_PARAMEQ" ]			= MONKY_DSP_TYPE_PARAMEQ;
			dspTypes[ "MONKY_DSP_TYPE_PITCHSHIFT" ]			= MONKY_DSP_TYPE_PITCHSHIFT;
			dspTypes[ "MONKY_DSP_TYPE_CHORUS" ]				= MONKY_DSP_TYPE_CHORUS;
			dspTypes[ "MONKY_DSP_TYPE_VSTPLUGIN" ]			= MONKY_DSP_TYPE_VSTPLUGIN;
			dspTypes[ "MONKY_DSP_TYPE_ITECHO" ]				= MONKY_DSP_TYPE_ITECHO;
			dspTypes[ "MONKY_DSP_TYPE_COMPRESSOR" ]			= MONKY_DSP_TYPE_COMPRESSOR;
			dspTypes[ "MONKY_DSP_TYPE_SFXREVERB" ]			= MONKY_DSP_TYPE_SFXREVERB;
			dspTypes[ "MONKY_DSP_TYPE_LOWPASS_SIMPLE" ]		= MONKY_DSP_TYPE_LOWPASS_SIMPLE;
			dspTypes[ "MONKY_DSP_TYPE_DELAY" ]				= MONKY_DSP_TYPE_DELAY;
			dspTypes[ "MONKY_DSP_TYPE_TREMOLO" ]			= MONKY_DSP_TYPE_TREMOLO;
			dspTypes[ "MONKY_DSP_TYPE_LADSPAPLUGIN" ]		= MONKY_DSP_TYPE_LADSPAPLUGIN;
			dspTypes[ "MONKY_DSP_TYPE_HIGHPASS_SIMPLE" ]	= MONKY_DSP_TYPE_HIGHPASS_SIMPLE;
			dspTypes[ "MONKY_DSP_CUSTOM" ]					= MONKY_DSP_TYPE_CUSTOM;
		}

		std::string upperAsType = stringAsType;
		stringToUpperCase( upperAsType );
		auto iter = dspTypes.find( upperAsType );
		if( iter != dspTypes.end() )
			return dspTypes[ upperAsType ];
		else 
			return MONKY_DSP_TYPE_FORCEINT;
	}
	//----------------------------------------------------------------------------------
	DSP* DSP::createDSPFromXMLNodeBasedOnType( Monky_DSP_Type type, XMLParser& parser, XMLNode* node )
	{
		return nullptr;
	}
}