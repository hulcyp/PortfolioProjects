#pragma once

/*************************
* author: Preston Hulcy
* last updated: 3-2-13
*************************/

#include <cassert>
#include <ostream>
#include <istream>
#include <StringUtils.h>
#include "MathUtils.h"

namespace pdh
{

	template <class T>
	class Vector2
	{
	public:
		Vector2<T>( T x = 0, T y = 0 );	
		Vector2<T>( const Vector2<T>& vec );
		
		T getComp( int i );
		T dot( const Vector2<T>& vec ) const;
		T norm() const;
		T length() const;
		T lengthSqrd() const;
		void normalize();
		Vector2<T> normalized();
		bool isParallel( const Vector2<T>& vec ) const;
		void flipAxis( size_t index );

		const Vector2<T> operator=( const Vector2<T>& rhs );		
		T& operator[]( int i );
		const T& operator[]( int i ) const;	
		Vector2<T> operator+( const Vector2<T>& rhs ) const;
		Vector2<T> operator+( const T val ) const;
		Vector2<T> operator-( const Vector2<T>& rhs ) const;
		Vector2<T> operator-( const T val ) const;
		Vector2<T>& operator-();
		Vector2<T> operator*( const Vector2<T>& rhs ) const;
		Vector2<T> operator*( const T val ) const;
		Vector2<T> operator/( const Vector2<T>& rhs ) const;
		Vector2<T> operator/( const T val ) const;
		Vector2<T>& operator+=( const Vector2<T>& rhs );
		Vector2<T>& operator+=( const T val );
		Vector2<T>& operator-=( const Vector2<T>& rhs );
		Vector2<T>& operator-=( const T val );
		Vector2<T>& operator*=( const Vector2<T>& rhs );
		Vector2<T>& operator*=( const T val );
		Vector2<T>& operator/=( const Vector2<T>& rhs );
		Vector2<T>& operator/=( const T val );
		bool operator==( const Vector2<T> &rhs );
		bool operator!=( const Vector2<T> &rhs );

		Vector2<T> xx();
		Vector2<T> yx();
		Vector2<T> yy();
		T aimedAngle() const;
		void scaleToLength( T newLength );
		Vector2<T> scaledToLength( T newLength );
		bool isNormalized( T epsilon = EPSILON ) const;
		bool isZero( T epsilon = EPSILON ) const;
		bool isPerpendicular( const Vector2<T>& other, T epsilon = EPSILON ) const;
		std::string toString() const;
		std::string serializeToSTring() const;
		T sumOfElements() const;
		T productOfElements() const;
		T averageOfElements() const;
		T minElement() const;
		int minElementIndex() const;
		T maxElement() const;
		int maxElementIndex() const;
		Vector2<T> abs() const;
		Vector2<T> neg() const;
		Vector2<T> recip() const;
		Vector2<T> min( T ceil ) const;
		Vector2<T> min( const Vector2<T>& ceil ) const;
		Vector2<T> max( T floor ) const;
		Vector2<T> max( const Vector2<T>& floor ) const;
		Vector2<T> clamp( T ceil, T floor ) const;
		Vector2<T> clamp( const Vector2<T>& ceil, const Vector2<T>& floor ) const;
		T distance( const Vector2<T>& point ) const;
		T distanceSqrd( const Vector2<T>& point ) const;
		Vector2<T> perpindicular() const;
		Vector2<T> reflect( const Vector2<T>& normal ) const;
		Vector2<T> refract( const Vector2<T>& normal, T negSideRefractionIndex, T posSideRefractionIndex ) const;
		Vector2<T> projectTo( const Vector2<T>& direction ) const;
		Vector2<T> projectToNorm( const Vector2<T>& direction ) const;
		T angleBetween( const Vector2<T>& other ) const;
		T angleBetweenNorm( const Vector2<T>& normalizedVec ) const;
		Vector2<T> lerp( const Vector2<T>& b, T t ) const;
		void decompose( const Vector2<T>& direction, Vector2<T>& outParallel, Vector2<T>& outPerp ) const;

		static void orthogonalize( const Vector2<T>& a, Vector2<T>& b );
		static void othonormalize( Vector2<T>& a, Vector2<T>& b );
		static Vector2<T> fromString( const std::string& str );

	public:
		T x;
		T y;

		static const Vector2<T> ZERO;
		static const Vector2<T> ONE;
		static const Vector2<T> UNIT_X;
		static const Vector2<T> UNIT_Y;
	};
	
}

#include "Vector2.inl"