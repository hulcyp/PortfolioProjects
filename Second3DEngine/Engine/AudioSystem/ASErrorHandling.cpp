#include "ASErrorHandling.h"
#include <Assertion.h>
#include <fmod_errors.h>

namespace Monky
{
	void asErrorCheck( FMOD_RESULT result )
	{
		assertion( result == FMOD_OK, "FMOD error! (%d) %s", result, FMOD_ErrorString( result ) );
	}
}