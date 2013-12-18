#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class FlangeDSP : public DSP
	{
	public:
		FlangeDSP();
		FlangeDSP( XMLParser& parser, XMLNode* flangeNode );

		/* Volume of original signal. Range: 0.0 - 1.0 */
		void setDryMixVolume( float volume );
		float getDryMixVolume() const;
		/* Volume of flange signal to pass to output. Range: 0.0 - 1.0 */
		void setWetMixVolume( float volume );
		float getWetMixVolume() const;
		/* Flange depth ( percentage of 40ms delay ). Range: 0.01 - 1.0 */
		void setDepth( float depth );
		float getDepth() const;
		/* Flange speed in hz. Range: 0.0 - 20.0 */
		void setRate( float rate );
		float getRate() const;
	};
}
