#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "MathUtils.h"
#include <ErrorHandlingUtils.h>

namespace pdh
{
	template< typename T, typename precision >
	struct State
	{
		T s[2];

		State( T x = T(), T y = T() );
		State operator*( const precision val );
		State operator+( const State& rhs );
		State& operator=( const State& rhs );
		T& operator[]( int i );

	};

	template < typename T, typename precision >
	State< T, precision > fwdEuler( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt );

	template< typename T, typename precision >
	State< T, precision > velVerlet( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt );
	
	template< typename T, typename precision >
	State< T, precision > rk2Midpoint( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt );

	template< typename T, typename precision >
	State< T, precision > rk2Trapezoidal( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt );
	
	template< typename T, typename precision >
	State< T, precision > rk4( State< T, precision > s, State< T, precision > op( State< T, precision >, precision ), precision t, precision dt );

}

#include "StateSpaceNumericalMethods.inl"