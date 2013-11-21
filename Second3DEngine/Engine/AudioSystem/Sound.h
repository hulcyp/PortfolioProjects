#pragma once
#include <string>
#include <map>

namespace Monky
{
	class SoundImpl;
	class AudioSystem;
	class AudioSystemImpl;
	class SoundCueImpl;

	class Sound
	{
	public:
		friend AudioSystem;
		friend AudioSystemImpl;
		friend SoundCueImpl;

		enum SoundMode
		{
			MONKY_2D,
			MONKY_3D,
		};

		~Sound();

		const std::string getSoundName() const;
		

		static Sound* createOrGetSound( const std::string& soundName, const std::string& filePath, SoundMode mode );
		static void createSound( const std::string& soundName, const std::string& filePath, SoundMode mode );
		static Sound* getSound( const std::string& soundName );

		static void loadSoundsFromXMLFile( const std::string& filePath, const std::string& file );

	private:
		Sound( const std::string& soundName, const std::string& filePath, SoundMode mode );

		static SoundMode getModeFromString( const std::string& mode );

		SoundImpl* m_soundImpl;

		static std::map< std::string, Sound* > sm_sounds;
	};
}