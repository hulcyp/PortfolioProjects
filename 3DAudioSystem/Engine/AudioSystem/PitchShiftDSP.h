#pragma once
#include "DSP.h"

namespace Monky
{
	class PitchShiftDSP : public DSP
	{
	public:
		enum FFTWindowSize
		{
			FFT_256 = 256,
			FFT_512 = 512,
			FFT_1024 = 1024,
			FFT_2048 = 2048,
			FFT_4096 = 4096
		};

		PitchShiftDSP();
		PitchShiftDSP( XMLParser& parser, XMLNode* pitchShiftNode );

		/* Range: 0.5 - 2.0 */
		void setPitch( float pitch );
		float getPitch( float pitch );
		/* FFT window size. */
		void setFFTWinowSize( FFTWindowSize windowSize );
		FFTWindowSize getFFTWindowSize() const;
		/* Max channels supported. Range: 0 - 16*/
		void setMaxChannels( int channels );
		int getMaxChannels() const;

	private:
		FFTWindowSize getWindowSizeFromString( const std::string& windowSize );
	};
}