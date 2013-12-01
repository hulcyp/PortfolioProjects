#pragma once
#include "Quaternion.h"
#include "Matrix4.h"
#include "Vector3.h"
#include <memory>


namespace pdh
{
	template< class T >
	Matrix4<T> convertToMatrix( const Quaternion<T>& q );

	template< class T >
	Quaternion<T> makeQuaternionFromAxisAngle( T angle, Vector3<T> axis );

	template< class T >
	Quaternion<T> slerp( const Quaternion<T>& q1, const Quaternion<T>& q2, T t );
}

#include "QuaternionLibrary.inl"