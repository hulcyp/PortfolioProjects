#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class NormalizeDSP : public DSP
	{
	public:
		NormalizeDSP();
		NormalizeDSP( XMLParser& parser, XMLNode* normNode );

		/*Time to ramp the silence to full in ms. Range: 0.0 - 20000.0*/
		void setFadeTimeMS( float timeMS );
		float getFadeTimeMS() const;
		/* Lower volume range threshold to ignore. Range: 0.0 - 1.0*/
		void setThreshold( float threshold );
		float getThreshold() const;
		/*Max amplification allowed. Range: 1.0 - 100000.0*/
		void setMaxAmp( float maxAmp );
		float getMaxAmp() const;
	};
}