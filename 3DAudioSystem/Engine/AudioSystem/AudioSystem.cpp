#include "AudioSystem.h"
#include <Assertion.h>
#include <cstring>
#include "ASErrorHandling.h"
#include "AudioSystemImpl.h"
#include <CommonEngineIncludes.h>
#include "DSP.h"
#include "ASGeometry.h"
#include "DSPImpl.h"
#include "CustomDSPImpl.h"


namespace Monky
{
	AudioSystem g_audioSystem;

#pragma warning( disable:4355 )
	AudioSystem::AudioSystem()
		:	Singleton( this )
	{
		m_asImpl = new AudioSystemImpl();
		registerASGeometryEvents();
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
		switch( mode )
		{
		case Sound::MONKY_3D:
			return m_asImpl->createSound( filepath, sound.m_soundImpl, FMOD_3D );
		case Sound::MONKY_2D:
		default:
			return m_asImpl->createSound( filepath, sound.m_soundImpl, FMOD_2D );
		}
	}
	//----------------------------------------------------
	void AudioSystem::playSoundCue( const SoundCue& soundCue ) const
	{
		m_asImpl->playSoundCue( soundCue.m_soundCueImpl );
		
	}
	//----------------------------------------------------
	void AudioSystem::playSound( const std::string& sound, float volume ) const
	{
		Sound* soundPtr = Sound::getSound( sound );
		if( soundPtr != nullptr )
			m_asImpl->playSound( soundPtr->m_soundImpl, volume );
	}
	//----------------------------------------------------
	void AudioSystem::playSound( const std::string& sound, const vec3f& pos, float volume ) const
	{
		Sound* soundPtr = Sound::getSound( sound );
		if( soundPtr != nullptr )
			m_asImpl->playSound( soundPtr->m_soundImpl, pos );
	}
	//----------------------------------------------------
	void AudioSystem::createDSP( Monky_DSP_Type type, DSP& dsp )
	{
		m_asImpl->createDSP( type, dsp.m_dspImpl );
	}
	//----------------------------------------------------------------------
	void AudioSystem::createCustomDSP( CustomDSP& dsp )
	{
		CustomDSPImpl* dspImpl = (CustomDSPImpl*)( dsp.m_dspImpl );
		m_asImpl->createCustomDSP( dspImpl );
	}
	//----------------------------------------------------------------------
	void AudioSystem::createReverbArea( ReverbArea& reverbArea )
	{
		m_asImpl->createReverbArea( reverbArea.m_impl );
	}
}