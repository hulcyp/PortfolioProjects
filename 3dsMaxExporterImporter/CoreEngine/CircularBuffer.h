#pragma once
#include <vector>
#include <ErrorHandlingUtils.h>
#include <MathFuncs.h>

namespace pdh
{
	template< typename T >
	class CircularBuffer
	{
	public:
		CircularBuffer( int bufferSize );
		~CircularBuffer();

		T& operator[]( int index );
		size_t size();
		void pushFront( const T& element );
		void clear();

	private:
		std::vector< T > m_buffer;
		int m_frontIndex;
		int m_backIndex;
		const int MAX_BUFFER_SIZE;
		bool m_bufferFull;
	};

	template< typename T >
	CircularBuffer<T>::CircularBuffer( int bufferSize )
		:	m_frontIndex( 0 )
		,	m_backIndex( 0 )
		,	MAX_BUFFER_SIZE( bufferSize )
		,	m_bufferFull( false )
	{
		m_buffer.reserve( MAX_BUFFER_SIZE );
	}

	template< typename T >
	CircularBuffer<T>::~CircularBuffer()
	{
		m_buffer.clear();
	}

	template< typename T >
	T& CircularBuffer<T>::operator[]( int index )
	{
		ASSERT( index >= 0 && index < (int)m_buffer.size() );
		size_t relIndex = MathFuncs<int>::wrap( m_frontIndex - index, 0, (int)m_buffer.size() - 1 );
		return m_buffer[ relIndex ];
	}
	template< typename T >
	size_t CircularBuffer<T>::size()
	{
		return m_buffer.size();
	}
	template< typename T >
	void CircularBuffer<T>::pushFront( const T& element )
	{
		int newIndex = 0;
		int backIndex = m_backIndex;
		if( m_buffer.size() == MAX_BUFFER_SIZE )
		{
			m_bufferFull = true;
		}
		if( m_bufferFull )
		{
			newIndex = MathFuncs<int>::wrap( m_frontIndex + 1, 0, (int)m_buffer.size() - 1 );
			backIndex = MathFuncs<int>::wrap( backIndex + 1, 0, (int)m_buffer.size() - 1 );
			m_buffer[ newIndex ] = element;
		}
		else
		{
			m_buffer.push_back( element );	
			newIndex = MathFuncs<int>::wrap( m_frontIndex + 1, 0, (int)m_buffer.size() - 1 );		
		}
		m_frontIndex = newIndex;
		m_backIndex = backIndex;
	}

	template< typename T >
	void CircularBuffer<T>::clear()
	{
		m_buffer.clear();
		m_bufferFull = false;
		m_frontIndex = 0;
		m_backIndex = 0;
	}
}