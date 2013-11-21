#pragma once
#include <fmod.hpp>
#include <Singleton.h>
#include <CommonTypeDefines.h>
#include "DSP.h"

namespace Monky
{
	class SoundCueImpl;
	class SoundImpl;
	class DSPImpl;
	class ASGeometryImpl;
	class CustomDSPImpl;
	class ReverbAreaImpl;

	class AudioSystemImpl
	{
	public:
		friend ASGeometryImpl;

		AudioSystemImpl();
		~AudioSystemImpl();

		void update();

		void setListenerAttributes( const vec3f& pos, const vec3f& vel, const vec3f& forward, const vec3f& up );

		void createSound( const std::string& filepath, SoundImpl* sound, FMOD_MODE mode );
		void playSoundCue( SoundCueImpl* soundCueImpl );
		void playSound( SoundImpl* soundImpl, float volume = 1.0f ) const;
		void playSound( SoundImpl* soundImpl, const vec3f& pos, float volume = 1.0f ) const;

		void createDSP( Monky_DSP_Type type, DSPImpl* dsp );
		void createCustomDSP( CustomDSPImpl* dsp );

		void createReverbArea( ReverbAreaImpl* reverbImpl );

	protected:
		FMOD_DSP_TYPE getFmodDSPTypeMapping( Monky_DSP_Type type ) const;

	private:
		FMOD::System* m_system;
		const float DISTANCE_FACTOR;
		char* m_soundBuffer;
		int m_soundBufferSize;
	};
}