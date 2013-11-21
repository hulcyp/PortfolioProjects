#include "SFXReverbDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	SFXReverbDSP::SFXReverbDSP()
		:	DSP( MONKY_DSP_TYPE_SFXREVERB )
	{}
	//--------------------------------------------------------------
	SFXReverbDSP::SFXReverbDSP( XMLParser& parser, XMLNode* sfxReverbNode )
		:	DSP( MONKY_DSP_TYPE_SFXREVERB )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( sfxReverbNode, "", 
			"type,dryLevel,LFRoomLevel,HFRoomLevel,decayLFTimeSec,decayHFRatio,reflectionLevel,reflectionDelaySec,reverbLevel,reverbDelaySec,diffusionPercentage,densityPercentage,HFReference,roomLFLevel,LFReference" );

		float dryLevel				= parser.getXMLAttributeAsFloat( sfxReverbNode, "dryLevel", NO_VALUE_SPECIFIED );
		float LFRoomLevel			= parser.getXMLAttributeAsFloat( sfxReverbNode, "LFRoomLevel", NO_VALUE_SPECIFIED );
		float HFRoomLevel			= parser.getXMLAttributeAsFloat( sfxReverbNode, "HFRoomLevel", NO_VALUE_SPECIFIED );
		float decayLFTimeSec		= parser.getXMLAttributeAsFloat( sfxReverbNode, "decayLFTimeSec", NO_VALUE_SPECIFIED );
		float decayHFRatio			= parser.getXMLAttributeAsFloat( sfxReverbNode, "decayHFRatio", NO_VALUE_SPECIFIED );
		float reflectionLevel		= parser.getXMLAttributeAsFloat( sfxReverbNode, "reflectionLevel", NO_VALUE_SPECIFIED );
		float reflectionDelaySec	= parser.getXMLAttributeAsFloat( sfxReverbNode, "reflectionDelaySec", NO_VALUE_SPECIFIED );
		float reverbLevel			= parser.getXMLAttributeAsFloat( sfxReverbNode, "reverbLevel", NO_VALUE_SPECIFIED );
		float reverbDelaySec		= parser.getXMLAttributeAsFloat( sfxReverbNode, "reverbDelaySec", NO_VALUE_SPECIFIED );
		float diffusionPercentage	= parser.getXMLAttributeAsFloat( sfxReverbNode, "diffusionPercentage", NO_VALUE_SPECIFIED );
		float densityPercentage		= parser.getXMLAttributeAsFloat( sfxReverbNode, "densityPercentage", NO_VALUE_SPECIFIED );
		float HFReference			= parser.getXMLAttributeAsFloat( sfxReverbNode, "HFReference", NO_VALUE_SPECIFIED );
		float roomLFLevel			= parser.getXMLAttributeAsFloat( sfxReverbNode, "roomLFLevel", NO_VALUE_SPECIFIED );
		float LFReference			= parser.getXMLAttributeAsFloat( sfxReverbNode, "LFReference", NO_VALUE_SPECIFIED );

		if( dryLevel != NO_VALUE_SPECIFIED )	
			setDryLevel( dryLevel );
		if( LFRoomLevel	!= NO_VALUE_SPECIFIED )
			setLowFreqRoomLevel( LFRoomLevel ); 
		if( HFRoomLevel	!= NO_VALUE_SPECIFIED )
			setHighFreqRoomLevel( HFRoomLevel );
		if( decayLFTimeSec != NO_VALUE_SPECIFIED )
			setDecayLFTimeSec( decayLFTimeSec );
		if( decayHFRatio != NO_VALUE_SPECIFIED )
			setDecayHFRatio( decayHFRatio );
		if( reflectionLevel != NO_VALUE_SPECIFIED )
			setReflectionLevel( reflectionLevel );
		if( reflectionDelaySec != NO_VALUE_SPECIFIED )
			setReflectionDelaySec( reflectionDelaySec );
		if( reverbLevel	!= NO_VALUE_SPECIFIED )
			setReverbLevel( reverbLevel );
		if( reverbDelaySec != NO_VALUE_SPECIFIED )
			setReverbDelaySec( reverbDelaySec );
		if( diffusionPercentage	!= NO_VALUE_SPECIFIED )
			setDiffusionPercentage( diffusionPercentage );
		if( densityPercentage != NO_VALUE_SPECIFIED )
			setDensityPercentage( densityPercentage );
		if( HFReference	!= NO_VALUE_SPECIFIED )
			setHFReference( HFReference );
		if( roomLFLevel	!= NO_VALUE_SPECIFIED )
			setRoomLFLevel( roomLFLevel );
		if( LFReference	!= NO_VALUE_SPECIFIED )
			setLFReferenceLevel( LFReference );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setDryLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_DRYLEVEL, level );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getDryLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_DRYLEVEL );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setLowFreqRoomLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_ROOM, level );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getLowFreqRoomLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_ROOM );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setHighFreqRoomLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_ROOMHF, level );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getHighFreqRoomLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_ROOMHF );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setDecayLFTimeSec( float timeSec )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_DECAYTIME, timeSec );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getDecayLFTimeSec() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_DECAYTIME );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setDecayHFRatio( float ratio )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_DECAYHFRATIO, ratio );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getDecayHFRatio() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_DECAYHFRATIO );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setReflectionLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_REFLECTIONSLEVEL, level );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getReflectionLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_REFLECTIONSLEVEL );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setReflectionDelaySec( float timeSec )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_REFLECTIONSDELAY, timeSec );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getReflectionDelaySec() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_REFLECTIONSDELAY );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setReverbLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_REVERBLEVEL, level );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getReverbLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_REVERBLEVEL );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setReverbDelaySec( float timeSec )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_REVERBDELAY, timeSec );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getReverbDelaySec() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_REVERBDELAY );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setDiffusionPercentage( float percent )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_DIFFUSION, percent );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getDiffusionPercentage() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_DIFFUSION );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setDensityPercentage( float percent )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_DENSITY, percent );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getDensityPercentage() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_DENSITY );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setHFReference( float reference )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_HFREFERENCE, reference );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getHFReference() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_HFREFERENCE );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setRoomLFLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_ROOMLF, level );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP:: getRoomLFLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_ROOMLF );
	}
	//--------------------------------------------------------------
	void SFXReverbDSP::setLFReferenceLevel( float level )
	{
		m_dspImpl->setParameter( FMOD_DSP_SFXREVERB_LFREFERENCE, level );
	}
	//--------------------------------------------------------------
	float SFXReverbDSP::getLFReferenceLevel() const
	{
		return m_dspImpl->getParameter( FMOD_DSP_SFXREVERB_LFREFERENCE );
	}
}