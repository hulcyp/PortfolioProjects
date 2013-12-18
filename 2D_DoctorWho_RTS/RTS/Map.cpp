#pragma once
#include "Map.h"
#include <CommonEngineIncludes.h>
#include <Renderer.h>
#include "TerrainType.h"
#include <set>
#include <algorithm>
#include <Camera.h>
#include "RTSWorld.h"
#include <VtxBufferAccessor.h>
#include "MapTypeBlueprint.h"
#include <GLBuffer.h>
#include "TerrainTypeBlueprint.h"

namespace pdh
{
	Map::MapTile::MapTile()
		:	m_resourceType( NONE )
		,	m_lastAIPathNumberClosedOn( -1 )
		,	m_lastAIPathNumberOpenedOn( -1 )
		,	m_GCost( 1 )
		,	lastFrameUpdated( -1 )
	{}
	//--------------------------------------------
	bool Map::MapTile::hasResource() const
	{
		return ( m_resourceType != NONE ) && m_resourceAmount > 0;
	}
	//--------------------------------------------
	int Map::MapTile::getResourceAmount() const
	{
		return m_resourceAmount;
	}
	//--------------------------------------------
	int Map::MapTile::getMaxCollectAmountPerTrip() const
	{
		return m_terrainType->getResourceInfo().maxCollectPerTrip;
	}
	//--------------------------------------------
	int Map::MapTile::getH( const TileCoords& goal ) const
	{
		return std::abs( goal.x - m_tileCoords.x ) + std::abs( goal.y - m_tileCoords.y );
	}
	//--------------------------------------------
	const TerrainType* Map::MapTile::getTerrainType() const
	{
		return m_terrainType;
	}
	//--------------------------------------------
	Map::ResourceType Map::MapTile::getResourceType() const
	{
		return m_resourceType;
	}
	//--------------------------------------------
	const TileCoords& Map::MapTile::getTilecoords() const
	{
		return m_tileCoords;
	}
	//--------------------------------------------
	Map::Map( RTSWorld* world )
		:	m_world( world )
		,	m_mapMesh( nullptr )
		,	m_mapUpdatedThisFrame( false )
	{
		m_cardinalDirections[0] = Map::CD_NORTH;
		m_cardinalDirections[1] = Map::CD_EAST;
		m_cardinalDirections[2] = Map::CD_SOUTH;
		m_cardinalDirections[3] = Map::CD_WEST;
	}
	//--------------------------------------------
	Map::~Map()
	{
		SAFE_DELETE( m_mapMesh );
	}
	//--------------------------------------------
	void Map::initializeMap( const std::string& terrainType, int width, int height )
	{
		m_blueprint = m_world->getMap()->m_blueprint;
		const TerrainType* terrain = TerrainTypeBlueprint::getBlueprint( terrainType )->createTerrain();
		m_tiles.resize( width * height );
		m_width = width;
		m_height = height;
		m_worldBounds = aabb2f( vec2f::ZERO, (float)m_width, (float)m_height );
		for( int y = 0; y < height; ++y )
		{
			for( int x = 0; x < width; ++x )
			{
				TileCoords tileCoords( x, y );
				int i = getTileIndexFromTileCoords( tileCoords ); 
				m_tiles[i].m_terrainType = terrain;
				m_tiles[i].m_map = this;
				m_tiles[i].m_tileCoords = tileCoords;
				m_tiles[i].m_worldBounds = aabb2f( vec2f( (float)( x - 0.5f ), (float)( y - 0.5f) ), vec2f( (float)( x + 0.5f ), (float)( y + 0.5f ) ) );
				m_tiles[i].m_resourceType = terrain->getResourceInfo().resourceType;
				m_tiles[i].m_resourceAmount = terrain->getResourceInfo().amount;
			}
		}
		m_blueprint->generateMesh( this );
	}
	//--------------------------------------------
	TileIndex Map::getTileIndexFromTileCoords( const TileCoords& tileCoords ) const
	{
		if(	0 <= tileCoords.x  &&
					tileCoords.x < m_width &&
					0 <= tileCoords.y &&
					tileCoords.y < m_height )
		{
			return tileCoords.x + tileCoords.y * m_width;	
		}
		else
		{
			return -1;
		}
		
	}
	//--------------------------------------------
	TileCoords Map::getTileCoordsFromTileIndex( TileIndex index ) const
	{
		if( 0 <= index && index < (int)m_tiles.size() )
		{
			return m_tiles[ index ].getTilecoords();
		}
		else
		{
			return TileCoords( -1, -1 );
		}
	}
	//--------------------------------------------
	WorldCoords Map::getWorldCoordsFromTileCoords( const TileCoords& tileCoords ) const
	{
		WorldCoords temp = WorldCoords( (float)tileCoords.x, (float)tileCoords.y );
		if( m_worldBounds.contains( temp ) )
		{
			return temp;
		}
		else
		{
			return WorldCoords( -1.0f, -1.0f );
		}
	}
	//--------------------------------------------
	TileCoords Map::getTileCoordsFromWorldCoords( const WorldCoords& worldCoords ) const
	{
		if( m_worldBounds.contains( worldCoords ) )
		{
			return TileCoords( (int)( worldCoords.x + 0.5f ), (int)( worldCoords.y + 0.5f ) );
		}
		else
		{
			return TileCoords( -1, -1 );
		}
	}
	//--------------------------------------------
	const Map::MapTile& Map::getMapTile( const TileCoords& tileCoords ) const
	{
		assertion(	0 <= tileCoords.x  &&
					tileCoords.x < m_width &&
					0 <= tileCoords.y &&
					tileCoords.y < m_height, "Tile Coords are out of bounds" );//, tileCoords.toString().c_str() );
		return m_tiles[ getTileIndexFromTileCoords( tileCoords ) ];

	}
	//--------------------------------------------
	Map::MapTile& Map::getMapTile( const TileCoords& tileCoords )
	{
		assertion(	0 <= tileCoords.x  &&
					tileCoords.x < m_width &&
					0 <= tileCoords.y &&
					tileCoords.y < m_height, "Tile Coords are out of bounds" );//, tileCoords.toString().c_str() );
		return m_tiles[ getTileIndexFromTileCoords( tileCoords ) ];
	}	
	//--------------------------------------------
	int Map::collectResourceFromTile( MapTile& mapTile )
	{
		mapTile.m_resourceAmount -= 1;
		if( mapTile.m_resourceAmount <= 0 )
		{
			changeTileBaseOnSurroundings( mapTile );
		}

		return 1;
	}
	//--------------------------------------------
	TileCoords Map::getNextClosestTileCoordsWithResource( const TileCoords& tileCoords, ResourceType type )
	{
		int radius = 2;
		TileCoords newResource( -1, -1 );
		for( int y = tileCoords.y - radius; y <= tileCoords.y + radius && y >= 0 && y < m_height; ++y )
		{
			for( int x = tileCoords.x - radius; x <= tileCoords.x + radius && x >= 0 && x < m_width; ++x )
			{
				TileCoords tempTileCoords( x, y );
				MapTile& mapTile = getMapTile( tempTileCoords );
				if( mapTile.getResourceType() == type )
					newResource = tempTileCoords;
			}
		}
		return newResource;
	}
	//--------------------------------------------
	void Map::render()
	{
		if( m_mapUpdatedThisFrame )
		{
			m_mapMesh->getTriangleBatch( 0 )->vboBuffer->sendToOpenGL();
			m_mapUpdatedThisFrame = false;
		}

		std::vector< MapTile* > tiles;
		//Render some text
		std::string fontName = "mainFont_72";		
		float fontHeight = 1.0f;
		Color4f color = color::WHITE;
		vec2f posRel = Font::BOTCENTER;
		NamedProperties textParams;
		textParams.set( "fontName", fontName );
		
		textParams.set( "fontHeight", fontHeight );
		textParams.set( "color", color );
		textParams.set( "posRel", posRel );
				
		vec2f minPoint = m_world->getWorldCoordsFromScreenCoords( vec2i( 0, m_world->getScreenHeight() ) );
		vec2f maxPoint = m_world->getWorldCoordsFromScreenCoords( vec2i( m_world->getScreenWidth(), 0 ) );
		aabb2f screenBounds = aabb2f( minPoint, maxPoint );


		matStackf matStack;
		matStack.loadIdentity();
		NamedProperties params;
		params.set( "mesh", m_mapMesh );
		params.set( "modelMatrix", matStack.top() );
		fireEvent( "renderMesh", params );
		
		for( size_t i = 0; i < tiles.size(); ++i )
		{	
			WorldCoords worldCoords = getWorldCoordsFromTileCoords( tiles[i]->getTilecoords() );
			if( screenBounds.contains( worldCoords ) )
			{			
				if( tiles[i]->hasResource() && m_debugMode )
				{
					std::string text = toString( tiles[i]->getResourceAmount() );
					vec3f pos = vec3f( worldCoords, 0.1f );
					pos.y += 0.5f;
			
					textParams.set( "text", text );
					textParams.set( "pos", pos );
					fireEvent( "renderText", textParams );
				}
		
				matStack.pop();
			}
		}
	}
	//--------------------------------------------
	void Map::enableDebugRender( bool enabled )
	{
		m_debugMode = enabled;
	}
	//--------------------------------------------
	const TileCoords& Map::getPlayerStartLocation( int index ) const
	{
		assertion( 0 <= index && index < (int)m_playerStartLocations.size(), "Invalid index for player start locations" );
		return m_playerStartLocations[ index ];
	}
	//--------------------------------------------
	Map::ResourceType Map::getResourceType( std::string type )
	{
		ResourceType resourceType = NONE;

		std::transform( type.begin(), type.end(), type.begin(), ::tolower );
		if( type == "fishfingers" )
		{
			resourceType = FISH_FINGERS;
		}
		else if( type == "custard" )
		{
			resourceType = CUSTARD;
		}
		else if( type == "timeenergy" )
		{
			resourceType = TIME_ENERGY;
		}
		return resourceType;
	}
	//--------------------------------------------
	void Map::changeTileBaseOnSurroundings( Map::MapTile& mapTile )
	{
		std::map< const TerrainType*, int > terrainTypeFreq;
		for( int i = 0; i < 4; ++i )
		{
			TileCoords tileCoords = mapTile.m_tileCoords + m_cardinalDirections[i];
			if( m_worldBounds.contains( getWorldCoordsFromTileCoords( tileCoords ) ) )
			{
				if( !getMapTile( tileCoords ).hasResource() )
					++terrainTypeFreq[ getMapTile( tileCoords ).m_terrainType ];
			}
		}
		std::pair< const TerrainType*, int > newType = std::make_pair< const TerrainType*, int >( nullptr, 0 );
				
		for( auto iter = terrainTypeFreq.begin(); iter != terrainTypeFreq.end(); ++iter )
		{
			if( iter->second > newType.second )
			{
				newType.first = iter->first;
				newType.second = iter->second;
			}
		}
		
		if( newType.first != nullptr )
		{
			changeTileType( mapTile, newType.first );
		}
		else
		{
			changeTileType( mapTile, m_blueprint->getPlayerSpawnTerrainType() );
		}
	}
	//--------------------------------------------
	void Map::changeTileType( MapTile& mapTile, const TerrainType* newType )
	{
		mapTile.m_terrainType = newType;
		mapTile.m_resourceType = newType->getResourceInfo().resourceType;
		mapTile.m_resourceAmount = newType->getResourceInfo().amount;

		int index = getTileIndexFromTileCoords( mapTile.m_tileCoords );
		vec2i texCoordIndex = newType->getTexCoords();

		float texCoordWidth = m_blueprint->getTexCoordWidth();
		float texCoordHeight = m_blueprint->getTexCoordHeight();

		VtxBufferAccessor* vtxAccessor = m_mapMesh->getTriangleBatch( 0 )->vboAccessor;

		vtxAccessor->getUV( index*4 )[0] = texCoordWidth * texCoordIndex.x;
		vtxAccessor->getUV( index*4 )[1] = texCoordHeight * texCoordIndex.y;
		vtxAccessor->getUV( index*4 + 1 )[0] = texCoordWidth * ( texCoordIndex.x + 1 );
		vtxAccessor->getUV( index*4 + 1 )[1] = texCoordHeight * texCoordIndex.y;
		vtxAccessor->getUV( index*4 + 2 )[0] = texCoordWidth * ( texCoordIndex.x + 1 );
		vtxAccessor->getUV( index*4 + 2 )[1] = texCoordHeight * ( texCoordIndex.y + 1 );
		vtxAccessor->getUV( index*4 + 3 )[0] = texCoordWidth * texCoordIndex.x;
		vtxAccessor->getUV( index*4 + 3 )[1] = texCoordHeight * ( texCoordIndex.y + 1 );

		vtxAccessor->getColor( index*4 )[0] = newType->getColor().r;
		vtxAccessor->getColor( index*4 )[1] = newType->getColor().g;
		vtxAccessor->getColor( index*4 )[2] = newType->getColor().b;
		vtxAccessor->getColor( index*4 )[3] = newType->getColor().a;

		vtxAccessor->getColor( index*4 + 1 )[0] = newType->getColor().r;
		vtxAccessor->getColor( index*4 + 1 )[1] = newType->getColor().g;
		vtxAccessor->getColor( index*4 + 1 )[2] = newType->getColor().b;
		vtxAccessor->getColor( index*4 + 1 )[3] = newType->getColor().a;

		vtxAccessor->getColor( index*4 + 2 )[0] = newType->getColor().r;
		vtxAccessor->getColor( index*4 + 2 )[1] = newType->getColor().g;
		vtxAccessor->getColor( index*4 + 2 )[2] = newType->getColor().b;
		vtxAccessor->getColor( index*4 + 2 )[3] = newType->getColor().a;

		vtxAccessor->getColor( index*4 + 3 )[0] = newType->getColor().r;
		vtxAccessor->getColor( index*4 + 3 )[1] = newType->getColor().g;
		vtxAccessor->getColor( index*4 + 3 )[2] = newType->getColor().b;
		vtxAccessor->getColor( index*4 + 3 )[3] = newType->getColor().a;

		m_mapUpdatedThisFrame = true;
	}
	//--------------------------------------------
	void Map::changeTileColor( int index, const Color4f& color )
	{
		VtxBufferAccessor* vtxAccessor = m_mapMesh->getTriangleBatch( 0 )->vboAccessor;
		
		std::memcpy( vtxAccessor->getColor( index*4 ), &color, sizeof( Color4f ) );
		std::memcpy( vtxAccessor->getColor( index*4 + 1 ), &color, sizeof( Color4f ) );
		std::memcpy( vtxAccessor->getColor( index*4 + 2 ), &color, sizeof( Color4f ) );
		std::memcpy( vtxAccessor->getColor( index*4 + 3 ), &color, sizeof( Color4f ) );

		m_mapUpdatedThisFrame = true;
	}
	//--------------------------------------------
	bool Map::areTileCoordsInBounds( const TileCoords& tileCoords ) const
	{
		return  0 <= tileCoords.x && tileCoords.x < m_width &&
				0 <= tileCoords.y && tileCoords.y < m_height;
	}

	const vec2i Map::CD_NORTH = vec2i( 0, 1 );
	const vec2i Map::CD_EAST = vec2i( 1, 0 );
	const vec2i Map::CD_SOUTH = vec2i( 0, -1 );
	const vec2i Map::CD_WEST = vec2i( -1, 0 );	
}