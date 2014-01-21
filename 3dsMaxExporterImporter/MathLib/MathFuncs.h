#pragma once
#include <cassert>

namespace pdh
{
	template< typename Real >
	class MathFuncs
	{
	public:
		static Real degreesToRadians( Real degrees );
		static Real radiansToDegrees( Real radians );
		static Real fractional( Real x );
		static Real clamp( Real x, Real min, Real max );
		static void swap( Real& a, Real& b );
		static Real wrap( Real x, Real min, Real max );
		static Real getOppositeSign( Real x );
		static int getSign( Real x );
		static bool isInRange( Real x, Real min, Real max );
		static const Real PI;
		static const Real EPSILON;
	};

	template< typename Real >
	Real MathFuncs<Real>::degreesToRadians( Real degrees )
	{
		return degrees / static_cast<Real>(180) * PI;
	}

	template< typename Real >
	Real MathFuncs<Real>::radiansToDegrees( Real radians )
	{
		return radians / PI * static_cast<Real>( 180 );
	}

	template< typename Real >
	Real MathFuncs<Real>::fractional( Real x )
	{
		return x - std::floor( x );
	}
	
	template< typename Real >
	Real MathFuncs<Real>::clamp( Real x, Real min, Real max )
	{
		assert( max >= min );
		return x < min ? min : ( x > max ? max : x );
	}
	
	template< typename Real >
	void MathFuncs<Real>::swap( Real& a, Real& b )
	{
		Real temp = a;
		a = b;
		b = temp;
	}

	template< typename Real >
	Real MathFuncs<Real>::wrap( Real x, Real min, Real max )
	{
		assert( max >= min );
		Real range_size = max - min + 1;
		if( x < min )
		{
			x += range_size * ( ( min - x ) / range_size + 1 );
		}
		
		return min + ( x - min ) % range_size;
	}

	template< typename Real >
	Real MathFuncs<Real>::getOppositeSign( Real x )
	{
		return static_cast<Real>( x > 0 ? -1 : 1 );
	}

	template< typename Real >
	int MathFuncs<Real>::getSign( Real x )
	{
		return ( x >= 0 ? 1 : -1 );
	}

	template< typename Real >
	bool MathFuncs<Real>::isInRange( Real x, Real min, Real max )
	{
		return ( min <= x && x <= max );
	}
	
	template< typename Real >
	const Real MathFuncs<Real>::PI = static_cast<Real>( 3.141592654 );
	template< typename Real >
	const Real MathFuncs<Real>::EPSILON = 1e-6;

}