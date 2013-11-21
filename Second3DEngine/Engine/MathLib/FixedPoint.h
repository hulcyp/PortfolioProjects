#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 7/11/2013
//	Description: Fixed point class uses 16.16 split for the repesentation of the number
//
//---------------------------------------------
#include <ostream>

namespace Monky
{
	class FixedPoint
	{
	public:
		FixedPoint();
		FixedPoint( const FixedPoint& copy );
		FixedPoint( int intDefaultValue );
		FixedPoint( unsigned int uIntDefaultValue );
		FixedPoint( float floatDefaultValue );
		FixedPoint( double doubleDefaultValue );
		~FixedPoint();

		int getAsInt() const;
		float getAsFloat() const;
		double getAsDouble() const;

		operator int() const;
		operator unsigned int() const;
		operator float() const;
		operator double() const;
		
		const FixedPoint& operator=( const FixedPoint& copy );
		const FixedPoint& operator=( int& copy );
		const FixedPoint& operator=( float& copy );
		const FixedPoint& operator=( double& copy );
		
		FixedPoint operator+( const FixedPoint& rhs ) const;
		FixedPoint operator-( const FixedPoint& rhs ) const;
		FixedPoint operator*( const FixedPoint& rhs ) const;
		FixedPoint operator/( const FixedPoint& rhs ) const;

		FixedPoint& operator+=( const FixedPoint& rhs );
		FixedPoint& operator-=( const FixedPoint& rhs );
		FixedPoint& operator*=( const FixedPoint& rhs );
		FixedPoint& operator/=( const FixedPoint& rhs );

		FixedPoint operator+( const int rhs ) const;
		FixedPoint operator-( const int rhs ) const;
		FixedPoint operator*( const int rhs ) const;
		FixedPoint operator/( const int rhs ) const;

		FixedPoint& operator+=( const int rhs );
		FixedPoint& operator-=( const int rhs );
		FixedPoint& operator*=( const int rhs );
		FixedPoint& operator/=( const int rhs );

		FixedPoint operator+( const unsigned int rhs ) const;
		FixedPoint operator-( const unsigned int rhs ) const;
		FixedPoint operator*( const unsigned int rhs ) const;
		FixedPoint operator/( const unsigned int rhs ) const;

		FixedPoint& operator+=( const unsigned int rhs );
		FixedPoint& operator-=( const unsigned int rhs );
		FixedPoint& operator*=( const unsigned int rhs );
		FixedPoint& operator/=( const unsigned int rhs );

		FixedPoint operator+( const float rhs ) const;
		FixedPoint operator-( const float rhs ) const;
		FixedPoint operator*( const float rhs ) const;
		FixedPoint operator/( const float rhs ) const;

		FixedPoint& operator+=( const float rhs );
		FixedPoint& operator-=( const float rhs );
		FixedPoint& operator*=( const float rhs );
		FixedPoint& operator/=( const float rhs );

		FixedPoint operator+( const double rhs ) const;
		FixedPoint operator-( const double rhs ) const;
		FixedPoint operator*( const double rhs ) const;
		FixedPoint operator/( const double rhs ) const;

		FixedPoint& operator+=( const double rhs );
		FixedPoint& operator-=( const double rhs );
		FixedPoint& operator*=( const double rhs );
		FixedPoint& operator/=( const double rhs );

		FixedPoint operator-() const;
		FixedPoint operator+() const;
		
		bool operator!() const;
		bool operator<( const FixedPoint& rhs ) const;
		bool operator>( const FixedPoint& rhs ) const;
		bool operator==( const FixedPoint& rhs ) const;
		bool operator<=( const FixedPoint& rhs ) const;
		bool operator>=( const FixedPoint& rhs ) const;
		bool operator!=( const FixedPoint& rhs ) const;

		bool operator<( const int rhs ) const;
		bool operator>( const int rhs ) const;
		bool operator==( const int rhs ) const;
		bool operator<=( const int rhs ) const;
		bool operator>=( const int rhs ) const;
		bool operator!=( const int rhs ) const;

		bool operator<( const unsigned int rhs ) const;
		bool operator>( const unsigned int rhs ) const;
		bool operator==( const unsigned int rhs ) const;
		bool operator<=( const unsigned int rhs ) const;
		bool operator>=( const unsigned int rhs ) const;
		bool operator!=( const unsigned int rhs ) const;

		bool operator<( const float rhs ) const;
		bool operator>( const float rhs ) const;
		bool operator==( const float rhs ) const;
		bool operator<=( const float rhs ) const;
		bool operator>=( const float rhs ) const;
		bool operator!=( const float rhs ) const;
		
		bool operator<( const double rhs ) const;
		bool operator>( const double rhs ) const;
		bool operator==( const double rhs ) const;
		bool operator<=( const double rhs ) const;
		bool operator>=( const double rhs ) const;
		bool operator!=( const double rhs ) const;

		FixedPoint operator&( const FixedPoint& rhs ) const;
		FixedPoint operator|( const FixedPoint& rhs ) const;
		FixedPoint operator~() const;
		FixedPoint operator^( const FixedPoint& rhs ) const;
		FixedPoint operator<<( int numBitsToTheLeft ) const;
		FixedPoint operator>>( int numBItsToTheRight ) const;

		FixedPoint operator&( const int rhs ) const;
		FixedPoint operator|( const int rhs ) const;
		FixedPoint operator^( const int rhs ) const;

		FixedPoint operator&( const unsigned int rhs ) const;
		FixedPoint operator|( const unsigned int rhs ) const;
		FixedPoint operator^( const unsigned int rhs ) const;

		FixedPoint& operator&=( const FixedPoint& rhs );
		FixedPoint& operator|=( const FixedPoint& rhs );
		FixedPoint& operator^=( const FixedPoint& rhs );
		FixedPoint& operator<<=( int numBitsToTheLeft );
		FixedPoint& operator>>=( int numBItsToTheRight );

		FixedPoint& operator&=( const int rhs );
		FixedPoint& operator|=( const int rhs );
		FixedPoint& operator^=( const int rhs );

		FixedPoint& operator&=( const unsigned int rhs );
		FixedPoint& operator|=( const unsigned int rhs );
		FixedPoint& operator^=( const unsigned int rhs );

	private:
		int m_data;
	};

	std::ostream& operator<<( std::ostream& os, const FixedPoint& fp );

	FixedPoint operator+( const int lhs, const FixedPoint& rhs );
	FixedPoint operator-( const int lhs, const FixedPoint& rhs );
	FixedPoint operator*( const int lhs, const FixedPoint& rhs );
	FixedPoint operator/( const int lhs, const FixedPoint& rhs );

	FixedPoint operator+( const unsigned int lhs, const FixedPoint& rhs );
	FixedPoint operator-( const unsigned int lhs, const FixedPoint& rhs );
	FixedPoint operator*( const unsigned int lhs, const FixedPoint& rhs );
	FixedPoint operator/( const unsigned int lhs, const FixedPoint& rhs );

	FixedPoint operator+( const float lhs, const FixedPoint& rhs );
	FixedPoint operator-( const float lhs, const FixedPoint& rhs );
	FixedPoint operator*( const float lhs, const FixedPoint& rhs );
	FixedPoint operator/( const float lhs, const FixedPoint& rhs );

	FixedPoint operator+( const double lhs, const FixedPoint& rhs );
	FixedPoint operator-( const double lhs, const FixedPoint& rhs );
	FixedPoint operator*( const double lhs, const FixedPoint& rhs );
	FixedPoint operator/( const double lhs, const FixedPoint& rhs );

	bool operator<( const int lhs, const FixedPoint& rhs );
	bool operator>( const int lhs, const FixedPoint& rhs );
	bool operator==( const int lhs, const FixedPoint& rhs );
	bool operator<=( const int lhs, const FixedPoint& rhs );
	bool operator>=( const int lhs, const FixedPoint& rhs );
	bool operator!=( const int lhs, const FixedPoint& rhs );

	bool operator<( const unsigned int lhs, const FixedPoint& rhs );
	bool operator>( const unsigned int lhs, const FixedPoint& rhs );
	bool operator==( const unsigned int lhs, const FixedPoint& rhs );
	bool operator<=( const unsigned int lhs, const FixedPoint& rhs );
	bool operator>=( const unsigned int lhs, const FixedPoint& rhs );
	bool operator!=( const unsigned int lhs, const FixedPoint& rhs );

	bool operator<( const float lhs, const FixedPoint& rhs );
	bool operator>( const float lhs, const FixedPoint& rhs );
	bool operator==( const float lhs, const FixedPoint& rhs );
	bool operator<=( const float lhs, const FixedPoint& rhs );
	bool operator>=( const float lhs, const FixedPoint& rhs );
	bool operator!=( const float lhs, const FixedPoint& rhs );

	bool operator<( const double lhs, const FixedPoint& rhs );
	bool operator>( const double lhs, const FixedPoint& rhs );
	bool operator==( const double lhs, const FixedPoint& rhs );
	bool operator<=( const double lhs, const FixedPoint& rhs );
	bool operator>=( const double lhs, const FixedPoint& rhs );
	bool operator!=( const double lhs, const FixedPoint& rhs );
	
	FixedPoint operator&( const int lhs, const FixedPoint& rhs );
	FixedPoint operator|( const int lhs, const FixedPoint& rhs );
	FixedPoint operator^( const int lhs, const FixedPoint& rhs );

	FixedPoint operator&( const unsigned int lhs, const FixedPoint& rhs );
	FixedPoint operator|( const unsigned int lhs, const FixedPoint& rhs );
	FixedPoint operator^( const unsigned int lhs, const FixedPoint& rhs );
}