#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class EchoDSP : public DSP
	{
	public:
		EchoDSP();
		EchoDSP( XMLParser& parser, XMLNode* echoNode );

		/* Range: 10 - 5000 */
		void setDelayMS( int delayMS );
		int getDelayMS() const;
		/* Range: 0.0 - 1.0 */
		void setDecayRatio( float decay );
		float getDecayRatio() const;
		/* Range: 0 - 16 */
		void setMaxNumChannels( int maxChannels );
		int getMaxNumChannels() const;
		/* Range: 0.0 - 1.0 */
		void setDryMixVolume( float volume );
		float getDryMixVolume() const;
		/* Range: 0.0 - 1.0 */
		void setWetMixVolume( float volume );
		float getWetMixVolume() const;
	};
}