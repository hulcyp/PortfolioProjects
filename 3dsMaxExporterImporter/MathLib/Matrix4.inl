#include "MathFuncs.h"

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
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>::Matrix4( T arr[16] )
	{
		for( int i = 0; i < 16; ++i )
		{
			m_matrix[i] = arr[i];
		}
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>::Matrix4( T arr[4][3] )
	{
		m_matrix[0] = arr[0][0];
		m_matrix[1] = arr[0][1];
		m_matrix[2] = arr[0][2];
		m_matrix[3] = 0;
		m_matrix[4] = arr[1][0];
		m_matrix[5] = arr[1][1];
		m_matrix[6] = arr[1][2];
		m_matrix[7] = 0;
		m_matrix[8] = arr[2][0];
		m_matrix[9] = arr[2][1];
		m_matrix[10] = arr[2][2];
		m_matrix[11] = 0;
		m_matrix[12] = arr[3][0];
		m_matrix[13] = arr[3][1];
		m_matrix[14] = arr[3][2];
		m_matrix[15] = 1;
	}
	//------------------------------------------------------
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
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>::Matrix4( const Matrix4<T> &copy )
	{
		for( int i = 0; i < 16; ++i)
		{
			m_matrix[i] = copy.m_matrix[i];
		}
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>::Matrix4( const Matrix3<T>& copy )
	{
		T zero = static_cast<T>( 0 );
		m_matrix[0] = copy[0];
		m_matrix[1] = copy[1];
		m_matrix[2] = copy[2];
		m_matrix[3] = zero;
		m_matrix[4] = copy[3];
		m_matrix[5] = copy[4];
		m_matrix[6] = copy[5];
		m_matrix[7] = zero;
		m_matrix[8] = copy[6];
		m_matrix[9] = copy[7];
		m_matrix[10] = copy[8];
		m_matrix[11] = zero;
		m_matrix[12] = zero;
		m_matrix[13] = zero;
		m_matrix[14] = zero;
		m_matrix[15] = static_cast<T>( 1 );
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T> Matrix4<T>::getTranspose() const
	{
		Matrix4<T> mat( *this );
		mat.transpose();
		return mat;
	}
	//------------------------------------------------------
	template< typename T >
	T Matrix4<T>::getTrace()
	{
		return m_matrix[0] + m_matrix[5] + m_matrix[10] + m_matrix[15];
	}
	//------------------------------------------------------
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
	//------------------------------------------------------
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
	//------------------------------------------------------
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
	//------------------------------------------------------
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
	//------------------------------------------------------
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
	//------------------------------------------------------
	template< typename T >
	T* Matrix4<T>::getRawMatrix()
	{
		T* temp = m_matrix;
		return temp;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix4<T>::getUpper3x3() const
	{
		return Matrix3<T>(	m_matrix[0], m_matrix[1], m_matrix[2],
							m_matrix[4], m_matrix[5], m_matrix[6],
							m_matrix[8], m_matrix[9], m_matrix[10] );
	}
	//------------------------------------------------------
	// Retrieved from: ftp://download.intel.com/design/PentiumIII/sml/24504301.pdf
	template< typename T >
	Matrix4<T> Matrix4<T>::inverse() const
	{
		T tmp[12];
		Matrix4<T> src = getTranspose();
		Matrix4<T> dst;
		T det;
		
		tmp[0] = src[10] * src[15];
		tmp[1] = src[11] * src[14];
		tmp[2] = src[9] * src[15];
		tmp[3] = src[11] * src[13];
		tmp[4] = src[9] * src[14];
		tmp[5] = src[10] * src[13];
		tmp[6] = src[8] * src[15];
		tmp[7] = src[11] * src[12];
		tmp[8] = src[8] * src[14];
		tmp[9] = src[10] * src[12];
		tmp[10] = src[8] * src[13];
		tmp[11] = src[9] * src[12];

		dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
		dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
		dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
		dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
		dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
		dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
		dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
		dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
		dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
		dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
		dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
		dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
		dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
		dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
		dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
		dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

		tmp[0] = src[2]*src[7];
		tmp[1] = src[3]*src[6];
		tmp[2] = src[1]*src[7];
		tmp[3] = src[3]*src[5];
		tmp[4] = src[1]*src[6];
		tmp[5] = src[2]*src[5];
		tmp[6] = src[0]*src[7];
		tmp[7] = src[3]*src[4];
		tmp[8] = src[0]*src[6];
		tmp[9] = src[2]*src[4];
		tmp[10] = src[0]*src[5];
		tmp[11] = src[1]*src[4];

		dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
		dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
		dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
		dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
		dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
		dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
		dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
		dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
		dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
		dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
		dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
		dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
		dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
		dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
		dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
		dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

			
		det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];

		if( std::fabs( det ) > 0 )
		{
			det = 1/det;
			for( int j = 0; j < 16; j++ )
				dst[j] *= det;
			return dst;
		}
		else
		{
			return Matrix4<T>::ZERO;
		}

	}
	//------------------------------------------------------
	template< typename T >
	T& Matrix4<T>::operator[]( int i )
	{
		return m_matrix[i];
	}
	//------------------------------------------------------
	template< typename T >
	const T& Matrix4<T>::operator[]( int i ) const
	{
		return m_matrix[i];
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::loadIdentity()
	{
		*this = Matrix4<T>::IDENTITY;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& Matrix4<T>::operator*=( const Matrix4<T>& rhs )
	{
		*this = *this * rhs;
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& Matrix4<T>::operator*=( T val )
	{
		*this = *this * val;
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T> Matrix4<T>::operator+( T val )
	{	
		Matrix4<T> temp;
		for( size_t i = 0; i < 16; ++i )
		{
			temp[i] = m_matrix[i] + val;
		}
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& Matrix4<T>::operator+=( const Matrix4<T>& rhs )
	{
		*this = *this + rhs;
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& Matrix4<T>::operator+=( T val )
	{
		*this = *this + val;
		return *this;
	}
	//------------------------------------------------------
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
	//------------------------------------------------------
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
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& Matrix4<T>::operator-()
	{
		for( size_t i = 0; i < 16; ++i )
		{
			m_matrix[i] = -m_matrix[i];
		}
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& Matrix4<T>::operator-=( const Matrix4<T>& rhs )
	{
		*this = *this - rhs;
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& Matrix4<T>::operator-=( T val )
	{
		*this = *this - val;
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::rotate( const Vector3<T>& axis, T angDegCCW )
	{
		T angRad = MathFuncs<T>::degreesToRadians( angDegCCW );
		rotateRadians( axis, angRad );
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::rotateRadians( const Vector3<T>& axis, T angRadCCW )
	{
		Quaternion<T> quat = Quaternion<T>::makeQuaternionFromAxisAngle( angRadCCW, axis );
		Matrix4<T> rot = createMatrixFromQuaternion( quat );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::rotateX( T angRadCCW )
	{
		Matrix4<T> rot(	1,	0,						0,						0,
						0,	std::cos( angRadCCW ),	-std::sin( angRadCCW ),	0,
						0,	std::sin( angRadCCW ),	std::cos( angRadCCW ),	0,
						0,	0,						0,						1 );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::rotateY( T angRadCCW )
	{
		Matrix4<T> rot(	std::cos( angRadCCW ),	0,	std::sin( angRadCCW ),	0,
						0,						1,	0,						0,
						-std::sin( angRadCCW),	0,	std::cos( angRadCCW ),	0,
						0,						0,	0,						1 );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::rotateZ( T angRadCCW )
	{
		Matrix4<T> rot(	std::cos( angRadCCW ),	-std::sin( angRadCCW ),	0,	0,
						std::sin( angRadCCW ),	std::cos( angRadCCW ),	0,	0,
						0,						0,						1,	0,
						0,						0,						0,	1 );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::rotate( const Quaternion<T>& quat )
	{
		Matrix4<T> rot = createMatrixFromQuaternion( quat );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix4<T>::getRotation() const
	{
		Matrix3<T> upper3x3 = getUpper3x3();
		Vector3<T> x( upper3x3[0], upper3x3[1], upper3x3[2] );
		Vector3<T> y( upper3x3[3], upper3x3[4], upper3x3[5] );
		Vector3<T> z( upper3x3[6], upper3x3[7], upper3x3[8] );
	
		Vector3<T>::othonormalize( x, y, z );

		return Matrix3<T>( x, y, z );
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::translate( const Vector3<T>& trans )
	{
		Matrix4<T> tempTrans = Matrix4<T>::identity();
		tempTrans[12] = trans.x;
		tempTrans[13] = trans.y;
		tempTrans[14] = trans.z;

		*this *= tempTrans;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::translate( T transX, T transY, T transZ )
	{
		Matrix4<T> tempTrans = Matrix4<T>::identity();
		tempTrans[12] = transX;
		tempTrans[13] = transY;
		tempTrans[14] = transZ;

		*this *= tempTrans;
	}
	//------------------------------------------------------
	template< typename T >
	Vector3<T> Matrix4<T>::getTranslation() const
	{
		return Vector3<T>( m_matrix[12], m_matrix[13], m_matrix[14] );
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::scale( const Vector3<T>& scaleVec )
	{
		scale( scaleVec.x, scaleVec.y, scaleVec.z );
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::scale( T scaleX, T scaleY, T scaleZ )
	{
		Matrix4<T> tempScale = Matrix4<T>::identity();
		tempScale[0] = scaleX;
		tempScale[5] = scaleY;
		tempScale[10] = scaleZ;

		*this *= tempScale;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix4<T>::scale( T uniformScale )
	{
		scale( uniformScale, uniformScale, uniformScale );		
	}
	//------------------------------------------------------
	template< typename T >
	Vector3<T> Matrix4<T>::getUniformScaleFactors() const
	{
		Matrix3<T> orthogonal = getRotation();
		Vector3<T> xScale( m_matrix[0], m_matrix[1], m_matrix[2] );
		Vector3<T> yScale( m_matrix[4], m_matrix[5], m_matrix[6] );
		Vector3<T> zScale( m_matrix[8], m_matrix[9], m_matrix[10] );

		T xFactor = orthogonal.getRow( 0 ).dot( xScale );
		T yFactor = orthogonal.getRow( 1 ).dot( yScale );
		T zFactor = orthogonal.getRow( 2 ).dot( zScale );
		return Vector3<T>( xFactor, yFactor, zFactor );
	}
	
	//------------------------------------------------------
	// static functions
	//------------------------------------------------------
	template< typename T >
	Matrix4<T> Matrix4<T>::identity()
	{
		return Matrix4<T>::IDENTITY;
	}
	//------------------------------------------------------
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
						0,	  0,    0,    1 );

		return rot * trans;
	}
	//------------------------------------------------------
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
	//------------------------------------------------------
	template< typename T >
	Matrix4<T> Matrix4<T>::perspective( T fovX, T aspect, T znear, T zfar )
	{
		T two = static_cast<T>( 2 );
		T PI_OVER_180 = static_cast<T>( MathFuncs<T>::PI/ static_cast<T>( 180.0 ) );
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
	//------------------------------------------------------
	template< typename T >
	Matrix4<T> Matrix4<T>::createMatrixFromQuaternion( const Quaternion<T>& q )
	{
		T w = q.getReal();
		T x = q.getVector().x;
		T y = q.getVector().y;
		T z = q.getVector().z;

		T i_x = 1 - 2*y*y - 2* z*z;
		T i_y = 2*x*y + 2*z*w;
		T i_z = 2*x*z - 2*y*w;
		
		// T(j)
		T j_x = 2*x*y - 2*z*w;
		T j_y = 1 - 2*x*x - 2*z*z;
		T j_z = 2*y*z + 2*x*w;
		
		// T(k)
		T k_x = 2*x*z + 2*y*w;
		T k_y = 2*y*z - 2*x*w;
		T k_z = 1 - 2*x*x - 2*y*y;

		Matrix4<T> rotMatrix(	i_x, i_y, i_z, 0,
								j_x, j_y, j_z, 0,
								k_x, k_y, k_z, 0,
								0	, 0	 , 0 , static_cast<T>( 1 ) );
		rotMatrix.transpose();
		return rotMatrix;
	}

	//------------------------------------------------------
	// Utility functions
	//------------------------------------------------------
	template< typename T >
	Vector3<T> transformVector( const Vector3<T>& vec, const Matrix4<T>& mat, T w = 1 )
	{
		Vector3<T> prod;

		prod.x = vec.x * mat[0] + vec.y * mat[4] + vec.z * mat[8]  + w * mat[12];
		prod.y = vec.x * mat[1] + vec.y * mat[5] + vec.z * mat[9]  + w * mat[13];
		prod.z = vec.x * mat[2] + vec.y * mat[6] + vec.z * mat[10] + w * mat[14];

		return prod;
	}
	//------------------------------------------------------
	template< typename T >
	Vector3<T> transformVector( const Matrix4<T>& mat, const Vector3<T>& vec, T w = 1 )
	{
		Vector3<T> prod;

		prod.x = vec.x * mat[0] + vec.y * mat[1] + vec.z * mat[2] + w * mat[3];
		prod.y = vec.x * mat[4] + vec.y * mat[5] + vec.z * mat[6] + w * mat[7];
		prod.x = vec.x * mat[8] + vec.y * mat[9] + vec.z * mat[10] + w * mat[11];

		return prod;
	}	
	//------------------------------------------------------
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
	//------------------------------------------------------
	template< typename T >
	const Matrix4<T> Matrix4<T>::IDENTITY = Matrix4<T>(	1, 0, 0, 0,
														0, 1, 0, 0,
														0, 0, 1, 0,
														0, 0, 0, 1 );
	template< typename T >
	const Matrix4<T> Matrix4<T>::ZERO = Matrix4<T>( static_cast<T>(0) );
	template< typename T >
	const Matrix4<T> Matrix4<T>::ONE = Matrix4<T>( 1 );
}