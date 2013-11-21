#pragma once
#include "Brush.h"
#include "MapTypeBlueprint.h"

namespace pdh
{
	class BlobBrush : public Brush
	{
	public:
		BlobBrush( const std::string& name, float radius, float density, float roundness );
		virtual ~BlobBrush();

		virtual void draw( Map* map, const TerrainType* type, const TileCoords& loc, 
			const std::vector< std::string >& canOverrideRules, const std::vector< std::string >& cannotOverrideRules );

		void setRadius( float radius ) { m_radius = radius; }
		void setDensity( float density ) { m_density = density; }
		void setRoundness( float roundness ) { m_roundness = roundness; }


	private:
		float m_radius;
		float m_density;
		float m_roundness;
	};
}
