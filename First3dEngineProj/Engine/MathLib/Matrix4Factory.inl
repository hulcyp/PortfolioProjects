
namespace pdh
{
	template< typename T >
	Matrix4<T> createTranslateMatrix( Vector3<T>& pos )
	{
		T tran[] = {	1.0, 0.0, 0.0, pos.x,
						0.0, 1.0, 0.0, pos.y,
						0.0, 0.0, 1.0, pos.z,
						0.0, 0.0, 0.0, 1.0};

		return tran;
	}

	template< typename T >
	Matrix4<T> addTranslateToMatrix( Vector3<T>& pos, const Matrix4<T>& matrix )
	{
		Matrix4<T> translate = createTranslateMatrix( pos );
		
		return matrix * translate;
	}

	template< typename T >
	Matrix4<T> alignWithAxis( Vector3<T>& i, Vector3<T>& j, Vector3<T>& k )
	{
		T rotMatrix[] = {	i.x, i.y, i.z, 0.0,
							j.x, j.y, j.z, 0.0,
							k.x, k.y, k.z, 0.0,
							0.0, 0.0, 0.0, 1.0};

		return rotMatrix;
	}

	template< typename T >
	Matrix4<T> gl_perspectiveProjection( T fov, T aspect, T znear, T zfar )
	{
		T fovRad = fov * PI/180;
		T f = 1/tan( fovRad/2 );

		T xymax = znear * tan( fovRad/2 );
		T ymin = -xymax;
		T xmin = -xymax;

		T width = xymax - xmin;
		T height = xymax - ymin;

		T depth = zfar - znear;
		T q = -(zfar + znear)/depth;
		T qn = -2 * (zfar * znear )/ depth;

		T w = 2 * znear / width;
		w = w / aspect;
		T h = 2 * znear / height;

		Matrix4<T> temp(	w,	0,	0,	0,
							0,	h,	0,	0,
							0,	0,	q,	-1,
							0,	0,	qn,	0 );


		return temp;
	}

	template< typename T >
	Matrix4<T> gl_orthographicProjection( T left, T right, T top, T bot, T znear, T zfar )
	{
		Matrix4<T> temp (	2/(right - left),		0,				0,					-((right + left)/(right - left)),
							0,						2/(top - bot),	0,					-((top + bot)/(top - bot)),
							0,						0,				-2/(zfar - znear),	-((zfar + znear)/(zfar / znear)),
							0,						0,				0,					1 );
		temp.transpose();
		return temp;
	}
}