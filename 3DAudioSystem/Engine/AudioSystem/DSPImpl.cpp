#include "DSPImpl.h"
#include "ASErrorHandling.h"
#include "CommonEngineIncludes.h"

namespace Monky
{
	DSPImpl::DSPImpl()
		:	m_dsp( nullptr )
	{}
	//--------------------------------------------------
	DSPImpl::~DSPImpl()
	{
		if( m_dsp )
			m_dsp->release();
		SAFE_DELETE( m_dsp );
	}
	//--------------------------------------------------
	void DSPImpl::setParameter( int index, float value )
	{
		m_dsp->setParameter( index, value );
	}
	//--------------------------------------------------
	float DSPImpl::getParameter( int index ) const
	{
		float value = 0.0f;
		char str[16];
		FMOD_RESULT result = m_dsp->getParameter( index, &value, str, 16 );
		asErrorCheck( result );
		return value;
	}
}