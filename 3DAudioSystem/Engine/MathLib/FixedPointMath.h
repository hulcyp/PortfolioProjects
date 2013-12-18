#pragma once
#include <limits.h>
#include "FixedPoint.h"
#include "FileManager.h"

namespace Monky
{	
	void generateSineFunctionData();
	void loadSineFunctionData();

	FixedPoint sin( FixedPoint angleInDegrees );
	FixedPoint cos( FixedPoint angleInDegrees );
}