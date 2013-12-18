#include "VtxBufferAccessor.h"
#include "CommonEngineIncludes.h"

namespace Monky
{
	VtxBufferAccessor::VtxBufferAccessor( Buffer* buffer, DefaultMapping mapping )
		:	BufferAccessor( buffer )
	{
		buildMapping( mapping );
	}
	//-------------------------------------------------------------------------------------------------
	VtxBufferAccessor::VtxBufferAccessor( Buffer* buffer, std::map< AttributeType, int >& attribMapping )
		:	BufferAccessor( buffer )
		,	m_attributeMapping( attribMapping )
	{}
	//-------------------------------------------------------------------------------------------------
	bool VtxBufferAccessor::hasAttribute( AttributeType type )
	{
		return m_attributeMapping.find( type ) != m_attributeMapping.end();
	}
	//-------------------------------------------------------------------------------------------------
	float* VtxBufferAccessor::getPosition( int i ) 
	{ 
		return (float*)( m_buffer->getBufferLayout()->attributeOffset( m_attributeMapping[ POSITION ] ) + m_buffer->at( i ) ); 
	}
	//-------------------------------------------------------------------------------------------------
	bool VtxBufferAccessor::hasPosition() 
	{ 
		return hasAttribute( POSITION ); 
	}
	//-------------------------------------------------------------------------------------------------
	float* VtxBufferAccessor::getNormal( int i ) 
	{ 
		return (float*)( m_buffer->getBufferLayout()->attributeOffset( m_attributeMapping[ NORMAL ] ) + m_buffer->at( i ) ); 
	}
	//-------------------------------------------------------------------------------------------------
	bool VtxBufferAccessor::hasNormal() 
	{ 
		return hasAttribute( NORMAL ); 
	}
	//-------------------------------------------------------------------------------------------------
	float* VtxBufferAccessor::getColor( int i ) 
	{ 
		return (float*)( m_buffer->getBufferLayout()->attributeOffset( m_attributeMapping[ COLOR ] ) + m_buffer->at( i ) ); 
	}
	//-------------------------------------------------------------------------------------------------
	bool VtxBufferAccessor::hasColor() 
	{ 
		return hasAttribute( COLOR ); 
	}
	//-------------------------------------------------------------------------------------------------
	float* VtxBufferAccessor::getUV( int i ) 
	{ 
		return (float*)( m_buffer->getBufferLayout()->attributeOffset( m_attributeMapping[ UV ] ) + m_buffer->at( i ) ); 
	}
	//-------------------------------------------------------------------------------------------------
	bool VtxBufferAccessor::hasUVs() 
	{ 
		return hasAttribute( UV ); 
	}
	//-------------------------------------------------------------------------------------------------
	float* VtxBufferAccessor::getTangent( int i ) 
	{ 
		return (float*)( m_buffer->getBufferLayout()->attributeOffset( m_attributeMapping[ TANGENT ] ) + m_buffer->at( i ) ); 
	}
	//-------------------------------------------------------------------------------------------------
	bool VtxBufferAccessor::hasTangent() 
	{ 
		return hasAttribute( TANGENT ); 
	}
	//-------------------------------------------------------------------------------------------------
	float* VtxBufferAccessor::getBitangent( int i ) 
	{ 
		return (float*)( m_buffer->getBufferLayout()->attributeOffset( m_attributeMapping[ BITANGENT ] ) + m_buffer->at( i ) ); 
	}
	//-------------------------------------------------------------------------------------------------
	bool VtxBufferAccessor::hasBitangent() 
	{ 
		return hasAttribute( BITANGENT ); 
	}
	//-------------------------------------------------------------------------------------------------
	void VtxBufferAccessor::buildMapping( DefaultMapping mapping )
	{
		switch( mapping )
		{
		case POS_NORMAL:
			m_attributeMapping[ POSITION ] = 0;
			m_attributeMapping[ NORMAL ] = 1;
			break;
		case POS_NORMAL_COLOR:
			m_attributeMapping[ POSITION ]	= 0;
			m_attributeMapping[ NORMAL ]	= 1;
			m_attributeMapping[ COLOR ]		= 2;
			break;
		case POS_NORMAL_COLOR_UV:
			m_attributeMapping[ POSITION ]	= 0;
			m_attributeMapping[ NORMAL ]	= 1;
			m_attributeMapping[ COLOR ]		= 2;
			m_attributeMapping[ UV ]		= 3;
			break;
		case POS_NORMAL_UV:
			m_attributeMapping[ POSITION ]	= 0;
			m_attributeMapping[ NORMAL ]	= 1;
			m_attributeMapping[ UV ]		= 2;
			break;
		case POS_NORMAL_UV_TAN_BI:
			m_attributeMapping[ POSITION ]	= 0;
			m_attributeMapping[ NORMAL ]	= 1;
			m_attributeMapping[ UV ]		= 2;
			m_attributeMapping[ TANGENT ]	= 3;
			m_attributeMapping[ BITANGENT ]	= 4;
			break;
		case POS_NORMAL_COLOR_UV_TAN_BI:
			m_attributeMapping[ POSITION ]	= 0;
			m_attributeMapping[ NORMAL ]	= 1;
			m_attributeMapping[ COLOR ]		= 2;
			m_attributeMapping[ UV ]		= 3;
			m_attributeMapping[ TANGENT ]	= 4;
			m_attributeMapping[ BITANGENT ] = 5;
		}
	}
}