#pragma once
#include "MathUtils.h"

namespace pdh
{
	template< typename T >
	Vector3<T> randVec3InRange( T min, T max );

	template< typename T >
	Vector2<T> randVec2InRange( T min, T max );
}

#include "VectorUtils.inl"