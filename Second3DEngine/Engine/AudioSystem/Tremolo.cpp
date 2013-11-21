#include "TremoloDSP.h"
#include "DSPImpl.h"

namespace Monky
{	
	TremoloDSP::TremoloDSP()
		:	DSP( MONKY_DSP_TYPE_TREMOLO )
	{}
	//---------------------------------------------------
	TremoloDSP::TremoloDSP( XMLParser& parser, XMLNode* tremoloNode )
		:	DSP( MONKY_DSP_TYPE_TREMOLO )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( tremoloNode, "", "type,frequency,depth,shape,timeSkewing,duty,flatness,phase,spread" );
		float frequency = parser.getXMLAttributeAsFloat( tremoloNode, "frequency", NO_VALUE_SPECIFIED );
		float depth = parser.getXMLAttributeAsFloat( tremoloNode, "depth", NO_VALUE_SPECIFIED );
		float shape = parser.getXMLAttributeAsFloat( tremoloNode, "shape", NO_VALUE_SPECIFIED );
		float timeSkewing = parser.getXMLAttributeAsFloat( tremoloNode, "timeSkewing", NO_VALUE_SPECIFIED );
		float duty = parser.getXMLAttributeAsFloat( tremoloNode, "duty", NO_VALUE_SPECIFIED );
		float flatness = parser.getXMLAttributeAsFloat( tremoloNode, "flatness", NO_VALUE_SPECIFIED );
		float phase = parser.getXMLAttributeAsFloat( tremoloNode, "phase", NO_VALUE_SPECIFIED );
		float spread = parser.getXMLAttributeAsFloat( tremoloNode, "spread", NO_VALUE_SPECIFIED );

		if( frequency != NO_VALUE_SPECIFIED )
			setFrequency( frequency );
		if( depth != NO_VALUE_SPECIFIED )
			setDepth( depth );
		if( shape != NO_VALUE_SPECIFIED )
			setShape( shape );
		if( timeSkewing != NO_VALUE_SPECIFIED )
			setTimeSkewing( timeSkewing );
		if( duty != NO_VALUE_SPECIFIED )
			setDuty( duty );
		if( flatness != NO_VALUE_SPECIFIED )
			setFlatness( flatness );
		if( phase != NO_VALUE_SPECIFIED )
			setPhase( phase );
		if( spread != NO_VALUE_SPECIFIED )
			setSpread( spread );
	}
	//---------------------------------------------------
	void TremoloDSP::setFrequency( float frequency )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_FREQUENCY, frequency );
	}
	//---------------------------------------------------
	float TremoloDSP::getFrequency() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_FREQUENCY );
	}
	//---------------------------------------------------
	void TremoloDSP::setDepth( float depth )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_DEPTH, depth );
	}
	//---------------------------------------------------
	float TremoloDSP::getDepth() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_DEPTH );
	}
	//---------------------------------------------------
	void TremoloDSP::setShape( float shape )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_SHAPE, shape );
	}
	//---------------------------------------------------
	float TremoloDSP::getShape() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_SHAPE );
	}
	//---------------------------------------------------
	void TremoloDSP::setTimeSkewing( float skewing )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_SKEW, skewing );
	}
	//---------------------------------------------------
	float TremoloDSP::getTImeSkewing() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_SKEW );
	}
	//---------------------------------------------------
	void TremoloDSP::setDuty( float duty )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_DUTY, duty );
	}
	//---------------------------------------------------
	float TremoloDSP::getDuty() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_DUTY );
	}
	//---------------------------------------------------
	void TremoloDSP::setFlatness( float flatness )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_SQUARE, flatness );
	}
	//---------------------------------------------------
	float TremoloDSP::getFlatness() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_SQUARE );
	}
	//---------------------------------------------------
	void TremoloDSP::setPhase( float phase )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_PHASE, phase );
	}
	//---------------------------------------------------
	float TremoloDSP::getPhase() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_PHASE );
	}
	//---------------------------------------------------
	void TremoloDSP::setSpread( float spread )
	{
		m_dspImpl->setParameter( FMOD_DSP_TREMOLO_SPREAD, spread );	
	}
	//---------------------------------------------------
	float TremoloDSP::getSPread() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_TREMOLO_SPREAD );
	}
}