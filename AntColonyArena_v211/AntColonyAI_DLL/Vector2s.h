#pragma once

class Vector2s
{
public:
	Vector2s( short x = 0, short  y = 0 );

	Vector2s operator+( const Vector2s& rhs ) const;
	Vector2s operator-( const Vector2s& rhs ) const;
	Vector2s operator*( const float val ) const;
	Vector2s& operator+=( const Vector2s& rhs );
	Vector2s& operator-=( const Vector2s& rhs );
	bool operator==( const Vector2s& rhs ) const;
	bool operator!=( const Vector2s& rhs ) const;
	Vector2s operator-() const;
	float lengthSqrd() const;

	Vector2s yx() const;

	short x;
	short y;
};