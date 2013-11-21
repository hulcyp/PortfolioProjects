#pragma once
#include <CommonEngineIncludes.h>
#include <string>
#include "Map.h"
#include <vector>

namespace pdh
{
	class TerrainType;

	class Brush
	{
	public:
		Brush( const std::string& name );
		virtual ~Brush();

		virtual void draw( Map* map, const TerrainType* type, const TileCoords& loc, 
			const std::vector< std::string >& canOverideRules, const std::vector< std::string >& cannotOverrideRules ) = 0;

		const std::string& getName() { return m_name; }

	private:
		std::string m_name;
	};
}