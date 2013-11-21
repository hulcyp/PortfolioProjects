#include "DSP.h"
#include <XMLParser.h>

namespace Monky
{
	class OscillatorDSP : public DSP
	{
	public:
		enum OscillatorType
		{
			SINE = 0,
			SQUARE,
			SAWUP,
			SAWDOWN,
			TRIANGLE,
			NOISE,
		};


		OscillatorDSP();
		OscillatorDSP( XMLParser& parser, XMLNode* oscillatorNode );

		void setType( OscillatorType type );
		OscillatorType getType() const;
		/* Freqncy of sine wave in hz. Range: 1.0 - 22000.0 */
		void setFrequency( float freq );
		float getFrequency() const;

	protected:
		OscillatorType getOscillatorType( const std::string& type ) const;
	};
}