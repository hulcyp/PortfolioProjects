#include "FixedPointMath.h"

namespace Monky
{
	const unsigned int NUM_OF_INDEXED_TRIG_FUNC = (unsigned int)( ( 360 << 16 ) - 0 );
	FixedPoint g_sineMap[ NUM_OF_INDEXED_TRIG_FUNC ];
	bool g_trigFunctionsLoaded = false;

	void generateSineFunctionData()
	{
		for( unsigned int i = 0; i < NUM_OF_INDEXED_TRIG_FUNC; ++i )
		{
			g_sineMap[i] = FixedPoint( std::sin( MathFuncs<float>::degreesToRadians( (float)i ) ) );
		}

		getFileManager().writeDataToFile( "math/sin.bin", (char*)g_sineMap, NUM_OF_INDEXED_TRIG_FUNC * sizeof( FixedPoint ) );
	}
	//----------------------------------------------------------------------
	void loadSineFunctionData()
	{
		int bufferSize = -1;
		char* buffer = getFileManager().readDataFromFile( "math/sin.bin", bufferSize );
		if( bufferSize != -1 )
			std::memcpy( g_sineMap, buffer, NUM_OF_INDEXED_TRIG_FUNC * sizeof( FixedPoint ) );
		else
		{
			generateSineFunctionData();
		}
		g_trigFunctionsLoaded = true;
	}
	//----------------------------------------------------------------------
	FixedPoint sin( FixedPoint angleInDegrees )
	{
		if( !g_trigFunctionsLoaded )
		{
			loadSineFunctionData();
		}
		while( angleInDegrees > 360 )
		{
			angleInDegrees -= 360;
		}
		while( angleInDegrees < 0 )
		{
			angleInDegrees += 360;
		}

		return g_sineMap[ angleInDegrees ];
	}
	//----------------------------------------------------------------------
	FixedPoint cos( FixedPoint angleInDegrees )
	{
		return sin( 90 - angleInDegrees );
	}
}