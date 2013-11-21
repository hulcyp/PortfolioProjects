#pragma once
#include "Vector3.h"
#include "LineSegment.h"
#include "Ray.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace Monky
{
	template< typename Real >
	class Line
	{
	public:
		Line();
		Line( const Vector3<Real>& pos, const Vector3<Real>& dir );
		Line( const LineSegment<Real>& lingSeg );
		Line( const Ray<Real>& ray );

		Vector3<Real> GetPoint( Real dist ) const;
		void Translate( const Vector3<Real>& trans );
		//void Transform( const Matrix3<Real>& trans );
		//void Transform( const Matrix4<Real>& trans );
		//void Transform( const Quaternion<Real>& trans );
		//bool Contains( const Vector3<Real>& point ) const;
		//bool Contains( const LineSegment<Real>& lingSeg ) const;
		//bool Contains( const Ray<Real>& ray ) const;
		//bool Equals( const Line<Real>& line, Real epsilon ) const;
		//Real Distance( const Vector3<Real>& point ) const;
		//Real Distance( const Ray<Real>& ray ) const;
		//Real Distance( const Line<Real>& otherLine ) const;
		//Real Distance( const LineSegment<Real>& lineSeg ) const;
		//Vector3<Real> ClosestPoint( const Vector3<Real>& targetPoint ) const;
		//Vector3<Real> ClosestPoint( const Ray<Real>& ray ) const;
		//Vector3<Real> ClosestPoint( const LineSegment<Real>& lineSeg ) const;
		//bool Intersects( const Line<Real>& otherLine ) const;
		//bool Intersects( const Ray<Real>& ray ) const;
		//bool Intersects( const LineSegemnt<Real>& lineSeg ) const;
		//Ray<Real> ToRay();
		//LineSegemnt<Real> ToLineSegment();
		//std::string ToString() const;

	private:
		Vector3<Real> m_pos;
		Vector3<Real> m_dir;
	};

	template< typename Real >
	Line<Real>::Line()
	{}
	template< typename Real >
	Line<Real>::Line( const Vector3<Real>& pos, const Vector3<Real>& dir )
		:	m_pos( pos )
		,	m_dir( dir )
	{
		m_dir.Normalize();
	}
	template< typename Real >
	Line<Real>::Line( const LineSegment<Real>& lingSeg )
		:	m_pos( lineSeg.PointA() )
		,	m_dir( lineSeg.Dir() )
	{}
	template< typename Real >
	Line<Real>::Line( const Ray<Real>& ray )
		:	m_pos( ray.Pos() )
		,	m_dir( ray.Dir() )
	{}
	template< typename Real >
	Vector3<Real> Line<Real>::GetPoint( Real dist ) const
	{
		return m_pos + m_dir * dist;
	}
	template< typename Real >
	void Line<Real>::Translate( const Vector3<Real>& trans )
	{
		m_pos += trans;
	}
	//template< typename Real >
	//void Line<Real>::Transform( const Matrix3<Real>& trans )
	//{
	//	// TODO
	//}
	//void Transform( const Matrix4<Real>& trans );
	//void Transform( const Quaternion<Real>& trans );
	//template< typename Real >
	//bool Line<Real>::Contains( const Vector3<Real>& point ) const
	//{
	//	
	//}
	//bool Contains( const LineSegment<Real>& lingSeg ) const;
	//bool Contains( const Ray<Real>& ray ) const;
	//bool Equals( const Line<Real>& line, Real epsilon ) const;
	//Real Distance( const Vector3<Real>& point ) const;
	//Real Distance( const Ray<Real>& ray ) const;
	//Real Distance( const Line<Real>& otherLine ) const;
	//Real Distance( const LineSegment<Real>& lineSeg ) const;
	//Vector3<Real> ClosestPoint( const Vector3<Real>& targetPoint ) const;
	//Vector3<Real> ClosestPoint( const Ray<Real>& ray ) const;
	//Vector3<Real> ClosestPoint( const LineSegment<Real>& lineSeg ) const;
	//bool Intersects( const Line<Real>& otherLine ) const;
	//bool Intersects( const Ray<Real>& ray ) const;
	//bool Intersects( const LineSegemnt<Real>& lineSeg ) const;
	//Ray<Real> ToRay();
	//LineSegemnt<Real> ToLineSegment();
	//std::string ToString() const;
}