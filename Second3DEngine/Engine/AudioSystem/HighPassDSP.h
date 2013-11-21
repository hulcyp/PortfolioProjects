#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class HighPassDSP : public DSP
	{
	public:
		HighPassDSP();
		HighPassDSP( XMLParser& parser, XMLNode* hpNode );

		/*  Cutoff frequency in hz. Range: 1.0 to 22000.0 */
		void setCutoff( float cutoff );
		float getCutoff() const;
		/* Resonance value. Range: 1.0 to 10.0 */
		void setResonance( float resonance );
		float getResonance() const;		
	};
}