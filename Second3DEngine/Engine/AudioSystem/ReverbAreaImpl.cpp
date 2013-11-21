#include "ReverbAreaImpl.h"
#include "AudioSystem.h"
#include "ASErrorHandling.h"
#include "MeshFactory.h"
#include "Mesh.h"
#include "Camera.h"

namespace Monky
{
	ReverbAreaImpl::ReverbAreaImpl( const std::string& name )
		:	m_reverb( nullptr )
		,	m_minDist( 1.0f )
		,	m_maxDist( 15.0f )
		,	m_name( name )
	{
		m_minSphere = MeshFactory::generateSphere( m_minDist, 100, 100, "SoundWaveSphereMat", color::GREEN );
		m_maxSphere = MeshFactory::generateSphere( m_maxDist, 100, 100, "SoundWaveSphereMat", color::GREEN );

		m_fontParams.set( "fontName", std::string( "mainFont_72" ) );
		m_fontParams.set( "fontHeight", 0.5f );
		m_fontParams.set( "color", color::WHITE );
		m_fontParams.set( "posRel", vec2f( -1.0f, 2.0f ) );
		m_fontParams.set( "shadow", false );
		m_fontParams.set( "text", name );
	}
	//----------------------------------------------------------------------
	ReverbAreaImpl::~ReverbAreaImpl()
	{
		FMOD_RESULT result;
		result = m_reverb->release();
		asErrorCheck( result );
		SAFE_DELETE( m_minSphere );
		SAFE_DELETE( m_maxSphere );
	}
	//----------------------------------------------------------------------
	void ReverbAreaImpl::set3DAttributes( const vec3f& pos, float min, float max )
	{
		FMOD_VECTOR position = { pos.x, pos.y, pos.z };
		FMOD_RESULT result;
		result = m_reverb->set3DAttributes( &position, min, max );
		asErrorCheck( result );

		m_position = pos;
		m_minDist = min;
		m_maxDist = max;

		SAFE_DELETE( m_minSphere );
		SAFE_DELETE( m_maxSphere );
		m_minSphere = MeshFactory::generateSphere( m_minDist, 100, 100, "SoundWaveSphereMat", color::GREEN );
		m_maxSphere = MeshFactory::generateSphere( m_maxDist, 100, 100, "SoundWaveSphereMat", color::GREEN );		
	}
	//----------------------------------------------------------------------
	void ReverbAreaImpl::setPosition( const vec3f& pos )
	{
		FMOD_VECTOR position = { pos.x, pos.y, pos.z };
		FMOD_RESULT result;
		result = m_reverb->set3DAttributes( &position, m_minDist, m_maxDist );
		asErrorCheck( result );

		m_position = pos;
	}
	//----------------------------------------------------------------------
	void ReverbAreaImpl::setMinMaxDist( float min, float max )
	{
		FMOD_RESULT result;
		result = m_reverb->set3DAttributes( nullptr, m_minDist, m_maxDist );
		asErrorCheck( result );
		m_minDist = min;
		m_maxDist = max;

		SAFE_DELETE( m_minSphere );
		SAFE_DELETE( m_maxSphere );
		m_minSphere = MeshFactory::generateSphere( m_minDist, 100, 100, "SoundWaveSphereMat", color::GREEN );
		m_maxSphere = MeshFactory::generateSphere( m_maxDist, 100, 100, "SoundWaveSphereMat", color::GREEN );
	}
	//----------------------------------------------------------------------
	void ReverbAreaImpl::setProperties( const MonkyReverbProperties* reverbProp )
	{
		FMOD_REVERB_PROPERTIES props = convertToFmodReverbProperties( reverbProp );
		m_reverb->setProperties( &props );
	}
	//----------------------------------------------------------------------
	const vec3f& ReverbAreaImpl::getPosition() const
	{
		return m_position;
	}
	//----------------------------------------------------------------------
	void ReverbAreaImpl::getMinMaxDist( float& minOut, float& maxOut ) const
	{
		minOut = m_minDist;
		maxOut = m_maxDist;
	}
	//----------------------------------------------------------------------
	void ReverbAreaImpl::renderDebugInfo( Camera* camera )
	{
		NamedProperties params;
		mat4f model( mat4f::IDENTITY );
		model.translate( m_position );
		params.set( "wireFrame", true );
		params.set( "enableBackFaceCulling", false );
		params.set( "camera", camera );
		params.set( "modelMatrix", model );
		params.set( "mesh", m_minSphere );
		fireEvent( "renderMesh", params );
		params.set( "mesh", m_maxSphere );
		fireEvent( "renderMesh", params );

		createOrientationGDO( 1.0f, m_position, mat3f::IDENTITY );

		model.loadIdentity();
		model *= mat3f::lookAt( camera->getPosition(), m_position );
		model.translate( m_position );
		m_fontParams.set( "camera", camera );
		m_fontParams.set( "modelMatrix", model );
		fireEvent( "renderText", m_fontParams );
	}

	//----------------------------------------------------------------------
	// Utility function
	//----------------------------------------------------------------------
	FMOD_REVERB_PROPERTIES convertToFmodReverbProperties( const MonkyReverbProperties* reverbProperties )
	{
		FMOD_REVERB_PROPERTIES props;
		std::memcpy( &props, reverbProperties, sizeof( MonkyReverbProperties ) );
		return props;
	}
}