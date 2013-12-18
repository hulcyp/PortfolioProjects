#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 7/2/2013
//	Description:
//
//---------------------------------------------
#include "CommonEngineIncludes.h"
#include "ReverbAreaProperties.h"

namespace Monky
{
	class ReverbAreaImpl;
	class AudioSystem;
	class Camera;

	class ReverbArea
	{
	public:
		friend AudioSystem;

		ReverbArea( const std::string& name );
		~ReverbArea();

		void set3DAttributes( const vec3f& pos, float min, float max );
		void setPosition( const vec3f& pos );
		void setMinMaxDist( float min, float max );

		void setProperties( const MonkyReverbProperties& reverbProp );

		const vec3f& getPosition() const;
		void getMinMaxDist( float& minOut, float& maxOut ) const;

		void renderDebugInfo( Camera* camera );

		static ReverbArea* createOrGetReverbArea( const std::string& reverbAreaName, const MonkyReverbProperties& reverbProps );
		static void createReverbArea( const std::string& reverbAreaName, const MonkyReverbProperties& reverbProps );
		static ReverbArea* getReverbArea( const std::string& reverbAreaName );
		static void deleteReverbArea( const std::string& reverbAreaName );

		static void loadReverbAreasFromXMLFile( const std::string& filePath, const std::string& file );
		static void cleanupReverbAreas();
		static void renderAllReverbAreaDebugInfo( Camera* camera );

	private:
		ReverbAreaImpl* m_impl;

		static std::map< std::string, ReverbArea* > sm_reverbAreas;
	};

	MonkyReverbProperties getMonkyReverbPresetFromString( const std::string& reverbPreset );
}