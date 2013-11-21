#pragma once

#include "Vector3.h"

namespace Monky
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
		Quaternion<T> inverse() const;
		
		//------------------------------------------------------
		// Static functions
		//------------------------------------------------------
		static Quaternion<T> makeQuaternionFromAxisAngle( T angle, Vector3<T> axis );
		static Quaternion<T> slerp( const Quaternion<T>& q1, const Quaternion<T>& q2, T t );

	private:
		Vector3<T> m_vector;
		T m_real;
	};
}

#include "Quaternion.inl"