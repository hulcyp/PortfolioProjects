#include "AudioSystem.h"
#include <Assertion.h>
#include <cstring>
#include "AudioSystemImpl.h"
#include <CommonEngineIncludes.h>
#include "DSP.h"


namespace Monky
{
	AudioSystem g_audioSystem;

#pragma warning( disable:4355 )
	AudioSystem::AudioSystem()
		:	Singleton( this )
	{
		m_asImpl = new AudioSystemImpl();
	}
#pragma warning( default:4355 )
	//----------------------------------------------------
	AudioSystem::~AudioSystem()
	{
		SAFE_DELETE( m_asImpl );
	}
	//----------------------------------------------------
	void AudioSystem::update()
	{
		m_asImpl->update();
	}
	//----------------------------------------------------
	void AudioSystem::setListenerAttributes( const vec3f& pos, const vec3f& vel, const vec3f& forward, const vec3f& up )
	{
		m_asImpl->setListenerAttributes( pos, vel, forward, up );
	}

	void AudioSystem::createSound( const std::string& filepath, Sound& sound, Sound::SoundMode mode ) const
	{
	}
	//----------------------------------------------------
	void AudioSystem::playSoundCue( const SoundCue& soundCue ) const
	{		
	}
	//----------------------------------------------------
	void AudioSystem::playSound( const std::string& sound, float volume ) const
	{
	}
	//----------------------------------------------------
	void AudioSystem::playSound( const std::string& sound, const vec3f& pos, float volume ) const
	{
	}
	//----------------------------------------------------
	void AudioSystem::createDSP( Monky_DSP_Type type, DSP& dsp )
	{
	}
	//----------------------------------------------------------------------
	void AudioSystem::createCustomDSP( CustomDSP& dsp )
	{
	}
	//----------------------------------------------------------------------
	void AudioSystem::createReverbArea( ReverbArea& reverbArea )
	{
	}
}