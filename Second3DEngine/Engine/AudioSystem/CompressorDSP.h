#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class CompressorDSP : public DSP
	{
	public:
		CompressorDSP();
		CompressorDSP( XMLParser& parser, XMLNode* compressorNode );

		/* Set level in dB. Range: -60 - 0 */
		void setCompressorThreshold( float dbLevel );
		float getCompressorThreshold() const;
		/* Gain reduction attack time(ms). Range: 10 - 200 */
		void setGainReductionAttackTimeMS( float timeMS );
		float getGainReductionAttackTimeMS() const;
		/* Gain reduction release time(ms). Range: 20 - 1000 */
		void setGainReductionReleaseTimeMS( float timeMS );
		float getGainReductionReleaseTimeMS( float timeMS );
		/* Make-up gain(dB) applide after limiting. Range: 0 - 30 */
		void setGainMakeup( float gainMakeup );
		float getGainMakeup() const;
	};
}