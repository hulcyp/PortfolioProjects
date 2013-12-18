#include "ProfileSection.h"
#include "TimeUtils.h"
#include "ProfileSystem.h"
#include "ProfileData.h"

namespace Monky
{
	ProfileSection::ProfileSection( int index, int extraData )
		:	m_index( index )
		,	m_extraData( extraData )
	{
		m_startTime = TimeUtils::GetAbsoluteTimeSeconds();
	}
	//--------------------------------------------------------
	ProfileSection::~ProfileSection()
	{
		double endTime = TimeUtils::GetAbsoluteTimeSeconds();
		double deltaTime = endTime - m_startTime;
		
		ProfileData& profileData = ProfileSystem::getInstance()->getProfileData( m_index );
		profileData.addData( deltaTime, m_extraData );
	}
}