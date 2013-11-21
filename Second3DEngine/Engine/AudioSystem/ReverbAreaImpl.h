#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 7/2/2013
//	Description:
//
//---------------------------------------------
#include "CommonEngineIncludes.h"
#include <fmod.hpp>

namespace Monky
{
	class AudioSystemImpl;
	class Mesh;
	struct MonkyReverbProperties;

	class ReverbAreaImpl
	{
	public:
		friend AudioSystemImpl;

		ReverbAreaImpl( const std::string& name );
		~ReverbAreaImpl();

		void set3DAttributes( const vec3f& pos, float min, float max );
		void setPosition( const vec3f& pos );
		void setMinMaxDist( float min, float max );

		void setProperties( const MonkyReverbProperties* reverbProp );

		const vec3f& getPosition() const;
		void getMinMaxDist( float& minOut, float& maxOut ) const;
		
		void renderDebugInfo( Camera* camera );

	private:
		FMOD::Reverb* m_reverb;
		vec3f m_position;
		float m_minDist;
		float m_maxDist;

		Mesh* m_minSphere;
		Mesh* m_maxSphere;

		std::string m_name;
		NamedProperties m_fontParams;
	};

	FMOD_REVERB_PROPERTIES convertToFmodReverbProperties( const MonkyReverbProperties* reverbProperties );
}