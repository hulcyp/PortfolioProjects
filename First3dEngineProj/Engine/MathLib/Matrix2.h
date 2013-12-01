#pragma once
#include <ErrorHandlingUtils.h>

#include "Vector2.h"

namespace pdh
{
	template< typename T >
	class Matrix2
	{
	public:
		Matrix2<T>( T init = 0 );
		Matrix2<T>( T arr[4] );
		Matrix2<T>( T m0, T m1, T m2, T m3 );
		Matrix2<T>( const Matrix2<T> &copy );


		Matrix2<T>& operator=( const Matrix2<T>& rhs );
		Matrix2<T> operator*( const Matrix2<T>& rhs );
		Matrix2<T> operator+( const Matrix2<T>& rhs );
		Matrix2<T> operator-( const Matrix2<T>& rhs );
		Matrix2<T>& operator+=( const Matrix2<T>& rhs );
		Matrix2<T>& operator-=( const Matrix2<T>& rhs );
		Matrix2<T>& operator*=( const Matrix2<T>& rhs );
		Matrix2<T>& operator-();
		const T& operator[]( int i ) const;
		bool operator==( const Matrix2<T>& rhs );
		bool operator!=( const Matrix2<T>& rhs );

		void transpose();
		void loadIdentity();
		T* getRawMatrix();

	private:
		T m_matrix[4];
	};



	template< typename T >
	Matrix2<T>::Matrix2( T init = 0 )
	{
		for( int i = 0; i < 4; ++i )
		{
			m_matrix[i] = init;
		}
	}

	template< typename T >
	Matrix2<T>::Matrix2( T arr[4] )
	{
		for( int i = 0; i < 4; ++i )
		{
			m_matrix[i] = arr[i];
		}
	}

	template< typename T >
	Matrix2<T>::Matrix2( T m0, T m1, T m2, T m3 )
	{
		m_matrix[0] = m0;
		m_matrix[1] = m1;
		m_matrix[2] = m2;
		m_matrix[3] = m3;
	}

	template< typename T >
	Matrix2<T>::Matrix2( const Matrix2<T>& copy )
	{
		for( int i = 0; i < 4; ++i )
		{
			m_matrix[i] = copy.m_matrix[i];
		}
	}

	template< typename T >
	Matrix2<T>& Matrix2<T>::operator=( const Matrix2<T>& rhs )
	{
		if( this != &rhs )
		{
			for( int i = 0; i < 4; ++i )
			{
				m_matrix[i] = rhs.m_matrix[i];
			}
		}
		return *this;
	}

	template< typename T >
	Matrix2<T> Matrix2<T>::operator*( const Matrix2<T>& rhs )
	{
		Matrix2<T> temp;
		temp[0] = m_matrix[0] * rhs.m_matrix[0] + m_matrix[1] * rhs.m_matrix[2];
		temp[1] = m_matrix[0] * rhs.m_matrix[1] + m_matrix[1] * rhs.m_matrix[3];
		temp[2] = m_matrix[2] * rhs.m_matrix[0] + m_matrix[3] * rhs.m_matrix[2];
		temp[3] = m_matrix[2] * rhs.m_matrix[1] + m_matrix[3] * rhs.m_matrix[3];

		return temp;
	}

	template< typename T >
	Matrix2<T> Matrix2<T>::operator+( const Matrix2<T>& rhs )
	{
		Matrix2<T> temp;
		for( int i = 0; i < 4; ++i )
		{
			temp.m_matrix[i] = m_matrix[i] * rhs.m_matrix[i];
		}

		return temp;
	}

	template< typename T >
	Matrix2<T> Matrix2<T>::operator-( const Matrix2<T>& rhs )
	{
		Matrix2<T> temp;
		for( int i = 0; i < 4; ++i )
		{
			temp.m_matrix[i] = m_matrix[i] - rhs.m_matrix[i];
		}
		
		return temp;
	}

	template< typename T >
	Matrix2<T>& Matrix2<T>::operator+=( const Matrix2<T>& rhs )
	{
		for( int i = 0; i < 4; ++i )
		{
			m_matrix[i] += rhs.m_matrix[i];
		}
		
		return *this;
	}

	template< typename T >
	Matrix2<T>& Matrix2<T>::operator-=( const Matrix2<T>& rhs )
	{
		for( int i = 0; i < 4; ++i )
		{
			m_matrix[i] -= rhs.m_matrix[i]
		}

		return *this;
	}

	template< typename T >
	Matrix2<T>& Matrix2<T>::operator*=( const Matrix2<T>& rhs )
	{
		*this = *this * rhs;
		return *this;
	}

	template< typename T >
	Matrix2<T>& Matrix2<T>::operator-()
	{
		for( int i = 0; i < 4; ++i )
		{
			m_matrix[i] = -m_matrix[i];
		}

		return *this;
	}

	//template< typename T >
	//T& Matrix2<T>::operator[]( int i )
	//{
	//	ASSERT( 0 <= i && i < 4 );
	//	return m_matrix[i];
	//}

	template< typename T >
	const T& Matrix2<T>::operator[]( int i ) const
	{
		ASSERT( 0 <= i && i < 4 );
		return m_matrix[i];	
	}

	template< typename T >
	bool Matrix2<T>::operator==( const Matrix2<T>& rhs )
	{
		bool equal = true;
		for( int i = 0; i < 4; ++i )
		{
			equal = equal && ( m_matrix[i] == rhs.m_matrix[i] );
		}
		return equal;
	}

	template< typename T >
	bool Matrix2<T>::operator!=( const Matrix2<T>& rhs )
	{
		bool notequal = false;
		for( int i = 0; i < 4; ++i )
		{
			notequal = notequal || ( m_matrix[i] != rhs.m_matrix[i] );
		}

		return notequal;
	}

	template< typename T >
	void Matrix2<T>::transpose()
	{
		std::swap( m_matrix[1], m_matrix[2] );
	}

	template< typename T >
	void Matrix2<T>::loadIdentity()
	{
		m_matrix[0] = 1;
		m_matrix[1] = 0;
		m_matrix[2] = 0;
		m_matrix[3] = 1;
	}

	template< typename T >
	T* Matrix2<T>::getRawMatrix()
	{
		return m_matrix;
	}

	template< typename T >
	Vector2<T> operator*( const Matrix2<T>& lhs, const Vector2<T>& rhs )
	{
		Vector2<T> temp;
		temp.x = lhs[0] * rhs.x + lhs[1] * rhs.y;
		temp.y = lhs[2] * rhs.x + lhs[3] * rhs.y;
		return temp;
	}
	template< typename T >
	Vector2<T> operator*( const Vector2<T>& lhs, const Matrix2<T>& rhs )
	{
		Vector2<T> temp;
		temp.x = lhs.x * rhs[0] + lhs.y * rhs[2];
		temp.y = lhs.x * rhs[1] + lhs.y * rhs[3];
		return temp;
	}
}	


