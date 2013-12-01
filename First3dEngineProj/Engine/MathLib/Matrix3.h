#pragma once
#include <ErrorHandlingUtils.h>

#include "Vector3.h"

namespace pdh
{
	template< typename T >
	class Matrix3
	{
	public:
		Matrix3<T>( T init = 0 );
		Matrix3<T>( T arr[9] );
		Matrix3<T>( T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8 );
		Matrix3<T>( const Matrix3<T> &copy );


		Matrix3<T>& operator=( const Matrix3<T>& rhs );
		Matrix3<T> operator*( const Matrix3<T>& rhs );
		Matrix3<T> operator+( const Matrix3<T>& rhs );
		Matrix3<T> operator-( const Matrix3<T>& rhs );
		Matrix3<T>& operator+=( const Matrix3<T>& rhs );
		Matrix3<T>& operator-=( const Matrix3<T>& rhs );
		Matrix3<T>& operator*=( const Matrix3<T>& rhs );
		Matrix3<T>& operator-();
		const T& operator[]( int i ) const;
		bool operator==( const Matrix3<T>& rhs );
		bool operator!=( const Matrix3<T>& rhs );

		void transpose();
		void loadIdentity();
		T* getRawMatrix();

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

	template< typename T >
	Matrix3<T>::Matrix3( T arr[9] )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] = arr[i];
		}
	}

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
		m_matrix[9] = m9;
	}

	template< typename T >
	Matrix3<T>::Matrix3( const Matrix3<T>& copy )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] = copy.m_matrix[i];
		}
	}

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

	template< typename T >
	Matrix3<T> Matrix3<T>::operator*( const Matrix3<T>& rhs )
	{
		Matrix2<T> temp;
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

	template< typename T >
	Matrix3<T> Matrix3<T>::operator+( const Matrix3<T>& rhs )
	{
		Matrix3<T> temp;
		for( int i = 0; i < 9; ++i )
		{
			temp.m_matrix[i] = m_matrix[i] * rhs.m_matrix[i];
		}

		return temp;
	}

	template< typename T >
	Matrix3<T> Matrix3<T>::operator-( const Matrix3<T>& rhs )
	{
		Matrix3<T> temp;
		for( int i = 0; i < 9; ++i )
		{
			temp.m_matrix[i] = m_matrix[i] - rhs.m_matrix[i];
		}
		
		return temp;
	}

	template< typename T >
	Matrix3<T>& Matrix3<T>::operator+=( const Matrix3<T>& rhs )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] += rhs.m_matrix[i];
		}
		
		return *this;
	}

	template< typename T >
	Matrix3<T>& Matrix3<T>::operator-=( const Matrix3<T>& rhs )
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] -= rhs.m_matrix[i]
		}

		return *this;
	}

	template< typename T >
	Matrix3<T>& Matrix3<T>::operator*=( const Matrix3<T>& rhs )
	{
		*this = *this * rhs;
		return *this;
	}

	template< typename T >
	Matrix3<T>& Matrix3<T>::operator-()
	{
		for( int i = 0; i < 9; ++i )
		{
			m_matrix[i] = -m_matrix[i];
		}

		return *this;
	}

	template< typename T >
	const T& Matrix3<T>::operator[]( int i ) const
	{
		ASSERT( 0 <= i && i < 9 );
		return m_matrix[i];	
	}

	template< typename T >
	bool Matrix3<T>::operator==( const Matrix3<T>& rhs )
	{
		bool equal = true;
		for( int i = 0; i < 9; ++i )
		{
			equal = equal && ( m_matrix[i] == rhs.m_matrix[i] );
		}
		return equal;
	}

	template< typename T >
	bool Matrix3<T>::operator!=( const Matrix3<T>& rhs )
	{
		bool notequal = false;
		for( int i = 0; i < 9; ++i )
		{
			notequal = notequal || ( m_matrix[i] != rhs.m_matrix[i] );
		}

		return notequal;
	}

	template< typename T >
	void Matrix3<T>::transpose()
	{
		std::swap( m_matrix[1], m_matrix[3] );
		std::swap( m_matrix[2], m_matrix[6] );
		std::swap( m_matrix[5], m_matrix[7] );
	}

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

	template< typename T >
	T* Matrix3<T>::getRawMatrix()
	{
		return m_matrix;
	}

	template< typename T >
	Vector3<T> operator*( const Matrix3<T>& lhs, const Vector3<T>& rhs )
	{
		Vector3<T> temp;
		temp.x = lhs[0] * rhs.x + lhs[1] * rhs.y + lhs[2] * rhs.z;
		temp.y = lhs[3] * rhs.x + lhs[4] * rhs.y + lhs[5] * rhs.z;
		temp.z = lhs[6] * rhs.x + lhs[7] * rhs.y + lhs[8] * rhs.z;
		return temp;
	}
	template< typename T >
	Vector3<T> operator*( const Vector3<T>& lhs, const Matrix3<T>& rhs )
	{
		Vector3<T> temp;
		temp.x = lhs.x * rhs[0] + lhs.y * rhs[3] + lhs.z * rhs[6];
		temp.y = lhs.x * rhs[1] + lhs.y * rhs[4] + lhs.z * rhs[7];
		temp.z = lhs.x * rhs[2] + lhs.y * rhs[5] + lhs.z * rhs[8];
		return temp;
	}
}