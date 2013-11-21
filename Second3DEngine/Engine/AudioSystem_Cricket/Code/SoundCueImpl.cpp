#include "SoundCueImpl.h"
#include "MeshFactory.h"
#include "Sound.h"
#include "SoundImpl.h"
#include "Mesh.h"

namespace Monky
{
	SoundCueImpl::SoundCueImpl( const std::string& soundName )
	{
		m_sound = Sound::getSound( soundName );
		init();
	}
	//----------------------------------------------------------------------
	SoundCueImpl::SoundCueImpl( const std::string& soundName, const vec3f& pos, const vec3f& vel, 
		const vec2f& attenuationFallOff, bool isLooping, bool startPaused  )
		:	m_position( pos )
		,	m_velocity( vel )
		,	m_attenuationMinMaxDist( attenuationFallOff )
		,	m_isLooping( isLooping )
		,	m_isPaused( startPaused )
	{
		m_sound = Sound::getSound( soundName );
		init();
	}
	//----------------------------------------------------------------------
	SoundCueImpl::~SoundCueImpl()
	{
		stop();
		SAFE_DELETE( m_iconMesh );
		SAFE_DELETE( m_innerSphere );
	}
	//----------------------------------------------------------------------
	void SoundCueImpl::init()
	{
		m_widthOfVolumeBar = 0.75f;
		m_heightOfVolumeBar = 0.05f;
		m_iconWidth = 1.0f;
		m_iconMesh = MeshFactory::generateXYPlane( m_iconWidth, "AudioSymbolMat" );
		m_innerSphere = MeshFactory::generateSphere( m_attenuationMinMaxDist.x, 100, 100, "SoundWaveSphereMat", color::RED );
		m_volumeBar = MeshFactory::generateAAPlaneXY( m_widthOfVolumeBar, m_heightOfVolumeBar, "VolumeBarMat", color::GREEN );
		m_volumeBGBar = MeshFactory::generateAAPlaneXY( m_widthOfVolumeBar, m_heightOfVolumeBar, "VolumeBarMat", color::BLACK );

		m_fontHeight = 0.5f;
		m_fontParams.set( "fontName", std::string( "mainFont_72" ) );
		m_fontParams.set( "fontHeight", m_fontHeight );
		m_fontParams.set( "color", color::WHITE );
		//This makes transformation relative to bottom center of text
		m_fontParams.set( "posRel", vec2f( -1.0f, 2.0f ) );
		m_fontParams.set( "shadow", false );
	}
	//----------------------------------------------------------------------
	void SoundCueImpl::renderDebugInfo( Camera* camera )
	{}
	//----------------------------------------------------------------------
	void SoundCueImpl::setLooping( bool looping )
	{
		if( looping )
		{
			m_sound->m_soundImpl->m_sound->setLoopCount( -1 );
		}
		else
			m_sound->m_soundImpl->m_sound->setLoopCount( 0 );
		m_isLooping = looping;
	}
	//----------------------------------------------------------------------
	bool SoundCueImpl::getLooping() const
	{
		return m_isLooping;
	}
	//----------------------------------------------------------------------
	void SoundCueImpl::pause()
	{
		m_sound->m_soundImpl->m_sound->setPaused( true );
		m_isPaused = true;
	}
	//----------------------------------------------------------------------
	void SoundCueImpl::unpause()
	{
		m_sound->m_soundImpl->m_sound->setPaused( false );
		m_isPaused = false;
	}
	//----------------------------------------------------------------------
	void SoundCueImpl::stop()
	{
		m_sound->m_soundImpl->m_sound->stop();
	}
	//----------------------------------------------------------------------
	bool SoundCueImpl::isPaused() const
	{
		return m_isPaused;
	}
	//----------------------------------------------------------------------
	bool SoundCueImpl::isPlaying() const
	{
		return m_sound->m_soundImpl->m_sound->isPlaying();
	}
	//----------------------------------------------------------------------
	void SoundCueImpl::set3DAttributes( const vec3f& pos, const vec3f& vel )
	{
		m_sound->m_soundImpl->m_sound->set3dPosition( pos.x, pos.y, pos.z );
		m_sound->m_soundImpl->m_sound->set3dVelocity( vel.x, vel.y, vel.z );
	}
	//----------------------------------------------------------------------
	void SoundCueImpl::set3DConeSettings( float insideAngleDegrees, float outsideConeAngleDegrees, float outsideVolume )
	{}
	//----------------------------------------------------------------------
	void SoundCueImpl::set3DConeOrientation( const vec3f& forward, const vec3f& up, const vec3f& right )
	{}
	void SoundCueImpl::set3DConeOrientation( const mat3f& orientation )
	{}
	//----------------------------------------------------------------------
	void SoundCueImpl::setVolume( float volume )
	{
		m_sound->m_soundImpl->m_sound->setVolume( volume );
		m_volume = volume;
	}
	float SoundCueImpl::getVolume() const
	{
		return m_sound->m_soundImpl->m_sound->getVolume();
	}

	void SoundCueImpl::setFrequency( float freq )
	{
		
	}
	float SoundCueImpl::getFrequency() const
	{
		return 0.0f;
	}
}