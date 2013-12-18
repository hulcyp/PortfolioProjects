#include "Sound.h"
#include "SoundImpl.h"
#include <CommonEngineIncludes.h>
#include "AudioSystem.h"
#include <XMLParser.h>

namespace Monky
{
	std::map< std::string, Sound* > Sound::sm_sounds;
	Sound::~Sound()
	{
		SAFE_DELETE( m_soundImpl );
	}
	//----------------------------------------------------
	const std::string Sound::getSoundName() const
	{
		return m_soundImpl->getSoundName();
	}
	//------------------------------------------------
	//	Static functions for sound creation
	//------------------------------------------------
	Sound* Sound::createOrGetSound( const std::string& soundName, const std::string& filePath, Sound::SoundMode mode )
	{
		Sound*& newSound = sm_sounds[ soundName ];
		if( !newSound )
		{
			newSound = new Sound( soundName, filePath, mode );
		}
		return newSound;
	}
	//------------------------------------------------
	void Sound::createSound( const std::string& soundName, const std::string& filePath, Sound::SoundMode mode )
	{
		createOrGetSound( soundName, filePath, mode );
	}
	//------------------------------------------------
	Sound* Sound::getSound( const std::string& soundName )
	{
		auto iter = sm_sounds.find( soundName );
		if( iter != sm_sounds.end() )
			return iter->second;
		else
			return nullptr;
	}
	//------------------------------------------------
	void Sound::loadSoundsFromXMLFile( const std::string& filePath, const std::string& file )
	{
		std::string fullFilePath = filePath + file;
		XMLParser parser( fullFilePath.c_str() );
		XMLDocument& doc = parser.getDocument();

		if( !doc.Error() )
		{
			XMLNode* root = doc.FirstChildElement( "Sounds" );
			XMLNode* soundElement;
			for( soundElement = root->FirstChildElement( "Sound" ); soundElement != 0; soundElement = soundElement->NextSiblingElement( "Sound" ) )
			{
				parser.validateXMLAttributes( soundElement, "name,mode", "" );
				std::string soundName = parser.getXMLAttributeAsString( soundElement, "name", "" );
				std::string soundMode = parser.getXMLAttributeAsString( soundElement, "mode", "" );
				const char* soundFile = soundElement->GetText();
				Sound::createOrGetSound( soundName, filePath + std::string( soundFile ), getModeFromString( soundMode ) );
			}
		}
	}


	//------------------------------------------------
	// Private constructor
	//------------------------------------------------
	Sound::Sound( const std::string& soundName, const std::string& filePath, Sound::SoundMode mode )
		:	m_soundImpl( new SoundImpl( soundName, filePath ) )
	{
		AudioSystem::getInstance()->createSound( filePath, *this, mode );
	}
	
	//------------------------------------------------
	// Static util functions
	//------------------------------------------------	
	Sound::SoundMode Sound::getModeFromString( const std::string& mode )
	{
		Sound::SoundMode soundMode;
		if( mode == "MONKY_2D" )
		{
			soundMode = MONKY_2D;
		}
		else if( mode == "MONKY_3D" )
		{
			soundMode = MONKY_3D;
		}
		return soundMode;
	}
}