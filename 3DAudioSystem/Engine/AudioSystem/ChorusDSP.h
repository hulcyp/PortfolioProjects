#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class ChorusDSP : public DSP
	{
	public:
		ChorusDSP();
		ChorusDSP( XMLParser& parser, XMLNode* chorusNode );

		void setDryMixVolume( float volume );
		float getDryMixVolume() const;
		/* Set volume for chorus tap 1, 2 or 3 */
		void setWetMixVolume( int chorusTap, float volume );
		float getWetMixVolume( int chorusTap ) const;
		void setDelayMS( float delayMS );
		float getDelayMS() const;
		/* Modulation rate in hz. Range: 0.0 - 20.0 */
		void setModulationRate( float modRate );
		float getModulationRate() const;
		/*Modulation depth. Range: 0.0 - 1.0*/
		void setModulationDepth( float modDepth );
		float getModulationDepth() const;
	};
}