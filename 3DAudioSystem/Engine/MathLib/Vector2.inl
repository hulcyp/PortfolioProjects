#include "RandNumGen.h"

namespace Monky
{
	template< typename T >
	Vector2<T>::Vector2( T x = 0, T y = 0 )
		:	x( x )
		,	y( y )
	{}

	template< typename T >
	Vector2<T>::Vector2( const Vector2<T>& vec )
	{
		x = vec.x;
		y = vec.y;
	}

	template< typename T >
	const Vector2<T>& Vector2<T>::operator=( const Vector2<T>& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		
		return *this;
	}

	template< typename T >
	T Vector2<T>::getComp( int i )
	{
		assert( i < 2 );
		return *(&x + i );
	}

	template< typename T >
	T& Vector2<T>::operator[]( int i )
	{
		assert( i < 2 );
		return *(&x + i );
	}
	
	template< typename T >
	const T& Vector2<T>::operator[]( int i ) const
	{
		assert( i < 2 );
		return *(&x + i );
	}

	template< typename T >
	T Vector2<T>::dot( const Vector2<T>& vec ) const
	{
		return ( x * vec.x + y * vec.y );
	}

	template< typename T >
	T Vector2<T>::norm() const
	{
		return std::sqrt( this->dot( *this ) );
	}

	template< typename T >
	T Vector2<T>::length() const
	{
		return norm();
	}

	template< typename T >
	T Vector2<T>::lengthSqrd() const
	{
		return this->dot( *this );
	}

	template< typename T >
	void Vector2<T>::normalize()
	{
		const T n = norm();
		if( n != 0 )
		{
			x /= n;
			y /= n;
		}
	}

	template< typename T >
	Vector2<T> Vector2<T>::normalized()
	{
		Vector2<T> temp( *this );
		temp.normalize();
		return temp;
	}

	template< typename T >
	bool Vector2<T>::isParallel( const Vector2<T>& vec ) const
	{	
		float cosAngle = dot( vec ) / ( norm() * vec.norm() );
		return ( cosAngle == 1 || cosAngle == -1 );
	}

	template< typename T >
	void Vector2<T>::flipAxis( size_t index )
	{
		assert( index < 2 );
		*(&x + i ) = -*(&x + i );		
	}

	template< typename T >
	Vector2<T> Vector2<T>::operator+( const Vector2<T>& rhs ) const
	{
		Vector2<T> temp;

		temp.x = x + rhs.x;
		temp.y = y + rhs.y;

		return temp;
	}

	template< typename T >
	Vector2<T> Vector2<T>::operator+( const T val ) const
	{
		Vector2<T> temp;
		temp.x = x + val;
		temp.y = y + val;
		
		return temp;
	}

	template< typename T >
	Vector2<T> Vector2<T>::operator-( const Vector2<T>& rhs ) const
	{
		Vector2<T> temp;

		temp.x = x - rhs.x;
		temp.y = y- rhs.y;

		return temp;
	}

	template< typename T >
	Vector2<T> Vector2<T>::operator-( const T val ) const
	{
		Vector2<T> temp;

		temp.x = x - val;
		temp.y = y - val;

		return temp;
	}

	template< typename T >
	Vector2<T>& Vector2<T>::operator-()
	{
		x = -x;
		y = -y;

		return *this;
	}
	
	template< typename T >
	Vector2<T> Vector2<T>::operator*( const Vector2<T>& rhs ) const
	{
		Vector2<T> temp;
						
		temp.x = x*rhs.x;
		temp.y = y*rhs.y;

		return temp;
	}

	template< typename T >
	Vector2<T> Vector2<T>::operator*( T val ) const
	{
		Vector2<T> temp;
						
		temp.x = x*val;
		temp.y = y*val;

		return temp;
	}

	template< typename T >
	Vector2<T> Vector2<T>::operator/( const Vector2<T>& rhs ) const
	{
		Vector2<T> temp;
						
		temp.x = x/rhs.x;
		temp.y = y/rhs.y;

		return temp;
	}

	template< typename T >
	Vector2<T> Vector2<T>::operator/( const T val ) const
	{
		Vector2<T> temp;
						
		temp.x = x/val;
		temp.y = y/val;

		return temp;
	}

	template< typename T >
	Vector2<T>& Vector2<T>::operator+=( const Vector2<T>& rhs )
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	template< typename T >
	Vector2<T>& Vector2<T>::operator+=( const T val )
	{
		x += val;
		y += val;

		return *this;
	}

	template< typename T >
	Vector2<T>& Vector2<T>::operator-=( const Vector2<T>& rhs )
	{
		x-= rhs.x;
		y-= rhs.y;

		return *this;
	}

	template< typename T >
	Vector2<T>& Vector2<T>::operator-=( const T val )
	{
		x-= val;
		y-= val;

		return *this;
	}
	
	template< typename T >
	Vector2<T>& Vector2<T>::operator*=( const Vector2<T>& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;

		return *this;
	}

	template< typename T >
	Vector2<T>& Vector2<T>::operator*=( const T val )
	{
		x *= val;
		y *= val;

		return *this;
	}
	
	template< typename T >
	Vector2<T>& Vector2<T>::operator/=( const Vector2<T>& rhs )
	{
		x /= rhs.x;
		y /= rhs.y;

		return *this;
	}

	template< typename T >
	Vector2<T>& Vector2<T>::operator/=( const T val )
	{
		x /= val;
		y /= val;

		return *this;
	}

	template< typename T >
	bool Vector2<T>::operator==( const Vector2<T> &rhs ) const
	{
		return ( ( x== rhs.x ) && ( y== rhs.y ) );
	}

	template< typename T >
	bool Vector2<T>::operator!=( const Vector2<T> &rhs ) const
	{
		return ( ( x != rhs.x ) || ( y!= rhs.y) );
	}

	template< typename T >
	Vector2<T> Vector2<T>::xx()
	{
		return Vector2<T>( x, x );
	}
	template< typename T >
	Vector2<T> Vector2<T>::yx()
	{
		return Vector2<T>( y, x );
	}
	template< typename T >
	Vector2<T> Vector2<T>::yy()
	{
		return Vector2<T>( y, y );
	}
	template< typename T >
	T Vector2<T>::aimedAngle() const
	{
		return std::atan2( y, x );
	}
	template< typename T >
	void Vector2<T>::scaleToLength( T newLength )
	{
		T length = length();

		if( length > static_cast<T>( 0 ) )
		{
			T scalar = newLength / length;
			x *= scalar;
			y *= scalar;
		}
		else
		{
			x = newLength;
			y = 0;
		}
	}
	template< typename T >
	Vector2<T> Vector2<T>::scaledToLength( T newLength )
	{
		Vector2<T> temp = *this;
		temp.scaledToLength( newLength );
		return temp;
	}
	template< typename T >
	bool Vector2<T>::isNormalized( T epsilon ) const
	{
		return std::abs( lengthSqrd() - static_cast<T>( 1 ) ) <= epsilon;
	}
	template< typename T >
	bool Vector2<T>::isZero( T epsilon ) const
	{
		return std::abs( lengthSqrd() ) <= epsilon;
	}
	template< typename T >
	bool Vector2<T>::isPerpendicular( const Vector2<T>& other, T epsilon ) const
	{
		return std::abs( dot( other ) ) <= epsilon;
	}
	template< typename T >
	std::string Vector2<T>::toString() const
	{
		std::stringstream ss;
		ss << "(" << x << ", " << y << ")";
		return std::string( ss.str() );
	}
	template< typename T >
	std::string Vector2<T>::serializeToSTring() const
	{
		std::stringstream ss;;
		ss << x << "," << y;
		std::string str;
		ss >> str;
		return str;
	}
	template< typename T >
	T Vector2<T>::sumOfElements() const
	{
		return x + y;
	}
	template< typename T >
	T Vector2<T>::productOfElements() const
	{
		return x * y;
	}
	template< typename T >
	T Vector2<T>::averageOfElements() const
	{
		return ( x + y ) / static_cast<T>( 2 );
	}
	template< typename T >
	T Vector2<T>::minElement() const
	{
		return std::min( x, y );
	}
	template< typename T >
	int Vector2<T>::minElementIndex() const
	{
		return ( x <= y ) ? 0 : 1;
	}
	template< typename T >
	T Vector2<T>::maxElement() const
	{
		return std::max( x, y );
	}
	template< typename T >
	int Vector2<T>::maxElementIndex() const
	{
		return ( x >= y ) ? 0 : 1;
	}
	template< typename T >
	Vector2<T> Vector2<T>::abs() const
	{
		return Vector2<T>( std::abs( x ), std::abs( y ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::neg() const
	{
		return Vector2<T>( -x, -y );
	}
	template< typename T >
	Vector2<T> Vector2<T>::recip() const
	{
		return Vector2<T>( static_cast<T>(1)/x, static_cast<T>(1)/y );
	}
	template< typename T >
	Vector2<T> Vector2<T>::min( T ceil ) const
	{
		return Vector2<T>( std::min( x, ceil ), std::min( y, ceil ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::min( const Vector2<T>& ceil ) const
	{
		return Vector2<T>( std::min( x, ceil.x ), std::min( y, ceil.y ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::max( T floor ) const
	{
		return Vector2<T>( std::max( x, floor ), std::max( y, floor ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::max( const Vector2<T>& floor ) const
	{
		return Vector2<T>( std::max( x, floor.x ), std::max( y, floor.y ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::clamp( T floor, T ceil ) const
	{
		return Vector2<T>( MathFuncs<T>::clamp( x, floor, ceil ), MathFuncs<T>::clamp( y, floor, ceil ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::clamp( const Vector2<T>& floor, const Vector2<T>& ceil ) const
	{
		return Vector2<T>( MathFuncs<T>::clamp( x, floor.x, ceil.x ), MathFuncs<T>::clamp( y, floor.y, ceil.y ) );
	}
	template< typename T >
	T Vector2<T>::distance( const Vector2<T>& point ) const
	{
		return std::sqrt( distanceSqrd() );
	}
	template< typename T >
	T Vector2<T>::distanceSqrd( const Vector2<T>& point ) const
	{
		T dx = x - point.x;
		T dy = y - point.y;
		return dx*dx + dy*dy;
	}
	template< typename T >
	Vector2<T> Vector2<T>::perpindicular() const
	{
		return Vector2<T>( -y, x );
	}
	template< typename T >
	Vector2<T> Vector2<T>::reflect( const Vector2<T>& normal ) const
	{
		return static_cast<T>(2) * projectToNorm( normal ) - *this;
	}
	template< typename T >
	Vector2<T> Vector2<T>::refract( const Vector2<T>& normal, T negSideRefractionIndex, T posSideRefractionIndex ) const
	{
		T one = static_cast<T>( 1 );
		T n = negSideRefractionIndex / posSideRefractionIndex;
		T cosI = dot( normal );
		T sinT2 = n*n*( one - cosI * cosI );
		if( sinT2 > one )	// Total internal reflection occurs
			return (-*this).reflect( normal );

		return ( *this ) * n - ( n + std::sqrt( one - sinT2 ) ) * normal;
	}
	template< typename T >
	Vector2<T> Vector2<T>::projectTo( const Vector2<T>& direction ) const
	{
		ASSERT( !direction.isZero() );
		return direction * dot( direction ) / direction.lengthSqrd();
	}
	template< typename T >
	Vector2<T> Vector2<T>::projectToNorm( const Vector2<T>& direction ) const
	{
		ASSERT( direction.isNormalized() );
		return direction * dot( direction );
	}
	template< typename T >
	T Vector2<T>::angleBetween( const Vector2<T>& other ) const
	{
		return std::acos( dot( other ) ) / std::sqrt( lengthSqrd() * other.lengthSqrd() );
	}
	template< typename T >
	T Vector2<T>::angleBetweenNorm( const Vector2<T>& normalizedVec ) const
	{
		ASSERT( isNormalized() );
		ASSERT( normalizedVec.isNormalized() );
		return std::acos( dot( normalizedVec ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::lerp( const Vector2<T>& b, T t ) const
	{
		T one = static_cast<T>( 1 );
		ASSERT( static_cast<T>( 0 ) <= t && t <= one );
		return *this * ( one - t ) + b * t;
	}
	template< typename T >
	void Vector2<T>::decompose( const Vector2<T>& direction, Vector2<T>& outParallel, Vector2<T>& outPerp ) const
	{
		ASSERT( direction.isNormalized() ):
		outParallel = direction * dot( direction );
		outPerp = *this - outParallel;
	}

	template< typename T >
	Vector2<T> Vector2<T>::randDir()
	{
		const float rads = RandNumGen::randInRangeUnit() * MathFuncs<T>::PI * 2;
		return Vector2<T>( std::cos( rads ), std::sin( rads ) );
	}
	template< typename T >
	void Vector2<T>::orthogonalize( const Vector2<T>& a, Vector2<T>& b )
	{
		ASSERT( !a.isZero() );
		b -= a * ( a.dot( b ) / a.length() );
	}
	template< typename T >
	void Vector2<T>::othonormalize( Vector2<T>& a, Vector2<T>& b )
	{
		ASSERT( !a.isZero() );
		a.normalize();
		b -= a * ( a.dot( b ) );
	}
	template< typename T >
	Vector2<T> Vector2<T>::fromString( const std::string& str )
	{
		std::vector< std::string > tokens;
		stringTokenizer( str, tokens, "," );
		Vector2<T> temp;
		for( size_t i = 0; i < 2; ++i )
		{
			if( !stringToType( tokens[i], &temp[i] ) )
			{
				temp[i] = 0;
			}
		}
		return temp;
	}

	template< typename T >
	std::ostream& operator<<( std::ostream& os, const Vector2<T>& vec)
	{
		os << vec.x << "," << vec.y;
		return os;
	}

	template< typename T >
	Vector2<T> parseStringToVec2( const std::string& str, const Vector2<T>& defaultValue = Vector2<T>::ZERO )
	{
		std::vector< std::string > tokens;
		stringTokenizer( str, tokens, "," );
		Vector2<T> temp = defaultValue;
		size_t numValues = tokens.size();

		if( numValues < 2 )
			throw std::exception( "Not enough values to fill a Vector2" );

		for( size_t i = 0; i < numValues && i < 2; ++i )
		{
			if( !stringToType( tokens[i], &temp[i] ) )
			{
				temp[i] = defaultValue[i];
			}
		}
		return temp;
	}

	template< typename T >
	const Vector2<T> Vector2<T>::ZERO = Vector2<T>( 0, 0 );
	template< typename T >
	const Vector2<T> Vector2<T>::ONE = Vector2<T>( 1, 1 );
	template< typename T >
	const Vector2<T> Vector2<T>::UNIT_X = Vector2<T>( 1, 0 );
	template< typename T >
	const Vector2<T> Vector2<T>::UNIT_Y = Vector2<T>( 0, 1 );
}