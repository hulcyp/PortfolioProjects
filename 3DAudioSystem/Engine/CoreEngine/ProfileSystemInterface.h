#pragma once
#include "ProfileSystem.h"
#include "ProfileSection.h"

#define MONKY_PROFILING_ENABLED

#ifdef MONKY_PROFILING_ENABLED
	#define PROFILE_SECTION_WITH_DATA( tag, extraData )								\
		static int index = ProfileSystem::getInstance()->getIndexMapping( tag );	\
		ProfileSection p( index, extra );												
#else
	#define PROFILE_SECTION( tag, extraData )
#endif

#ifdef MONKY_PROFILING_ENABLED
	#define PROFILE_SECTION( tag )													\
		static int index = ProfileSystem::getInstance()->getIndexMapping( tag );	\
		ProfileSection p( index );
#else
	#define PROFILE_SECTION( tag )
#endif

#ifdef MONKY_PROFILING_ENABLED
	#define START_PROFILE_SECTION_WITH_DATA( tag, extraData )						\
		{																			\
			static int index = ProfileSystem::getInstance()->getIndexMapping( tag );\
			ProfileSection p( index, extraData );
#else
	#define START_PROFILE_SECTION( tag, extraData )
#endif

#ifdef MONKY_PROFILING_ENABLED
	#define START_PROFILE_SECTION( tag )											\
		{																			\
			static int index = ProfileSystem::getInstance()->getIndexMapping( tag );\
			ProfileSection p( index );
#else
	#define START_PROFILE_SECTION( tag )
#endif

#ifdef MONKY_PROFILING_ENABLED
	#define END_PROFILE_SECTION	}
#else
	#define END_PROFILE_SECTION
#endif

#ifdef MONKY_PROFILING_ENABLED
	#define ENABLE_RENDERING_OF_PROFILE( tag )												\
		int index = ProfileSystem::getInstance()->getIndexMapping( tag );					\
		ProfileData& profileData = ProfileSystem::getInstance()->getProfileData( index );	\
		profileData.setShouldRender( true );
#else
	#define ENABLE_RENDERING_OF_PROFILE( tag )
#endif

#ifdef MONKY_PROFILING_ENABLED
	#define DISABLE_RENDERING_OF_PROFILE( tag )												\
		int index = ProfileSystem::getInstance()->getIndexMapping( tag );					\
		ProfileData& profileData = ProfileSystem::getInstance()->getProfileData( index );	\
		profileData.setShouldRender( false );
#else
	#define DISABLE_RENDERING_OF_PROFILE( tag )
#endif		