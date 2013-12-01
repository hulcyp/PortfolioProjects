#pragma once

namespace pdh
{
	const float PI = 3.141592654f;
	const float EPSILON = 1e-6f;
	
	template< class T >
	T randInRange( T min, T max );

	template < class T>
	T wrap( T x, T min, T max );
	
	template < class T>
	T clamp( T x, T min, T max );

	template< class T >
	bool isInRange( T x, T min, T max );

	template< class T >
	T degreesToRadians( T degrees );
	
	template< class T >
	T radianToDegrees( T radians );
	
	template< class T >
	T getOppositeSign( T x );

	template< typename T >
	int getSign( T x );
}

#include "MathUtils.inl"