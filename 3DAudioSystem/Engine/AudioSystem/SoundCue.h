#pragma once
#include <string>
#include <CommonTypeDefines.h>
#include "Sound.h"
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class AudioSystem;
	class AudioSystemImpl;
	class SoundCueImpl;
	class Camera;

	class SoundCue
	{
	public:

		friend AudioSystem;
		friend AudioSystemImpl;

		~SoundCue();

		void renderDebugInfo( Camera* camera ) const;

		void setLooping( bool looping );
		bool getLooping() const;
		void play();
		void pause();
		void unpause();
		void stop();
		bool isPaused() const;
		bool isPlaying() const;

		void pushDSP( DSP* dsp );
		// popping a dsp will only take effect after the sound has been stopped and started again.
		void popDSP();

		void set3DAttributes( const vec3f& pos, const vec3f& vel );
		void set3DConeSettings( float insideAngleDegrees, float outsideConeAngleDegrees, float outsideVolume );
		void set3DConeOrientation( const vec3f& forward, const vec3f& up, const vec3f& right );
		void set3DConeOrientation( const mat3f& orientation );

		void setVolume( float volume );
		float getVolume() const;

		void setFrequency( float freq );
		float getFrequency() const;

		const vec3f& getPosition() const;
		const vec3f& getVelocity() const;

		static SoundCue* createOrGetSoundCue( const std::string& name, const std::string& soundName, const vec3f& pos,
			const vec3f& vel, float attenuationStartDist, bool isLooping, bool startPaused = false ); 
		static void createSoundCue( const std::string& name, const std::string& soundName, const vec3f& pos,
			const vec3f& vel, float attenuationStartDist, bool isLooping, bool startPaused = false ); 
		static SoundCue* getSoundCue( const std::string& name );
		static void deleteSoundCue( const std::string& name );
		
		static void loadSoundCuesFromXMLFile( const std::string& filePath, const std::string& file );
		static void cleanupSoundCues();
		static void renderSoundCueDebugInfo( Camera* camera );
		

	private:
		SoundCue( const std::string& soundName );
		SoundCue( const std::string& soundName, const vec3f& pos , const vec3f& vel, 
			const vec2f& attenuationFallOff, bool isLooping, bool startPaused = false  );

		static void addDSPsToSoundCue( SoundCue* soundCue, XMLNode* soundCueNode, XMLParser& parser );

		SoundCueImpl* m_soundCueImpl;

		static std::map< std::string, SoundCue* > sm_soundCues;
	};
}