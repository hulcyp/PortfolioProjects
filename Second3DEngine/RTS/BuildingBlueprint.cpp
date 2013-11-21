#include "BuildingBlueprint.h"
#include "Building.h"
#include <Renderer.h>
#include <MeshFactory.h>

namespace pdh
{
	BuildingBlueprint::BuildingBlueprint( XMLParser& parser, const XMLNode* buildingBlueprintNode )
		:	m_supplyProvided( 0 )
	{
		parser.validateXMLChildElements( buildingBlueprintNode, "Cost,Health,Vision,UnitsProduced,Visual,Construction", "SupplyProvided" );
		parser.validateXMLAttributes( buildingBlueprintNode, "name", "" );

		m_name = parser.getXMLAttributeAsString( buildingBlueprintNode, "name", "" );

		const XMLNode* visual = buildingBlueprintNode->FirstChildElement( "Visual" );
		parser.validateXMLAttributes( visual, "color,width,height,selectionTexture", "texture" );

		Color4f color = parser.getXMLAttributeAsColor( visual, "color", color::WHITE );
		m_width = parser.getXMLAttributeAsFloat( visual, "width", 0.0f );
		m_height = parser.getXMLAttributeAsFloat( visual, "height", 0.0f );
		std::string textureName = parser.getXMLAttributeAsString( visual, "texture", "" );
		std::string selectionTextureName = parser.getXMLAttributeAsString( visual, "selectionTexture", "" );
		
		Texture* texture = Renderer::getTexture( textureName );
		Texture* selectionTexture = Renderer::getTexture( selectionTextureName );


		Material* mat = Renderer::createMaterial( m_name + "_mat", "RTSEntityShader" );
		mat->addUniform( "uUseDiffuseMap", 0 );
		mat->addUniform( "uColorTint", color::WHITE );
		mat->addUniform( "uTeamColor", color::WHITE );
		if( texture )
		{
			mat->addTexture( "uDiffuseMap", texture );
			mat->updateUniform( "uUseDiffuseMap", 1 );
		}
		m_mesh = MeshFactory::generateAAPlaneXY( m_width, m_height, m_name + "_mat", color ); 

		Material* selectMat = Renderer::createMaterial( m_name + "_selection_mat", "RTSEntityShader" );
		selectMat->addUniform( "uUseDiffuseMap", 0 );
		selectMat->addUniform( "uColorTint", color::WHITE );
		selectMat->addUniform( "uTeamColor", color::WHITE );
		if( texture )
		{
			selectMat->addTexture( "uDiffuseMap", selectionTexture );
			selectMat->updateUniform( "uUseDiffuseMap", 1 );
		}
		m_selectionMesh = MeshFactory::generateAAPlaneXY( m_width, m_height, m_name + "_selection_mat", color );

		Material* healthMat = Renderer::createMaterial( m_name + "_health_mat", "RTSEntityShader" );
		healthMat->addUniform( "uUseDiffuseMap", 0 );
		healthMat->addUniform( "uColorTint", color::WHITE );
		healthMat->addUniform( "uTeamColor", color::WHITE );
		m_healthBar = MeshFactory::generateAAPlaneXY( m_width, 0.1f, m_name + "_health_mat", color::WHITE );

		const XMLNode* cost = buildingBlueprintNode->FirstChildElement( "Cost" );
		parser.validateXMLAttributes( cost, "time,fishFingers", "custard,timeEnergy" );

		m_cost.fishFingers	= parser.getXMLAttributeAsInt( cost, "fishFingers", 0 );
		m_cost.custard		= parser.getXMLAttributeAsInt( cost, "custard", 0 );
		m_cost.timeEnergy	= parser.getXMLAttributeAsInt( cost, "timeEnergy", 0 );
		m_cost.time			= parser.getXMLAttributeAsInt( cost, "time", 0 );

		const XMLNode* construction = buildingBlueprintNode->FirstChildElement( "Construction" );
		parser.validateXMLAttributes( construction, "canBeBuiltOn", "mustBeBuiltAdjTo" );

		m_constructionRules.canBeBuiltOn = parser.getXMLAttributeAsListOfStrings( construction, "canBeBuiltOn" );
		m_constructionRules.mustBeBuiltAdjTo = parser.getXMLAttributeAsListOfStrings( construction, "mustBeBuiltAdjTo" );

		const XMLNode* health = buildingBlueprintNode->FirstChildElement( "Health" );
		parser.validateXMLAttributes( health, "max", "" );

		m_maxHealth		= parser.getXMLAttributeAsFloat( health, "max", 0.0f );

		const XMLNode* visionRange = buildingBlueprintNode->FirstChildElement( "Vision" );
		parser.validateXMLAttributes( visionRange, "range", "" );
		m_visionRange = parser.getXMLAttributeAsFloat( visionRange, "range", 0.0f );

		const XMLNode* unitsProduced = buildingBlueprintNode->FirstChildElement( "UnitsProduced" );
		parser.validateXMLChildElements( unitsProduced, "Unit", "" );

		for( const XMLNode* unit = unitsProduced->FirstChildElement( "Unit" ); unit != nullptr; unit = unit->NextSiblingElement( "Unit" ) )
		{
			parser.validateXMLAttributes( unit, "name,hotkey", "" );
			std::string name = parser.getXMLAttributeAsString( unit, "name", "" );
			std::string hotkey = parser.getXMLAttributeAsString( unit, "hotkey", "" );
			if( hotkey.size() > 0 )
				m_unitsProduced[ hotkey[0] ] = name;			
		}

		const XMLNode* supplyProvided = buildingBlueprintNode->FirstChildElement( "SupplyProvided" );
		if( supplyProvided )
		{
			parser.validateXMLAttributes( supplyProvided, "amount", "" );
			m_supplyProvided = parser.getXMLAttributeAsInt( supplyProvided, "amount", 0 );
		}	

		registerBP( m_name );
	}
	//----------------------------------------------------------------
	BuildingBlueprint::~BuildingBlueprint()
	{}
	//----------------------------------------------------------------
	Building* BuildingBlueprint::createBuilding( RTSWorld* world ) const
	{
		Building* building = new Building( this, world );

		building->m_cost.fishFingers = m_cost.fishFingers;
		building->m_cost.custard	 = m_cost.custard;
		building->m_cost.timeEnergy	 = m_cost.timeEnergy;
		building->m_cost.buildTime	 = m_cost.time;

		building->m_maxHealth		= m_maxHealth;
		building->m_currentHealth	= 0.0f;

		building->m_selectionMesh = m_selectionMesh;
		building->m_healthBar = m_healthBar;

		building->m_visionRange = m_visionRange;

		return building;
	}

	//---------------------------------------------------------------------
	// Static memebers
	//---------------------------------------------------------------------
	BuildingBlueprint* BuildingBlueprint::getBlueprint( const std::string& name )
	{
		std::map< std::string, BuildingBlueprint* >::iterator iter = sm_blueprints.find( name );

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
	//--------------------------------------------------------
	void BuildingBlueprint::registerBP( const std::string& name )
	{
		BuildingBlueprint*& blueprint = sm_blueprints[ name ];
		SAFE_DELETE( blueprint ); 
		blueprint = this;
	}

	std::map< std::string, BuildingBlueprint* > BuildingBlueprint::sm_blueprints;
}