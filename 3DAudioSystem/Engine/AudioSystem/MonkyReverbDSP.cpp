#include "MonkyReverbDSP.h"
#include "MonkyReverbDSPImpl.h"
#include "AudioSystem.h"

namespace Monky
{
	MonkyReverbDSP::MonkyReverbDSP()
		:	CustomDSP( "Monky Reverb" )
	{}
	//----------------------------------------------------------------------
	MonkyReverbDSP::~MonkyReverbDSP()
	{}
	//----------------------------------------------------------------------
	void MonkyReverbDSP::initialize()
	{
		if( !m_dspImpl )
		{
			m_dspImpl = new MonkyReverbDSPImpl( m_dspName );
			AudioSystem::getInstance()->createCustomDSP( *this );
		}
	}
}