
namespace pdh
{
	template< typename T >
	Vector4<T>::Vector4( T x, T y, T z, T w )
		:	x( x )
		,	y( y )
		,	z( z )
		,	w( w )
	{}

	template< typename T >
	Vector4<T>::Vector4( const Vector3<T>& vec, T w = 0 )
		:	x( vec.x )
		,	y( vec.y )
		,	z( vec.z )
		,	w( w )
	{}

	template< typename T >
	Vector4<T>::Vector4( const Vector4<T>& vec )
		:	x( vec.x )
		,	y( vec.y )
		,	z( vec.z )
		,	w( vec.w )
	{}

	template< typename T >
	Vector4<T>& Vector4<T>::operator=( const Vector4<T>& rhs )
	{
		if( this != &rhs )
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
		}
	}

	template< typename T >
	T& Vector4<T>::getComp( int i )
	{
		assert( i < 4 );
		return *( &x + i );
	}

	template< typename T >
	T& Vector4<T>::operator[]( int i )
	{
		assert( i < 4 );
		return *( &x + i );
	}

	template< typename T >
	const T& Vector4<T>::operator[]( int i ) const
	{
		assert( i < 4 );
		return *( &x + i );
	}
	
	template< typename T >
	T Vector4<T>::dot( const Vector4<T>& vec ) const
	{
		return ( x * vec.x + y * vec.y + z * vec.z + w * vec.w );
	}

	template< typename T >
	T Vector4<T>::length() const
	{
		return std::sqrt( dot( *this ) );
	}

	template< typename T >
	T Vector4<T>::lengthSqrd() const
	{
		return dot( *this );
	}

	template< typename T >
	void Vector4<T>::normalize()
	{
		const T length = length();
		if( length != 0 )
		{
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}
	}

	template< typename T >
	Vector4<T> Vector4<T>::normalized() const
	{
		Vector4<T> temp = *this;
		temp.normalize();
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator+( const Vector4<T>& rhs ) const
	{
		Vector4<T> temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		temp.z = z + rhs.z;
		temp.w = w + rhs.w;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator+( const T val ) const
	{
		Vector4<T> temp;
		temp.x = x + val;
		temp.y = y + val;
		temp.z = z + val;
		temp.w = w + val;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator-( const Vector4<T>& rhs ) const
	{
		Vector4<T> temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		temp.z = z - rhs.z;
		temp.w = w - rhs.w;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator-( const T val ) const
	{
		Vector4<T> temp;
		temp.x = x - val;
		temp.y = y - val;
		temp.z = z - val;
		temp.w = w - val;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator*( const Vector4<T>& rhs ) const
	{
		Vector4<T> temp;
		temp.x = x * rhs.x;
		temp.y = y * rhs.y;
		temp.z = z * rhs.z;
		temp.w = w * rhs.w;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator*( const T val ) const
	{
		Vector4<T> temp;
		temp.x = x * val;
		temp.y = y * val;
		temp.z = z * val;
		temp.w = w * val;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator/( const Vector4<T>& rhs ) const
	{
		Vector4<T> temp;
		temp.x = x / rhs.x;
		temp.y = y / rhs.y;
		temp.z = z / rhs.z;
		temp.w = w / rhs.w;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator/( const T val ) const
	{
		Vector4<T> temp;
		temp.x = x / val;
		temp.y = y / val;
		temp.z = z / val;
		temp.w = w / val;
		return temp;
	}

	template< typename T >
	Vector4<T> Vector4<T>::operator-() const
	{
		return Vector4<T>( -x, -y, -z, -w );
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator+=( const Vector4<T>& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator+=( const T real )
	{
		x += real;
		y += real;
		z += real;
		w += real;
		return *this;
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator-=( const Vector4<T>& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator-=( const T real )
	{
		x -= real;
		y -= real;
		z -= real;
		w -= real;
		return *this;
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator/=( const Vector4<T>& rhs )
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator/=( const T real )
	{
		x /= real;
		y /= real;
		z /= real;
		w /= real;
		return *this;
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator*=( const Vector4<T>& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	template< typename T >
	Vector4<T>& Vector4<T>::operator*=( const T real )
	{
		x *= real;
		y *= real;
		z *= real;
		w *= real;
		return *this;
	}

	template< typename T >
	bool Vector4<T>::operator==( const Vector4<T>& rhs ) const
	{
		return ( ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z ) && ( w == rhs.w ) );
	}

	template< typename T >
	bool Vector4<T>::operator!=( const Vector4<T>& rhs ) const
	{
		return ( ( x != rhs.x ) || ( y != rhs.y ) || ( z != rhs.z ) || ( w != rhs.w ) );
	}

	template< typename T >
	bool Vector4<T>::isParallel( const Vector4<T>& rhs, T epsilon = MathFuncs<T>::EPSILON ) const
	{
		return ( cross( rhs ).lengthSqrd() <= epsilon );
	}

	template< typename T >
	Vector2<T> Vector4<T>::xx() { return Vector2<T>( x, x ); }
	template< typename T >
	Vector2<T> Vector4<T>::xy() { return Vector2<T>( x, y ); }
	template< typename T >
	Vector2<T> Vector4<T>::yx() { return Vector2<T>( y, x ); }
	template< typename T >
	Vector2<T> Vector4<T>::yy() { return Vector2<T>( y, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::xxx() { return Vector3<T>( x, x, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::xxy() { return Vector3<T>( x, x, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::xxz() { return Vector3<T>( x, x, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::xyx() { return Vector3<T>( x, y, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::xyy() { return Vector3<T>( x, y, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::xyz() { return Vector3<T>( x, y, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::xzx() { return Vector3<T>( x, z, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::xzy() { return Vector3<T>( x, z, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::xzz() { return Vector3<T>( x, z, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::yxx() { return Vector3<T>( y, x, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::yxy() { return Vector3<T>( y, x, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::yxz() { return Vector3<T>( y, x, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::yyx() { return Vector3<T>( y, y, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::yyy() { return Vector3<T>( y, y, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::yyz() { return Vector3<T>( y, y, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::yzx() { return Vector3<T>( y, z, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::yzy() { return Vector3<T>( y, z, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::yzz() { return Vector3<T>( y, z, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::zxx() { return Vector3<T>( z, x, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::zxy() { return Vector3<T>( z, x, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::zxz() { return Vector3<T>( z, x, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::zyx() { return Vector3<T>( z, y, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::zyy() { return Vector3<T>( z, y, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::zyz() { return Vector3<T>( z, y, z ); }
	template< typename T >
	Vector3<T> Vector4<T>::zzx() { return Vector3<T>( z, z, x ); }
	template< typename T >
	Vector3<T> Vector4<T>::zzy() { return Vector3<T>( z, z, y ); }
	template< typename T >
	Vector3<T> Vector4<T>::zzz() { return Vector3<T>( z, z, z ); }

	template< typename T >
	void Vector4<T>::scaleToLength( T newLength )
	{
		T length = lengthSqrd();
		if( length > 0 )
		{
			length = std::sqrt( length );
			T scalar = newLength / length;
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
		}
		else
		{
			x = newLength;
			y = 0;
			z = 0;
			w = 0;
		}
	}

	template< typename T >
	Vector4<T> Vector4<T>::scaledToLength( T newLength )
	{
		Vector4<T> temp = *this;
		temp.scaledToLength( newLength );
		return temp;
	}

	template< typename T >
	bool Vector4<T>::isNormalized( T epsilon ) const
	{
		return std::abs( lengthSqrd() - static_cast<T>( 1 ) ) <= epsilon;
	}

	template< typename T >
	bool Vector4<T>::isZero( T epsilon ) const
	{
		return std::abs( lengthSqrd() <= epsilon );
	}

	template< typename T >
	bool Vector4<T>::isPerpendicular( const Vector4<T>& other, T epsilon ) const
	{
		return std::abs( dot( other ) ) <= epsilon;
	}

	template< typename T >
	std::string Vector4<T>::toString() const
	{
		std::stringstream ss;
		std::string str;
		ss << "( " << x << ", " << y << ", " << z << ", " << w << " )";
		ss >> str;
		return str;
	}

	template< typename T >
	std::string Vector4<T>::serializeToSTring() const
	{
		std::stringstream ss;
		std::string str;
		ss << x << "," << y << "," << z << "," << w;
		ss >> str;
		return str;
	}

	template< typename T >
	T Vector4<T>::sumOfElements() const
	{
		return x + y + z + w;
	}

	template< typename T >
	T Vector4<T>::productOfElements() const
	{
		return x * y * z * w;
	}

	template< typename T >
	T Vector4<T>::averageOfElements() const
	{
		return ( x + y + z + w ) / static_cast<T>( 4 );
	}

	template< typename T >
	T Vector4<T>::minElement() const
	{
		std::min( std::min( x, y ), std::min( z, w ) );
	}

	template< typename T >
	int Vector4<T>::minElementIndex() const
	{
		if (x < y) 
		{
			if (z < w)
					return (x < z) ? 0 : 2;
			else
					return (x < w) ? 0 : 3;
		}
		else
		{
			if (z < w)
				return (y < z) ? 1 : 2;
			else
				return (y < w) ? 1 : 3;
		}
	}

	template< typename T >
	T Vector4<T>::maxElement() const
	{
		return std::max( std::max( x, y ), std::max( z, w ) );
	}

	template< typename T >
	int Vector4<T>::maxElementIndex() const
	{
		if (x > y) 
		{
			if (z > w)
					return (x > z) ? 0 : 2;
			else
					return (x > w) ? 0 : 3;
		}
		else
		{
			if (z > w)
				return (y > z) ? 1 : 2;
			else
				return (y > w) ? 1 : 3;
		}
	}

	template< typename T >
	Vector4<T> Vector4<T>::abs() const
	{
		return Vector4<T>( std::abs( x ), std::abs( y ), std::abs( z ), std::abs( w ) );
	}

	template< typename T >
	Vector4<T> Vector4<T>::neg() const
	{
		return Vector4<T>( -x, -y, -z, -w );
	}

	template< typename T >
	Vector4<T> Vector4<T>::recip() const
	{
		return Vector4<T>( 1/x, 1/y, 1/z, 1/w );
	}

	template< typename T >
	Vector4<T> Vector4<T>::min( T ceil ) const
	{
		return Vector4<T>( std::min( x, ceil ), std::min( y, ceil ), std::min( z, ceil ), std::min( w, ceil ) );
	}

	template< typename T >
	Vector4<T> Vector4<T>::min( const Vector4<T>& ceil ) const
	{
		return Vector4<T>( std::min( x, ceil.x ), std::min( y, ceil.y ), std::min( z, ceil.z ), std::min( w, ceil.w ) );
	}

	template< typename T >
	Vector4<T> Vector4<T>::max( T floor ) const
	{
		return Vector4<T>( std::max( x, floor ), std::max( y, floor ), std::max( z, floor ), std::max( w, floor ) );
	}

	template< typename T >
	Vector4<T> Vector4<T>::max( const Vector4<T>& floor ) const
	{
		return Vector4<T>( std::max( x, floor.x ), std::max( y, floor.y ), std::max( z, floor.z ), std::max( w, floor.w ) );
	}

	template< typename T >
	Vector4<T> Vector4<T>::clamp( T floor, T ceil ) const
	{
		return min( ceil ).max( floor );
	}

	template< typename T >
	Vector4<T> Vector4<T>::clamp( const Vector4<T>& floor, const Vector4<T>& ceil ) const
	{
		return min( ceil ).max( floor );
	}

	template< typename T >
	T Vector4<T>::distance( const Vector4<T>& point ) const
	{
		std::sqrt( distanceSqrd() );
	}

	template< typename T >
	T Vector4<T>::distanceSqrd( const Vector4<T>& point ) const
	{
		Vector4<T> delta = *this - rhs;
		return delta.dot( delta );
	}
	
	template< typename T >
	Vector4<T> Vector4<T>::fromString( const std::string& str )
	{
		std::vector< std::string > tokens;
		stringTokenizer( str, tokens, "," );
		Vector4<T> temp;
		for( size_t i = 0; i < 4; ++i )
		{
			if( !stringToType( tokens[i], &temp[i] ) )
			{
				temp[i] = 0;
			}
		}
		return temp;
	}

	template< typename T >
	std::ostream& operator<<( std::ostream& os, const Vector4<T>& vec)
	{
		os << "( " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w " )";
		return os;
	}

	template< typename T >
	const Vector4<T> Vector4<T>::ZERO = Vector4<T>( 0, 0, 0, 0 );
	template< typename T >
	const Vector4<T> Vector4<T>::ONE = Vector4<T>( 1, 1, 1, 1 );
	template< typename T >
	const Vector4<T> Vector4<T>::UNIT_X = Vector4<T>( 1, 0, 0, 0 );
	template< typename T >
	const Vector4<T> Vector4<T>::UNIT_Y = Vector4<T>( 0, 1, 0, 0 );
	template< typename T >
	const Vector4<T> Vector4<T>::UNIT_Z = Vector4<T>( 0, 0, 1, 0 );
	template< typename T >
	const Vector4<T> Vector4<T>::UNIT_W = Vector4<T>( 0, 0, 0, 1 );
}