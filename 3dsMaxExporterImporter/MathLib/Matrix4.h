#pragma once

/*************************
* author: Preston Hulcy
* last updated: 10-29-12
*************************/

#include "Vector3.h"
#include "Matrix3.h"
#include "Quaternion.h"

namespace pdh
{
	template < class T >
	class Matrix4
	{

	public:
		Matrix4<T>( T init = 0);
		Matrix4<T>( T arr[16] );
		Matrix4<T>( T arr[4][3] );
		Matrix4<T>( T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, 
			T m8, T m9, T m10, T m11, T m12, T m13, T m14, T m15 );
		Matrix4<T>( const Matrix4<T> &copy );
		Matrix4<T>( const Matrix3<T>& copy );

		//T getDeterminant();							//implement
		Matrix4<T> getTranspose() const;
		T getTrace();
		void loadIdentity();		
		void transpose();
		T* getRawMatrix();
		Matrix3<T> getUpper3x3() const;
		Matrix4<T> inverse() const;
		

		Matrix4<T>& operator=( const Matrix4<T> &rhs );
		Matrix4<T> operator*( const Matrix4<T>& rhs ) const;
		Matrix4<T> operator*( const T val );
		Matrix4<T>& operator*=( const Matrix4<T>& rhs );
		Matrix4<T>& operator*=( T val );
		Matrix4<T> operator+( Matrix4<T> rhs ) const;
		Matrix4<T> operator+( T val );
		Matrix4<T>& operator+=( const Matrix4<T>& rhs );
		Matrix4<T>& operator+=( T val );
		Matrix4<T> operator-( const Matrix4<T>& rhs ) const;
		Matrix4<T> operator-( T val ) const;
		Matrix4<T>& operator-();
		Matrix4<T>& operator-=( const Matrix4<T>& rhs );
		Matrix4<T>& operator-=( T val );

		T& operator[]( int i );
		const T& operator[]( int i ) const;

		//------------------------------------------------------
		// Rotation functions
		//------------------------------------------------------
		void rotate( const Vector3<T>& axis, T angDegCCW );
		void rotateRadians( const Vector3<T>& axis, T angRadCCW );
		void rotateX( T angRadCCW );
		void rotateY( T angRadCCW );
		void rotateZ( T angRadCCW );
		void rotate( const Quaternion<T>& quat );
		Matrix3<T> getRotation() const;

		//------------------------------------------------------
		// Translation functions
		//------------------------------------------------------
		void translate( const Vector3<T>& trans );
		void translate( T transX, T transY, T transZ );
		Vector3<T> getTranslation() const;

		//------------------------------------------------------
		// Scaling functions
		//------------------------------------------------------
		void scale( const Vector3<T>& scaleVec );
		void scale( T scaleX, T scaleY, T scaleZ );
		void scale( T uniformScale );
		Vector3<T> getUniformScaleFactors() const;
		
		
		//------------------------------------------------------
		// Static functions
		//------------------------------------------------------
		static Matrix4<T> identity();
		static Matrix4<T> lookAt( const Vector3<T>& pos, const Vector3<T>& lookat );
		static Matrix4<T> ortho( T left, T right, T top, T bot, T znear, T zfar );
		static Matrix4<T> perspective( T fov, T aspect, T znear, T zfar );
		static Matrix4<T> createMatrixFromQuaternion( const Quaternion<T>& q );

		//------------------------------------------------------
		// Static const members
		//------------------------------------------------------
		static const Matrix4<T> IDENTITY;
		static const Matrix4<T> ZERO;
		static const Matrix4<T> ONE;
		
	private:
		T m_matrix[16];
	};
}

#include "Matrix4.inl"