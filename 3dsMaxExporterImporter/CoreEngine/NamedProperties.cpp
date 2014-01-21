#include "NamedProperties.h"
#include <algorithm>

namespace pdh
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
}