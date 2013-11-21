#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class DistortionDSP : public DSP
	{
	public:
		DistortionDSP();
		DistortionDSP( XMLParser& parser, XMLNode* distortionNode );

		/*Range: 0.0 - 1.0*/
		void setDistortionLevel( float level );
		float getDistortionLevel() const;
	};
}