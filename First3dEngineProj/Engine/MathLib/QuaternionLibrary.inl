
namespace pdh
{

	template< class T >
	Matrix4<T> convertToMatrix( const Quaternion<T>& q )
	{
		T w = q.getReal();
		T x = q.getVector().x;
		T y = q.getVector().y;
		T z = q.getVector().z;

		T i_x = 1 - 2*y*y - 2* z*z;
		T i_y = 2*x*y + 2*z*w;
		T i_z = 2*x*z - 2*y*w;
		
		// T(j)
		T j_x = 2*x*y - 2*z*w;
		T j_y = 1 - 2*x*x - 2*z*z;
		T j_z = 2*y*z + 2*x*w;
		
		// T(k)
		T k_x = 2*x*z + 2*y*w;
		T k_y = 2*y*z - 2*x*w;
		T k_z = 1 - 2*x*x - 2*y*y;

		Matrix4<T> rotMatrix(	i_x, i_y, i_z, 0,
								j_x, j_y, j_z, 0,
								k_x, k_y, k_z, 0,
								0	, 0	 , 0 , static_cast<T>( 1 ) );
		
		return Matrix4<T>( rotMatrix );
	}
	
	template< class T >
	Quaternion<T> makeQuaternionFromAxisAngle( T angle, Vector3<T> axis )
	{
		T hAngle = angle / static_cast<T>( 2 );
		return Quaternion<T>( std::cos( hAngle ), axis * std::sin( hAngle ) );
	}

	template< class T >
	Quaternion<T> slerp( const Quaternion<T>& q1, const Quaternion<T>& q2, T t )
	{
		Quaternion<T> temp;

		T d = q1->dot( *q2 );
		if( d < 0 )
		{
			*q2 = -(*q2);
			d = -d;
		}
		T epsilon = static_cast<T>( 0.0001 );

		if( d >= 1 - epsilon )
		{
			*temp = ( (*q1) * ( 1 - t ) + (*q2) * t );
			temp->normalize();
		}
		else
		{
			T angle = std::acos( d );
			T w1 = std::sin( angle * ( 1 - t ) ) / std::sin( angle );
			T w2 = std::sin( angle * t ) / std::sin( angle );
			Quaternion<T> temp1 = (*q1) * w1;
			Quaternion<T> temp2 = (*q2) * w2;
			temp = temp1 + temp2;
		}

		return temp;
	}
}