#include "SoundImpl.h"

namespace Monky
{
	SoundImpl::SoundImpl( const std::string& soundName, const std::string& filePath )
	{
		m_sound = CkSound::newStreamSound( filePath.c_str(), kCkPathType_FileSystem );
	}
	//----------------------------------------------------------------------
	SoundImpl::~SoundImpl()
	{
		m_sound->destroy();
	}
	//----------------------------------------------------------------------
	const std::string SoundImpl::getSoundName() const
	{
		return m_soundName;
	}
}