#pragma once
#include <memory>
#include "Matrix4.h"
#include "Vector3.h"
#include "MathUtils.h"

namespace pdh
{
	template< class T >
	Matrix4<T> createTranslateMatrix( Vector3<T>& pos );

	template< class T >
	Matrix4<T> addTranslateToMatrix( Vector3<T>& pos, const Matrix4<T>& matrix );

	template< class T >
	Matrix4<T> alignWithAxis( Vector3<T>& i, Vector3<T>& j, Vector3<T>& k );

	template< class T >
	Matrix4<T> gl_perspectiveProjection( T fov, T aspect, T znear, T zfar );

	template< class T >
	Matrix4<T> gl_orthographicProjection( T left, T right, T top, T bot, T near, T far );
}

#include "Matrix4Factory.inl"