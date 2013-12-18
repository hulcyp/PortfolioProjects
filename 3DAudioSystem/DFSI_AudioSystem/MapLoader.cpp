#include "MapLoader.h"
#include <XMLParser.h>
#include <GameApp.h>
#include <MeshFactory.h>

namespace Monky
{
	MapLoader::MapLoader( const std::string& filePath, GameApp* game )
		:	m_filePath( filePath )
		,	m_theGame( game )
		,	WALL_WIDTH( 10.0f )
		,	WALL_DEPTH( 2.5f )
	{
		m_wallMesh = MeshFactory::generateCube( WALL_WIDTH, "StoneWallMat" );
	}
	//--------------------------------------------------------------
	void MapLoader::loadMap()
	{
		XMLParser parser( m_filePath.c_str() );
		XMLNode* map = parser.getDocument().FirstChildElement( "Map" );
		int width = parser.getXMLAttributeAsInt( map, "width", 0 );
		int height = parser.getXMLAttributeAsInt( map, "height", 0 );
		std::string theMap = parser.getXMLElementPCDataAsString( map );
		
		m_floorMesh = MeshFactory::generateAAPlaneXZ( width*WALL_WIDTH, height*WALL_WIDTH, "DirtFloorMat" );
		Actor* floorActor = new Actor( "The Floor", m_floorMesh );
		m_theGame->spawn( floorActor );

		int y = 0;
		for( XMLNode* row = map->FirstChildElement( "Row" ); row != nullptr; row = row->NextSiblingElement( "Row" ), ++y )
		{
			std::string strRow = parser.getXMLElementPCDataAsString( row );
			assertion( strRow.size() == width, "Number of tiles does not equal the specified width" );
			for( int x = 0; x < width; ++x )
			{
				char tileChar = strRow[ x ];
				Actor* actor = nullptr;
				switch( tileChar )
				{
				case '|':
					actor = new Actor( toString( x ) + toString( y ), m_wallMesh );
					actor->setScale( vec3f( WALL_DEPTH / WALL_WIDTH, 1.0f, 1.0f ) );
					break;
				case '-':
					actor = new Actor( toString( x ) + toString( y ), m_wallMesh );
					actor->setScale( vec3f( 1.0f, 1.0f, WALL_DEPTH / WALL_WIDTH ) );
					break;
				}
				if( actor != nullptr )
				{
					actor->setPosition( vec3f( x * WALL_WIDTH, 0.5f * WALL_WIDTH, y * WALL_WIDTH ) );
					m_theGame->spawn( actor );
				}
			}
		}
	}
}