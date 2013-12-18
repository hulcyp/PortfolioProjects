#include "FixedPoint.h"
#include "MathFuncs.h"

namespace Monky
{
	FixedPoint::FixedPoint()
		:	m_data( 0 )
	{}
	//----------------------------------------------------------------------
	FixedPoint::FixedPoint( const FixedPoint& copy )
		:	m_data( copy.m_data )
	{}
	//----------------------------------------------------------------------
	FixedPoint::FixedPoint( int intDefaultValue )
		:	m_data( 0 )
	{
		m_data |= ( intDefaultValue << 16 );
	}
	//----------------------------------------------------------------------
	FixedPoint::FixedPoint( unsigned int uIntDefaultValue )
		:	m_data( 0 )
	{
		m_data |= ( uIntDefaultValue << 16 );
	}
	//----------------------------------------------------------------------
	FixedPoint::FixedPoint( float floatDefaultValue )
		:	m_data( 0 )
	{
		int intPart = (int)floatDefaultValue;

		float decimalPart = std::abs( floatDefaultValue - intPart );
		m_data |= ( intPart << 16 );
		float currentDecimalPlaceVal = 0.5f;
		int currentBit = 15;
		while( decimalPart >= 0.0f && currentBit >= 0 )
		{
			if( decimalPart >= currentDecimalPlaceVal )
			{
				m_data |= ( 1 << currentBit );
				decimalPart -= currentDecimalPlaceVal;
			}
			currentDecimalPlaceVal *= 0.5f;
			--currentBit;
		}
	}
	//----------------------------------------------------------------------
	FixedPoint::FixedPoint( double doubleDefaultValue )
	{
		int intPart = (int)doubleDefaultValue;

		double decimalPart = std::abs( doubleDefaultValue - intPart );
		m_data |= ( intPart << 16 );
		double currentDecimalPlaceVal = 0.5;
		int currentBit = 15;
		while( decimalPart >= 0.0f && currentBit >= 0 )
		{
			if( decimalPart >= currentDecimalPlaceVal )
			{
				m_data |= ( 1 << currentBit );
				decimalPart -= currentDecimalPlaceVal;
			}
			currentDecimalPlaceVal *= 0.5;
			--currentBit;
		}
	}
	//----------------------------------------------------------------------
	FixedPoint::~FixedPoint()
	{}
	//----------------------------------------------------------------------
	int FixedPoint::getAsInt() const
	{
		int dataAsInt = (int)(short)( m_data >> 16 );	
		return dataAsInt;
	}
	//----------------------------------------------------------------------
	float FixedPoint::getAsFloat() const
	{
		float dataAsFloat = (float)getAsInt();
		float currentDecimalPlaceVal = MathFuncs<float>::getSign( dataAsFloat )*0.5f;
		for( int currentBit = 15; currentBit >= 0; --currentBit )
		{
			if( ( 1 & ( m_data >> currentBit ) ) == 1 )
				dataAsFloat += currentDecimalPlaceVal; 
			currentDecimalPlaceVal *= 0.5f;
		}
		return dataAsFloat;
	}
	//----------------------------------------------------------------------
	double FixedPoint::getAsDouble() const
	{
		return (double)getAsFloat();
	}
	//----------------------------------------------------------------------
	FixedPoint::operator int() const
	{
		return getAsInt();
	}
	//----------------------------------------------------------------------
	FixedPoint::operator unsigned int() const
	{
		return ( unsigned int ) getAsInt();
	}
	//----------------------------------------------------------------------
	FixedPoint::operator float() const
	{
		return getAsFloat();
	}
	//----------------------------------------------------------------------
	FixedPoint::operator double() const
	{
		return getAsDouble();
	}
	//----------------------------------------------------------------------
	const FixedPoint& FixedPoint::operator=( const FixedPoint& copy )
	{
		if( this != &copy )
			m_data = copy.m_data;
		return *this;
	}
	//----------------------------------------------------------------------
	const FixedPoint& FixedPoint::operator=( int& copy )
	{
		*this = FixedPoint( copy );
		return *this;
	}
	//----------------------------------------------------------------------
	const FixedPoint& FixedPoint::operator=( float& copy )
	{
		*this = FixedPoint( copy );
		return *this;
	}
	//----------------------------------------------------------------------
	const FixedPoint& FixedPoint::operator=( double& copy )
	{
		*this = FixedPoint( copy );
		return *this;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator+( const FixedPoint& rhs ) const
	{
		FixedPoint temp;
		temp.m_data = m_data + rhs.m_data;
		return temp;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator-( const FixedPoint& rhs ) const
	{
		FixedPoint temp;
		temp.m_data = m_data - rhs.m_data;
		return temp;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator*( const FixedPoint& rhs ) const
	{
		long long int tempLhs = (long)m_data;
		long long int tempRhs = (long)rhs.m_data;
		long long int tempAns = tempLhs * tempRhs;
		int answer = (int)( tempAns >> 16 );
		FixedPoint temp;
		temp.m_data = answer;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator/( const FixedPoint& rhs ) const
	{
		long long int tempLhs = (long)m_data;
		long long int tempRhs = (long)rhs.m_data;
		long long int tempAns = ( tempLhs << 16 ) / tempRhs;
		int answer = (int)( tempAns );
		FixedPoint temp;
		temp.m_data = answer;
		return temp;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator+=( const FixedPoint& rhs )
	{
		*this = *this + rhs;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator-=( const FixedPoint& rhs )
	{
		*this = *this - rhs;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator*=( const FixedPoint& rhs )
	{
		*this = *this * rhs;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator/=( const FixedPoint& rhs )
	{
		*this = *this / rhs;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator+( const int rhs ) const
	{
		return operator+( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator-( const int rhs ) const
	{
		return operator-( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator*( const int rhs ) const
	{
		return operator*( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator/( const int rhs ) const
	{
		return operator/( FixedPoint( rhs ) );		
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator+=( const int rhs )
	{
		return operator+=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator-=( const int rhs )
	{
		return operator-=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator*=( const int rhs )
	{
		return operator*=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator/=( const int rhs )
	{
		return operator/=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator+( const unsigned int rhs ) const
	{
		return operator+( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator-( const unsigned int rhs ) const
	{
		return operator-( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator*( const unsigned int rhs ) const
	{
		return operator*( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator/( const unsigned int rhs ) const
	{
		return operator/( FixedPoint( rhs ) );		
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator+=( const unsigned int rhs )
	{
		return operator+=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator-=( const unsigned int rhs )
	{
		return operator-=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator*=( const unsigned int rhs )
	{
		return operator*=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator/=( const unsigned int rhs )
	{
		return operator/=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator+( const float rhs ) const
	{
		return operator+( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator-( const float rhs ) const
	{
		return operator-( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator*( const float rhs ) const
	{
		return operator*( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator/( const float rhs ) const
	{
		return operator/( FixedPoint( rhs ) );		
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator+=( const float rhs )
	{
		return operator+=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator-=( const float rhs )
	{
		return operator-=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator*=( const float rhs )
	{
		return operator*=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator/=( const float rhs )
	{
		return operator/=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator+( const double rhs ) const
	{
		return operator+( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator-( const double rhs ) const
	{
		return operator-( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator*( const double rhs ) const
	{
		return operator*( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator/( const double rhs ) const
	{
		return operator/( FixedPoint( rhs ) );		
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator+=( const double rhs )
	{
		return operator+=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator-=( const double rhs )
	{
		return operator-=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator*=( const double rhs )
	{
		return operator*=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator/=( const double rhs )
	{
		return operator/=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator-() const
	{
		FixedPoint temp;
		temp.m_data = -m_data;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator+() const
	{
		return *this;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator!() const
	{
		return !m_data;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<( const FixedPoint& rhs ) const
	{
		return m_data < rhs.m_data;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>( const FixedPoint& rhs ) const
	{
		return m_data > rhs.m_data;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator==( const FixedPoint& rhs ) const
	{
		return m_data == rhs.m_data;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<=( const FixedPoint& rhs ) const
	{
		return m_data <= rhs.m_data;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>=( const FixedPoint& rhs ) const
	{
		return m_data >= rhs.m_data;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator!=( const FixedPoint& rhs ) const
	{
		return m_data != rhs.m_data;
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<( const int rhs ) const
	{
		return operator<( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>( const int rhs ) const
	{
		return operator>( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator==( const int rhs ) const
	{
		return operator==( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<=( const int rhs ) const
	{
		return operator<=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>=( const int rhs ) const
	{
		return operator>=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator!=( const int rhs ) const
	{
		return operator!=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<( const unsigned int rhs ) const
	{
		return operator<( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>( const unsigned int rhs ) const
	{
		return operator>( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator==( const unsigned int rhs ) const
	{
		return operator==( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<=( const unsigned int rhs ) const
	{
		return operator<=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>=( const unsigned int rhs ) const
	{
		return operator>=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator!=( const unsigned int rhs ) const
	{
		return operator!=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<( const float rhs ) const
	{
		return operator<( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>( const float rhs ) const
	{
		return operator>( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator==( const float rhs ) const
	{
		return operator==( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<=( const float rhs ) const
	{
		return operator<=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>=( const float rhs ) const
	{
		return operator>=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator!=( const float rhs ) const
	{
		return operator!=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<( const double rhs ) const
	{
		return operator<( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>( const double rhs ) const
	{
		return operator>( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator==( const double rhs ) const
	{
		return operator==( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator<=( const double rhs ) const
	{
		return operator<=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator>=( const double rhs ) const
	{
		return operator>=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	bool FixedPoint::operator!=( const double rhs ) const
	{
		return operator!=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator&( const FixedPoint& rhs ) const
	{
		FixedPoint temp;
		temp.m_data = m_data & rhs.m_data;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator|( const FixedPoint& rhs ) const
	{
		FixedPoint temp;
		temp.m_data = m_data | rhs.m_data;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator~() const
	{
		FixedPoint temp;
		temp.m_data = ~m_data;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator^( const FixedPoint& rhs ) const
	{
		FixedPoint temp;
		temp.m_data = m_data ^ rhs.m_data;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator<<( int numBitsToTheLeft ) const
	{
		FixedPoint temp;
		temp.m_data = m_data << numBitsToTheLeft;
		return temp;
	}
	//----------------------------------------------------------------------
	Monky::FixedPoint FixedPoint::operator>>( int numBItsToTheRight ) const
	{
		FixedPoint temp;
		temp.m_data = m_data << numBItsToTheRight;
		return temp;
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator&( const int rhs ) const
	{
		return operator&( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator|( const int rhs ) const
	{
		return operator|( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator^( const int rhs ) const
	{
		return operator*( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator&( const unsigned int rhs ) const
	{
		return operator&( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator|( const unsigned int rhs ) const
	{
		return operator|( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint FixedPoint::operator^( const unsigned int rhs ) const
	{
		return operator*( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator&=( const FixedPoint& rhs )
	{
		*this = *this & rhs;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator|=( const FixedPoint& rhs )
	{
		*this = *this | rhs;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator^=( const FixedPoint& rhs )
	{
		*this = *this ^ rhs;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator<<=( int numBitsToTheLeft )
	{
		*this = *this << numBitsToTheLeft;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator>>=( int numBItsToTheRight )
	{
		*this = *this << numBItsToTheRight;
		return *this;
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator&=( const int rhs )
	{
		return operator&=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator|=( const int rhs )
	{
		return operator|=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator^=( const int rhs )
	{
		return operator^=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator&=( const unsigned int rhs )
	{
		return operator&=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator|=( const unsigned int rhs )
	{
		return operator|=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	FixedPoint& FixedPoint::operator^=( const unsigned int rhs )
	{
		return operator^=( FixedPoint( rhs ) );
	}
	//----------------------------------------------------------------------
	// Global output stream function for FixedPoint class
	//----------------------------------------------------------------------
	std::ostream& operator<<( std::ostream& os, const FixedPoint& fp )
	{
		os << (float)fp;
		return os;
	}

	//----------------------------------------------------------------------
	// Global operators for FixedPoint class
	//----------------------------------------------------------------------
	FixedPoint operator+( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) + rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator-( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) - rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator*( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) * rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator/( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) / rhs;
	}
	FixedPoint operator+( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) + rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator-( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) - rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator*( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) * rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator/( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) / rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator+( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) + rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator-( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) - rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator*( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) * rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator/( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) / rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator+( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) + rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator-( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) - rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator*( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) * rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator/( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) / rhs;
	}
	//----------------------------------------------------------------------
	bool operator<( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) < rhs;
	}
	//----------------------------------------------------------------------
	bool operator>( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) > rhs;
	}
	//----------------------------------------------------------------------
	bool operator==( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) == rhs;
	}
	//----------------------------------------------------------------------
	bool operator<=( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) <= rhs;
	}
	//----------------------------------------------------------------------
	bool operator>=( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) >= rhs;
	}
	//----------------------------------------------------------------------
	bool operator!=( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) != rhs;
	}
	//----------------------------------------------------------------------
	bool operator<( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) < rhs;
	}
	//----------------------------------------------------------------------
	bool operator>( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) > rhs;
	}
	//----------------------------------------------------------------------
	bool operator==( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) == rhs;
	}
	//----------------------------------------------------------------------
	bool operator<=( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) <= rhs;
	}
	//----------------------------------------------------------------------
	bool operator>=( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) >= rhs;
	}
	//----------------------------------------------------------------------
	bool operator!=( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) != rhs;
	}
	//----------------------------------------------------------------------
	bool operator<( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) < rhs;
	}
	//----------------------------------------------------------------------
	bool operator>( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) > rhs;
	}
	//----------------------------------------------------------------------
	bool operator==( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) == rhs;
	}
	//----------------------------------------------------------------------
	bool operator<=( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) <= rhs;
	}
	//----------------------------------------------------------------------
	bool operator>=( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) >= rhs;
	}
	//----------------------------------------------------------------------
	bool operator!=( const float lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) != rhs;
	}
	//----------------------------------------------------------------------
	bool operator<( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) < rhs;
	}
	//----------------------------------------------------------------------
	bool operator>( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) > rhs;
	}
	//----------------------------------------------------------------------
	bool operator==( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) == rhs;
	}
	//----------------------------------------------------------------------
	bool operator<=( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) <= rhs;
	}
	//----------------------------------------------------------------------
	bool operator>=( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) >= rhs;
	}
	//----------------------------------------------------------------------
	bool operator!=( const double lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) != rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator&( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) & rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator|( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) | rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator^( const int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) ^ rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator&( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) & rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator|( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) | rhs;
	}
	//----------------------------------------------------------------------
	FixedPoint operator^( const unsigned int lhs, const FixedPoint& rhs )
	{
		return FixedPoint( lhs ) ^ rhs;
	}
}