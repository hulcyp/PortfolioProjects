#pragma once
#include "CommonEngineIncludes.h"
#include "Sound.h"


namespace Monky
{
	class Mesh;

	class SoundCueImpl
	{
	public:
		SoundCueImpl( const std::string& soundName );
		SoundCueImpl( const std::string& soundName, const vec3f& pos, const vec3f& vel, 
			const vec2f& attenuationFallOff, bool isLooping, bool startPaused = false  );
		~SoundCueImpl();

		void init();
		void renderDebugInfo( Camera* camera );

		void setLooping( bool looping );
		bool getLooping() const;
		void pause();
		void unpause();
		void stop();
		bool isPaused() const;
		bool isPlaying() const;

		void set3DAttributes( const vec3f& pos, const vec3f& vel );
		void set3DConeSettings( float insideAngleDegrees, float outsideConeAngleDegrees, float outsideVolume );
		void set3DConeOrientation( const vec3f& forward, const vec3f& up, const vec3f& right );
		void set3DConeOrientation( const mat3f& orientation );

		void setVolume( float volume );
		float getVolume() const;

		void setFrequency( float freq );
		float getFrequency() const;

		const vec3f& getPosition() const { return m_position; }
		const vec3f& getVelocity() const { return m_velocity; }
		
	private:
		Sound* m_sound;
		Mesh* m_iconMesh;
		Mesh* m_innerSphere;
		Mesh* m_volumeBar;
		Mesh* m_volumeBGBar;
		NamedProperties m_fontParams;
		float m_fontHeight;
		float m_iconWidth;
		float m_widthOfVolumeBar;
		float m_heightOfVolumeBar;
		vec3f m_position;
		vec3f m_velocity;
		mat3f m_orientation;
		bool m_isLooping;
		bool m_isPaused;
		vec2f m_attenuationMinMaxDist;
		float m_volume;
	};
}