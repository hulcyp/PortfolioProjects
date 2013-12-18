#include "DSP.h"
#include "AudioSystem.h"
#include "DSPImpl.h"
#include "ChorusDSP.h"
#include "CompressorDSP.h"
#include "DelayDSP.h"
#include "DistortionDSP.h"
#include "EchoDSP.h"
#include "FlangeDSP.h"
#include "HighPassDSP.h"
#include "LowPassDSP.h"
#include "NormalizeDSP.h"
#include "OscillatorDSP.h"
#include "ParameqDSP.h"
#include "PitchShiftDSP.h"
#include "SFXReverbDSP.h"
#include "TremoloDSP.h"

namespace Monky
{
	DSP::DSP( Monky_DSP_Type dspType )
		:	m_dspImpl( nullptr )
		,	m_dspType( dspType )
	{}
	//----------------------------------------------------------------------------------
	DSP::~DSP()
	{
		SAFE_DELETE( m_dspImpl );
	}
	//----------------------------------------------------------------------------------
	void DSP::initialize()
	{
		if( !m_dspImpl )
		{
			m_dspImpl = new DSPImpl();
			AudioSystem::getInstance()->createDSP( m_dspType, *this );
		}
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
		DSP* dsp = nullptr;
		switch( type )
		{
		case MONKY_DSP_TYPE_UNKNOWN:
		case MONKY_DSP_TYPE_MIXER:
		case MONKY_DSP_TYPE_ITLOWPASS:
		case MONKY_DSP_TYPE_VSTPLUGIN:
		case MONKY_DSP_TYPE_ITECHO:
		case MONKY_DSP_TYPE_LOWPASS_SIMPLE:
		case MONKY_DSP_TYPE_LADSPAPLUGIN:
		case MONKY_DSP_TYPE_HIGHPASS_SIMPLE:
			dsp = new DSP( type );
			break;
		case MONKY_DSP_TYPE_OSCILLATOR:
			dsp = new OscillatorDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_LOWPASS:
			dsp = new LowPassDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_HIGHPASS:
			dsp = new HighPassDSP( parser, node );
			break;
		case MONKY_DSP_TYPE_ECHO:
			dsp = new EchoDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_FLANGE:
			dsp = new FlangeDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_DISTORTION:
			dsp = new DistortionDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_NORMALIZE:
			dsp = new NormalizeDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_PARAMEQ:
			dsp = new ParameqDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_PITCHSHIFT:
			dsp = new PitchShiftDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_CHORUS:
			dsp = new ChorusDSP( parser, node );
			break;			
		case MONKY_DSP_TYPE_COMPRESSOR:
			dsp = new CompressorDSP( parser, node );
			break;
		case MONKY_DSP_TYPE_SFXREVERB:
			dsp = new SFXReverbDSP( parser, node );
			break;
		case MONKY_DSP_TYPE_DELAY:
			dsp = new DelayDSP( parser, node );
			break;
		case MONKY_DSP_TYPE_TREMOLO:
			dsp = new TremoloDSP( parser, node );
			break;
		}
		if( dsp )
			dsp->initialize();
		return dsp;
	}
}