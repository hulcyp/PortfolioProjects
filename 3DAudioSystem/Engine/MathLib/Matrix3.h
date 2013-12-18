#pragma once
#include <MonkyException.h>
#include <Assertion.h>

#include "Vector3.h"
#include "MathFuncs.h"
#include "Quaternion.h"

namespace Monky
{
	template< typename T >
	class Matrix3
	{
	public:
		Matrix3<T>( T init = 0 );
		Matrix3<T>( T arr[9] );
		Matrix3<T>( T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8 );
		Matrix3<T>( const Matrix3<T> &copy );
		Matrix3<T>( const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z );


		Matrix3<T>& operator=( const Matrix3<T>& rhs );
		Matrix3<T> operator*( const Matrix3<T>& rhs ) const;
		Matrix3<T> operator+( const Matrix3<T>& rhs ) const;
		Matrix3<T> operator-( const Matrix3<T>& rhs ) const;
		Matrix3<T>& operator+=( const Matrix3<T>& rhs );
		Matrix3<T>& operator-=( const Matrix3<T>& rhs );
		Matrix3<T>& operator*=( const Matrix3<T>& rhs );
		Matrix3<T>& operator-();
		T& operator[]( int i );
		const T& operator[]( int i ) const;
		bool operator==( const Matrix3<T>& rhs ) const;
		bool operator!=( const Matrix3<T>& rhs ) const;

		void transpose();
		Matrix3<T> getTranspose() const;
		void loadIdentity();
		T* getRawMatrix();

		Vector3<T> getRow( int i ) const;

		//------------------------------------------------------
		// Rotation functions
		//------------------------------------------------------
		void rotate( const Vector3<T>& axis, T angDegCCW );
		void rotate( float yaw, float pitch, float roll );
		void rotateRadians( const Vector3<T>& axis, T angRadCCW );
		void rotateX( T angRadCCW );
		void rotateY( T angRadCCW );
		void rotateZ( T angRadCCW );
		void rotate( const Quaternion<T>& quat );
		
		//------------------------------------------------------
		// Scaling functions
		//------------------------------------------------------
		void scale( const Vector3<T>& scaleVec );
		void scale( T scaleX, T scaleY, T scaleZ );
		void scale( T uniformScale );
		

		//------------------------------------------------------
		// Static functions
		//------------------------------------------------------
		static Matrix3<T> identity();
		static Matrix3<T> lookAt( const Vector3<T>& pos, const Vector3<T>& lookat );
		static Matrix3<T> createMatrixFromQuaternion( const Quaternion<T>& q );
		
		//------------------------------------------------------
		// Static const members
		//------------------------------------------------------
		static const Matrix3<T> IDENTITY;
		static const Matrix3<T> ZERO;
		static const Matrix3<T> ONE;

	private:
		T m_matrix[9];
	};

	template< typename T >
	Matrix3<T>::Matrix3( T init = 0 )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] = init;
		}
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>::Matrix3( T arr[9] )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] = arr[i];
		}
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>::Matrix3( T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8 )
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
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>::Matrix3( const Matrix3<T>& copy )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] = copy.m_matrix[i];
		}
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>::Matrix3( const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z )
	{
		m_matrix[0] = x.x;
		m_matrix[1] = x.y;
		m_matrix[2] = x.z;
		m_matrix[3] = y.x;
		m_matrix[4] = y.y;
		m_matrix[5] = y.z;
		m_matrix[6] = z.x;
		m_matrix[7] = z.y;
		m_matrix[8] = z.z;

	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>& Matrix3<T>::operator=( const Matrix3<T>& rhs )
	{
		if( this != &rhs )
		{
			for( int i = 0; i < 9; ++i )
			{
				m_matrix[i] = rhs.m_matrix[i];
			}
		}
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix3<T>::operator*( const Matrix3<T>& rhs ) const
	{
		Matrix3<T> temp;
		temp[0] = m_matrix[0] * rhs.m_matrix[0] + m_matrix[1] * rhs.m_matrix[3] + m_matrix[2] * rhs.m_matrix[6];
		temp[1] = m_matrix[0] * rhs.m_matrix[1] + m_matrix[1] * rhs.m_matrix[4] + m_matrix[2] * rhs.m_matrix[7];
		temp[2] = m_matrix[0] * rhs.m_matrix[2] + m_matrix[1] * rhs.m_matrix[5] + m_matrix[2] * rhs.m_matrix[8];
		
		temp[3] = m_matrix[3] * rhs.m_matrix[0] + m_matrix[4] * rhs.m_matrix[3] + m_matrix[5] * rhs.m_matrix[6];
		temp[4] = m_matrix[3] * rhs.m_matrix[1] + m_matrix[4] * rhs.m_matrix[4] + m_matrix[5] * rhs.m_matrix[7];
		temp[5] = m_matrix[3] * rhs.m_matrix[2] + m_matrix[4] * rhs.m_matrix[5] + m_matrix[5] * rhs.m_matrix[8];

		temp[6] = m_matrix[6] * rhs.m_matrix[0] + m_matrix[7] * rhs.m_matrix[3] + m_matrix[8] * rhs.m_matrix[6];
		temp[7] = m_matrix[6] * rhs.m_matrix[1] + m_matrix[7] * rhs.m_matrix[4] + m_matrix[8] * rhs.m_matrix[7];
		temp[8] = m_matrix[6] * rhs.m_matrix[2] + m_matrix[7] * rhs.m_matrix[5] + m_matrix[8] * rhs.m_matrix[8];

		return temp;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix3<T>::operator+( const Matrix3<T>& rhs ) const
	{
		Matrix3<T> temp;
		for( int i = 0; i < 9; ++i )
		{
			temp.m_matrix[i] = m_matrix[i] * rhs.m_matrix[i];
		}

		return temp;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix3<T>::operator-( const Matrix3<T>& rhs ) const
	{
		Matrix3<T> temp;
		for( int i = 0; i < 9; ++i )
		{
			temp.m_matrix[i] = m_matrix[i] - rhs.m_matrix[i];
		}
		
		return temp;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>& Matrix3<T>::operator+=( const Matrix3<T>& rhs )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] += rhs.m_matrix[i];
		}
		
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>& Matrix3<T>::operator-=( const Matrix3<T>& rhs )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] -= rhs.m_matrix[i]
		}

		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>& Matrix3<T>::operator*=( const Matrix3<T>& rhs )
	{
		*this = *this * rhs;
		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T>& Matrix3<T>::operator-()
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] = -m_matrix[i];
		}

		return *this;
	}
	//------------------------------------------------------
	template< typename T >
	T& Matrix3<T>::operator[]( int i )
	{
		assertion( 0 <= i && i < 9, "Matrix 3 index out of range" );
		return m_matrix[i];
	}
	//------------------------------------------------------
	template< typename T >
	const T& Matrix3<T>::operator[]( int i ) const
	{
		assertion( 0 <= i && i < 9, "Matrix 3 index out of range" );
		return m_matrix[i];	
	}
	//------------------------------------------------------
	template< typename T >
	bool Matrix3<T>::operator==( const Matrix3<T>& rhs ) const
	{
		bool equal = true;
		for( int i = 0; i < 9; ++i )
		{
			equal = equal && ( m_matrix[i] == rhs.m_matrix[i] );
		}
		return equal;
	}
	//------------------------------------------------------
	template< typename T >
	bool Matrix3<T>::operator!=( const Matrix3<T>& rhs ) const
	{
		bool notequal = false;
		for( int i = 0; i < 9; ++i )
		{
			notequal = notequal || ( m_matrix[i] != rhs.m_matrix[i] );
		}

		return notequal;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix3<T>::transpose()
	{
		std::swap( m_matrix[1], m_matrix[3] );
		std::swap( m_matrix[2], m_matrix[6] );
		std::swap( m_matrix[5], m_matrix[7] );
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix3<T>::getTranspose() const
	{
		Matrix3<T> temp = *this;
		temp.transpose();
		return temp;
	}
	//------------------------------------------------------
	template< typename T >
	void Matrix3<T>::loadIdentity()
	{
		m_matrix[0] = 1;
		m_matrix[1] = 0;
		m_matrix[2] = 0;
		m_matrix[3] = 0;
		m_matrix[4] = 1;
		m_matrix[5] = 0;
		m_matrix[6] = 0;
		m_matrix[7] = 0;
		m_matrix[8] = 1;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix3<T>::lookAt( const Vector3<T>& pos, const Vector3<T>& lookat )
	{
		Vector3<T> kc( Vector3<T>( pos.x - lookat.x, pos.y - lookat.y, pos.z - lookat.z ) );
		kc.normalize();
		Vector3<T> ic( Vector3<T>( 0.0f, 1.0f, 0.0f ).cross( kc ) );
		ic.normalize();
		Vector3<T> jc( kc.cross( ic ) );


		Matrix3<T> rot(	ic.x, ic.y, ic.z,
						jc.x, jc.y, jc.z,
						kc.x, kc.y, kc.z );

		return rot;
	}
	//------------------------------------------------------
	template< typename T >
	T* Matrix3<T>::getRawMatrix()
	{
		return m_matrix;
	}
	//------------------------------------------------------
	template< typename T >
	Vector3<T> Matrix3<T>::getRow( int i ) const
	{
		assertion( 0 <= i && i < 3, "Invalid Matrix 3 row index: &d", i );
		return Vector3<T>( m_matrix[ i * 3 ], m_matrix[ i * 3 + 1 ], m_matrix[ i * 3 + 2 ] );
	}
	//------------------------------------------------------
	template< typename T >
	Vector3<T> operator*( const Matrix3<T>& lhs, const Vector3<T>& rhs )
	{
		Vector3<T> temp;
		temp.x = lhs[0] * rhs.x + lhs[1] * rhs.y + lhs[2] * rhs.z;
		temp.y = lhs[3] * rhs.x + lhs[4] * rhs.y + lhs[5] * rhs.z;
		temp.z = lhs[6] * rhs.x + lhs[7] * rhs.y + lhs[8] * rhs.z;
		return temp;
	}
	//------------------------------------------------------
	template< typename T >
	Vector3<T> operator*( const Vector3<T>& lhs, const Matrix3<T>& rhs )
	{
		Vector3<T> temp;
		temp.x = lhs.x * rhs[0] + lhs.y * rhs[3] + lhs.z * rhs[6];
		temp.y = lhs.x * rhs[1] + lhs.y * rhs[4] + lhs.z * rhs[7];
		temp.z = lhs.x * rhs[2] + lhs.y * rhs[5] + lhs.z * rhs[8];
		return temp;
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::rotate( const Vector3<T>& axis, T angDegCCW )
	{
		T angRad = MathFuncs<T>::degreesToRadians( angDegCCW );
		rotateRadians( axis, angRad );
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::rotate( float yaw, float pitch, float roll )
	{
		rotateZ( MathFuncs<T>::degreesToRadians( yaw ) );
		rotateY( MathFuncs<T>::degreesToRadians( pitch ) );
		rotateX( MathFuncs<T>::degreesToRadians( roll ) );
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::rotateRadians( const Vector3<T>& axis, T angRadCCW )
	{
		Quaternion<T> quat = Quaternion<T>::makeQuaternionFromAxisAngle( angRadCCW, axis );
		Matrix3<T> rot = createMatrixFromQuaternion( quat );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::rotateX( T angRadCCW )
	{
		Matrix3<T> rot(	1,	0,						0,					
						0,	std::cos( angRadCCW ),	-std::sin( angRadCCW ),
						0,	std::sin( angRadCCW ),	std::cos( angRadCCW )  );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::rotateY( T angRadCCW )
	{
		Matrix3<T> rot(	std::cos( angRadCCW ),	0,	std::sin( angRadCCW ),
						0,						1,	0,					
						-std::sin( angRadCCW),	0,	std::cos( angRadCCW ) );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::rotateZ( T angRadCCW )
	{
		Matrix3<T> rot(	std::cos( angRadCCW ),	-std::sin( angRadCCW ),	0,
						std::sin( angRadCCW),	std::cos( angRadCCW ),	0,					
						0,						0,						1);

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::rotate( const Quaternion<T>& quat )
	{
		Matrix3<T> rot = createMatrixFromQuaternion( quat );

		*this *= rot;
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::scale( const Vector3<T>& scaleVec )
	{
		scale( scaleVec.x, scaleVec.y, scaleVec.z );
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::scale( T scaleX, T scaleY, T scaleZ )
	{
		Matrix3<T> tempScale = Matrix3<T>::identity();
		tempScale[0] = scaleX;
		tempScale[4] = scaleY;
		tempScale[8] = scaleZ;

		*this *= tempScale;
	}
	//------------------------------------------------------
	template< typename T >	
	void Matrix3<T>::scale( T uniformScale )
	{
		scale( uniformScale, uniformScale, uniformScale );	
	}
		
	//------------------------------------------------------
	// Static functions
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix3<T>::identity()
	{
		return Matrix3<T>::IDENTITY;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix3<T> Matrix3<T>::createMatrixFromQuaternion( const Quaternion<T>& q )
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
	
		Matrix3<T> rotMatrix(	i_x, i_y, i_z,
								j_x, j_y, j_z,
								k_x, k_y, k_z );
		
		rotMatrix.transpose();
		return rotMatrix;		
	}

	template< typename T >
	const Matrix3<T> Matrix3<T>::IDENTITY = Matrix3<T>( 1, 0, 0,
														0, 1, 0,
														0, 0, 1 );
	template< typename T >
	const Matrix3<T> Matrix3<T>::ZERO = Matrix3<T>( 0 );
	template< typename T >
	const Matrix3<T> Matrix3<T>::ONE = Matrix3<T>( 1 );


	//------------------------------------------------------
	// Utility functions
	//------------------------------------------------------
	template< typename T >
	Vector3<T> transformVector( const Vector3<T>& vec, const Matrix3<T>& mat )
	{
		Vector3<T> prod;

		prod.x = vec.x * mat[0] + vec.y * mat[3] + vec.z * mat[6];
		prod.y = vec.x * mat[1] + vec.y * mat[4] + vec.z * mat[7]; 
		prod.z = vec.x * mat[2] + vec.y * mat[5] + vec.z * mat[8];

		return prod;
	}
	//------------------------------------------------------
	template< typename T >
	Vector3<T> transformVector( const Matrix3<T>& mat, const Vector3<T>& vec )
	{
		Vector3<T> prod;

		prod.x = vec.x * mat[0] + vec.y * mat[1] + vec.z * mat[2];
		prod.y = vec.x * mat[3] + vec.y * mat[4] + vec.z * mat[5]; 
		prod.x = vec.x * mat[6] + vec.y * mat[7] + vec.z * mat[8];

		return prod;
	}	
}