#include "DistortionDSP.h"
#include "DSPImpl.h"

namespace Monky
{	
	DistortionDSP::DistortionDSP()
		:	DSP( MONKY_DSP_TYPE_DISTORTION )
	{}
	//-----------------------------------------------------------
	DistortionDSP::DistortionDSP( XMLParser& parser, XMLNode* distortionNode )
		:	DSP( MONKY_DSP_TYPE_DISTORTION )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( distortionNode, "", "type,level" );
		float level = parser.getXMLAttributeAsFloat( distortionNode, "level", NO_VALUE_SPECIFIED );
		if( level != NO_VALUE_SPECIFIED )
			setDistortionLevel( level );
	}
	//-----------------------------------------------------------
	void DistortionDSP::setDistortionLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_DISTORTION_LEVEL, level );
	}
	//-----------------------------------------------------------
	float DistortionDSP::getDistortionLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_DISTORTION_LEVEL );
	}
}