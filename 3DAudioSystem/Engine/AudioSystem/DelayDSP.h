#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class DelayDSP : public DSP
	{
	public:
		DelayDSP();
		DelayDSP( XMLParser& parser, XMLNode* delayNode );

		/* Sets the delay for the specified channel. channel: range 0 - 15, delayMS: 0 - 10000 */
		void setChannelDelayMS( int channel, int delayMS );
		int getChannelDelayMS( int channel ) const;

		/* Sets the max delay value for all channels in ms. Range: 0 - 10000 */
		void setMaxDelayMS( int delayMS );
		int getMaxDelayMS() const;
	};
}