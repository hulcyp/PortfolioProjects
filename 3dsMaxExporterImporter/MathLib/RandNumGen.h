#pragma once

namespace pdh
{
	class RandNumGen
	{
	public:
		static void seed( unsigned int s );		
		static float randInRangeFloat( float min, float max );
		static float randInRangeUnit();
		static int randInRangeInt( int min, int max );

	};

}