#include "SoundImpl.h"
#include "ASErrorHandling.h"
#include "AudioSystem.h"

namespace Monky
{
	SoundImpl::SoundImpl( const std::string& soundName, const std::string& filePath )
		:	m_filePath( filePath )
		,	m_soundName( soundName )
	{}
	//----------------------------------------------------
	SoundImpl::~SoundImpl()
	{
		FMOD_RESULT result = m_sound->release();
		asErrorCheck( result );
		//SAFE_DELETE( m_sound );
	}
	//----------------------------------------------------
	const std::string SoundImpl::getSoundName() const
	{
		return m_soundName;
	}
}