#pragma once
#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class SFXReverbDSP : public DSP
	{
	public:
		SFXReverbDSP();
		SFXReverbDSP( XMLParser& parser, XMLNode* sfxReverbNode );

		/* Mix level of dry signal in mB. Range: -10000.0 - 0.0 */
		void setDryLevel( float level );
		float getDryLevel() const;
		/* Room effect level at low freq in mB. Range -10000.0 - 0.0 */
		void setLowFreqRoomLevel( float level );
		float getLowFreqRoomLevel() const;
		/* Room effect level at high freq in mB. Range -10000.0 - 0.0 */
		void setHighFreqRoomLevel( float level );
		float getHighFreqRoomLevel() const;
		/* Reverberation decay time at low freq in seconds. Range: 0.1 - 2.0 */
		void setDecayLFTimeSec( float timeSec );
		float getDecayLFTimeSec() const;
		/* High - low frequency decay time ratio. Range: 0.1 - 2.0 */
		void setDecayHFRatio( float ratio );
		float getDecayHFRatio() const;
		/* Early reflection level relative to room effect in mB. Range -10000.0 - 1000.0*/
		void setReflectionLevel( float level );
		float getReflectionLevel() const;
		/* Delay time of first reflection in seconds. Range: 0.0 - 0.3 */
		void setReflectionDelaySec( float timeSec );
		float getReflectionDelaySec() const;
		/* Late reverberation level relative to room effect in mB. Range: -10000.0 - 2000.0 */
		void setReverbLevel( float level );
		float getReverbLevel() const;
		/* Late reverberation delay time relative to first reflection in seconds. Range: 0.0 - 0.1 */
		void setReverbDelaySec( float timeSec );
		float getReverbDelaySec() const;
		/* Reverberation diffusion (echo density) in perecent. Range: 0.0 - 100.0 */
		void setDiffusionPercentage( float percent );
		float getDiffusionPercentage() const;
		/* Reverberation density (modal density) in percent. Range: 0.0 - 100.0 */
		void setDensityPercentage( float percent );
		float getDensityPercentage() const;
		/* Reference HF in Hz. Range: 20.0 - 20000.0 */
		void setHFReference( float reference );
		float getHFReference() const;
		/* Room effect LF level in mB. Range: -10000.0 - 0.0 */
		void setRoomLFLevel( float level );
		float getRoomLFLevel() const;
		/* Reference LF in hz. Range: 20.0 - 1000.0 */
		void setLFReferenceLevel( float level );
		float getLFReferenceLevel() const;
	};
}