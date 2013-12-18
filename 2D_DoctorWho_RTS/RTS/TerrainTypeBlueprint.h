#pragma once
#include <XMLParser.h>
#include <CommonEngineIncludes.h>
#include <map>
#include <string>
#include "Map.h"

namespace pdh
{
	class TerrainType;
	class Texture;
	class Mesh;

	class TerrainTypeBlueprint
	{
	public:
		

		TerrainTypeBlueprint( XMLParser& parser, const XMLNode* terrainBlueprintNode );
		~TerrainTypeBlueprint();

		struct Resource
		{
			int amount;
			int maxCollectPerTrip;
			float timePerResource;
			Map::ResourceType resourceType;
			Mesh* mesh;
		};

		TerrainType* createTerrain();
		const std::string& getName() { return m_name; }
		//Texture* getTexture() const { return m_texture; }
		

		static TerrainTypeBlueprint* getBlueprint( const std::string& name );

	protected:
		void registerBP( const std::string& name );

		void generateResourceMesh();

	private:
		std::string m_name;
		Color4f m_color;
		Resource* m_resource;
		vec2i m_tileCoords;

		static std::map< std::string, TerrainTypeBlueprint* > sm_blueprints;
	};
}