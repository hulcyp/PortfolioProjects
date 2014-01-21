#pragma once
#include "TypedDataBase.h"

namespace pdh
{
	template< typename T >
	class TypedData : public TypedDataBase
	{
	public:
		TypedData( const T& data );

		virtual const type_info& getTypeID() const;
		virtual TypedDataBase* clone() const; 
		const T& getData() const;


	private:
		T m_data;
	};

	template< typename T >
	TypedData<T>::TypedData( const T& data )
		:	m_data( data )
	{}
	//-------------------------------------------
	template< typename T >
	const type_info& TypedData<T>::getTypeID() const
	{
		return typeid( T );
	}
	//-------------------------------------------
	template< typename T >
	TypedDataBase* TypedData<T>::clone() const
	{
		return new TypedData( m_data );
	}
	//-------------------------------------------
	template< typename T >
	const T& TypedData<T>::getData() const
	{
		return m_data;
	}
}