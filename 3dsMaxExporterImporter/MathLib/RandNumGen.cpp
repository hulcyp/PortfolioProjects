#include "RandNumGen.h"
#include <cstdlib>

namespace pdh
{	
	void RandNumGen::seed( unsigned int s )
	{
		std::srand( s );
	}
	//-----------------------------------------------------
	float RandNumGen::randInRangeFloat( float min, float max )
	{
		const float r = rand() / static_cast< float >( RAND_MAX );
		return min + ( max - min ) * r;
	}
	//-----------------------------------------------------
	float RandNumGen::randInRangeUnit()
	{
		return randInRangeFloat( 0.0f, 1.0f );
	}
	//-----------------------------------------------------
	int RandNumGen::randInRangeInt( int min, int max )
	{
		float r = randInRangeFloat( 0.0f, 1.0f );
		return static_cast<int>( min + ( max - min ) * r );
	}
}