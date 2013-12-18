#pragma once
#include <map>
#include <string>
#include "TypedData.h"

namespace pdh
{
	class NamedProperties
	{
	public:
		enum GetResults
		{
			WRONG_TYPE,
			NO_SUCH_ENTRY,
			SUCCESS
		};

		NamedProperties();
		NamedProperties( const NamedProperties& copy );
		NamedProperties& operator=( const NamedProperties& copy );

		template< typename T >
		void set( const std::string& name, const T& value );

		template< typename T >
		GetResults get( const std::string& name, T& value ) const;

	private:
		std::map< std::string, TypedDataBase* > m_properties;
	};

	template< typename T >
	void NamedProperties::set( const std::string& name, const T& value )
	{
		TypedDataBase*& newProperty = m_properties[ name ];
		if( newProperty )
			delete newProperty;
		newProperty = new TypedData<T>( value );
	}
	//-----------------------------------------------------
	template< typename T >
	NamedProperties::GetResults NamedProperties::get( const std::string& name, T& valueOut ) const
	{
		auto found = m_properties.find( name );
		if( found == m_properties.end() )
			return NO_SUCH_ENTRY;

		if( typeid( T ) == found->second->getTypeID() )
		{
			valueOut = reinterpret_cast< TypedData<T>* >( found->second )->getData();
			return SUCCESS;
		}
		else
		{
			return WRONG_TYPE;
		}
	}
}