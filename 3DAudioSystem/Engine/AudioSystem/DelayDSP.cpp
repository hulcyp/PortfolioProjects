#include "DelayDSP.h"
#include "DSPImpl.h"
#include <MathFuncs.h>

namespace Monky
{
	DelayDSP::DelayDSP()
		:	DSP( MONKY_DSP_TYPE_DELAY )
	{}
	//------------------------------------------------------------------------
	DelayDSP::DelayDSP( XMLParser& parser, XMLNode* delayNode )
		:	DSP( MONKY_DSP_TYPE_DELAY )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLChildElements( delayNode, "ChannelDelay", "" );
		parser.validateXMLAttributes( delayNode, "", "type,maxDelayMS" );
		float maxDelayMS = parser.getXMLAttributeAsFloat( delayNode, "maxDelayMS", NO_VALUE_SPECIFIED );

		for( const XMLNode* channelNode = delayNode->FirstChildElement( "ChannelDelay" ); delayNode != nullptr; delayNode = delayNode->NextSiblingElement( "ChannelDelay" ) )
		{
			parser.validateXMLAttributes( channelNode, "channel,delayMS", "" );
			int channel = parser.getXMLAttributeAsInt( channelNode, "channel", -1 );
			int delayMS = parser.getXMLAttributeAsInt( channelNode, "delayMS", 0 );
			if( channel > 0 )
				setChannelDelayMS( channel, delayMS );
		}
		
	}
	//------------------------------------------------------------------------
	void DelayDSP::setChannelDelayMS( int channel, int delayMS )
	{
		int clampedChannel = MathFuncs<int>::clamp( channel, 0, 14 );
		m_dspImpl->setParameter( FMOD_DSP_DELAY_CH0 + clampedChannel, (float)delayMS );
	}
	//------------------------------------------------------------------------
	int DelayDSP::getChannelDelayMS( int channel ) const
	{
		int clampedChannel = MathFuncs<int>::clamp( channel, 0, 14 );
		return (int)m_dspImpl->getParameter( FMOD_DSP_DELAY_CH0 + clampedChannel );
	}
	//------------------------------------------------------------------------
	void DelayDSP::setMaxDelayMS( int delayMS )
	{
		m_dspImpl->setParameter( FMOD_DSP_DELAY_MAXDELAY, (float)delayMS );
	}
	//------------------------------------------------------------------------
	int DelayDSP::getMaxDelayMS() const
	{
		return (int)m_dspImpl->getParameter( FMOD_DSP_DELAY_MAXDELAY );
	}
}