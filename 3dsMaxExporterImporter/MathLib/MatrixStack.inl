
namespace pdh
{
	template< typename T >
	MatrixStack<T>::MatrixStack()
	{
		m_matrixStack.push( Matrix4<T>::identity() );
	}
	//------------------------------------------------------
	template< typename T >
	MatrixStack<T>::MatrixStack( const Matrix4<T>& initialMatrix )
	{
		m_matrixStack.push( initialMatrix );
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::push()
	{
		if( !m_matrixStack.empty() )
		{
			m_matrixStack.push( m_matrixStack.top() );
		}
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::pop()
	{
		m_matrixStack.pop();
		if( m_matrixStack.empty() )
		{
			m_matrixStack.push( Matrix4<T>::identity() );
		}
	}
	//------------------------------------------------------
	template< typename T >
	const Matrix4<T>& MatrixStack<T>::top() const
	{
		return m_matrixStack.top();
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T> MatrixStack<T>::top()
	{
		return m_matrixStack.top();
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::rotate( const Vector3<T>& axis, T angDegCCW )
	{
		T angRad = degreesToRadians( angDegCCW );
		rotateRadians( axis, angRad );
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::rotateRadians( const Vector3<T>& axis, T angRadCCW )
	{
		Quaternion<T> quat = makeQuaternionFromAxisAngle( angRadCCW, axis );
		Matrix4<T> rot = convertToMatrix( quat );

		m_matrixStack.top() *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::rotateX( T angRadCCW )
	{
		Matrix4<T> rot(	1,							0,						0, 0,
						0,	std::cos( angRadCCW ),	-std::sin( angRadCCW ),	0,
						0,	std::sin( angRadCCW ),	std::cos( angRadCCW ),	0,
						0,	0,						0,						1 );

		m_matrixStack.top() *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::rotateY( T angRadCCW )
	{
		Matrix4<T> rot(	std::cos( angRadCCW ),	0,	std::sin( angRadCCW ),	0,
						0,						1,	0,						0,
						-std::sin( angRadCCW),	0,	std::cos( angRadCCW ),	0,
						0,						0,	0,						1 );

		m_matrixStack.top() *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::rotateZ( T angRadCCW )
	{
		Matrix4<T> rot(	std::cos( angRadCCW ),	-std::sin( angRadCCW ),	0,	0,
						std::sin( angRadCCW ),	std::cos( angRadCCW ),	0,	0,
						0,						0,						1,	0,
						0,						0,						0,	1 );

		m_matrixStack.top() *= rot;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::translate( const Vector3<T>& trans )
	{
		Matrix4<T> tempTrans = Matrix4<T>::identity();
		tempTrans[12] = trans.x;
		tempTrans[13] = trans.y;
		tempTrans[14] = trans.z;

		Matrix4<T>& top = m_matrixStack.top();
		top *= tempTrans;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::translate( T transX, T transY, T transZ )
	{		
		Matrix4<T> tempTrans = Matrix4<T>::identity();
		tempTrans[12] = transX;
		tempTrans[13] = transY;
		tempTrans[14] = transZ;
		
		Matrix4<T>& top = m_matrixStack.top();
		top *= tempTrans;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::scale( const Vector3<T>& scaleVec )
	{
		scale( scaleVec.x, scaleVec.y, scaleVec.z );
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::scale( T scaleX, T scaleY, T scaleZ )
	{
		Matrix4<T> tempScale = Matrix4<T>::identity();
		tempScale[0] = scaleX;
		tempScale[5] = scaleY;
		tempScale[10] = scaleZ;

		Matrix4<T>& top = m_matrixStack.top();
		top *= tempScale;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::scale( T uniformScale )
	{
		scale( uniformScale, uniformScale, uniformScale );
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::pushMatrix( const Matrix4<T>& matrix )
	{
		m_matrixStack.push( matrix );
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::applyMatrix( const Matrix4<T>& matrix )
	{
		m_matrixStack.top() *= matrix;
	}
	//------------------------------------------------------
	template< typename T >
	Matrix4<T>& MatrixStack<T>::operator*=( const Matrix4<T>& matrix )
	{
		m_matrixStack.top() *= matrix;
	}
	//------------------------------------------------------
	template< typename T >
	void MatrixStack<T>::loadIdentity()
	{
		m_matrixStack.top() = Matrix4<T>::identity();
	}
}