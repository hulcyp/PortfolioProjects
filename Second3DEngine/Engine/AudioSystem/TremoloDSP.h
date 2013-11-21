#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class TremoloDSP : public DSP
	{
	public:
		TremoloDSP();
		TremoloDSP( XMLParser& parser, XMLNode* tremoloNode );

		/* Frq in hz. Range: 0.1 - 20.0 */
		void setFrequency( float frequency );
		float getFrequency() const;
		/* Range: 0.0 - 1.0 */
		void setDepth( float depth );
		float getDepth() const;
		/* Shape morph between triangle and sine. Range: 0 - 1 */
		void setShape( float shape );
		float getShape() const;
		/* Time skewing. Range: -1.0 - 1.0 */
		void setTimeSkewing( float skewing );
		float getTImeSkewing() const;
		/* LFO on-time. Range: 0 - 1 */
		void setDuty( float duty );
		float getDuty() const;
		/* Flatness of the LFO shape. Range: 0.0 - 1.0 */
		void setFlatness( float flatness );
		float getFlatness() const;
		/* Instantaneous LFO phase. Range: 0 - 1 */
		void setPhase( float phase );
		float getPhase() const;
		/* Rotation / auto-pan effect. Range: -1 - 1 */
		void setSpread( float spread );
		float getSPread() const;
	};
}