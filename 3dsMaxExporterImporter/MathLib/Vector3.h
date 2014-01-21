#pragma once

/*************************
* author: Preston Hulcy
* last updated: 3-2-13
*************************/

#include <cassert>
#include <ostream>
#include <StringUtils.h>
#include "MathFuncs.h"
#include "Vector2.h"

namespace pdh
{
	template <class T>
	class Vector3
	{

	public:

		Vector3<T>( T x = 0, T y = 0, T z = 0 );	
		Vector3<T>( const Vector3<T>& vec );
		Vector3<T>( const Vector2<T>& vec2, T z = 0 );

		Vector3<T>& operator=( const Vector3<T>& rhs );		
		T& getComp( int i );
		T& operator[]( int i );
		const T& operator[]( int i ) const;
			
		T dot( const Vector3<T>& vec ) const;
		T norm() const;
		T length() const;
		T lengthSqrd() const;
		void normalize();
		Vector3<T> normalized() const;
		Vector3<T> cross( const Vector3& vec );

		Vector3<T> operator+( const Vector3<T>& rhs ) const;
		Vector3<T> operator+( const T val ) const;			
		Vector3<T> operator-( const Vector3<T>& rhs ) const;
		Vector3<T> operator-( const T val ) const;			
		Vector3<T> operator*( const Vector3<T>& rhs ) const;
		Vector3<T> operator*( const T val ) const;
		Vector3<T> operator/( const Vector3<T>& rhs ) const;
		Vector3<T> operator/( const T val );
		Vector3<T> operator-() const;
		Vector3<T>& operator+=( const Vector3<T>& rhs );
		Vector3<T>& operator+=( const T real );
		Vector3<T>& operator-=( const Vector3<T>& rhs );
		Vector3<T>& operator-=( const T real );
		Vector3<T>& operator/=( const Vector3<T>& rhs );
		Vector3<T>& operator/=( const T real );
		Vector3<T>& operator*=( const Vector3<T>& rhs );
		Vector3<T>& operator*=( const T real );
		bool operator==( const Vector3<T>& rhs ) const;
		bool operator !=( const Vector3<T>& rhs ) const;
		bool isParallel( const Vector3<T>& rhs, T epsilon = MathFuncs<T>::EPSILON ) const;

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
		Vector3<T> scaledToLength( T newLength );
		bool isNormalized( T epsilon = EPSILON ) const;
		bool isZero( T epsilon = EPSILON ) const;
		bool isPerpendicular( const Vector3<T>& other, T epsilon = EPSILON ) const;
		std::string toString() const;
		std::string serializeToSTring() const;
		T sumOfElements() const;
		T productOfElements() const;
		T averageOfElements() const;
		T minElement() const;
		int minElementIndex() const;
		T maxElement() const;
		int maxElementIndex() const;
		Vector3<T> abs() const;
		Vector3<T> neg() const;
		Vector3<T> recip() const;
		Vector3<T> min( T ceil ) const;
		Vector3<T> min( const Vector3<T>& ceil ) const;
		Vector3<T> max( T floor ) const;
		Vector3<T> max( const Vector3<T>& floor ) const;
		Vector3<T> clamp( T floor, T ceil ) const;
		Vector3<T> clamp( const Vector3<T>& floor, const Vector3<T>& ceil ) const;
		T distance( const Vector3<T>& point ) const;
		T distanceSqrd( const Vector3<T>& point ) const;
		//Vector3<T> perpindicular() const;
		T scalarTripleProduct( const Vector3<T>& u, const Vector3<T>& v, const Vector3<T>& w );
		Vector3<T> reflect( const Vector3<T>& normal ) const;
		Vector3<T> refract( const Vector3<T>& normal, T negSideRefractionIndex, T posSideRefractionIndex ) const;
		Vector3<T> projectTo( const Vector3<T>& direction ) const;
		Vector3<T> projectToNorm( const Vector3<T>& direction ) const;
		T angleBetween( const Vector3<T>& other ) const;
		T angleBetweenNorm( const Vector3<T>& normalizedVec ) const;
		Vector3<T> lerp( const Vector3<T>& b, T t ) const;
		void decompose( const Vector3<T>& direction, Vector2<T>& outParallel, Vector3<T>& outPerp ) const;

		static void orthogonalize( const Vector3<T>& a, Vector3<T>& b );
		static void orthogonalize( const Vector3<T>& a, Vector3<T>& b, Vector3<T>& c );
		static bool areOrthogonal( const Vector3<T>& a, Vector3<T>& b, T epsilon = EPSILON );
		static bool areOrthogonal( const Vector3<T>& a, Vector3<T>& b, Vector3<T>& c, T epsilon = EPSILON );
		static void othonormalize( Vector3<T>& a, Vector3<T>& b );
		static void othonormalize( Vector3<T>& a, Vector3<T>& b, Vector3<T>& c );
		static Vector3<T> fromString( const std::string& str );

	public:
		T x;
		T y;
		T z;

		static const Vector3<T> ZERO;
		static const Vector3<T> ONE;
		static const Vector3<T> UNIT_X;
		static const Vector3<T> UNIT_Y;
		static const Vector3<T> UNIT_Z;

	};
}

#include "Vector3.inl"