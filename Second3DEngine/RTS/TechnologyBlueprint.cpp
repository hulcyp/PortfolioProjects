#include "TechnologyBlueprint.h"
#include "Technology.h"

namespace pdh
{
	TechnologyBlueprint::TechnologyBlueprint( XMLParser& parser, const XMLNode* techBlueprint )
	{
		parser.validateXMLChildElements( techBlueprint, "Cost", "" );
		parser.validateXMLAttributes( techBlueprint, "name", "" );

		m_name = parser.getXMLAttributeAsString( techBlueprint, "name", "" );

		const XMLNode* cost = techBlueprint->FirstChildElement( "Cost" );
		parser.validateXMLAttributes( cost, "mineral,time", "gas" );

		m_cost.mineral	= parser.getXMLAttributeAsInt( cost, "mineral", 0 );
		m_cost.gas		= parser.getXMLAttributeAsInt( cost, "gas", 0 );
		m_cost.time		= parser.getXMLAttributeAsInt( cost, "time", 0 );

		registerBP( m_name );
	}
	//----------------------------------------------------------------
	TechnologyBlueprint::~TechnologyBlueprint()
	{}
	//----------------------------------------------------------------
	Technology* TechnologyBlueprint::createTechnology()
	{
		Technology* tech = new Technology( m_name );

		tech->m_cost.mineral	= m_cost.mineral;
		tech->m_cost.gas		= m_cost.gas;
		tech->m_cost.time		= m_cost.time;

		return tech;
	}

	//---------------------------------------------------------------------
	// Static memebers
	//---------------------------------------------------------------------
	TechnologyBlueprint* TechnologyBlueprint::getBlueprint( const std::string& name )
	{
		std::map< std::string, TechnologyBlueprint* >::iterator iter = sm_blueprints.find( name );

		if( iter != sm_blueprints.end() )
		{
			return iter->second;
		}
		else
		{
			return nullptr;
		}
	}
	
	//---------------------------------------------------------------------
	// Protected member functions
	//---------------------------------------------------------------------
	void TechnologyBlueprint::registerBP( const std::string& name )
	{
		TechnologyBlueprint*& blueprint = sm_blueprints[ name ];
		SAFE_DELETE( blueprint ); 
		blueprint = this;
	}

	std::map< std::string, TechnologyBlueprint* > TechnologyBlueprint::sm_blueprints;
}