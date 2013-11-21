#pragma once
#include <fmod.hpp>

namespace Monky
{
	class AudioSystemImpl;
	class SoundCueImpl;

	class DSPImpl
	{
	public:
		friend AudioSystemImpl;
		friend SoundCueImpl;

		DSPImpl();
		virtual ~DSPImpl();

		void setParameter( int index, float value );
		float getParameter( int index ) const;

	private:
		FMOD::DSP* m_dsp;
		FMOD_DSP_TYPE m_type;
	};
}