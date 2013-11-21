#pragma once
#include <XMLParser.h>
#include <map>
#include <string>
#include <vector>
#include <RandItemPool.h>
#include "Map.h"
#include <WeightedRandPool.h>
#include <RandRangeFloat.h>
#include <RandRangeInt.h>

namespace pdh
{
	class Map;
	class TerrainType;
	class RTSWorld;
	class Texture;

	class MapTypeBlueprint
	{
	public:
		
		enum TerrainStructType
		{
			BLOBS,
			STREAMS
		};

		struct TerrainStruct
		{
			TerrainStruct()
				:	forEachPlayer( false )
			{}
			float numPerPlayer;
			bool forEachPlayer;
			WeightedRandPool< RandRangeFloat > distFromPlayerPH;
			std::vector< std::string > startsOn;
			std::vector< std::string > canOverride;
			std::vector< std::string > cannotOverride;
			std::vector< std::string > mustSpawnAdjTo;
			std::vector< std::string > cannotSpawnAdjTo;
		};

		struct Blobs : public TerrainStruct
		{			
			TerrainType* terrainType;
			WeightedRandPool< RandRangeInt > countRangePH;
			WeightedRandPool< RandRangeInt > radiusRangePH;
			float density;
			float roundness;
		};
		
		struct Streams : public TerrainStruct
		{
			TerrainType* terrainType;
			WeightedRandPool< RandRangeInt > countRangePH;
			WeightedRandPool< RandRangeInt > lengthRangePH;
			float chanceOfFork;
			float straightness;
			WeightedRandPool< RandRangeInt > widthPH;

		};

		struct Mutator
		{
			std::string terrainType;
			std::string typeAdjTo;
			std::string typeToChangeTo;
		};
		
		MapTypeBlueprint( XMLParser& parser, const XMLNode* mapTypeBlueprintNode );
		~MapTypeBlueprint();

		Map* createMap( int widht, int height, int numPlayers, RTSWorld* world );
		const std::string& getName() { return m_name; }
		
		void generateMesh( Map* map );

		static MapTypeBlueprint* getBlueprint( const std::string& name );

		float getTexCoordHeight() const { return m_texCoordHeight; }
		float getTexCoordWidth() const { return m_texCoordWidth; }

		const TerrainType* getPlayerSpawnTerrainType() const { return m_playerSpawnTerrain; }

	protected:
		void populateMapWithDefaultTerrain( Map* map );
		void applyBorderTerrain( Map* map );
		void applyBlobs( Map* map, const Blobs& blobs );
		void applyStreams( Map* map, const Streams& streams );
		void spawnStream( Map* map, int currentLength, const Streams& streams, const TileCoords& currentTileCoords, const TileCoords& prevTileCoords, int numForks, const int& maxLength );
		void applyMutators( Map* map );
		void addPlayerSpawns( Map* map );
		void clearPlayerSpawnArea( Map* map );

		void parseBlobs( XMLParser& parser, const XMLNode* blobsNode );
		void parseStreams( XMLParser& parser, const XMLNode* streamsNode );
		void parseTerrainStructs( XMLParser& parser, const XMLNode* structNode, TerrainStruct& terrainStruct );
		void parseMutator( XMLParser& parser, const XMLNode* mutatorNode );

		void registerBP( const std::string& name );
		void initializeSpawnDirections();


		bool checkAdjRulesForTerrainStruct( const std::string& terrainTypeName, const TerrainStruct* terrainStruct, Map* map, int tileIndex );

		struct MapVertex
		{
			MapVertex( const vec3f& pos, const Color4f& color, const vec2f& texCoords )
				:	pos( pos )
				,	color( color )
				,	texCoords( texCoords )
			{}
			vec3f pos;
			Color4f color;
			vec2f texCoords;
		};



	private:
		std::string m_name;
		TerrainType* m_defaultTerrain;
		TerrainType* m_borderTerrain;
		TerrainType* m_playerSpawnTerrain;
		
		std::vector< TerrainStructType > m_terrainStructTypes;
		std::vector< Blobs > m_blobs;
		std::vector< Streams > m_streams;
		std::vector< Mutator > m_mutators;

		

		RandItemPool<vec2i> m_cardinalPool;
		Texture* m_textureMap;
		int m_tileWidth;
		int m_tileHeight;
		float m_texCoordHeight;
		float m_texCoordWidth;

		static std::map< std::string, MapTypeBlueprint* > sm_blueprints;

		static std::vector< vec2f > sm_spawnDirections;
	};
}