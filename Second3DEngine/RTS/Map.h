#pragma once
#include <string>
#include <vector>
#include <CommonEngineIncludes.h>

/*****************************
* -Note TileCoords correspond to the center of the tile in WorldCoords
******************************/

namespace pdh
{
	typedef vec2i TileCoords;
	typedef int TileIndex;
	typedef vec2f WorldCoords;
	typedef vec2f ScreenCoords;

	class MapTypeBlueprint;
	class Camera;
	class BlobBrush;
	class TerrainType;
	class RTSWorld;
	class Mesh;
	class AStarPathGenerator;

	class Map
	{
	public:
		friend MapTypeBlueprint;
		friend BlobBrush;
		
		enum ResourceType 
		{
			NONE,
			FISH_FINGERS,
			CUSTARD,
			TIME_ENERGY
		};

		enum CardinalDirections
		{
			NORTH,
			EAST,
			SOUTH,
			WEST
		};

		class MapTile
		{
		public:
			friend Map;
			friend BlobBrush;
			friend MapTypeBlueprint;
			friend AStarPathGenerator;

			MapTile();
			bool hasResource() const;
			int getResourceAmount() const;
			int getMaxCollectAmountPerTrip() const;
			int getH( const TileCoords& goal ) const;
			const TerrainType* getTerrainType() const;
			ResourceType getResourceType() const;
			const TileCoords& getTilecoords() const;
			
			int lastFrameUpdated;
						
		private:
			TileCoords m_tileCoords;
			const TerrainType* m_terrainType;
			aabb2f m_worldBounds;
			int m_resourceAmount;
			ResourceType m_resourceType;

			vec2i m_directionToGetToTileForCurrentPath;
			int m_GCost;
			int m_G;
			int m_F;
			int m_lastAIPathNumberClosedOn;
			int m_lastAIPathNumberOpenedOn;
			Map* m_map;
		};

		Map( RTSWorld* world );
		~Map();

		void initializeMap( const std::string& terrainType, int width, int height );

		TileIndex getTileIndexFromTileCoords( const TileCoords& tileCoords ) const;
		TileCoords getTileCoordsFromTileIndex( TileIndex index ) const;
		WorldCoords getWorldCoordsFromTileCoords( const TileCoords& tileCoords ) const;
		TileCoords getTileCoordsFromWorldCoords( const WorldCoords& worldCoords ) const;

		const MapTile& getMapTile( const TileCoords& tileCoords ) const;
		MapTile& getMapTile( const TileCoords& tileCoords );

		int collectResourceFromTile( MapTile& mapTile );
		TileCoords getNextClosestTileCoordsWithResource( const TileCoords& tileCoords, ResourceType type );


		unsigned int getSeed() const { return m_seed; }
		int getMapWidth() const { return m_width; }
		int getMapHeight() const { return m_height; }

		void render();
		void enableDebugRender( bool enabled );

		const TileCoords& getPlayerStartLocation( int index ) const;

		static ResourceType getResourceType( std::string type );

		static const vec2i CD_NORTH;
		static const vec2i CD_EAST;
		static const vec2i CD_SOUTH;
		static const vec2i CD_WEST;
		
		void changeTileBaseOnSurroundings( MapTile& mapTile );
		void changeTileType( MapTile& mapTile, const TerrainType* newType );

		void changeTileColor( int index, const Color4f& color );

		bool areTileCoordsInBounds( const TileCoords& tileCoords ) const;
	
	private:
		MapTypeBlueprint* m_blueprint;
		std::vector< MapTile > m_tiles;
		aabb2f m_worldBounds;
		int m_width;
		int m_height;
		std::vector< TileCoords > m_playerStartLocations;
		bool m_debugMode;

		TileCoords m_cardinalDirections[4];

		unsigned int m_seed;
		RTSWorld* m_world;

		Mesh* m_mapMesh;
		bool m_mapUpdatedThisFrame;
	};

}