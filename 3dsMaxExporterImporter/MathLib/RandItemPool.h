#pragma once
#include <vector>
#include "RandNumGen.h"

namespace pdh
{
	template< typename T >
	class RandItemPool
	{
	public:
		RandItemPool();
		void addItem( T item );
		T getRandItem() const;
		T getRandItem( unsigned int seed ) const;

	private:
		std::vector< T > m_pool;
	};

	template< typename T >
	RandItemPool<T>::RandItemPool()
	{}
	//------------------------------------------------------
	template< typename T >
	void RandItemPool<T>::addItem( T item )
	{
		m_pool.push_back( item );
	}
	//------------------------------------------------------
	template< typename T >
	T RandItemPool<T>::getRandItem() const
	{
		int index = RandNumGen::randInRangeInt( 0, (int)m_pool.size() - 1 );
		return m_pool[ index ];
	}
	//------------------------------------------------------
	template< typename T >
	T RandItemPool<T>::getRandItem( unsigned int seed ) const
	{
		RandNumGen::seed( seed );
		int index = RandNumGen::randInRangeint( 0, (int)m_pool.size() - 1 );
		return m_pool[ index ];
	}
}