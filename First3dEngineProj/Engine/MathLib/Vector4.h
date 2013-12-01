#pragma once

/*************************
* author: Preston Hulcy
* last updated: 3-3-13
*************************/

#include <cassert>
#include <ostream>
#include <StringUtils.h>
#include "MathFuncs.h"
#include "Vector2.h"
#include "Vector3.h"

namespace pdh
{
	template< typename T >
	class Vector4
	{
	public:
		Vector4<T>( T x = 0, T y = 0, T z = 0, T w = 0 );
		Vector4<T>( const Vector4<T>& vec );
		Vector4<T>( const Vector3<T>& vec, T w = 0 );

		Vector4<T>& operator=( const Vector4<T>& rhs );		
		T& getComp( int i );
		T& operator[]( int i );
		const T& operator[]( int i ) const;
			
		T dot( const Vector4<T>& vec ) const;
		T length() const;
		T lengthSqrd() const;
		void normalize();
		Vector4<T> normalized() const;
		Vector4<T> cross( const Vector4& vec );

		Vector4<T> operator+( const Vector4<T>& rhs ) const;
		Vector4<T> operator+( const T val ) const;			
		Vector4<T> operator-( const Vector4<T>& rhs ) const;
		Vector4<T> operator-( const T val ) const;			
		Vector4<T> operator*( const Vector4<T>& rhs ) const;
		Vector4<T> operator*( const T val ) const;
		Vector4<T> operator/( const Vector4<T>& rhs ) const;
		Vector4<T> operator/( const T val ) const;
		Vector4<T> operator-() const;
		Vector4<T>& operator+=( const Vector4<T>& rhs );
		Vector4<T>& operator+=( const T real );
		Vector4<T>& operator-=( const Vector4<T>& rhs );
		Vector4<T>& operator-=( const T real );
		Vector4<T>& operator/=( const Vector4<T>& rhs );
		Vector4<T>& operator/=( const T real );
		Vector4<T>& operator*=( const Vector4<T>& rhs );
		Vector4<T>& operator*=( const T real );
		bool operator==( const Vector4<T>& rhs ) const;
		bool operator !=( const Vector4<T>& rhs ) const;
		bool isParallel( const Vector4<T>& rhs, T epsilon = MathFuncs<T>::EPSILON ) const;

		Vector2<T> xx();
		Vector2<T> xy();
		Vector2<T> yx();
		Vector2<T> yy();
		Vector3<T> xxx();
		Vector3<T> xxy();
		Vector3<T> xxz();
		Vector3<T> xyx();
		Vector3<T> xyy();
		Vector3<T> xyz();
		Vector3<T> xzx();
		Vector3<T> xzy();
		Vector3<T> xzz();
		Vector3<T> yxx();
		Vector3<T> yxy();
		Vector3<T> yxz();
		Vector3<T> yyx();
		Vector3<T> yyy();
		Vector3<T> yyz();
		Vector3<T> yzx();
		Vector3<T> yzy();
		Vector3<T> yzz();
		Vector3<T> zxx();
		Vector3<T> zxy();
		Vector3<T> zxz();
		Vector3<T> zyx();
		Vector3<T> zyy();
		Vector3<T> zyz();
		Vector3<T> zzx();
		Vector3<T> zzy();
		Vector3<T> zzz();

		void scaleToLength( T newLength );
		Vector4<T> scaledToLength( T newLength );
		bool isNormalized( T epsilon = MathFuncs<T>::EPSILON ) const;
		bool isZero( T epsilon = MathFuncs<T>::EPSILON ) const;
		bool isPerpendicular( const Vector4<T>& other, T epsilon = MathFuncs<T>::EPSILON ) const;
		std::string toString() const;
		std::string serializeToSTring() const;
		T sumOfElements() const;
		T productOfElements() const;
		T averageOfElements() const;
		T minElement() const;
		int minElementIndex() const;
		T maxElement() const;
		int maxElementIndex() const;
		Vector4<T> abs() const;
		Vector4<T> neg() const;
		Vector4<T> recip() const;
		Vector4<T> min( T ceil ) const;
		Vector4<T> min( const Vector4<T>& ceil ) const;
		Vector4<T> max( T floor ) const;
		Vector4<T> max( const Vector4<T>& floor ) const;
		Vector4<T> clamp( T floor, T ceil ) const;
		Vector4<T> clamp( const Vector4<T>& floor, const Vector4<T>& ceil ) const;
		T distance( const Vector4<T>& point ) const;
		T distanceSqrd( const Vector4<T>& point ) const;
		
		static Vector4<T> fromString( const std::string& str );

	public:
		T x;
		T y;
		T z;
		T w;

		static const Vector4<T> ZERO;
		static const Vector4<T> ONE;
		static const Vector4<T> UNIT_X;
		static const Vector4<T> UNIT_Y;
		static const Vector4<T> UNIT_Z;
		static const Vector4<T> UNIT_W;
	};
}

#include "Vector4.inl"