#pragma once
#include "DSP.h"

namespace Monky
{
	class ParameqDSP : public DSP
	{
	public:
		ParameqDSP();
		ParameqDSP( XMLParser& parser, XMLNode* parameqNode );

		/* Frequency center. Range: 20.0 - 22000.0 */
		void setFreqCenter( float freqCenter );
		float getFreqCenter() const;
		/* Octave range around the center. Range: 0.2 - 5.0 */
		void setBandwidth( float bandwidth );
		float getBandwidth() const;
		/* Frequency gain. Range: 0.05 - 3.0 */
		void setFreqGain( float freqGain );
		float getFreqGain() const;
	};
}