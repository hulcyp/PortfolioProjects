#include "SoundCueImpl.h"
#include "AudioSystem.h"
#include "ASErrorHandling.h"
#include <MeshFactory.h>
#include <EventSystemInterface.h>
#include <Camera.h>
#include <Material.h>
#include "DSPImpl.h"
#include "Mesh.h"

namespace Monky
{
	SoundCueImpl::SoundCueImpl( const std::string& soundName )
		:	m_sound( nullptr )
		,	m_channel( nullptr )
		,	m_isLooping( false )
		,	m_isPaused( false )
		,	m_attenuationMinMaxDist( 0.5f, 10.0f )
		,	m_innerConeMesh( nullptr )
		,	m_outerConeMesh( nullptr )
		,	m_orientation( mat3f::IDENTITY )
		,	m_volume( 1.0f ) 
	{
		m_sound = Sound::getSound( soundName );
		init();
	}
	//----------------------------------------------
	SoundCueImpl::SoundCueImpl( const std::string& filePath, const vec3f& pos, const vec3f& vel, 
			const vec2f& attenuationFallOff, bool isLooping, bool startPaused )
		:	m_sound( nullptr )
		,	m_channel( nullptr )
		,	m_isLooping( isLooping )
		,	m_isPaused( startPaused )
		,	m_attenuationMinMaxDist( attenuationFallOff )
		,	m_innerConeMesh( nullptr )
		,	m_outerConeMesh( nullptr )
		,	m_position( pos )
		,	m_velocity( vel )
		,	m_orientation( mat3f::IDENTITY )
		,	m_volume( 1.0f ) 
	{
		m_sound = Sound::getSound( filePath );
		init();
	}
	//----------------------------------------------
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
	//----------------------------------------------
	SoundCueImpl::~SoundCueImpl()
	{		
		stop();
		SAFE_DELETE( m_iconMesh );
		SAFE_DELETE( m_innerSphere );
		VECTOR_CLEANUP( m_dspEffects );
	}
	//----------------------------------------------
	void SoundCueImpl::renderDebugInfo( Camera* camera )
	{
		NamedProperties params;
		params.set( "mesh", m_iconMesh );
		matStackf modelStack;
		modelStack.translate( -m_iconWidth*0.5f, -m_iconWidth*0.5f, 0.0f );
		modelStack.applyMatrix( mat3f::lookAt( camera->getPosition(), m_position ) );
		modelStack.translate( m_position );
		params.set( "modelMatrix", modelStack.top() );
		params.set( "camera", camera );
		fireEvent( "renderMesh", params );

		float distFactorFromCenterOfIcon = 0.9f;
		
		//Sound name of the sound that is playing
		modelStack.loadIdentity();
		modelStack.applyMatrix( mat3f::lookAt( camera->getPosition(), m_position ) );
		modelStack.translate( m_position );
		modelStack.translate( 0.0f, m_iconWidth*distFactorFromCenterOfIcon, 0.0f );
		m_fontParams.set( "modelMatrix", modelStack.top() );
		m_fontParams.set( "camera", camera );
		m_fontParams.set( "text", m_sound->getSoundName() );
		fireEvent( "renderText", m_fontParams );

		params.set( "enableBackFaceCulling", false );

		//Volume bar	
		distFactorFromCenterOfIcon = 0.7f;
		modelStack.loadIdentity();
		modelStack.translate( -m_widthOfVolumeBar * 0.5f, -m_heightOfVolumeBar*0.5f, 0.0f );
		modelStack.applyMatrix( mat3f::lookAt( camera->getPosition(), m_position ) );
		modelStack.translate( m_position );
		modelStack.translate( 0.0f, m_iconWidth*distFactorFromCenterOfIcon, 0.0f );
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", m_volumeBGBar );
		fireEvent( "renderMesh", params );

		modelStack.loadIdentity();
		modelStack.scale( m_volume, 1.0f, 1.0f );
		modelStack.translate( -m_widthOfVolumeBar * 0.5f, -m_heightOfVolumeBar*0.5f, 0.01f );
		modelStack.applyMatrix( mat3f::lookAt( camera->getPosition(), m_position ) );
		modelStack.translate( m_position );
		modelStack.translate( 0.0f, m_iconWidth*distFactorFromCenterOfIcon, 0.0f );
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", m_volumeBar );
		fireEvent( "renderMesh", params );

		//Audibility bar
		distFactorFromCenterOfIcon = 0.6f;
		modelStack.loadIdentity();
		modelStack.translate( -m_widthOfVolumeBar * 0.5f, -m_heightOfVolumeBar*0.5f, 0.0f );
		modelStack.applyMatrix( mat3f::lookAt( camera->getPosition(), m_position ) );
		modelStack.translate( m_position );
		modelStack.translate( 0.0f, m_iconWidth*distFactorFromCenterOfIcon, 0.0f );
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", m_volumeBGBar );
		fireEvent( "renderMesh", params );

		float audibility = 0.0f;
		if( m_channel )
			m_channel->getAudibility( &audibility );

		modelStack.loadIdentity();
		modelStack.scale( audibility, 1.0f, 1.0f );
		modelStack.translate( -m_widthOfVolumeBar * 0.5f, -m_heightOfVolumeBar*0.5f, 0.01f );
		modelStack.applyMatrix( mat3f::lookAt( camera->getPosition(), m_position ) );
		modelStack.translate( m_position );
		modelStack.translate( 0.0f, m_iconWidth*distFactorFromCenterOfIcon, 0.0f );
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", m_volumeBar );
		fireEvent( "renderMesh", params );

		
		
		createOrientationGDO( 1.0f, m_position, m_orientation );
		params.set( "wireFrame", true );

		//This means that this is a cone sound cue
		if( m_innerConeMesh )
		{
			modelStack.loadIdentity();
			modelStack.applyMatrix( m_orientation );
			modelStack.translate( m_position );
			params.set( "modelMatrix", modelStack.top() );
			params.set( "mesh", m_innerConeMesh );
			fireEvent( "renderMesh", params );
			params.set( "mesh", m_outerConeMesh );
			fireEvent( "renderMesh", params );
		}
		else
		{
			modelStack.loadIdentity();
			modelStack.translate( m_position );
			params.set( "modelMatrix", modelStack.top() );
			params.set( "mesh", m_innerSphere );
			fireEvent( "renderMesh", params );
		}

	}
	//----------------------------------------------
	void SoundCueImpl::setLooping( bool looping )
	{
		m_isLooping = looping;
		if( looping && m_channel )
			m_channel->setMode( FMOD_LOOP_NORMAL );
		else if( m_channel )
			m_channel->setMode( FMOD_LOOP_OFF );
	}
	//----------------------------------------------
	bool SoundCueImpl::getLooping() const
	{
		return m_isLooping;
	}
	//----------------------------------------------
	void SoundCueImpl::pause()
	{
		m_isPaused = true;
		if( m_channel )
			m_channel->setPaused( m_isPaused );
	}
	//----------------------------------------------
	void SoundCueImpl::unpause()
	{
		m_isPaused = false;
		if( m_channel )
		{
			m_channel->setPaused( m_isPaused );
			m_channel->setVolume( m_volume );
		}
	}
	//----------------------------------------------
	void SoundCueImpl::stop()
	{
		if( m_channel )
		{
			m_channel->stop();
			m_channel = nullptr;
		}
	}
	//----------------------------------------------
	bool SoundCueImpl::isPaused() const 
	{ 
		return m_isPaused;
	}
	//----------------------------------------------
	bool SoundCueImpl::isPlaying() const
	{
		bool isPlaying = false;
		if( m_channel )
		{
			FMOD_RESULT result = m_channel->isPlaying( &isPlaying );
			asErrorCheck( result );
		}
		return isPlaying;
	}
	//----------------------------------------------
	void SoundCueImpl::applyDSPs()
	{
		if( m_channel )
		{
			for( int i = (int)m_dspEffects.size() - 1; i >= 0 ; --i )
			{
				m_channel->addDSP( m_dspEffects[i]->m_dsp, 0 );
			}
		}
	}
	//----------------------------------------------
	void SoundCueImpl::pushDSP( DSPImpl* dsp )
	{
		m_dspEffects.push_back( dsp );
		if( m_channel )
		{
			m_channel->addDSP( dsp->m_dsp, 0 );
		}
	}
	//----------------------------------------------
	void SoundCueImpl::popDSP()
	{
		m_dspEffects.pop_back();
	}
	//----------------------------------------------
	void SoundCueImpl::set3DAttributes( const vec3f& pos, const vec3f& vel )
	{
		FMOD_VECTOR fpos = { pos.x, pos.y, pos.z };
		FMOD_VECTOR fvel = { vel.x, vel.y, vel.z };
		if( m_channel )
			m_channel->set3DAttributes( &fpos, &fvel );
		m_position = pos;
		m_velocity = vel;
	}
	//----------------------------------------------
	void SoundCueImpl::set3DConeSettings( float insideAngleDegrees, float outsideConeAngleDegrees, float outsideVolume )
	{
		if( m_channel )
		{
			m_channel->set3DConeSettings( insideAngleDegrees, outsideConeAngleDegrees, outsideVolume );
			FMOD_VECTOR orientation[] = { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } };
			m_channel->set3DConeOrientation( orientation );
			
			SAFE_DELETE( m_innerConeMesh );
			SAFE_DELETE( m_outerConeMesh );
			m_innerConeMesh = MeshFactory::generateCone( m_attenuationMinMaxDist.x, MathFuncs<float>::degreesToRadians( insideAngleDegrees * 0.5f ), 20, "SimpleColorMaterial", color::YELLOW );
			m_outerConeMesh = MeshFactory::generateCone( m_attenuationMinMaxDist.x, MathFuncs<float>::degreesToRadians( outsideConeAngleDegrees * 0.5f ), 20, "SimpleColorMaterial", color::YELLOW );
		}
	}
	//----------------------------------------------
	void SoundCueImpl::set3DConeOrientation( const vec3f& forward, const vec3f& up, const vec3f& right )
	{
		m_orientation = mat3f( forward, up, right );
		if( m_channel )
		{
			FMOD_VECTOR orientation[] = { { forward.x, forward.y, forward.z }, { up.x, up.y, up.z }, { right.x, right.y, right.z } };
			m_channel->set3DConeOrientation( orientation );
		}
	}
	//----------------------------------------------
	void SoundCueImpl::set3DConeOrientation( const mat3f& orientation )
	{
		m_orientation = orientation;
		if( m_channel )
		{
			FMOD_VECTOR orient[] = { { orientation[0], orientation[1], orientation[2] },
									 { orientation[3], orientation[4], orientation[5] },
									 { orientation[6], orientation[7], orientation[8] } };
			m_channel->set3DConeOrientation( orient );
		}
	}
	//----------------------------------------------
	void SoundCueImpl::setVolume( float volume )
	{
		m_volume = MathFuncs<float>::clamp( volume, 0.0f, 1.0f );
		if( m_channel )
			m_channel->setVolume( m_volume );
	}
	//----------------------------------------------
	float SoundCueImpl::getVolume() const
	{
		return m_volume;
	}
	//----------------------------------------------
	void SoundCueImpl::setFrequency( float freq )
	{
		m_channel->setFrequency( freq );
	}
	//----------------------------------------------
	float SoundCueImpl::getFrequency() const
	{
		float freq;
		m_channel->getFrequency( &freq );
		return freq;
	}
}