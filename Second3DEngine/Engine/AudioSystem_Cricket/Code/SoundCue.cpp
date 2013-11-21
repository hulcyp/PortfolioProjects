#include "SoundCue.h"
#include "AudioSystem.h"
#include "SoundCueImpl.h"
#include <CommonEngineIncludes.h>
#include <XMLParser.h>

namespace Monky
{
	std::map< std::string, SoundCue* > SoundCue::sm_soundCues;

	SoundCue::~SoundCue()
	{
		SAFE_DELETE( m_soundCueImpl );
	}
	//----------------------------------------------
	void SoundCue::renderDebugInfo( Camera* camera ) const
	{
		m_soundCueImpl->renderDebugInfo( camera );
	}
	//----------------------------------------------
	void SoundCue::setLooping( bool looping )
	{
		m_soundCueImpl->setLooping( looping );
	}
	//----------------------------------------------
	bool SoundCue::getLooping() const
	{
		return m_soundCueImpl->getLooping();
	}
	//----------------------------------------------
	void SoundCue::play()
	{
		bool playing = isPlaying();
		AudioSystem::getInstance()->playSoundCue( *this );
		m_soundCueImpl->setLooping( m_soundCueImpl->getLooping() );
		m_soundCueImpl->unpause();
	}
	//----------------------------------------------
	void SoundCue::pause()
	{
		m_soundCueImpl->pause();
	}
	//----------------------------------------------
	void SoundCue::unpause()
	{
		m_soundCueImpl->unpause();
	}
	//----------------------------------------------
	void SoundCue::stop()
	{
		m_soundCueImpl->stop();
	}
	//----------------------------------------------
	bool SoundCue::isPaused() const 
	{ 
		return m_soundCueImpl->isPaused();
	}
	//----------------------------------------------
	bool SoundCue::isPlaying() const
	{
		return m_soundCueImpl->isPlaying();
	}
	//----------------------------------------------
	void SoundCue::pushDSP( DSP* dsp )
	{
	}
	//----------------------------------------------
	void SoundCue::popDSP()
	{
	}
	//----------------------------------------------
	void SoundCue::set3DAttributes( const vec3f& pos, const vec3f& vel )
	{
		m_soundCueImpl->set3DAttributes( pos, vel );
	}
	//----------------------------------------------
	void SoundCue::set3DConeSettings( float insideAngleDegrees, float outsideConeAngleDegrees, float outsideVolume )
	{
		m_soundCueImpl->set3DConeSettings( insideAngleDegrees, outsideConeAngleDegrees, outsideVolume );
	}
	//----------------------------------------------
	void SoundCue::set3DConeOrientation( const vec3f& forward, const vec3f& up, const vec3f& right )
	{
		m_soundCueImpl->set3DConeOrientation( forward, up, right );
	}
	//----------------------------------------------
	void SoundCue::set3DConeOrientation( const mat3f& orientation )
	{
		m_soundCueImpl->set3DConeOrientation( orientation );
	}
	//----------------------------------------------
	void SoundCue::setVolume( float volume )
	{
		m_soundCueImpl->setVolume( volume );
	}
	//----------------------------------------------
	float SoundCue::getVolume() const
	{
		return m_soundCueImpl->getVolume();
	}
	//----------------------------------------------
	void SoundCue::setFrequency( float freq )
	{
		m_soundCueImpl->setFrequency( freq );
	}
	//----------------------------------------------
	float SoundCue::getFrequency() const
	{
		return m_soundCueImpl->getFrequency();
	}
	//----------------------------------------------
	const vec3f& SoundCue::getPosition() const
	{
		return m_soundCueImpl->getPosition();
	}
	//----------------------------------------------
	const vec3f& SoundCue::getVelocity() const
	{
		return m_soundCueImpl->getVelocity();
	}
	//----------------------------------------------
	SoundCue* SoundCue::createOrGetSoundCue( const std::string& name, const std::string& soundName, const vec3f& pos,
		const vec3f& vel, float attenuationStartDist, bool isLooping, bool startPaused )
	{
		SoundCue*& soundCue = sm_soundCues[ name ];
		if( !soundCue )
		{
			soundCue = new SoundCue( soundName, pos, vel, vec2f( attenuationStartDist, 5000.0f ), isLooping, startPaused );
		}
		return soundCue;
	}
	//----------------------------------------------
	void SoundCue::createSoundCue( const std::string& name, const std::string& soundName, const vec3f& pos,
		const vec3f& vel, float attenuationStartDist, bool isLooping, bool startPaused )
	{
		createOrGetSoundCue( name, soundName, pos, vel, attenuationStartDist, isLooping, startPaused );
	}
	//----------------------------------------------
	SoundCue* SoundCue::getSoundCue( const std::string& name )
	{
		auto iter = sm_soundCues.find( name );
		if( iter != sm_soundCues.end() )
			return iter->second;
		else
			return nullptr;
	}
	//----------------------------------------------
	void SoundCue::deleteSoundCue( const std::string& name )
	{
		auto iter = sm_soundCues.find( name );
		if( iter != sm_soundCues.end() )
		{
			SAFE_DELETE( iter->second );
			sm_soundCues.erase( iter );
		}
	}
	//----------------------------------------------
	void SoundCue::loadSoundCuesFromXMLFile( const std::string& filePath, const std::string& file )
	{
		std::string fullFilePath = filePath + file;
		XMLParser parser( fullFilePath.c_str() );
		XMLDocument& doc = parser.getDocument();

		if( !doc.Error() )
		{
			XMLNode* root = doc.FirstChildElement( "SoundCues" );
			XMLNode* soundCue;
			for( soundCue = root->FirstChildElement( "SoundCue" ); soundCue != nullptr; soundCue = soundCue->NextSiblingElement( "SoundCue" ) )
			{
				parser.validateXMLChildElements( soundCue, "", "DSP,Cone" );
				parser.validateXMLAttributes( soundCue, "name,sound,volume,pos,attenuationStartDist", "vel,isLooping,startPaused,frequency" );
				std::string name = parser.getXMLAttributeAsString( soundCue, "name", "" );
				std::string soundName = parser.getXMLAttributeAsString( soundCue, "sound", "" );
				float volume = parser.getXMLAttributeAsFloat( soundCue, "volume", 1.0f );
				vec3f pos = parser.getXMLAttributeAsVec3( soundCue, "pos", vec3f() );
				vec3f vel = parser.getXMLAttributeAsVec3( soundCue, "vel", vec3f() );
				float atten = parser.getXMLAttributeAsFloat( soundCue, "attenuationStartDist", 0.1f );
				bool isLooping = parser.getXMLAttributeAsBool( soundCue, "isLooping", false );
				bool startPaused = parser.getXMLAttributeAsBool( soundCue, "startPaused", false );
				SoundCue* cue = SoundCue::createOrGetSoundCue( name, soundName, pos, vel, atten, isLooping, startPaused );
				cue->setVolume( volume );

				std::string frequencyStr = parser.getXMLAttributeAsString( soundCue, "frequency", "default" );
				float frequency = cue->getFrequency();
				std::vector< std::string > tokens;
				stringTokenizer( frequencyStr, tokens, "*" );
				if( tokens.size() > 1 )
				{
					float freqFactor = 1.0f;
					if( stringToType( tokens[0], &freqFactor ) && tokens[1] == "default" )
						frequency *= freqFactor;
					else if( stringToType( tokens[1], &freqFactor ) && tokens[0] == "default" )
						frequency *= freqFactor;
				}
				else if( !tokens.empty() )
				{
					if( !stringToType( tokens[0], &frequency ) )
						frequency = cue->getFrequency();
				}
				
				cue->setFrequency( frequency );
				SoundCue::addDSPsToSoundCue( cue, soundCue, parser );

				XMLNode* coneSettings = soundCue->FirstChildElement( "Cone" );
				if( coneSettings != nullptr )
				{
					parser.validateXMLAttributes( coneSettings, "insideAngle,outsideAngle,outsideVolume,yaw,pitch,roll", "" );
					float insideAngle = parser.getXMLAttributeAsFloat( coneSettings, "insideAngle", 0.0f );
					float outsideAngle = parser.getXMLAttributeAsFloat( coneSettings, "outsideAngle", 0.1f );
					float outsideVolume = parser.getXMLAttributeAsFloat( coneSettings, "outsideVolume", 1.0f );
					float yaw = parser.getXMLAttributeAsFloat( coneSettings, "yaw", 0.0f );
					float pitch = parser.getXMLAttributeAsFloat( coneSettings, "pitch", 0.0f );
					float roll = parser.getXMLAttributeAsFloat( coneSettings, "roll", 0.0f );

					cue->set3DConeSettings( insideAngle, outsideAngle, outsideVolume );
					mat3f orienation( mat3f::IDENTITY );
					orienation.rotate( yaw, pitch, roll );
					cue->set3DConeOrientation( orienation );
				}
			}
		}
	}
	
	//------------------------------------------------
	// Private constructors
	//------------------------------------------------
	SoundCue::SoundCue( const std::string& soundName )
		:	m_soundCueImpl( new SoundCueImpl( soundName ) )
	{}
	//----------------------------------------------
	SoundCue::SoundCue( const std::string& soundName, const vec3f& pos, const vec3f& vel, 
			const vec2f& attenuationFallOff, bool isLooping, bool startPaused )
		:	m_soundCueImpl( new SoundCueImpl( soundName, pos, vel, attenuationFallOff, isLooping, startPaused ) )
	{
		setLooping( isLooping );
		if( !startPaused )
			play();
	}
	
	//----------------------------------------------
	void SoundCue::addDSPsToSoundCue( SoundCue* soundCue, XMLNode* soundCueNode, XMLParser& parser )
	{
		
	}
	//----------------------------------------------
	void SoundCue::cleanupSoundCues()
	{
		MAP_CLEANUP( sm_soundCues );
	}
	//----------------------------------------------
	void SoundCue::renderSoundCueDebugInfo( Camera* camera )
	{
		for( auto iter = sm_soundCues.begin(); iter != sm_soundCues.end(); ++iter )
		{
			iter->second->renderDebugInfo( camera );
		}
	}
}