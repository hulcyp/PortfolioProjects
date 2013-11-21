#include "PitchShiftDSP.h"
#include "DSPImpl.h"

namespace Monky
{
	PitchShiftDSP::PitchShiftDSP()
		:	DSP( MONKY_DSP_TYPE_PITCHSHIFT )
	{}
	PitchShiftDSP::PitchShiftDSP( XMLParser& parser, XMLNode* pitchShiftNode )
		:	DSP( MONKY_DSP_TYPE_PITCHSHIFT )
	{
		const float NO_VALUE_SPECIFIED = -1000000000.0f;
		parser.validateXMLAttributes( pitchShiftNode, "", "type,pitch,windowSize,maxChannels" );
		float pitch = parser.getXMLAttributeAsFloat( pitchShiftNode, "pitch", NO_VALUE_SPECIFIED );
		std::string windowSizeStr = parser.getXMLAttributeAsString( pitchShiftNode, "windowSize", "FFT_1024" );
		int maxChannels = parser.getXMLAttributeAsInt( pitchShiftNode, "maxChannels", (int)NO_VALUE_SPECIFIED );

		if( pitch != NO_VALUE_SPECIFIED )
			setPitch( pitch );
		if( maxChannels != (int)NO_VALUE_SPECIFIED )
			setMaxChannels( maxChannels );
		setFFTWinowSize( getWindowSizeFromString( windowSizeStr ) );		
	}
	//-----------------------------------------------
	void PitchShiftDSP::setPitch( float pitch )
	{
		m_dspImpl->setParameter( FMOD_DSP_PITCHSHIFT_PITCH, pitch );
	}
	//-----------------------------------------------
	float PitchShiftDSP::getPitch( float pitch )
	{
		return m_dspImpl->getParameter( FMOD_DSP_PITCHSHIFT_PITCH );
	}
	//-----------------------------------------------
	void PitchShiftDSP::setFFTWinowSize( FFTWindowSize windowSize )
	{
		m_dspImpl->setParameter( FMOD_DSP_PITCHSHIFT_FFTSIZE, (float)windowSize );
	}
	//-----------------------------------------------
	PitchShiftDSP::FFTWindowSize PitchShiftDSP::getFFTWindowSize() const
	{
		return (FFTWindowSize)static_cast<int>( m_dspImpl->getParameter( FMOD_DSP_PITCHSHIFT_FFTSIZE ) );
	}
	//-----------------------------------------------
	void PitchShiftDSP::setMaxChannels( int channels )
	{
		m_dspImpl->setParameter( FMOD_DSP_PITCHSHIFT_MAXCHANNELS, (float)channels );
	}
	//-----------------------------------------------
	int PitchShiftDSP::getMaxChannels() const
	{
		return (int)m_dspImpl->getParameter( FMOD_DSP_PITCHSHIFT_MAXCHANNELS );
	}
	//-----------------------------------------------
	PitchShiftDSP::FFTWindowSize PitchShiftDSP::getWindowSizeFromString( const std::string& windowSize )
	{
		static std::map< std::string, FFTWindowSize > windowSizeMap;
		if( windowSizeMap.empty() )
		{
			windowSizeMap[ "FFT_256" ] = FFT_256;
			windowSizeMap[ "FFT_512" ] = FFT_512;
			windowSizeMap[ "FFT_1024" ] = FFT_1024;
			windowSizeMap[ "FFT_2048" ] = FFT_2048;
			windowSizeMap[ "FFT_4096" ] = FFT_4096;
		}
		
		std::string windowSizeUpper = windowSize;
		stringToUpperCase( windowSizeUpper );
		auto iter = windowSizeMap.find( windowSizeUpper );
		if( iter != windowSizeMap.end() )
			return iter->second;
		else
			return FFT_1024;
	}
}