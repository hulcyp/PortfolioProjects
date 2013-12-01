
namespace pdh
{
	template< typename T >
	Matrix4<T>::Matrix4(  T init = 0 )
	{
		for( int i = 0; i < 16; ++i )
		{
			m_matrix[i] = init;
		}
	}

	template< typename T >
	Matrix4<T>::Matrix4( T arr[16] )
	{
		for( int i = 0; i < 16; ++i )
		{
			m_matrix[i] = arr[i];
		}
	}

	template< typename T >
	Matrix4<T>::Matrix4( T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8, T m9, T m10, T m11, T m12, T m13, T m14, T m15 )
	{
		m_matrix[0] = m0;
		m_matrix[1] = m1;
		m_matrix[2] = m2;
		m_matrix[3] = m3;
		m_matrix[4] = m4;
		m_matrix[5] = m5;
		m_matrix[6] = m6;
		m_matrix[7] = m7;
		m_matrix[8] = m8;
		m_matrix[9] = m9;
		m_matrix[10] = m10;
		m_matrix[11] = m11;
		m_matrix[12] = m12;
		m_matrix[13] = m13;
		m_matrix[14] = m14;
		m_matrix[15] = m15;
	}

	template< typename T >
	Matrix4<T>::Matrix4( const Matrix4<T> &copy )
	{
		for( int i = 0; i < 16; ++i)
		{
			m_matrix[i] = copy.m_matrix[i];
		}
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::getTranspose()
	{
		Matrix4<T> mat( *this );
		mat.transpose();
		return mat;
	}

	template< typename T >
	T Matrix4<T>::getTrace()
	{
		return m_matrix[0] + m_matrix[5] + m_matrix[10] + m_matrix[15];
	}


	template< typename T >
	Matrix4<T>& Matrix4<T>::operator=( const Matrix4<T> &rhs )
	{
		if( this != &rhs )
		{
			for( int i = 0; i < 16; ++i )
			{
				m_matrix[i] = rhs.m_matrix[i];
			}
		}

		return *this;
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::operator*( const Matrix4<T>& rhs ) const
	{
		Matrix4<T> result;

		result.m_matrix[0] = m_matrix[0]*rhs.m_matrix[0] + m_matrix[1]*rhs.m_matrix[4] + m_matrix[2]*rhs.m_matrix[8] + m_matrix[3]*rhs.m_matrix[12];
		result.m_matrix[1] = m_matrix[0]*rhs.m_matrix[1] + m_matrix[1]*rhs.m_matrix[5] + m_matrix[2]*rhs.m_matrix[9] + m_matrix[3]*rhs.m_matrix[13];
		result.m_matrix[2] = m_matrix[0]*rhs.m_matrix[2] + m_matrix[1]*rhs.m_matrix[6] + m_matrix[2]*rhs.m_matrix[10] + m_matrix[3]*rhs.m_matrix[14];
		result.m_matrix[3] = m_matrix[0]*rhs.m_matrix[3] + m_matrix[1]*rhs.m_matrix[7] + m_matrix[2]*rhs.m_matrix[11] + m_matrix[3]*rhs.m_matrix[15];
				  
		result.m_matrix[4] = m_matrix[4]*rhs.m_matrix[0] + m_matrix[5]*rhs.m_matrix[4] + m_matrix[6]*rhs.m_matrix[8] + m_matrix[7]*rhs.m_matrix[12];
		result.m_matrix[5] = m_matrix[4]*rhs.m_matrix[1] + m_matrix[5]*rhs.m_matrix[5] + m_matrix[6]*rhs.m_matrix[9] + m_matrix[7]*rhs.m_matrix[13];
		result.m_matrix[6] = m_matrix[4]*rhs.m_matrix[2] + m_matrix[5]*rhs.m_matrix[6] + m_matrix[6]*rhs.m_matrix[10] + m_matrix[7]*rhs.m_matrix[14];
		result.m_matrix[7] = m_matrix[4]*rhs.m_matrix[3] + m_matrix[5]*rhs.m_matrix[7] + m_matrix[6]*rhs.m_matrix[11] + m_matrix[7]*rhs.m_matrix[15];
				  
		result.m_matrix[8] = m_matrix[8]*rhs.m_matrix[0] + m_matrix[9]*rhs.m_matrix[4] + m_matrix[10]*rhs.m_matrix[8] + m_matrix[11]*rhs.m_matrix[12];
		result.m_matrix[9] = m_matrix[8]*rhs.m_matrix[1] + m_matrix[9]*rhs.m_matrix[5] + m_matrix[10]*rhs.m_matrix[9] + m_matrix[11]*rhs.m_matrix[13];
		result.m_matrix[10] = m_matrix[8]*rhs.m_matrix[2] + m_matrix[9]*rhs.m_matrix[6] + m_matrix[10]*rhs.m_matrix[10] + m_matrix[11]*rhs.m_matrix[14];
		result.m_matrix[11] = m_matrix[8]*rhs.m_matrix[3] + m_matrix[9]*rhs.m_matrix[7] + m_matrix[10]*rhs.m_matrix[11] + m_matrix[11]*rhs.m_matrix[15];
				  
		result.m_matrix[12] = m_matrix[12]*rhs.m_matrix[0] + m_matrix[13]*rhs.m_matrix[4] + m_matrix[14]*rhs.m_matrix[8] + m_matrix[15]*rhs.m_matrix[12];
		result.m_matrix[13] = m_matrix[12]*rhs.m_matrix[1] + m_matrix[13]*rhs.m_matrix[5] + m_matrix[14]*rhs.m_matrix[9] + m_matrix[15]*rhs.m_matrix[13];
		result.m_matrix[14] = m_matrix[12]*rhs.m_matrix[2] + m_matrix[13]*rhs.m_matrix[6] + m_matrix[14]*rhs.m_matrix[10] + m_matrix[15]*rhs.m_matrix[14];
		result.m_matrix[15] = m_matrix[12]*rhs.m_matrix[3] + m_matrix[13]*rhs.m_matrix[7] + m_matrix[14]*rhs.m_matrix[11] + m_matrix[15]*rhs.m_matrix[15];

		return result;
	}

	template< typename T >
	void Matrix4<T>::transpose()
	{
		std::swap( m_matrix[1], m_matrix[4] );
		std::swap( m_matrix[2], m_matrix[8] );
		std::swap( m_matrix[3], m_matrix[12] );
		std::swap( m_matrix[6], m_matrix[9] );
		std::swap( m_matrix[7], m_matrix[13] );
		std::swap( m_matrix[11], m_matrix[14] );
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::operator+( Matrix4<T> rhs ) const
	{
		Matrix4<T> result;

		for( int i = 0; i < 16; ++i )
		{
			result.m_matrix[i] = m_matrix[i] + rhs.m_matrix[i];
		}

		return result;
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::operator*( const T val )
	{
		Matrix4<T> temp;

		for( int i = 0; i < 16; ++i )
		{
			temp[i] = m_matrix[i]*val;
		}

		return temp;
	}

	template< typename T >
	T* Matrix4<T>::getRawMatrix()
	{
		T* temp = m_matrix;
		return temp;
	}

	template< typename T >
	T& Matrix4<T>::operator[]( int i )
	{
		return m_matrix[i];
	}

	template< typename T >
	const T& Matrix4<T>::operator[]( int i ) const
	{
		return m_matrix[i];
	}

	template< typename T >
	void Matrix4<T>::loadIdentity()
	{
		m_matrix[0] = 1.0f;
		m_matrix[1] = 0.0f;
		m_matrix[2] = 0.0f;
		m_matrix[3] = 0.0f;
		m_matrix[4] = 0.0f;
		m_matrix[5] = 1.0f;
		m_matrix[6] = 0.0f;
		m_matrix[7] = 0.0f;
		m_matrix[8] = 0.0f;
		m_matrix[9] = 0.0f;
		m_matrix[10] = 1.0f;
		m_matrix[11] = 0.0f;
		m_matrix[12] = 0.0f;
		m_matrix[13] = 0.0f;
		m_matrix[14] = 0.0f;
		m_matrix[15] = 1.0f;
	}

	template< typename T >
	Matrix4<T>& Matrix4<T>::operator*=( const Matrix4<T>& rhs )
	{
		*this = *this * rhs;
		return *this;
	}

	template< typename T >
	Matrix4<T>& Matrix4<T>::operator*=( T val )
	{
		*this = *this * val;
		return *this;
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::operator+( T val )
	{	
		Matrix4<T> temp;
		for( size_t i = 0; i < 16; ++i )
		{
			temp[i] = m_matrix[i] + val;
		}
	}

	template< typename T >
	Matrix4<T>& Matrix4<T>::operator+=( const Matrix4<T>& rhs )
	{
		*this = *this + rhs;
		return *this;
	}

	template< typename T >
	Matrix4<T>& Matrix4<T>::operator+=( T val )
	{
		*this = *this + val;
		return *this;
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::operator-( const Matrix4<T>& rhs ) const
	{
		Matrix4<T> temp;
		for( size_t i = 0; i < 16; ++i )
		{
			temp[i] = m_matrix[i] - rhs[i];
		}
		return temp;
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::operator-( T val ) const
	{
		Matrix4<T> temp;
		for( size_t i = 0; i < 16; ++i )
		{
			temp = m_matrix[i] - val;
		}
		return temp;
	}

	template< typename T >
	Matrix4<T>& Matrix4<T>::operator-()
	{
		for( size_t i = 0; i < 16; ++i )
		{
			m_matrix[i] = -m_matrix[i];
		}
		return *this;
	}

	template< typename T >
	Matrix4<T>& Matrix4<T>::operator-=( const Matrix4<T>& rhs )
	{
		*this = *this - rhs;
		return *this;
	}

	template< typename T >
	Matrix4<T>& Matrix4<T>::operator-=( T val )
	{
		*this = *this - val;
		return *this;
	}

	////////////////////////////////////////////////////////////
	// static functions
	////////////////////////////////////////////////////////////
	template< typename T >
	Matrix4<T> Matrix4<T>::identity()
	{
		return Matrix4<T>(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1 );
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::lookAt( const Vector3<T>& pos, const Vector3<T>& lookat )
	{
		Vector3<T> kc( Vector3<T>( pos.x - lookat.x, pos.y - lookat.y, pos.z - lookat.z ) );
		kc.normalize();
		Vector3<T> ic( Vector3<T>( 0.0f, 1.0f, 0.0f ).cross( kc ) );
		ic.normalize();
		Vector3<T> jc( kc.cross( ic ) );

		Matrix4<T> trans(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							pos.x, pos.y, pos.z, 1 );

		Matrix4<T> rot(	ic.x, ic.y, ic.z, 0,
						jc.x, jc.y, jc.z, 0,
						kc.x, kc.y, kc.z, 0,
						0, 0, 0, 0, 1 );

		return rot * trans;
	}

	template< typename T >
	Matrix4<T> Matrix4<T>::ortho( T left, T right, T top, T bot, T znear, T zfar )
	{
		T two = static_cast<T>( 2 );
		Matrix4<T> temp (	two/(right - left),		0,					0,						-((right + left)/(right - left)),
							0,						two/(top - bot),	0,						-((top + bot)/(top - bot)),
							0,						0,					-two/(zfar - znear),	-((zfar + znear)/(zfar - znear)),
							0,						0,					0,						1 );
		temp.transpose();
		return temp;
	}
	
	template< typename T >
	Matrix4<T> Matrix4<T>::perspective( T fovX, T aspect, T znear, T zfar )
	{
		T two = static_cast<T>( 2 );
		T PI_OVER_180 = static_cast<T>( PI/180.0f );
		T fovY = fovX / aspect;
		T fovRad = fovY * PI_OVER_180;
		T f = 1/tan( fovRad/two );

		T xymax = znear * tan( fovRad/two );
		T ymin = -xymax;
		T xmin = -xymax;

		T width = xymax - xmin;
		T height = xymax - ymin;

		T depth = zfar - znear;
		T q = -(zfar + znear)/depth;
		T qn = -two * (zfar * znear )/ depth;

		T w = two * znear / width;
		w = w / aspect;
		T h = two * znear / height;

		Matrix4<T> temp(	w,	0,	0,	0,
							0,	h,	0,	0,
							0,	0,	q,	-1,
							0,	0,	qn,	0 );

		return temp;
	}

	////////////////////////////////////////////////////////////
	// Utility functions
	////////////////////////////////////////////////////////////
	template< typename T >
	Vector3<T> transformVector( const Vector3<T>& vec, const Matrix4<T>& mat, T w = 1 )
	{
		Vector3<T> prod;

		prod.x = vec.x * mat[0] + vec.y * mat[4] + vec.z * mat[8]  + w * mat[12];
		prod.y = vec.x * mat[1] + vec.y * mat[5] + vec.z * mat[9]  + w * mat[13];
		prod.z = vec.x * mat[2] + vec.y * mat[6] + vec.z * mat[10] + w * mat[14];

		return prod;
	}

	template< typename T >
	Vector3<T> transformVector( const Matrix4<T>& mat, const Vector3<T>& vec, T w = 1 )
	{
		Vector3<T> prod;

		prod.x = vec.x * mat[0] + vec.y * mat[1] + vec.z * mat[2] + w * mat[3];
		prod.y = vec.x * mat[4] + vec.y * mat[5] + vec.z * mat[6] + w * mat[7];
		prod.x = vec.x * mat[8] + vec.y * mat[9] + vec.z * mat[10] + w * mat[11];

		return prod;
	}
	

	template< typename T >
	void orthoNormalize( Matrix4<T>& matrix )
	{
		Vector3<T> x( matrix[0], matrix[1], matrix[2] );
		Vector3<T> y( matrix[4], matrix[5], matrix[6] );
		Vector3<T> z;

		x.normalize();
		z = x.cross( y );
		z.normalize();
		y = z.cross( x );

		matrix[0] = x.x;
		matrix[1] = x.y;
		matrix[2] = x.z;
		matrix[4] = y.x;
		matrix[5] = y.y;
		matrix[6] = y.z;
		matrix[8] = z.x;
		matrix[9] = z.y;
		matrix[10] = z.z;
	}
}