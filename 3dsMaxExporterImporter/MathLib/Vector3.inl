
namespace pdh
{
	template< typename T >
	Vector3<T>::Vector3( T x, T y, T z )
		:	x( x ), y( y ), z( z )
	{}

	template< typename T >
	Vector3<T>::Vector3( const Vector3<T>& vec )
		:	x( vec.x )
		,	y( vec.y )
		,	z( vec.z )
	{}

	template< typename T >
	Vector3<T>::Vector3( const Vector2<T>& vec2, T z )
		:	x( vec2.x )
		,	y( vec2.y )
		,	z( z )
	{}

	template< typename T >
	Vector3<T>& Vector3<T>::operator=( const Vector3<T>& rhs )
	{
		if( this != &rhs )
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}

		return *this;
	}

	template< typename T >
	T& Vector3<T>::getComp( int i )
	{
		assert( i < 3 );
		return *( &x + i);
	}

	template< typename T >
	T& Vector3<T>::operator[]( int i )
	{
		assert( i < 3 );
		return *( &x + i );
	}

	template< typename T >
	const T& Vector3<T>::operator[]( int i ) const
	{
		assert( i < 3 );
		return *( &x + i );
	}

	template< typename T >
	T Vector3<T>::dot( const Vector3<T>& vec ) const
	{
		return ( x * vec.x + y * vec.y + z * vec.z );
	}

	template< typename T >
	T Vector3<T>::norm() const
	{
		return std::sqrt( x*x + y*y + z*z );
	}

	template< typename T >
	T Vector3<T>::length() const
	{
		return norm();
	}

	template< typename T >
	T Vector3<T>::lengthSqrd() const
	{
		return dot( *this );
	}

	template< typename T >
	void Vector3<T>::normalize()
	{
		const T length = norm();
		if( length != 0 )
		{
			x /= length;
			y /= length;
			z /= length;
		}
	}

	template< typename T >
	Vector3<T> Vector3<T>::normalized() const
	{
		Vector3<T> temp = *this;
		temp.normalize();
		return temp;
	}

	template< typename T >
	Vector3<T> Vector3<T>::cross( const Vector3& vec )
	{
		Vector3<T> temp;

		temp.x = y * vec.z - vec.y * z;
		temp.y = -( x*vec.z- z * vec.x );
		temp.z = x*vec.y- vec.x*y;

		return temp;
	}

	template< typename T >
	Vector3<T> Vector3<T>::operator+( const Vector3<T>& rhs ) const
	{
		Vector3<T> temp;

		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		temp.z = z + rhs.z;

		return temp;
	}
	
	template< typename T >
	Vector3<T> Vector3<T>::operator+( const T val ) const
	{
		Vector3<T> temp;

		temp.x = x + val;
		temp.y = y + val;
		temp.z = z + val;

		return temp;
	}

	template< typename T >
	Vector3<T> Vector3<T>::operator-( const Vector3<T>& rhs ) const
	{
		Vector3<T> temp;

		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		temp.z = z - rhs.z;

		return temp;
	}

	template< typename T >
	Vector3<T> Vector3<T>::operator-( const T val ) const
	{
		Vector3<T> temp;

		temp.x = x - val;
		temp.y = y - val;
		temp.z = z - val;

		return temp;
	}

	template< typename T >
	Vector3<T> Vector3<T>::operator*( const Vector3<T>& rhs ) const
	{
		Vector3<T> temp;

		temp.x = rhs.x*x;
		temp.y = rhs.y*y;
		temp.z = rhs.z*z;

		return temp;
	}

	template< typename T >
	Vector3<T> Vector3<T>::operator*( const T val ) const
	{
		Vector3<T> temp;

		temp.x = val*x;
		temp.y = val*y;
		temp.z = val*z;

		return temp;
	}

	template< typename T >
	Vector3<T> Vector3<T>::operator/( const Vector3<T>& rhs ) const
	{
		Vector3<T> temp;

		temp.x = x / rhs.x;
		temp.y = y / rhs.y;
		temp.z = z / rhs.z;

		return temp;
	}
	
	template< typename T >
	Vector3<T> Vector3<T>::operator/( const T val )
	{
		Vector3<T> temp;

		temp.x = x / val;
		temp.y = y / val;
		temp.z = z / val;

		return temp;
	}

	template< typename T>
	Vector3<T> Vector3<T>::operator-() const
	{
		return Vector3<T>( -x, -y, -z );
	}

	template< typename T >
	Vector3<T>& Vector3<T>::operator+=( const Vector3<T>& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}
	
	template< typename T >
	Vector3<T>& Vector3<T>::operator+=( const T real )
	{
		x += real;
		y += real;
		z += real;

		return *this;
	}

	template< typename T >
	Vector3<T>& Vector3<T>::operator-=( const Vector3<T>& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	template< typename T >
	Vector3<T>& Vector3<T>::operator-=( const T real )
	{
		x -= real;
		y -= real;
		z -= real;

		return *this;
	}

	template< typename T >
	Vector3<T>& Vector3<T>::operator/=( const Vector3<T>& rhs )
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
			
		return *this;
	}

	template< typename T >
	Vector3<T>& Vector3<T>::operator/=( const T real )
	{
		x /= real;
		y /= real;
		z /= real;
			
		return *this;
	}

	template< typename T >
	Vector3<T>& Vector3<T>::operator*=( const Vector3<T>& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}

	template< typename T >
	Vector3<T>& Vector3<T>::operator*=( const T real )
	{
		x *= real;
		y *= real;
		z *= real;

		return *this;
	}

	template< typename T >
	bool Vector3<T>::operator==( const Vector3<T>& rhs ) const
	{
		return ( ( x == rhs.x ) && ( y== rhs.y ) && ( z == rhs.z ) );
	}

	template< typename T >
	bool Vector3<T>::operator !=( const Vector3<T>& rhs ) const
	{
		return ( ( x != rhs.x ) || ( y != rhs.y ) || ( z != rhs.z ) );
	}

	template< typename T >
	bool Vector3<T>::isParallel( const Vector3<T>& rhs, T epsilon ) const
	{
		return ( cross( rhs ).lengthSqrd() <= epsilon );
	}

	template< typename T >
	Vector2<T> Vector3<T>::xx() { return Vector2<T>( x, x ); }
	template< typename T >
	Vector2<T> Vector3<T>::xy() { return Vector2<T>( x, y ); }
	template< typename T >
	Vector2<T> Vector3<T>::yx() { return Vector2<T>( y, x ); }
	template< typename T >
	Vector2<T> Vector3<T>::yy() { return Vector2<T>( y, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::xxx() { return Vector3<T>( x, x, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::xxy() { return Vector3<T>( x, x, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::xxz() { return Vector3<T>( x, x, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::xyx() { return Vector3<T>( x, y, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::xyy() { return Vector3<T>( x, y, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::xyz() { return Vector3<T>( x, y, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::xzx() { return Vector3<T>( x, z, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::xzy() { return Vector3<T>( x, z, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::xzz() { return Vector3<T>( x, z, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::yxx() { return Vector3<T>( y, x, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::yxy() { return Vector3<T>( y, x, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::yxz() { return Vector3<T>( y, x, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::yyx() { return Vector3<T>( y, y, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::yyy() { return Vector3<T>( y, y, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::yyz() { return Vector3<T>( y, y, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::yzx() { return Vector3<T>( y, z, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::yzy() { return Vector3<T>( y, z, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::yzz() { return Vector3<T>( y, z, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::zxx() { return Vector3<T>( z, x, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::zxy() { return Vector3<T>( z, x, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::zxz() { return Vector3<T>( z, x, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::zyx() { return Vector3<T>( z, y, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::zyy() { return Vector3<T>( z, y, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::zyz() { return Vector3<T>( z, y, z ); }
	template< typename T >
	Vector3<T> Vector3<T>::zzx() { return Vector3<T>( z, z, x ); }
	template< typename T >
	Vector3<T> Vector3<T>::zzy() { return Vector3<T>( z, z, y ); }
	template< typename T >
	Vector3<T> Vector3<T>::zzz() { return Vector3<T>( z, z, z ); }

	template< typename T >
	void Vector3<T>::scaleToLength( T newLength )
	{
		T length = lengthSqrd();
		if( length > 0 )
		{
			length = std::sqrt( length );
			T scalar = newLength / length;
			x *= scalar;
			y *= scalar;
			z *= scalar;
		}
		else
		{
			x = newLength;
			y = 0;
			z = 0;
		}
	}

	template< typename T >
	Vector3<T> Vector3<T>::scaledToLength( T newLength )
	{
		Vector3<T> temp = *this;
		temp.scaledToLength( newLength );
		return temp;
	}

	template< typename T >
	bool Vector3<T>::isNormalized( T epsilon ) const
	{
		return std::abs( lengthSqrd() - static_cast<T>( 1 ) ) <= epsilon;
	}

	template< typename T >
	bool Vector3<T>::isZero( T epsilon ) const
	{
		return std::abs( lengthSqrd() <= epsilon );
	}

	template< typename T >
	bool Vector3<T>::isPerpendicular( const Vector3<T>& other, T epsilon ) const
	{
		return std::abs( dot( other ) ) <= epsilon;
	}

	template< typename T >
	std::string Vector3<T>::toString() const		
	{
		std::stringstream ss;
		ss << "( " << x << ", " << y << ", " << z << " )";
		return std::string( ss.str() );
	}

	template< typename T >
	std::string Vector3<T>::serializeToSTring() const
	{
		std::stringstream ss;
		std::string str;
		ss << x << "," << y << "," << z;
		ss >> str;
		return ss.str();
	}

	template< typename T >
	T Vector3<T>::sumOfElements() const
	{
		return x + y + z;
	}

	template< typename T >
	T Vector3<T>::productOfElements() const
	{
		return x * y * z;
	}

	template< typename T >
	T Vector3<T>::averageOfElements() const
	{
		return ( x + y + z ) / static_cast<T>( 3 );
	}

	template< typename T >
	T Vector3<T>::minElement() const
	{
		return std::min( std::min( x, y ), z );
	}

	template< typename T >
	int Vector3<T>::minElementIndex() const
	{
		if( x <= y && x <= z )
			return 0;
		else
			return ( y <= z ) ? 1 : 2;
	}

	template< typename T >
	T Vector3<T>::maxElement() const
	{
		return std::max( std::max( x, y ), z );
	}

	template< typename T >
	int Vector3<T>::maxElementIndex() const
	{
		if( x >= y && x >= z )
			return 0;
		else
			return ( y >s= z ) ? 1 : 2;
	}

	template< typename T >
	Vector3<T> Vector3<T>::abs() const
	{
		return Vector3<T>( std::abs( x ), std::abs( y ), std::abs( z ) );
	}

	template< typename T >
	Vector3<T> Vector3<T>::neg() const
	{
		return Vector3<T>( -x, -y, -z );
	}

	template< typename T >
	Vector3<T> Vector3<T>::recip() const
	{
		return Vector3<T>( 1/x, 1/y, 1/z );
	}

	template< typename T >
	Vector3<T> Vector3<T>::min( T ceil ) const
	{
		return Vector3<T>( std::min( x, ceil ), std::min( y, ceil ), std::min( z, ceil ) );
	}

	template< typename T >
	Vector3<T> Vector3<T>::min( const Vector3<T>& ceil ) const
	{
		return Vector3<T>( std::min( x, ceil.x ), std::min( y, ceil.y ), std::min( z, ceil.z ) );
	}

	template< typename T >
	Vector3<T> Vector3<T>::max( T floor ) const
	{
		return Vector3<T>( std::max( x, floor ), std::max( y, floor ), std::max( z, floor ) );
	}

	template< typename T >
	Vector3<T> Vector3<T>::max( const Vector3<T>& floor ) const
	{
		return Vector3<T>( std::max( x, floor.x ), std::max( y, floor.y ), std::max( z, floor.z ) );
	}

	template< typename T >
	Vector3<T> Vector3<T>::clamp( T floor, T ceil ) const
	{
		return min( ceil ).max( floor );
	}

	template< typename T >
	Vector3<T> Vector3<T>::clamp( const Vector3<T>& floor, const Vector3<T>& ceil ) const
	{
		return min( ceil ).max( floor );
	}

	template< typename T >
	T Vector3<T>::distance( const Vector3<T>& point ) const
	{
		return std::sqrt( distanceSqrd() );
	}

	template< typename T >
	T Vector3<T>::distanceSqrd( const Vector3<T>& point ) const
	{
		Vector3<T> delta = *this - rhs;
		return delta.dot( delta );
	}

	//template< typename T >
	//Vector3<T> Vector3<T>::perpindicular() const
	//{
	//	
	//}

	template< typename T >
	T Vector3<T>::scalarTripleProduct( const Vector3<T>& u, const Vector3<T>& v, const Vector3<T>& w )
	{
		return u.cross( v ).dot( w );
	}

	template< typename T >
	Vector3<T> Vector3<T>::reflect( const Vector3<T>& normal ) const
	{
		return static_cast<T>( 2 ) * projectToNorm( normal ) - *this;
	}

	template< typename T >
	Vector3<T> Vector3<T>::refract( const Vector3<T>& normal, T negSideRefractionIndex, T posSideRefractionIndex ) const
	{
		T one = static_cast<T>( 1 );
		T n = negSideRefractionIndex / posSideRefractionIndex;
		T cosI = dot( normal );
		T sinT2 = n * n * ( one - cosI * cosI );

		if( sinT2 > one )  // Total internal reflection occurs
			return ( -*this ).reflect( normal );

		return *this * n - normal * ( n + std::sqrt( one - sinT2 ) );
	}

	template< typename T >
	Vector3<T> Vector3<T>::projectTo( const Vector3<T>& direction ) const
	{
		return direction * dot( direction ) / direction.lengthSqrd();
	}

	template< typename T >
	Vector3<T> Vector3<T>::projectToNorm( const Vector3<T>& direction ) const
	{
		return direction * dot( direction );
	}

	template< typename T >
	T Vector3<T>::angleBetween( const Vector3<T>& other ) const
	{
		T one = static_cast<T>( 1 );
		T cosa = dot( other ) / std::sqrt( lengthSqrd() * other.lengthSqrd() );
		if( cosa >= one )
			return 0;
		else if( cosa <= -one )
			return PI;
		else
			return std::acos( cosa );
	}

	template< typename T >
	T Vector3<T>::angleBetweenNorm( const Vector3<T>& normalizedVec ) const
	{
		ASSERT( isNormalized() );
		ASSERT( normalizedVec.isNormalized() );
		T one = static_cast<T>( 1 );
		T cosa = dot( normalizedVec );
		if( cosa >= one )
			return 0;
		else if( cosa <= -one )
			return PI;
		else
			return std::acos( cosa );
	}

	template< typename T >
	Vector3<T> Vector3<T>::lerp( const Vector3<T>& b, T t ) const
	{
		T one = static_cast<T>( 1 );
		ASSERT( static_cast<T>( 0 ) <= t && t <= one );
		return *this * ( one - t ) + b * t;
	}

	template< typename T >
	void Vector3<T>::decompose( const Vector3<T>& direction, Vector2<T>& outParallel, Vector3<T>& outPerp ) const
	{
		ASSERT( direction.isNormalized() );
		outParallel = projectToNorm( direction );
		outPerp = *this - outParallel;
	}

	template< typename T >
	void Vector3<T>::orthogonalize( const Vector3<T>& a, Vector3<T>& b )
	{
		if( !a.isZero() )
			b -= b.projectTo( a );
	}

	template< typename T >
	void Vector3<T>::orthogonalize( const Vector3<T>& a, Vector3<T>& b, Vector3<T>& c )
	{
		if( !a.isZero() )
		{
			b -= b.projectTo( a );
			c -= c.projectTo( a );
		}
		if( !b.isZero() )
			c -= c.projectTo( b );
	}
	
	template< typename T >
	bool Vector3<T>::areOrthogonal( const Vector3<T>& a, Vector3<T>& b, T epsilon )
	{
		return a.isPerpendicular( b, epsilon );
	}
	
	template< typename T >
	bool Vector3<T>::areOrthogonal( const Vector3<T>& a, Vector3<T>& b, Vector3<T>& c, T epsilon )
	{
		return	a.isPerpendicular( b, epsilon ) &&
				a.isPerpendicular( c, epsilon ) &&
				b.isPerpendicular( c, epsilon );
	}

	template< typename T >
	void othonormalize( Vector3<T>& a, Vector3<T>& b )
	{
		ASSERT( !a.isZero() );
		ASSERT( !b.isZero() );
		a.normalize();
		b -= b.projectToNorm( a );
		b.normalize();
	}

	template< typename T >
	void Vector3<T>::othonormalize( Vector3<T>& a, Vector3<T>& b, Vector3<T>& c )
	{
		ASSERT( !a.isZero() );
		a.normalize();
		b -= b.projectToNorm( a );
		b.normalize();
		c -= c.projectToNorm( a );
		c -= c.projectToNorm( b );
		ASSERT( !c.isZero() );
		c.normalize();
	}

	template< typename T >
	Vector3<T> Vector3<T>::fromString( const std::string& str )
	{
		std::vector< std::string > tokens;
		stringTokenizer( str, tokens, "," );
		Vector3<T> temp;
		for( size_t i = 0; i < 3; ++i )
		{
			if( !stringToType( tokens[i], &temp[i] ) )
			{
				temp[i] = 0;
			}
		}
		return temp;
	}	

	template< typename T >
	std::ostream& operator<<( std::ostream& os, const Vector3<T>& vec)
	{
		os << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )";
		return os;
	}

	template< typename T >
	Vector3<T> parseStringToVec3( const std::string& str, const Vector3<T>& defaultValue = Vector3<T>::ZERO )
	{
		std::vector< std::string > tokens;
		stringTokenizer( str, tokens, "," );
		Vector3<T> temp = defaultValue;
		size_t numValues = tokens.size();

		if( numValues < 3 )
			throw std::exception( "Not enough values to fill Vector3" );

		for( size_t i = 0; i < numValues && i < 3; ++i )
		{
			if( !stringToType( tokens[i], &temp[i] ) )
			{
				temp[i] = defaultValue[i];
			}
		}
		return temp;
	}

	template< typename T >
	const Vector3<T> Vector3<T>::ZERO = Vector3<T>( 0, 0, 0 );
	template< typename T >
	const Vector3<T> Vector3<T>::ONE = Vector3<T>( 1, 1, 1 );
	template< typename T >
	const Vector3<T> Vector3<T>::UNIT_X = Vector3<T>( 1, 0, 0 );
	template< typename T >
	const Vector3<T> Vector3<T>::UNIT_Y = Vector3<T>( 0, 1, 0 );
	template< typename T >
	const Vector3<T> Vector3<T>::UNIT_Z = Vector3<T>( 0, 0, 1 );
}