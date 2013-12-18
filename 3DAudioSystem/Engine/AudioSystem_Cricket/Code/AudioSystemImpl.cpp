#include "AudioSystemImpl.h"
#include "CommonEngineIncludes.h"
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/sound.h"

namespace Monky
{
	AudioSystemImpl::AudioSystemImpl()
	{
		CkConfig config;
		CkInit( &config );
	}
	//----------------------------------------------------------------------
	AudioSystemImpl::~AudioSystemImpl()
	{
		CkShutdown();
	}
	//----------------------------------------------------------------------
	void AudioSystemImpl::update()
	{
		CkUpdate();
	}
	//----------------------------------------------------------------------
	void AudioSystemImpl::setListenerAttributes( const vec3f& pos, const vec3f& vel, const vec3f& forward, const vec3f& up )
	{
		CkSound::set3dListenerPosition(	pos.x, pos.y, pos.z, pos.x + forward.x, pos.y + forward.y, pos.z + forward.z, up.x, up.y, up.z );
	}
}