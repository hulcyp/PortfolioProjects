#pragma once
#include "StdLibraryIncludes.h"
#include "Singleton.h"
#include "ProfileData.h"

namespace Monky
{
	class Camera;

	class ProfileSystem : public Singleton< ProfileSystem >
	{
	public:

		ProfileSystem();
		~ProfileSystem();

		const ProfileData& getProfileData( size_t index ) const;
		ProfileData& getProfileData( size_t index );
		int getIndexMapping( const std::string& tag );

		void renderData( const vec3f& startPos, Camera* camera );
		void clearFrameData();
		void toggleDataRenderState( ProfileData::DataToRender whatToToggle );

		void reportNewData( size_t profileIndex );

	protected:
		void renderChildren( int index, vec3f& pos );

	private:
		std::vector< ProfileData > m_profilingData;
		std::map< std::string, int > m_tagToindexMapping;

		int m_indexForProfileWithMaxTimeThisFrame;
		int m_indexForProfileWithMaxOverallTime;
		int m_indexForProfileWithMaxHitCountThisFrame;
		int m_indexForProfileWithMaxHitCountOverall;
		int m_indexForProfileWithMaxExtraData;
		int m_indexForProfileWithMaxExtraDataThisFrame;

		NamedProperties m_fontParams;
		float m_fontHeight;
		float m_childTabWidth;
	};
}