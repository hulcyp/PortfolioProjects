
namespace Monky
{
	template< typename T, typename precision >
	State< T, precision >::State( T x = T(), T y = T() )
	{
		s[0] = x;
		s[1] = y;
	}

	template< typename T, typename precision >
	State< T, precision > State< T, precision >::operator*( const precision val )
	{
		State temp;
		
		temp.s[0] = s[0]*val;
		temp.s[1] = s[1]*val;
		
		return temp;
	}

	template< typename T, typename precision >
	State< T, precision > State< T, precision >::operator+( const State& rhs )
	{
		State temp;

		temp.s[0] = s[0] + rhs.s[0];
		temp.s[1] = s[1] + rhs.s[1];

		return temp;
	}

	template< typename T, typename precision >
	State< T, precision >& State< T, precision >::operator=( const State& rhs )
	{
		s[0] = rhs.s[0];
		s[1] = rhs.s[1];

		return *this;
	}

	template< typename T, typename precision >
	T& State< T, precision >::operator[]( int i )
	{
		ASSERT( i >= 0 && i < 2 );
		return s[i];
	}

	template < typename T, typename precision >
	State< T, precision > fwdEuler( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt )
	{
		return s + op( s, t )*dt;
	}

	template< typename T, typename precision >
	State< T, precision > velVerlet( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt )
	{
		State< T, precision > nextState;
		nextState[1] = s[1] + op( s, t )[1]*(dt/2);
		nextState[0] = s[0] + nextState[1]*dt;
		nextState[1] = nextState[1] + op( nextState, t + dt )[1]*(dt/2); 

		return nextState;
	}

	template< typename T, typename precision >
	State< T, precision > rk2Midpoint( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt )
	{
		State< T, precision > midState = s + op( s, t ) * ( dt/2 );
		return s + op( midState, t + dt/2 ) * dt;
	}
	
	template< typename T, typename precision >
	State< T, precision > rk2Trapezoidal( State< T, precision > s, State< T, precision > op ( State< T, precision >, precision ) , precision t, precision dt )
	{
		State< T, precision > sDevPrev = op( s, t );
		State< T, precision > approxState = s + sDevPrev * dt;

		return s + ( sDevPrev + op( approxState, t + dt ) ) * 0.5 * dt;
	}

	template< typename T, typename precision >
	State< T, precision > rk4( State< T, precision > s, State< T, precision > op( State< T, precision >, precision ), precision t, precision dt )
	{		
		precision h = dt;
		precision thalf = t + h*.5;
		precision tnext = t + h;
		
		State< T, precision > s1 = s;
		
		State< T, precision > f1 = op( s1, t );
		State< T, precision > s2 = s + f1 * (h/2);
		
		State< T, precision > f2 = op( s2, thalf );
		State< T, precision > s3 = s + f2 * (h/2);

		State< T, precision > f3 = op( s3, thalf );
		State< T, precision > s4 = s + f3 * h;

		return s + ( f1 + f2 * 2 + f3 * 2 + op( s4, tnext ) ) * (h/6);
	}
}