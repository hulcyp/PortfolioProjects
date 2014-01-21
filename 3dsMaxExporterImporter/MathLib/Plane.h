#pragma once

namespace pdh
{
	template< typename Real >
	class Plane
	{
	public:
		Plane();
		Plane( const Vector3<Real>& normal, Real d );
		Plane( const Vector3<Real>& p0, const Vector3<Real>& p1, const Vector3<Real>& p2 );
		Plane( const Vector3<Real>& point, const Vector3<Real>& normal );
		Plane( const Line<Real>& line, const Vector3<Real>& normal );
		Plane( const LineSegement<Real>& lineSeg, const Vector3<Real> normal );
		Plane( const Ray<Real>& ray, const Vector3<Real> normal );

		bool IsDegenerate() const;
		void Set( const Vector3<Real>& v0, const Vector3<Real>& v1, const Vector3<Real>& v2 );
		void Set( const Vector3<Real>& point, const Vector3<Real>& normal );
		void ReverseNormal();
		Vector3<Real> PointOnPlane() const;
		Vector3<Real> Point( Real u, Real v ) const;
		Vector3<Real> Point( Real u, Real v, const Vector3<Real>& referenceOrigin ) const;
		void Translate( const Vector3<Real>& offset );
		void Transform( const Matrix3<Real>& transform );
		void Transform( const Matrix4<Real>& transform );
		void Transform( const Quaternion<Real>& transform );
		bool IsInPositiveDirection( const Vector3<Real>& directionVec ) const;
		bool IsOnPositiveSide( const Vector3<Real>& point );
		int ExamineSide( const Triangle<Real>& triangle ) const;
		bool AreOnSameSide( const Vector3<Real>& p0, const Vector3<Real>& p1 ) const;
		Real Distance( const Vector3<Real>& point  ) const;
		Real Distance( const LineSegment<Real>& lineSeg ) const;
		Real Distance( const Sphere<Real>& sphere ) const;
		Real SignedDistance( const Vector3<Real>& ponit ) const;
		Real SignedDistance( const AABB<Real>& aabb ) const;
		Real SignedDistance( const Line<Real>& line ) const;
		Real SignedDistance( const LineSegment<Real>& lineSeg ) const;
		Real SignedDistance( const Ray<Real>& ray ) const;
		//Real SignedDistance( const Polygon<Real>& polygon ) const;
		//Real SignedDistance( const Polyhedron<Real>& polyhedron ) const;
		Real SignedDistance( const Sphere<Real>& sphere ) const;
		Real SignedDistance( const Triangle<Real>& triangle ) const;



		bool IsParallel( const Plane<Real>& plane, Real epsilon );

		std::string ToString();
	};
}