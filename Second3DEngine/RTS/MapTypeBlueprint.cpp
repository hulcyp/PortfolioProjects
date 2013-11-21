#include "MapTypeBlueprint.h"
#include "TerrainType.h"
#include "TerrainTypeBlueprint.h"
#include <CommonEngineIncludes.h>
#include <StringUtils.h>
#include <Time.h>
#include <RandNumGen.h>
#include "BlobBrush.h"
#include <BufferLayout.h>
#include <GLBuffer.h>
#include <VtxBufferAccessor.h>
#include <Texture.h>
#include <Mesh.h>
#include <Renderer.h>

namespace pdh
{
	MapTypeBlueprint::MapTypeBlueprint( XMLParser& parser, const XMLNode* mapTypeBlueprintNode )
	{
		parser.validateXMLChildElements( mapTypeBlueprintNode, "DefaultTerrain", "AllowedMapSizes,Blobs,Streams,Mutator" );
		parser.validateXMLAttributes( mapTypeBlueprintNode, "name,textureMap,tileWidth,tileHeight", "" );

		m_name = parser.getXMLAttributeAsString( mapTypeBlueprintNode, "name", "" );
		std::string texMapName = parser.getXMLAttributeAsString( mapTypeBlueprintNode, "textureMap", "" );
		m_textureMap = Renderer::getTexture( texMapName );
		
		m_tileWidth = parser.getXMLAttributeAsInt( mapTypeBlueprintNode, "tileWidth", 0 );
		m_tileHeight = parser.getXMLAttributeAsInt( mapTypeBlueprintNode, "tileHeight", 0 );


		const XMLNode* defaultTerrain = mapTypeBlueprintNode->FirstChildElement( "DefaultTerrain" );
		parser.validateXMLAttributes( defaultTerrain, "fill,border,playerSpawn", "" );

		std::string fillTerrain		= parser.getXMLAttributeAsString( defaultTerrain, "fill", "" );
		std::string borderTerrain	= parser.getXMLAttributeAsString( defaultTerrain, "border", "" );
		std::string playerTerrain	= parser.getXMLAttributeAsString( defaultTerrain, "playerSpawn", "" );

		m_defaultTerrain = TerrainTypeBlueprint::getBlueprint( fillTerrain )->createTerrain();
		m_borderTerrain = TerrainTypeBlueprint::getBlueprint( borderTerrain )->createTerrain();
		m_playerSpawnTerrain = TerrainTypeBlueprint::getBlueprint( playerTerrain )->createTerrain();

		const XMLNode* terrainStruct;
		for( terrainStruct = mapTypeBlueprintNode->FirstChildElement(); terrainStruct != 0; terrainStruct = terrainStruct->NextSiblingElement() )
		{
			if( std::string( terrainStruct->Name() ) == "Blobs" )
			{
				parseBlobs( parser, terrainStruct );
			}
			else if( std::string( terrainStruct->Name() ) == "Streams" )
			{
				parseStreams( parser, terrainStruct );
			}
			else if( std::string( terrainStruct->Name() ) == "Mutator" )
			{
				parseMutator( parser, terrainStruct );				
			}
		}


		m_cardinalPool.addItem( Map::CD_NORTH );
		m_cardinalPool.addItem( Map::CD_EAST );
		m_cardinalPool.addItem( Map::CD_SOUTH );
		m_cardinalPool.addItem( Map::CD_WEST );

		registerBP( m_name );
		initializeSpawnDirections();
	}
	//--------------------------------------------------------
	MapTypeBlueprint::~MapTypeBlueprint()
	{}
	//--------------------------------------------------------
	Map* MapTypeBlueprint::createMap( int width, int height, int numPlayers, RTSWorld* world )
	{		
		Map* map = new Map( world );
		map->m_blueprint = this;
		map->m_width = width;
		map->m_height = height;
		map->m_worldBounds = aabb2f( vec2f::ZERO, vec2f( (float)width, (float)height ) );
		map->m_tiles.resize( width * height );
		
		numPlayers = MathFuncs<int>::clamp( numPlayers, 0, 8 );
		
		map->m_playerStartLocations.resize( numPlayers );
		
		addPlayerSpawns( map );

		map->m_seed = (unsigned int)GetAbsoluteTimeSeconds();
		srand( map->m_seed );

		populateMapWithDefaultTerrain( map );
		int b = 0;
		int s = 0;
		for( size_t i = 0; i < m_terrainStructTypes.size(); ++i )
		{
			switch( m_terrainStructTypes[i] )
			{
			case BLOBS:
				applyBlobs( map, m_blobs[b] );
				++b;
				break;
			case STREAMS:
				applyStreams( map, m_streams[s] );
				++s;
				break;
			}			
		}
		applyBorderTerrain( map );
		clearPlayerSpawnArea( map );
		applyMutators( map );
		generateMesh( map );
		return map;
	}
	//--------------------------------------------------------
	MapTypeBlueprint* MapTypeBlueprint::getBlueprint( const std::string& name )
	{
		std::map< std::string, MapTypeBlueprint* >::iterator iter = sm_blueprints.find( name );

		if( iter != sm_blueprints.end() )
		{
			return iter->second;
		}
		else
		{
			return nullptr;
		}
	}
	//--------------------------------------------------------
	// Protected member functions
	//--------------------------------------------------------
	void MapTypeBlueprint::populateMapWithDefaultTerrain( Map* map )
	{
		for( int y = 0; y < map->m_height; ++y )
		{
			for( int x = 0; x < map->m_width; ++x )
			{
				TileCoords tileCoords( x, y );
				int i = map->getTileIndexFromTileCoords( tileCoords );
				map->m_tiles[i].m_terrainType = m_defaultTerrain;
				map->m_tiles[i].m_tileCoords = tileCoords;
				map->m_tiles[i].m_worldBounds = aabb2f( vec2f( (float)( x - 0.5f ), (float)( y - 0.5f) ), vec2f( (float)( x + 0.5f ), (float)( y + 0.5f ) ) );
				map->m_tiles[i].m_resourceAmount = m_defaultTerrain->getResourceInfo().amount;
				map->m_tiles[i].m_resourceType = m_defaultTerrain->getResourceInfo().resourceType;
				map->m_tiles[i].m_map = map;
			}
		}
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::applyBorderTerrain( Map* map )
	{
		for( int y = 0; y < map->m_height; y += map->m_height - 1 )
		{
			for( int x = 0; x < map->m_width; ++x )
			{
				TileCoords tileCoords( x, y );
				int i = map->getTileIndexFromTileCoords( tileCoords );
				map->m_tiles[i].m_terrainType = m_borderTerrain;
				map->m_tiles[i].m_resourceAmount = m_borderTerrain->getResourceInfo().amount;
				map->m_tiles[i].m_resourceType = m_borderTerrain->getResourceInfo().resourceType;
			}
		}
		for( int x = 0; x < map->m_width; x += map->m_width - 1 )
		{
			for( int y = 1; y < map->m_height - 1; ++y )
			{
				TileCoords tileCoords( x, y );
				int i = map->getTileIndexFromTileCoords( tileCoords );
				map->m_tiles[i].m_terrainType = m_borderTerrain;
			}
		}			
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::applyBlobs( Map* map, const Blobs& blobs )
	{
		int area = map->m_height * map->m_width;
		float scaleFactor = 0.0001f * area;
		
		int count =  (int)( blobs.countRangePH.evaluate().evaluate() * scaleFactor );
		 
		if( blobs.numPerPlayer > 0 )
			count = (int)( count * blobs.numPerPlayer * map->m_playerStartLocations.size() );

		int currentNumBlobs = 0;
								
		if( count > 0 )
		{
			if( !blobs.forEachPlayer )
			{
				while( currentNumBlobs <= count )
				{
					for( int y = 0; y < map->m_height; ++y )
					{
						for( int x = 0; x < map->m_width; ++x )
						{
							int randNum = RandNumGen::randInRangeInt( 0, area );
							if( randNum <= count )
							{
								int index = map->getTileIndexFromTileCoords( TileCoords( x, y ) );
								
								bool passedSpawnRules = true;
								if( blobs.startsOn.size() > 0 )
								{
									passedSpawnRules = false;
									int listIdx = findStringInList( map->m_tiles[ index ].m_terrainType->getName(), blobs.startsOn );
									if( listIdx != -1 )
										passedSpawnRules = true;
								}

								
								if( passedSpawnRules )
									passedSpawnRules = checkAdjRulesForTerrainStruct( map->m_tiles[ index ].m_terrainType->getName(), reinterpret_cast< const TerrainStruct* >( &blobs ), map, index );

								if( passedSpawnRules )
								{
									map->m_tiles[ index ].m_terrainType = blobs.terrainType;
									int radius = (int)( blobs.radiusRangePH.evaluate().evaluate() * scaleFactor );
							
									BlobBrush brush( "brush", (float)radius, blobs.density, blobs.roundness );
									brush.draw( map, blobs.terrainType, TileCoords( x, y ), blobs.canOverride, blobs.cannotOverride );
							
									currentNumBlobs++;
									if( currentNumBlobs > count )
										break;
								}
							}
						}
					}
				}
			}
			else
			{
				for( size_t i = 0; i < map->m_playerStartLocations.size(); ++i )
				{
					currentNumBlobs = 0;
					while( currentNumBlobs < count )
					{
						TileCoords tileCoords( -1, -1 );
						while( tileCoords.x == -1 )
						{
							vec2f ranDir = vec2f::randDir();
							ranDir *= blobs.distFromPlayerPH.evaluate().evaluate() * scaleFactor;
							WorldCoords worldCoords = map->getWorldCoordsFromTileCoords( map->m_playerStartLocations[i] ) + ranDir + WorldCoords( .5f, .5f );
							tileCoords = map->getTileCoordsFromWorldCoords( worldCoords );
							int index = map->getTileIndexFromTileCoords( tileCoords );

							bool passedSpawnRules = true;
							if( blobs.startsOn.size() > 0 )
							{
								passedSpawnRules = false;
								int listIdx = findStringInList( map->m_tiles[ index ].m_terrainType->getName(), blobs.startsOn );
								if( listIdx != -1 )
									passedSpawnRules = true;
							}

							
							if( passedSpawnRules )
								passedSpawnRules = checkAdjRulesForTerrainStruct( map->m_tiles[ index ].m_terrainType->getName(), reinterpret_cast< const TerrainStruct* >( &blobs ), map, index );

							if( index != -1 && ( map->m_tiles[ index ].m_terrainType == blobs.terrainType || map->m_tiles[ index ].m_terrainType == m_borderTerrain ) && !passedSpawnRules )
								tileCoords.x = -1;
						}
						int radius = (int)( blobs.radiusRangePH.evaluate().evaluate() * scaleFactor );
							
						BlobBrush brush( "brush", (float)radius, blobs.density, blobs.roundness );
						brush.draw( map, blobs.terrainType, tileCoords, blobs.canOverride, blobs.cannotOverride );
							
						currentNumBlobs++;
						if( currentNumBlobs > count )
								break;
						++currentNumBlobs;
					}

				}
			}	
		}
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::applyStreams( Map* map, const Streams& streams )
	{

		int area = map->m_height * map->m_width;
		float scaleFactor = 0.0001f * area;
	
		int count =  (int)( streams.countRangePH.evaluate().evaluate() * scaleFactor );
		
		if( streams.numPerPlayer > 0 )
			count = (int)( count * streams.numPerPlayer * map->m_playerStartLocations.size() );
		
		int currentNumStreams = 0;

		if( count > 0 )
		{
			if( !streams.forEachPlayer )
			{
				while( currentNumStreams <= count )
				{					
					for( int y = 0; y < map->m_height; ++y )
					{
						for( int x = 0; x < map->m_width; ++x )
						{
							int randNum = RandNumGen::randInRangeInt( 0, area );
							if( randNum <= count )
							{
								bool passedSpawnRules = true;								
								int index = map->getTileIndexFromTileCoords( TileCoords( x, y ) );

								if( streams.startsOn.size() > 0 )
								{
									passedSpawnRules = false;
									int listIdx = findStringInList( map->m_tiles[ index ].m_terrainType->getName(), streams.startsOn );
									if( listIdx != -1 )
										passedSpawnRules = true;
								}

								if( passedSpawnRules )
									passedSpawnRules = checkAdjRulesForTerrainStruct( map->m_tiles[ index ].m_terrainType->getName(), reinterpret_cast< const TerrainStruct* >( &streams ), map, index );

								if( passedSpawnRules )
								{
									int length = (int)( streams.lengthRangePH.evaluate().evaluate() * scaleFactor );
									spawnStream( map, 0, streams, TileCoords( x, y ), TileCoords( x, y ) + m_cardinalPool.getRandItem(), 0, length );
									++currentNumStreams;
								}
							}
						}
					}
				
				}
			}
			else
			{				
				for( size_t i = 0; i < map->m_playerStartLocations.size(); ++i )
				{
					currentNumStreams = 0;
					while( currentNumStreams < count )
					{
						TileCoords tileCoords( -1, -1 );
						int index = -1;
						while( tileCoords.x == -1 )
						{
							vec2f ranDir = vec2f::randDir();
							ranDir *= streams.distFromPlayerPH.evaluate().evaluate() * scaleFactor;
							WorldCoords worldCoords = map->getWorldCoordsFromTileCoords( map->m_playerStartLocations[i] ) + ranDir + WorldCoords( .5f, .5f );
							tileCoords = map->getTileCoordsFromWorldCoords( worldCoords );
							index = map->getTileIndexFromTileCoords( tileCoords );

							if( index != -1 && ( map->m_tiles[ index ].m_terrainType == streams.terrainType || map->m_tiles[ index ].m_terrainType == m_borderTerrain ) )
								tileCoords.x = -1;
						}
						
						bool passedSpawnRules = true;
						if( index != -1 )
						{
							if( streams.startsOn.size() > 0 )
							{
								passedSpawnRules = false;
								int listIdx = findStringInList( map->m_tiles[ index ].m_terrainType->getName(), streams.startsOn );
								if( listIdx != -1 )
									passedSpawnRules = true;
							}
							
							if( passedSpawnRules )
								passedSpawnRules = checkAdjRulesForTerrainStruct( map->m_tiles[ index ].m_terrainType->getName(), reinterpret_cast< const TerrainStruct* >( &streams ), map, index );
						}

						if( passedSpawnRules )
						{
							int length = (int)( streams.lengthRangePH.evaluate().evaluate() * scaleFactor );
							spawnStream( map, 0, streams, tileCoords, tileCoords + m_cardinalPool.getRandItem(), 0, length );
							++currentNumStreams;
						}
					}

				}
			}
		}
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::spawnStream( Map* map, int currentLength, const Streams& streams, const TileCoords& currentTileCoords, const TileCoords& prevTileCoords, int numForks, const int& maxLength )
	{
		if( currentLength >= maxLength || numForks > 4 )
			return;

		int i = map->getTileIndexFromTileCoords( currentTileCoords );
		int width = streams.widthPH.evaluate().evaluate();
		
		
		BlobBrush brush( "brush", (float)width, 1.0f, 1.0f );
		brush.draw( map, streams.terrainType, currentTileCoords, streams.canOverride, streams.cannotOverride );
		
	
		TileCoords prevDir = currentTileCoords - prevTileCoords;
		TileCoords nextTileCoords;

		float bend = RandNumGen::randInRangeUnit();
		if( bend < streams.straightness )
		{
			nextTileCoords = currentTileCoords + prevDir.yx();
		}
		else
		{
			nextTileCoords = currentTileCoords + prevDir;
		}

		float fork = RandNumGen::randInRangeUnit();

		if( fork < streams.chanceOfFork )
		{
			++numForks;
			spawnStream( map, currentLength, streams, nextTileCoords, currentTileCoords, numForks, maxLength );
		}

		++currentLength;
		spawnStream( map, currentLength, streams, nextTileCoords, currentTileCoords, numForks, maxLength );
	}
	//-------------------------------------------------------
	void MapTypeBlueprint::applyMutators( Map* map )
	{
		for( size_t mut = 0; mut < m_mutators.size(); ++mut )
		{
			for( int y = 1; y < map->m_height - 1; ++y )
			{
				for( int x = 1; x < map->m_width - 1; ++x )
				{
					TileCoords currentTile = TileCoords( x, y );
					Map::MapTile& mapTile = map->getMapTile( currentTile );
					if( mapTile.m_terrainType->getName() == m_mutators[ mut ].terrainType )
					{
						for( int card = 0; card < 4; ++card )
						{
							int index = map->getTileIndexFromTileCoords( currentTile + map->m_cardinalDirections[card] );
							if( index != -1 )
							{
								if( m_mutators[ mut ].typeAdjTo.size() > 0 )
								{
									if( map->m_tiles[ index ].m_terrainType->getName() == m_mutators[ mut ].typeAdjTo )
									{
										TerrainType* terrainType = TerrainTypeBlueprint::getBlueprint( m_mutators[ mut ].typeToChangeTo )->createTerrain();
										mapTile.m_terrainType = terrainType;
										mapTile.m_resourceType = terrainType->getResourceInfo().resourceType;
										mapTile.m_resourceAmount = terrainType->getResourceInfo().amount;
									}
								}
								else
								{
									if( map->m_tiles[ index ].m_terrainType->getName() != mapTile.m_terrainType->getName() && map->m_tiles[ index ].m_terrainType->getName() != m_mutators[ mut ].typeToChangeTo )
									{
										TerrainType* terrainType = TerrainTypeBlueprint::getBlueprint( m_mutators[ mut ].typeToChangeTo )->createTerrain();
										mapTile.m_terrainType = terrainType;
										mapTile.m_resourceType = terrainType->getResourceInfo().resourceType;
										mapTile.m_resourceAmount = terrainType->getResourceInfo().amount;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//-------------------------------------------------------
	void MapTypeBlueprint::addPlayerSpawns( Map* map )
	{
		int spawnInsetX = (int)(map->m_width*0.1f);
		int spawnInsetY = (int)(map->m_height*0.1f);
		int spawnCubeHeight = map->m_height - spawnInsetX * 2;
		int spawnCubeWidth = map->m_width - spawnInsetY * 2;

		map->m_playerStartLocations[0] = TileCoords( spawnInsetX, spawnInsetY );
		
		for( size_t i = 1; i < map->m_playerStartLocations.size(); ++i )
		{
			TileCoords nextLocVec = TileCoords( (int)( sm_spawnDirections[ i - 1 ].x * spawnCubeWidth ), (int)( sm_spawnDirections[ i - 1 ].y * spawnCubeHeight ) );
			map->m_playerStartLocations[ i ] = map->m_playerStartLocations[ i - 1 ] + nextLocVec;
						
		}
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::clearPlayerSpawnArea( Map* map )
	{
		std::vector< std::string > canOverride;
		std::vector< std::string > cannotOverride;
		BlobBrush brush( "brush", (float)map->getMapWidth()*0.07f, 1.0f, 1.0f );	
		for( size_t i = 0; i < map->m_playerStartLocations.size(); ++i )
		{
			int index = map->getTileIndexFromTileCoords( map->m_playerStartLocations[i] );
			brush.draw( map, m_playerSpawnTerrain, map->m_playerStartLocations[i], canOverride, cannotOverride );
		}
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::parseBlobs( XMLParser& parser, const XMLNode* blobsNode )
	{
		Blobs blobs;

		parser.validateXMLAttributes( blobsNode, "type,countPH,radiusPH,density,roundness", "perPlayer" );
		parser.validateXMLChildElements( blobsNode, "", "ForEachPlayer,SpawnRules" );
		std::string type = parser.getXMLAttributeAsString( blobsNode, "type", "" );
		blobs.terrainType = TerrainTypeBlueprint::getBlueprint( type )->createTerrain();
		blobs.countRangePH = parser.getXMLAttributeAsWeightedRandPoolIntRange( blobsNode, "countPH" );
		blobs.radiusRangePH = parser.getXMLAttributeAsWeightedRandPoolIntRange( blobsNode, "radiusPH" );

		blobs.density = parser.getXMLAttributeAsFloat( blobsNode, "density", 1.0f );
		blobs.roundness = parser.getXMLAttributeAsFloat( blobsNode, "roundness", 1.0f );
		blobs.numPerPlayer	= parser.getXMLAttributeAsFloat( blobsNode, "perPlayer", 0.0f );
		
		parseTerrainStructs( parser, blobsNode, blobs );

		m_blobs.push_back( blobs );
		m_terrainStructTypes.push_back( BLOBS );
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::parseStreams( XMLParser& parser, const XMLNode* streamsNode )
	{
		Streams streams;
		
		parser.validateXMLAttributes( streamsNode, "type,countPH,lengthPH,chanceOfFork,straightness,widthPH", "perPlayer" );
		parser.validateXMLChildElements( streamsNode, "", "ForEachPlayer,SpawnRules" );
		std::string type = parser.getXMLAttributeAsString( streamsNode, "type", "" );
		streams.terrainType	= TerrainTypeBlueprint::getBlueprint( type )->createTerrain();
		streams.countRangePH	= parser.getXMLAttributeAsWeightedRandPoolIntRange( streamsNode, "countPH" );
		streams.lengthRangePH	= parser.getXMLAttributeAsWeightedRandPoolIntRange( streamsNode, "lengthPH" );
		streams.chanceOfFork	= parser.getXMLAttributeAsFloat( streamsNode, "chanceOfFork", 0.0f );
		streams.straightness	= parser.getXMLAttributeAsFloat( streamsNode, "straightness", 0.0f );
		streams.widthPH			= parser.getXMLAttributeAsWeightedRandPoolIntRange( streamsNode, "widthPH" ); 
		streams.numPerPlayer	= parser.getXMLAttributeAsFloat( streamsNode, "perPlayer", 0.0f );
		
		parseTerrainStructs( parser, streamsNode, streams );

		m_streams.push_back( streams );
		m_terrainStructTypes.push_back( STREAMS );
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::parseTerrainStructs( XMLParser& parser, const XMLNode* structNode, TerrainStruct& terrainStruct )
	{
		const XMLNode* forEachPlayer = structNode->FirstChildElement( "ForEachPlayer" );
		if( forEachPlayer != nullptr )
		{
			terrainStruct.forEachPlayer = true;
			terrainStruct.distFromPlayerPH = parser.getXMLAttributeAsWeightedRandPoolFloatRange( forEachPlayer, "distFromPlayerPH" );
		}

		const XMLNode* spawnRules = structNode->FirstChildElement( "SpawnRules" );
		if( spawnRules != nullptr )
		{
			parser.validateXMLAttributes( spawnRules, "", "startsOn,canOverride,cannotOverride,mustSpawnAdjTo,cannotSpawnAdjTo" );
			const char* startsOn = spawnRules->Attribute( "startsOn" );
			if( startsOn && strlen( startsOn ) )
			{
				std::vector< std::string > startsOnTokens;
				stringTokenizer( startsOn, startsOnTokens, "," );
				for( size_t i = 0; i < startsOnTokens.size(); ++i )
				{
					terrainStruct.startsOn.push_back( startsOnTokens[i] );
				}
			}
			const char* canOverride = spawnRules->Attribute( "canOverride" );
			if( canOverride && strlen( canOverride ) )
			{
				std::vector< std::string > canOverrideTokens;
				stringTokenizer( canOverride, canOverrideTokens, "," );
				for( size_t i = 0; i < canOverrideTokens.size(); ++i )
				{
					terrainStruct.canOverride.push_back( canOverrideTokens[i] );
				}
			}
			const char* cannotOverride = spawnRules->Attribute( "cannotOverride" );
			if( cannotOverride && strlen( cannotOverride ) )
			{
				std::vector< std::string > cannotOverrideTokens;
				stringTokenizer( cannotOverride, cannotOverrideTokens, "," );
				for( size_t i = 0; i < cannotOverrideTokens.size(); ++i )
				{
					terrainStruct.cannotOverride.push_back( cannotOverrideTokens[i] );
				}
			}
			const char* mustSpawnAdjTo = spawnRules->Attribute( "mustSpawnAdjTo" );
			if( mustSpawnAdjTo && strlen( mustSpawnAdjTo ) )
			{
				std::vector< std::string > mustSpawnAdjToTokens;
				stringTokenizer( mustSpawnAdjTo, mustSpawnAdjToTokens, "," );
				for( size_t i = 0; i < mustSpawnAdjToTokens.size(); ++i )
				{
					terrainStruct.mustSpawnAdjTo.push_back( mustSpawnAdjToTokens[i] );
				}
			}
			const char* cannotSpawnAdjTo = spawnRules->Attribute( "cannotSpawnAdjTo" );
			if( cannotSpawnAdjTo && strlen( cannotSpawnAdjTo ) )
			{
				std::vector< std::string > cannotSpawnAdjToTokens;
				stringTokenizer( cannotSpawnAdjTo, cannotSpawnAdjToTokens, "," );
				for( size_t i = 0; i < cannotSpawnAdjToTokens.size(); ++i )
				{
					terrainStruct.cannotSpawnAdjTo.push_back( cannotSpawnAdjToTokens[i] );
				}
			}
		}
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::parseMutator( XMLParser& parser, const XMLNode* mutatorNode )
	{
		parser.validateXMLAttributes( mutatorNode, "terrainType,typeAdjTo,typeToChangeTo", "" );

		Mutator mutator;
		mutator.terrainType = parser.getXMLAttributeAsString( mutatorNode, "terrainType", "" );
		mutator.typeAdjTo = parser.getXMLAttributeAsString( mutatorNode, "typeAdjTo", "" );
		mutator.typeToChangeTo = parser.getXMLAttributeAsString( mutatorNode, "typeToChangeTo", "" );

		m_mutators.push_back( mutator );
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::registerBP( const std::string& name )
	{
		MapTypeBlueprint*& blueprint = sm_blueprints[ name ];
		SAFE_DELETE( blueprint ); 
		blueprint = this;
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::initializeSpawnDirections()
	{
		if( sm_spawnDirections.size() == 0 )
		{
			sm_spawnDirections.push_back( vec2f( 1.0f, 1.0f ) );
			sm_spawnDirections.push_back( vec2f( 0.0f, -1.0f ) );
			sm_spawnDirections.push_back( vec2f( -1.0f, 1.0f ) );
			sm_spawnDirections.push_back( vec2f( 0.5f, 0.0f ) );
			sm_spawnDirections.push_back( vec2f( 0.5f, -0.5f ) );
			sm_spawnDirections.push_back( vec2f( -0.5f, -0.5f ) );
			sm_spawnDirections.push_back( vec2f( -0.5f, 0.5f ) );
		}
	}
	bool MapTypeBlueprint::checkAdjRulesForTerrainStruct( const std::string& terrainTypeName, const TerrainStruct* terrainStruct, Map* map, int tileIndex )
	{
		TileCoords tileCoords = map->getTileCoordsFromTileIndex( tileIndex );
		bool passedAdjRules = true;
		if( terrainStruct->mustSpawnAdjTo.size() > 0 )
		{			
			passedAdjRules = false;
			for( int i = 0; i < 4; ++i )
			{
				TileCoords temp = tileCoords + map->m_cardinalDirections[i];
				int teampIndex = map->getTileIndexFromTileCoords( temp );
				if( teampIndex != -1 )
				{
					int listIdx = findStringInList( map->m_tiles[ teampIndex ].m_terrainType->getName(), terrainStruct->mustSpawnAdjTo );
					if( listIdx != -1 )
						passedAdjRules = true;
				}
			}
		}

		if( terrainStruct->cannotSpawnAdjTo.size() > 0 && passedAdjRules )
		{
			for( int i = 0; i < 4; ++i )
			{
				TileCoords temp = tileCoords + map->m_cardinalDirections[i];
				int teampIndex = map->getTileIndexFromTileCoords( temp );
				if( teampIndex != -1 )
				{
					int listIdx = findStringInList( map->m_tiles[ teampIndex ].m_terrainType->getName(), terrainStruct->cannotSpawnAdjTo );
					if( listIdx != -1 )
						passedAdjRules = false;
				}
			}
		}

		return passedAdjRules;
	}
	//--------------------------------------------------------
	void MapTypeBlueprint::generateMesh( Map* map )
	{		
		int width = m_textureMap->GetWidth();
		int height = m_textureMap->GetHeight();
		m_texCoordWidth = (float)m_tileWidth / width;
		m_texCoordHeight = (float)m_tileHeight / width;

		size_t numTiles = map->getMapHeight() * map->getMapWidth() ;
		size_t numVertices = numTiles * 4;
		std::vector< MapVertex > vertices;
		std::vector< unsigned int > indices;
		
		for( size_t i = 0; i < numTiles; ++i )
		{
			vec3f tilePos = vec3f( map->getWorldCoordsFromTileCoords( map->getTileCoordsFromTileIndex( i ) ), 0.1f );
			vec2i texCoordIndex = map->getMapTile( map->getTileCoordsFromTileIndex( i ) ).m_terrainType->getTexCoords();

			vertices.push_back( MapVertex( vec3f( tilePos.x - 0.5f, tilePos.y + 0.5f, tilePos.z ),
								map->getMapTile( map->getTileCoordsFromTileIndex( i ) ).m_terrainType->getColor(),
								vec2f( m_texCoordWidth * texCoordIndex.x, m_texCoordHeight * texCoordIndex.y ) ) );

			vertices.push_back( MapVertex( vec3f( tilePos.x + 0.5f, tilePos.y + 0.5f, tilePos.z ),
								map->getMapTile( map->getTileCoordsFromTileIndex( i ) ).m_terrainType->getColor(),
								vec2f( m_texCoordWidth * ( texCoordIndex.x + 1 ), m_texCoordHeight * texCoordIndex.y ) ) );

			vertices.push_back( MapVertex( vec3f( tilePos.x + 0.5f, tilePos.y - 0.5f, tilePos.z ),
								map->getMapTile( map->getTileCoordsFromTileIndex( i ) ).m_terrainType->getColor(),
								vec2f( m_texCoordWidth * ( texCoordIndex.x + 1 ), m_texCoordHeight * ( texCoordIndex.y + 1 ) ) ) );
			
			vertices.push_back( MapVertex( vec3f( tilePos.x - 0.5f, tilePos.y - 0.5f, tilePos.z ),
								map->getMapTile( map->getTileCoordsFromTileIndex( i ) ).m_terrainType->getColor(),
								vec2f( m_texCoordWidth * texCoordIndex.x, m_texCoordHeight * ( texCoordIndex.y + 1 ) ) ) );
			
			indices.push_back( 4*i );
			indices.push_back( 4*i + 2 );
			indices.push_back( 4*i + 3 );
			indices.push_back( 4*i + 2 );
			indices.push_back( 4*i );
			indices.push_back( 4*i + 1 );
		}
		
		BufferLayout* vtxBufferLayout = new BufferLayout( 3, 
			BufferLayout::FLOAT3, 
			BufferLayout::FLOAT4, 
			BufferLayout::FLOAT2 );
		size_t vtxBufferSize = vtxBufferLayout->stride() * vertices.size();
		char* vtxBufferData = new char[ vtxBufferSize ];
		std::memcpy( vtxBufferData, (char*)vertices.data(), vtxBufferSize );

		BufferLayout* idxBufferLayout = new BufferLayout( 1, BufferLayout::INT );
		size_t idxBufferSize = idxBufferLayout->stride() * indices.size();
		char* idxBufferData = new char[ idxBufferSize ];
		std::memcpy( idxBufferData, (char*)indices.data(), idxBufferSize );

		Material* mat = Renderer::createMaterial( m_name + "_map_mat", "RTSMapTileShader" );
		mat->addUniform( "uUseDiffuseMap", 0 );
		if( m_textureMap )
		{
			mat->addTexture( "uDiffuseMap", m_textureMap );
			mat->updateUniform( "uUseDiffuseMap", 1 );
		}

		GLBuffer* vtxBuffer = new GLBuffer( vtxBufferData, vertices.size(), vtxBufferLayout, GL_ARRAY_BUFFER );
		GLBuffer* idxBuffer = new GLBuffer( idxBufferData, indices.size(), idxBufferLayout, GL_ELEMENT_ARRAY_BUFFER ); 

		vtxBuffer->sendToOpenGL();
		idxBuffer->sendToOpenGL();

		std::map< VtxBufferAccessor::AttributeType, int > vtxMapping;
		vtxMapping[ VtxBufferAccessor::POSITION ] = 0;
		vtxMapping[ VtxBufferAccessor::COLOR ] = 1;
		vtxMapping[ VtxBufferAccessor::UV ] = 2;
		VtxBufferAccessor* vtxAccessor = new VtxBufferAccessor( vtxBuffer, vtxMapping );

		map->m_mapMesh = new Mesh( vtxBuffer, idxBuffer, vtxAccessor, m_name + "_map_mat" );

	}

	std::map< std::string, MapTypeBlueprint* > MapTypeBlueprint::sm_blueprints;
	std::vector< vec2f > MapTypeBlueprint::sm_spawnDirections;
}