#pragma once

/*************************
* author: Preston Hulcy
* last updated: 10-29-12
*************************/

#include "Vector3.h"

namespace pdh
{
	template < class T >
	class Matrix4
	{

	public:
		Matrix4<T>( T init = 0);
		Matrix4<T>( T arr[16] );
		Matrix4<T>( T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, 
			T m8, T m9, T m10, T m11, T m12, T m13, T m14, T m15 );
		Matrix4<T>( const Matrix4<T> &copy );

		T getDeterminant();							//implement
		Matrix4<T> getTranspose();
		T getTrace();
		void loadIdentity();		
		void transpose();
		T* getRawMatrix();	
		

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
		
		/////////////////////////////////////////////////////////
		// Static functions
		////////////////////////////////////////////////////////
		static Matrix4<T> identity();
		static Matrix4<T> lookAt( const Vector3<T>& pos, const Vector3<T>& lookat );
		static Matrix4<T> ortho( T left, T right, T top, T bot, T znear, T zfar );
		static Matrix4<T> perspective( T fov, T aspect, T znear, T zfar );
		
	private:
		T m_matrix[16];
	};
}

#include "Matrix4.inl"