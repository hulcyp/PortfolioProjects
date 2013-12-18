#pragma once
#include "CommonEngineIncludes.h"

namespace Monky
{
	class AudioSystemImpl
	{
	public:
		AudioSystemImpl();
		~AudioSystemImpl();

		void update();

		void setListenerAttributes( const vec3f& pos, const vec3f& vel, const vec3f& forward, const vec3f& up );
	};
}