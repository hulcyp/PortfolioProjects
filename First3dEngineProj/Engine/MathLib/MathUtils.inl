
namespace pdh
{
	template< class T >
	T randInRange( T min, T max )
	{
		const T r = rand() / static_cast< T >( RAND_MAX );
		return min + ( max - min ) * r;
	} 

	template < class T>
	T wrap( T x, T min, T max )
	{
		assert( max >= min );
		T range_size = max - min + 1;
		if( x < min )
		{
			x += range_size * ( ( min - x ) / range_size + 1 );
		}
		
		return min + ( x - min ) % range_size;
	}

	template < class T>
	T clamp( T x, T min, T max )
	{
		assert( max >= min );
		return x < min ? min : x > max ? max : x;
	}

	template< class T >
	bool isInRange( T x, T min, T max )
	{
		return ( x > min && x < max );
	}
	
	template< class T >
	T degreesToRadians( T degrees )
	{
		return degrees / static_cast<T>(180.0) * PI;
	}
	template< class T >
	T radianToDegrees( T radians)
	{
		return radians / PI * 180.0;
	}

	template< class T >
	T getOppositeSign( T x )
	{
		return static_cast<T>( x > 0 ? -1 : 1 );
	}

	template< typename T >
	int getSign( T x )
	{
		return ( x >= 0 ? 1 : -1 );
	}
}