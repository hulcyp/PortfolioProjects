#pragma once
#include "ck/sound.h"
#include "CommonEngineIncludes.h"

namespace Monky
{
	class Sound;
	class SoundCueImpl;

	class SoundImpl
	{
	public:
		friend Sound;
		friend SoundCueImpl;

		SoundImpl( const std::string& soundName, const std::string& filePath );
		~SoundImpl();

		const std::string getSoundName() const;

	private:
		std::string m_filePath;
		std::string m_soundName;
		CkSound* m_sound;
	};
}