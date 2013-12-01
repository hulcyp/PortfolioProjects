#pragma once

#include "Vector3.h"

namespace pdh
{
	template< class T >
	class Quaternion
	{
	public:
		Quaternion<T>();
		Quaternion<T>( T real, Vector3<T>& vector );
		Quaternion<T>( const Quaternion& copy );

		Quaternion<T>& operator=( const Quaternion& rhs );
		T dot( const Quaternion<T>& rhs ) const;
		Quaternion<T> operator-();
		Quaternion<T> operator+( const Quaternion<T>& rhs ) const;
		Quaternion<T> operator*( const T real ) const;
		Quaternion<T> conjugate() const;
		T getReal() const;
		Vector3<T> getVector() const;
		T length() const;
		void normalize();

	private:
		T m_real;
		Vector3<T> m_vector;
	};
}

#include "Quaternion.inl"