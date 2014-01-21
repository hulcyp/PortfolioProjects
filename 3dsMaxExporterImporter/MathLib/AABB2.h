#pragma once
#include "Vector2.h"

namespace pdh
{
	template< typename T >
	class AABB2
	{
	public:
		AABB2();
		AABB2( const Vector2<T>& minPoint, const Vector2<T>& maxPoint );
		AABB2( const Vector2<T>& minPoint, T width, T height );

		const Vector2<T>& minPoint() const;
		const Vector2<T>& maxPoint() const;
		void setMinPoint( const Vector2<T>& minPoint );
		void setMaxPoint( const Vector2<T>& maxPoint );
		T getWidth() const;
		T getHeight() const;

		/* Readjusts the min and max points so that the min point is the smalles x and y and the max point is the largest x and y*/
		void adjustMinMaxPoints();

		bool contains( const Vector2<T>& point ) const;
		bool intersects( const AABB2<T>& box ) const;

	private:
		Vector2<T> m_minPoint;
		Vector2<T> m_maxPoint;
		T m_width;
		T m_height;
	};

	template< typename T >
	AABB2<T>::AABB2()
	{}
	//-------------------------------------------------------------
	template< typename T >
	AABB2<T>::AABB2( const Vector2<T>& minPoint, const Vector2<T>& maxPoint )
		:	m_minPoint( minPoint )
		,	m_maxPoint( maxPoint )
	{
		m_width = m_maxPoint.x - m_minPoint.x;
		m_height = m_maxPoint.y - m_minPoint.y;
	}
	//-------------------------------------------------------------
	template< typename T >
	AABB2<T>::AABB2( const Vector2<T>& minPoint, T width, T height )
		:	m_minPoint( minPoint )
		,	m_width( width )
		,	m_height( height )
	{
		m_maxPoint.x = minPoint.x + width;
		m_maxPoint.y = minPoint.y + height;
	}
	//-------------------------------------------------------------
	template< typename T >
	const Vector2<T>& AABB2<T>::minPoint() const
	{
		return m_minPoint;
	}
	//-------------------------------------------------------------
	template< typename T >
	const Vector2<T>& AABB2<T>::maxPoint() const
	{
		return m_maxPoint;
	}
	//-------------------------------------------------------------
	template< typename T >
	void AABB2<T>::setMinPoint( const Vector2<T>& minPoint )
	{
		m_minPoint = minPoint;		
		m_width = m_maxPoint.x - m_minPoint.x;
		m_height = m_maxPoint.y - m_minPoint.y;
	}
	//-------------------------------------------------------------
	template< typename T >
	void AABB2<T>::setMaxPoint( const Vector2<T>& maxPoint )
	{
		m_maxPoint = maxPoint;		
		m_width = m_maxPoint.x - m_minPoint.x;
		m_height = m_maxPoint.y - m_minPoint.y;
	}
	//----------------------------------------------------------------
	template< typename T >
	T AABB2<T>::getWidth() const
	{
		return m_width;
	}
	//----------------------------------------------------------------
	template< typename T >
	T AABB2<T>::getHeight() const
	{
		return m_height;
	}
	//----------------------------------------------------------------
	template< typename T >
	void AABB2<T>::adjustMinMaxPoints()
	{
		T newMinX = std::min( m_minPoint.x, m_maxPoint.x );
		T newMinY = std::min( m_minPoint.y, m_maxPoint.y );
		
		T newMaxX = std::max( m_minPoint.x, m_maxPoint.x );
		T newMaxY = std::max( m_minPoint.y, m_maxPoint.y );

		m_minPoint.x = newMinX;
		m_minPoint.y = newMinY;
		m_maxPoint.x = newMaxX;
		m_maxPoint.y = newMaxY;

		m_width = m_maxPoint.x - m_minPoint.x;
		m_height = m_maxPoint.y - m_minPoint.y;
	}
	//-------------------------------------------------------------
	template< typename T >
	bool AABB2<T>::contains( const Vector2<T>& point ) const
	{
		return(	m_minPoint.x <= point.x && 
				m_minPoint.y <= point.y &&
				m_maxPoint.x >= point.x &&
				m_maxPoint.y >= point.y );
	}
	//-------------------------------------------------------------
	template< typename T >
	bool AABB2<T>::intersects( const AABB2<T>& box ) const
	{
		//return	( std::abs( m_minPoint.x - box.m_minPoint.x ) * 2 < ( m_width + box.m_width ) ) &&
		//		( std::abs( m_minPoint.y - box.m_minPoint.y ) * 2 < ( m_height + box.m_height ) );
		return ( ( m_minPoint.x <= box.m_maxPoint.x ) && 
				 ( m_minPoint.y <= box.m_maxPoint.y ) &&
				 ( m_maxPoint.x >= box.m_minPoint.x ) &&
				 ( m_maxPoint.y >= box.m_minPoint.y ) );
	}
}