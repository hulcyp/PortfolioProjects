#include "Vector2s.h"

Vector2s::Vector2s( short x, short y )
	:	x( x )
	,	y( y )
{}
//----------------------------------------------
Vector2s Vector2s::operator+( const Vector2s& rhs ) const
{
	return Vector2s( x + rhs.x, y + rhs.y );
}
//----------------------------------------------
Vector2s Vector2s::operator-( const Vector2s& rhs ) const
{
	return Vector2s( x - rhs.x, y - rhs.y );
}
//----------------------------------------------
Vector2s Vector2s::operator*( const float val ) const
{
	return Vector2s( x * val, y * val );
}
//----------------------------------------------
Vector2s& Vector2s::operator+=( const Vector2s& rhs )
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}
//----------------------------------------------
Vector2s& Vector2s::operator-=( const Vector2s& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}
//----------------------------------------------
bool Vector2s::operator==( const Vector2s& rhs ) const
{
	return ( x == rhs.x && y == rhs.y );
}
//----------------------------------------------
bool Vector2s::operator!=( const Vector2s& rhs ) const
{
	return ( x != rhs.x || y != rhs.y );
}
//----------------------------------------------------------------------
Vector2s Vector2s::operator-() const
{
	return Vector2s( -x, -y );
}
//----------------------------------------------------------------------
float Vector2s::lengthSqrd() const
{
	return (float)( x*x + y*y );
}
//----------------------------------------------------------------------
Vector2s Vector2s::yx() const
{
	return Vector2s( y, x );
}