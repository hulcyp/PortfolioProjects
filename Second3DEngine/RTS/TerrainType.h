#pragma once
#include <string>
#include <CommonEngineIncludes.h>
#include "Map.h"

namespace pdh
{
	class TerrainTypeBlueprint;
	class Mesh;

	class TerrainType
	{
	public:
		friend TerrainTypeBlueprint;

		struct Resource
		{
			int amount;
			int maxCollectPerTrip;
			float timePerResource;
			Map::ResourceType resourceType;
		};

		TerrainType( const std::string& name );
		~TerrainType();

		const std::string& getName() const { return m_name; }
		const Resource& getResourceInfo() const { return m_resource; }
		const Color4f& getColor() const { return m_color; }
		const vec2i& getTexCoords() const { return m_tileCoords; }

	private:
		const std::string& m_name;
		Color4f m_color;
		vec2i m_tileCoords;
		Resource m_resource;
	};
}