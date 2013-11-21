#include "NamedProperties.h"

namespace Monky
{
	NamedProperties::NamedProperties()
	{}
	//----------------------------------------------------------
	NamedProperties::NamedProperties( const NamedProperties& copy )
	{
		std::for_each( copy.m_properties.begin(), copy.m_properties.end(),
			[&]( const std::pair< std::string, TypedDataBase* >& prop )
		{
			m_properties[ prop.first ] = prop.second->clone();	
		} );
	}
	//----------------------------------------------------------
	NamedProperties::~NamedProperties()
	{
		MAP_CLEANUP( m_properties );
	}
	//----------------------------------------------------------
	NamedProperties& NamedProperties::operator=( const NamedProperties& copy )
	{
		m_properties.clear();
		std::for_each( copy.m_properties.begin(), copy.m_properties.end(),
			[&]( const std::pair< std::string, TypedDataBase* >& prop )
		{
			m_properties[ prop.first ] = prop.second->clone();	
		} );
		return *this;
	}
	//----------------------------------------------------------
	bool NamedProperties::isEmpty() const
	{
		return m_properties.empty();
	}
}