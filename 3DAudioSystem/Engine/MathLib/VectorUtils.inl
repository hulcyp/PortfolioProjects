
namespace Monky
{
	template< typename T >
	Vector3<T> randVec3InRange( T min, T max )
	{
		T x = randInRange( min, max );
		T y = randInRange( min, max );
		T z = randInRange( min, max );
		return Vector3<T>( x, y, z );
	}

	template< typename T >
	Vector2<T> randVec2InRange( T min, T max )
	{
		T x = randInRange( min, max );
		T y = randInRange( min, max );
		return Vector2<T>( x, y );
	}	
}