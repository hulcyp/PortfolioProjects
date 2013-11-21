#include "UnitBlueprint.h"
#include "Unit.h"
#include <Renderer.h>
#include <MeshFactory.h>

namespace pdh
{
	UnitBlueprint::UnitBlueprint( XMLParser& parser, const XMLNode* unitBlueprintNode )
	{
		parser.validateXMLChildElements( unitBlueprintNode, "Visual,Cost,Health,Movement,Attack", "BuildingsConstructed,Peon" );
		parser.validateXMLAttributes( unitBlueprintNode, "name", "" );

		m_name = parser.getXMLAttributeAsString( unitBlueprintNode, "name", "Unit" );

		const XMLNode* visual = unitBlueprintNode->FirstChildElement( "Visual" );
		parser.validateXMLAttributes( visual, "color,width,height,selectionTexture", "texture" );
		m_width = parser.getXMLAttributeAsFloat( visual, "width", 0.0f );
		m_height = parser.getXMLAttributeAsFloat( visual, "height", 0.0f );

		Color4f color = parser.getXMLAttributeAsColor( visual, "color", color::WHITE );
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
		

		const XMLNode* cost = unitBlueprintNode->FirstChildElement( "Cost" );
		parser.validateXMLAttributes( cost, "time,fishFingers,supply", "custard,timeEnergy" );
		m_cost.buildTime	= parser.getXMLAttributeAsInt( cost, "time", 0 );
		m_cost.fishFingers	= parser.getXMLAttributeAsInt( cost, "fishFingers", 0 );
		m_cost.custard		= parser.getXMLAttributeAsInt( cost, "custard", 0 );
		m_cost.timeEnergy	= parser.getXMLAttributeAsInt( cost, "timeEnergy", 0 );
		m_cost.supply			= parser.getXMLAttributeAsInt( cost, "supply", 0 );

		const XMLNode* health = unitBlueprintNode->FirstChildElement( "Health" );
		parser.validateXMLAttributes( health, "max", "" );
		m_maxHealth = parser.getXMLAttributeAsFloat( health, "max", 0 );

		const XMLNode* movement = unitBlueprintNode->FirstChildElement( "Movement" );
		parser.validateXMLAttributes( movement, "speed,canMoveOn,vision", "" );
		m_movement.speed = parser.getXMLAttributeAsFloat( movement, "speed", 0.0f );
		m_movement.canMoveOn = parser.getXMLAttributeAsListOfStrings( movement, "canMoveOn" );
		m_movement.vision = parser.getXMLAttributeAsFloat( movement, "vision", 0.0f );
		
		const XMLNode* attack = unitBlueprintNode->FirstChildElement( "Attack" );
		parser.validateXMLAttributes( attack, "damage,speed,range", "" );
		m_attack.damage = parser.getXMLAttributeAsFloat( attack, "damage", 0 );
		m_attack.speed	= parser.getXMLAttributeAsFloat( attack, "speed", 0 );
		m_attack.range = parser.getXMLAttributeAsFloat( attack, "range", 0 );

		const XMLNode* buildingsConstructed = unitBlueprintNode->FirstChildElement( "BuildingsConstructed" );
		if( buildingsConstructed )
		{
			parser.validateXMLChildElements( buildingsConstructed, "Building", "" );
			for( const XMLNode* building = buildingsConstructed->FirstChildElement( "Building" ); building != nullptr; building = building->NextSiblingElement( "Building" ) )
			{
				parser.validateXMLAttributes( building, "name,hotkey", "" );
				std::string name = parser.getXMLAttributeAsString( building, "name", "" );
				std::string hotkey = parser.getXMLAttributeAsString( building, "hotkey", "" );
				if( hotkey.size() > 0 )
				{
					m_buildingsCanProduce[ hotkey[0] ] = name;
					m_hotkeyPool.addItem( hotkey[0] );
				}
			}
		}

		m_isPeon = ( unitBlueprintNode->FirstChildElement( "Peon" ) != nullptr );

		registerBP( m_name );
	}
	
	UnitBlueprint::~UnitBlueprint()
	{}
	
	Unit* UnitBlueprint::createUnit( RTSWorld* world ) const
	{
		Unit* unit = new Unit( this, world );
		unit->m_cost.buildTime = m_cost.buildTime;
		unit->m_cost.supply = m_cost.supply;
		unit->m_cost.fishFingers = m_cost.fishFingers;
		unit->m_cost.custard = m_cost.custard;
		unit->m_cost.timeEnergy = m_cost.timeEnergy;

		unit->m_attack.damage = m_attack.damage;
		unit->m_attack.speed = m_attack.speed;
		unit->m_attack.range = m_attack.range;

		unit->m_movement.speed = m_movement.speed;
		unit->m_movement.vision = m_movement.vision;

		unit->m_maxHealth = m_maxHealth;
		unit->m_currentHealth = m_maxHealth;

		unit->m_selectionMesh = m_selectionMesh;
		unit->m_healthBar = m_healthBar;

		return unit;
	}
	//---------------------------------------------------------------------
	Mesh* UnitBlueprint::getMesh() const
	{ 
		return m_mesh;
	}
	//---------------------------------------------------------------------
	// Static memebers
	//---------------------------------------------------------------------
	UnitBlueprint* UnitBlueprint::getBluePrint( const std::string& name )
	{
		std::map< std::string, UnitBlueprint* >::iterator iter = sm_blueprints.find( name );

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
	void UnitBlueprint::registerBP( const std::string& name )
	{
		UnitBlueprint*& blueprint = sm_blueprints[ name ];
		SAFE_DELETE( blueprint ); 
		blueprint = this;
	}

	std::map< std::string, UnitBlueprint* > UnitBlueprint::sm_blueprints;
}