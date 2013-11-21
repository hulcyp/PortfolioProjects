#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class LowPassDSP : public DSP
	{
	public:
		LowPassDSP();
		LowPassDSP( XMLParser& parser, XMLNode* lpNode );

		/* Cutoff frequency in hz. Range: 10.0 to 22000.0 */
		void setCutoff( float freq );
		float getCutoff() const;
		/* Resonance value. Range 1.0 to 10.0 */
		void setResonance( float resonance );
		float getResonance() const;
	};
}