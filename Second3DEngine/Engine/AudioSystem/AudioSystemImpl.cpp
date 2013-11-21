#include "AudioSystemImpl.h"
#include <fmod_errors.h>
#include <Assertion.h>
#include <cstring>
#include "ASErrorHandling.h"
#include "SoundCueImpl.h"
#include "SoundImpl.h"
#include "Sound.h"
#include "DSPImpl.h"
#include "FileManager.h"
#include "CommonEngineIncludes.h"
#include "CustomDSPImpl.h"
#include "ReverbAreaImpl.h"

namespace Monky
{
	AudioSystemImpl::AudioSystemImpl()
		:	DISTANCE_FACTOR( 1.0f )
		,	m_soundBuffer( nullptr )
		,	m_soundBufferSize( 0 )
	{
		FMOD_RESULT result;
		unsigned int version;
		int numDrivers;

		FMOD_SPEAKERMODE speakerMode;
		FMOD_CAPS caps;
		char name[256];

		result = FMOD::System_Create( &m_system );
		asErrorCheck( result );

		result = m_system->getVersion( &version );
		asErrorCheck( result );

		assertion( version >= FMOD_VERSION, "Error! You are using an old version of FMOD %08x. This program requires %08x", version, FMOD_VERSION );
		
		result = m_system->getNumDrivers( &numDrivers );
		asErrorCheck( result );

		if( numDrivers == 0 )
		{
			result = m_system->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
			asErrorCheck( result );
		}
		else
		{
			result = m_system->getDriverCaps( 0, &caps, 0, &speakerMode );
			asErrorCheck( result );

			if( caps & FMOD_CAPS_HARDWARE_EMULATED )
			{
				result = m_system->setDSPBufferSize(1024, 10);
				asErrorCheck(result);
			}

			result = m_system->getDriverInfo( 0, name, 256, 0 );
			asErrorCheck(result);

			if( strstr( name, "SigmaTel" ) ) /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
			{
				result = m_system->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR );
				asErrorCheck( result );
			}
		}

		result = m_system->init( 100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED | FMOD_INIT_ENABLE_PROFILE, 0 );
		if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )	/* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
		{
			result = m_system->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
			asErrorCheck( result );

			result = m_system->init( 100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0 );
			asErrorCheck( result );
		}

		// Set distance units
		result = m_system->set3DSettings( 1.0f, DISTANCE_FACTOR, 1.0f );
		asErrorCheck( result );

		FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_OFF;
		result = m_system->setReverbAmbientProperties( &prop );
	}
	//----------------------------------------------------
	AudioSystemImpl::~AudioSystemImpl()
	{
		FMOD_RESULT result;
		result = m_system->close();
		asErrorCheck( result );
		result = m_system->release();
		asErrorCheck( result );
		SAFE_DELETE( m_soundBuffer );
		m_soundBufferSize = 0;
	}
	//----------------------------------------------------
	void AudioSystemImpl::update()
	{
		m_system->update();
	}
	//----------------------------------------------------
	void AudioSystemImpl::setListenerAttributes( const vec3f& pos, const vec3f& vel, const vec3f& forward, const vec3f& up )
	{
		FMOD_VECTOR fpos = { pos.x, pos.y, pos.z };
		FMOD_VECTOR fvel = { vel.x, vel.y, vel.z };
		FMOD_VECTOR fforward = { forward.x, forward.y, forward.z };
		FMOD_VECTOR fup = { up.x, up.y, up.z };

		m_system->set3DListenerAttributes( 0, &fpos, &fvel, &fforward, &fup );
	}
	//----------------------------------------------------
	void AudioSystemImpl::createSound( const std::string& filepath, SoundImpl* sound, FMOD_MODE mode )
	{
		FMOD::Sound* snd;
		
		m_soundBuffer = getFileManager().readDataFromFile( filepath, m_soundBufferSize );
		FMOD_CREATESOUNDEXINFO info = {sizeof(FMOD_CREATESOUNDEXINFO)};
		info.length = m_soundBufferSize;
		
		FMOD_RESULT result = m_system->createSound( m_soundBuffer, mode | FMOD_OPENMEMORY | FMOD_CREATECOMPRESSEDSAMPLE, &info, &snd ); 
		asErrorCheck( result );
		sound->m_sound = snd;
	}
	//----------------------------------------------------
	void AudioSystemImpl::playSoundCue( SoundCueImpl* soundCueImpl )
	{
		FMOD_RESULT result = m_system->playSound( FMOD_CHANNEL_FREE, 
			soundCueImpl->m_sound->m_soundImpl->m_sound, 
			soundCueImpl->m_isPaused, &soundCueImpl->m_channel );
		
		asErrorCheck( result );
		
		FMOD_VECTOR fpos = {	soundCueImpl->m_position.x, 
								soundCueImpl->m_position.y, 
								soundCueImpl->m_position.z };
		FMOD_VECTOR fvel = {	soundCueImpl->m_velocity.x, 
								soundCueImpl->m_velocity.y, 
								soundCueImpl->m_velocity.z };
		
		soundCueImpl->setLooping( soundCueImpl->m_isLooping );
		soundCueImpl->m_channel->set3DAttributes( &fpos, &fvel );

	}
	//----------------------------------------------------
	void AudioSystemImpl::playSound( SoundImpl* soundImpl, float volume ) const
	{
		FMOD::Channel* channel;
		FMOD_RESULT result = m_system->playSound( FMOD_CHANNEL_FREE, soundImpl->m_sound, false, &channel );
		asErrorCheck( result );
		if( channel != nullptr )
			channel->setVolume( volume );
	}
	void AudioSystemImpl::playSound( SoundImpl* soundImpl, const vec3f& pos, float volume ) const
	{
		FMOD::Channel* channel;
		FMOD_RESULT result = m_system->playSound( FMOD_CHANNEL_FREE, soundImpl->m_sound, false, &channel );
		asErrorCheck( result );
		if( channel != nullptr )
		{
			channel->setVolume( volume );
			FMOD_VECTOR fpos = {	pos.x, 
									pos.y, 
									pos.z };

			channel->set3DAttributes( &fpos, nullptr );
		}
	}
	//----------------------------------------------------
	void AudioSystemImpl::createDSP( Monky_DSP_Type type, DSPImpl* dsp )
	{
		dsp->m_type = getFmodDSPTypeMapping( type );
		m_system->createDSPByType( dsp->m_type, &dsp->m_dsp );
	}
	//----------------------------------------------------
	void AudioSystemImpl::createCustomDSP( CustomDSPImpl* dsp )
	{
		FMOD_RESULT result;
		result = m_system->createDSP( &dsp->m_dspDesc, &dsp->m_dsp );
		asErrorCheck( result );
	}
	//----------------------------------------------------
	void AudioSystemImpl::createReverbArea( ReverbAreaImpl* reverbImpl )
	{
		FMOD_RESULT result;
		result = m_system->createReverb( &reverbImpl->m_reverb );
		FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_GENERIC;
		reverbImpl->m_reverb->setProperties( &prop );
		asErrorCheck( result );
	}
	//----------------------------------------------------
	FMOD_DSP_TYPE AudioSystemImpl::getFmodDSPTypeMapping( Monky_DSP_Type type ) const
	{
		return (FMOD_DSP_TYPE)type;
	}
}