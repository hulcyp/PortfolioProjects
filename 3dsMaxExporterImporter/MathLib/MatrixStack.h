////////////////////////////////////////////////////////////
// Author: Preston Hulcy
// Last Modified: 2/13/2013
// Description:
///////////////////////////////////////////////////////////


#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "MathFuncs.h"
#include <stack>

namespace pdh
{
	template< typename T >
	class MatrixStack
	{
	public:
		MatrixStack<T>();
		MatrixStack<T>( const Matrix4<T>& initialMatrix );

		void push();
		void pop();
		const Matrix4<T>& top() const;
		Matrix4<T> top();

		///////////////////////////////////////////////////////////
		// Rotation functions
		///////////////////////////////////////////////////////////
		void rotate( const Vector3<T>& axis, T angDegCCW );
		void rotateRadians( const Vector3<T>& axis, T angRadCCW );
		void rotateX( T angRadCCW );
		void rotateY( T angRadCCW );
		void rotateZ( T angRadCCW );

		/////////////////////////////////////////////////////////
		// Translation functions
		/////////////////////////////////////////////////////////
		void translate( const Vector3<T>& trans );
		void translate( T transX, T transY, T transZ );

		/////////////////////////////////////////////////////////
		// Scaling functions
		/////////////////////////////////////////////////////////
		void scale( const Vector3<T>& scaleVec );
		void scale( T scaleX, T scaleY, T scaleZ );
		void scale( T uniformScale );

		void pushMatrix( const Matrix4<T>& matrix );
		void applyMatrix( const Matrix4<T>& matrix );
		Matrix4<T>& operator *=( const Matrix4<T>& matrix );
		void loadIdentity();

	private:
		std::stack< Matrix4<T> > m_matrixStack;
	};
}

#include "MatrixStack.inl"