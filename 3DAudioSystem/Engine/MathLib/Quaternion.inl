
namespace Monky
{
	template< typename T >
	Quaternion<T>::Quaternion()
		:	m_real ( 0 )
		,	m_vector()
	{}

	template< typename T >
	Quaternion<T>::Quaternion( T real, Vector3<T>& vector )
		:	m_real( real )
		,	m_vector( vector )
	{}

	template< typename T >
	Quaternion<T>::Quaternion( const Quaternion& copy )
	{
		m_real = copy.m_real;
		m_vector = copy.m_vector;
	}

	template< typename T >
	Quaternion<T>& Quaternion<T>::operator=( const Quaternion& rhs )
	{
		if( this != &rhs )
		{
			m_real = rhs.m_real;
			m_vector = rhs.m_vector;
		}
		return *this;
	}

	template< typename T >
	T Quaternion<T>::dot( const Quaternion<T>& rhs ) const
	{
		return ( m_real*rhs.m_real + m_vector.dot( rhs.m_vector ) );
	}

	template< typename T >
	Quaternion<T> Quaternion<T>::operator-()
	{
		Quaternion<T> temp;
		temp.m_real = -m_real;
		temp.m_vector = -m_vector;

		return temp;
	}

	template< typename T >
	Quaternion<T> Quaternion<T>::operator+( const Quaternion<T>& rhs ) const
	{
		Quaternion<T> temp;
		temp.m_real = m_real + rhs.m_real;
		temp.m_vector = m_vector + rhs.m_vector;
		return temp;
	}

	template< typename T >
	Quaternion<T> Quaternion<T>::operator*( const T real ) const
	{
		Quaternion<T> temp;
			
		temp.m_real = m_real * real;
		temp.m_vector = (m_vector * real);

		return temp;
	}

	template< typename T >
	Quaternion<T> Quaternion<T>::conjugate() const
	{
		return Quaternion( m_real, -m_vector );
	}

	template< typename T >
	T Quaternion<T>::getReal() const
	{
		return m_real;
	}

	template< typename T >
	Vector3<T> Quaternion<T>::getVector() const
	{
		return m_vector;
	}

	template< typename T >
	T Quaternion<T>::length() const
	{
		return std::sqrt( dot( *this ) );
	}
	
	template< typename T >
	void Quaternion<T>::normalize()
	{
		const T norm = length();

		m_real /= norm;
		m_vector = m_vector * ( static_cast<T>(1)/norm );
	}

	template< typename T >
	Quaternion<T> Quaternion<T>::inverse() const
	{
		T dotProd = this->dot( *this );
		
		return Quaternion<T>( w / dotProd, -vector / dotProd );
	}
	//------------------------------------------------------
	// Static functions
	//------------------------------------------------------
	template< typename T >
	Quaternion<T> Quaternion<T>::makeQuaternionFromAxisAngle( T angle, Vector3<T> axis )
	{
		T hAngle = angle / static_cast<T>( 2 );
		return Quaternion<T>( std::cos( hAngle ), axis * std::sin( hAngle ) );
	}
	//------------------------------------------------------
	template< typename T >
	Quaternion<T> slerp( const Quaternion<T>& q1, const Quaternion<T>& q2, T t )
	{
		Quaternion<T> temp;

		T d = q1->dot( *q2 );
		if( d < 0 )
		{
			*q2 = -(*q2);
			d = -d;
		}
		T epsilon = static_cast<T>( 0.0001 );

		if( d >= 1 - epsilon )
		{
			*temp = ( (*q1) * ( 1 - t ) + (*q2) * t );
			temp->normalize();
		}
		else
		{
			T angle = std::acos( d );
			T w1 = std::sin( angle * ( 1 - t ) ) / std::sin( angle );
			T w2 = std::sin( angle * t ) / std::sin( angle );
			Quaternion<T> temp1 = (*q1) * w1;
			Quaternion<T> temp2 = (*q2) * w2;
			temp = temp1 + temp2;
		}

		return temp;
	}
}