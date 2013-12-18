#pragma once
#include <fmod.hpp>
#include <string>

namespace Monky
{
	class AudioSystem;
	class AudioSystemImpl;

	class SoundImpl
	{
	public:
		friend AudioSystem;
		friend AudioSystemImpl;

		SoundImpl( const std::string& soundName, const std::string& filePath );
		~SoundImpl();

		const std::string getSoundName() const;
	
	private:
		std::string m_filePath;
		std::string m_soundName;
		FMOD::Sound* m_sound;
	};
}