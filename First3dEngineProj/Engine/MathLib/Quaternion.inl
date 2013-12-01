
namespace pdh
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
}