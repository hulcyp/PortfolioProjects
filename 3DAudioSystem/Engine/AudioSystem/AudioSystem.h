#pragma once
#include <vector>
#include <Singleton.h>
#include <CommonTypeDefines.h>
#include "SoundCue.h"
#include "Sound.h"
#include "DSP.h"
#include "CustomDSP.h"
#include "ReverbArea.h"

namespace Monky
{
	class AudioSystemImpl;
	class ASGeometryImpl;

	class AudioSystem : public Singleton< AudioSystem >
	{
	public:
		friend ASGeometryImpl;

		AudioSystem();
		~AudioSystem();

		void update();

		void setListenerAttributes( const vec3f& pos, const vec3f& vel, const vec3f& forward, const vec3f& up );
		
		void createSound( const std::string& filepath, Sound& sound, Sound::SoundMode mode ) const;
		void playSoundCue( const SoundCue& soundCue ) const;
		void playSound( const std::string& sound, float volume = 1.0f ) const;
		/* Specifying the position only works if the sound  is loaded as a 3D sound with MONKY_3D */
		void playSound( const std::string& sound, const vec3f& pos, float volume = 1.0f ) const;

		void createDSP( Monky_DSP_Type type, DSP& dsp );
		void createCustomDSP( CustomDSP& dsp );

		void createReverbArea( ReverbArea& reverbArea );

	private:
		AudioSystemImpl* m_asImpl;
	};
}