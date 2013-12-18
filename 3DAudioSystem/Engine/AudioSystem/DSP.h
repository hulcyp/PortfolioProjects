#pragma once
#include <string>
#include <XMLParser.h>

namespace Monky
{
	enum Monky_DSP_Type
	{
		MONKY_DSP_TYPE_UNKNOWN,
		MONKY_DSP_TYPE_MIXER,
		MONKY_DSP_TYPE_OSCILLATOR,
		MONKY_DSP_TYPE_LOWPASS,
		MONKY_DSP_TYPE_ITLOWPASS,
		MONKY_DSP_TYPE_HIGHPASS,
		MONKY_DSP_TYPE_ECHO,
		MONKY_DSP_TYPE_FLANGE,
		MONKY_DSP_TYPE_DISTORTION,
		MONKY_DSP_TYPE_NORMALIZE,
		MONKY_DSP_TYPE_PARAMEQ,
		MONKY_DSP_TYPE_PITCHSHIFT,
		MONKY_DSP_TYPE_CHORUS,
		MONKY_DSP_TYPE_VSTPLUGIN,
		MONKY_DSP_TYPE_ITECHO,
		MONKY_DSP_TYPE_COMPRESSOR,
		MONKY_DSP_TYPE_SFXREVERB,
		MONKY_DSP_TYPE_LOWPASS_SIMPLE,
		MONKY_DSP_TYPE_DELAY,
		MONKY_DSP_TYPE_TREMOLO,
		MONKY_DSP_TYPE_LADSPAPLUGIN,
		MONKY_DSP_TYPE_HIGHPASS_SIMPLE,
		MONKY_DSP_TYPE_CUSTOM,
		MONKY_DSP_TYPE_FORCEINT = 65536		/* Makes sure this enum is signed 32bit. */
	};

	class DSPImpl;
	class AudioSystem;
	class SoundCue;

	class DSP
	{
	public:
		friend AudioSystem;
		friend SoundCue;

		DSP( Monky_DSP_Type dspType );
		virtual ~DSP();

		virtual void initialize();

		static Monky_DSP_Type getDSPTypeFromString( const std::string& stringAsType );
		static DSP* createDSPFromXMLNodeBasedOnType( Monky_DSP_Type type, XMLParser& parser, XMLNode* node );

	protected:
		DSPImpl* m_dspImpl;
		Monky_DSP_Type m_dspType;
	};
}