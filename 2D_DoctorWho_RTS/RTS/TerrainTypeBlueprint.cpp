#include "TerrainTypeBlueprint.h"
#include <Renderer.h>
#include "TerrainType.h"
#include <MeshFactory.h>
#include <algorithm>
#include <Mesh.h>

namespace pdh
{
	TerrainTypeBlueprint::TerrainTypeBlueprint( XMLParser& parser, const XMLNode* terrainBlueprintNode )
		:	m_resource( nullptr )
	{
		parser.validateXMLAttributes( terrainBlueprintNode, "name,color", "texCoord" );
		parser.validateXMLChildElements( terrainBlueprintNode, "", "Resource" );

		m_name = parser.getXMLAttributeAsString( terrainBlueprintNode, "name", "" );
		m_color = parser.getXMLAttributeAsColor( terrainBlueprintNode, "color", color::WHITE );
		m_tileCoords = parser.getXMLAttributeAsVec2i( terrainBlueprintNode, "texCoord", vec2i() );

		const XMLNode* resource = terrainBlueprintNode->FirstChildElement( "Resource" );
		m_resource = new Resource();
		if( resource )
		{
			parser.validateXMLAttributes( resource, "type,amount,maxCollectPerTrip,timePerResource", "" );
			std::string resourceType = parser.getXMLAttributeAsString( resource, "type", "" );
			
			m_resource->resourceType = Map::getResourceType( resourceType );
			m_resource->amount = parser.getXMLAttributeAsInt( resource, "amount", 0 );
			m_resource->timePerResource = parser.getXMLAttributeAsFloat( resource, "timePerResource", 0.0f );
			m_resource->maxCollectPerTrip = parser.getXMLAttributeAsInt( resource, "maxCollectPerTrip", 0 );
		}
		else
		{
			m_resource->resourceType = Map::NONE;
			m_resource->amount = 0;
		}

		//m_texture = Renderer::getTexture( textureName );
		registerBP( m_name );
	}
	//----------------------------------------------------------------
	TerrainTypeBlueprint::~TerrainTypeBlueprint()
	{
		SAFE_DELETE( m_resource );
	}
	//----------------------------------------------------------------
	TerrainType* TerrainTypeBlueprint::createTerrain()
	{
		TerrainType* terrain = new TerrainType( m_name );

		terrain->m_color = m_color;
		terrain->m_tileCoords = m_tileCoords;

		//Material* mat = Renderer::createMaterial( m_name + "_mat", "simpleShader" );
		//mat->addUniform( "uUseDiffuseMap", 0 );
		//if( m_texture )
		//{
		//	mat->addTexture( "uDiffuseMap", m_texture );
		//	mat->updateUniform( "uUseDiffuseMap", 1 );
		//}
		//terrain->m_mesh = MeshFactory::generateAAPlaneXY( 1.0f, 1.0f, m_name + "_mat", m_color );
		terrain->m_resource.amount = m_resource->amount;
		terrain->m_resource.maxCollectPerTrip = m_resource->maxCollectPerTrip;
		terrain->m_resource.timePerResource = m_resource->timePerResource;
		terrain->m_resource.resourceType = m_resource->resourceType;

		return terrain;
	}
	//---------------------------------------------------------------------
	// Static memebers
	//---------------------------------------------------------------------
	TerrainTypeBlueprint* TerrainTypeBlueprint::getBlueprint( const std::string& name )
	{
		std::map< std::string, TerrainTypeBlueprint* >::iterator iter = sm_blueprints.find( name );

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
	void TerrainTypeBlueprint::registerBP( const std::string& name )
	{
		TerrainTypeBlueprint*& blueprint = sm_blueprints[ name ];
		SAFE_DELETE( blueprint ); 
		blueprint = this;
	}

	std::map< std::string, TerrainTypeBlueprint* > TerrainTypeBlueprint::sm_blueprints;
}