#pragma once
#include "StdLibraryIncludes.h"
#include <cstdlib>
#include "RandNumGen.h"
#include <string>

namespace Monky
{
	template< typename T >
	class WeightedRandPool
	{
	public:
		WeightedRandPool();
		WeightedRandPool( const std::string& str );
		~WeightedRandPool();

		void add( T item, float weight );
		T evaluate() const;
		T evaluate( unsigned int seed ) const;

	private:
		std::vector< std::pair< T, float > > m_pool;
		float m_totalWeighting;
	};
	//------------------------------------------
	template< typename T >
	WeightedRandPool<T>::WeightedRandPool()
		:	m_totalWeighting( 0.0f )
	{}
	//------------------------------------------
	template< typename T >
	WeightedRandPool<T>::~WeightedRandPool()
	{}
	//------------------------------------------
	template< typename T >
	void WeightedRandPool<T>::add( T item, float weight )
	{
		m_totalWeighting += weight;
		m_pool.push_back( std::pair< T, float >( item, weight ) );
	}
	//------------------------------------------
	template< typename T >
	T WeightedRandPool<T>::evaluate() const
	{
		T poolValue;
		float randNum = RandNumGen::randInRangeFloat( 0.0f, m_totalWeighting );
		for( size_t i = 0; i < m_pool.size(); ++i )
		{
			if( randNum < m_pool[i].second )
			{
				poolValue = m_pool[i].first;
				break;
			}
			else
			{
				randNum -= m_pool[i].second;
			}
		}

		return poolValue;
	}
	//------------------------------------------
	template< typename T >
	T WeightedRandPool<T>::evaluate( unsigned int seed ) const
	{
		RanNumGen::seed( seed );
		return evaluate();
	}

}