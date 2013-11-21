#include "BlobBrush.h"
#include "TerrainType.h"
#include <RandNumGen.h>

namespace pdh
{
	BlobBrush::BlobBrush( const std::string& name, float radius, float density, float roundness )
		:	Brush( name )
		,	m_radius( radius )
		,	m_density( density )
		,	m_roundness( roundness )
	{}
	//--------------------------------------------------------
	BlobBrush::~BlobBrush()
	{}
	//--------------------------------------------------------
	void BlobBrush::draw( Map* map, const TerrainType* type, const TileCoords& loc, 
			const std::vector< std::string >& canOverrideRules, const std::vector< std::string >& cannotOverrideRules )
	{
		int yr = (int)( loc.y - m_radius >= 0 ? loc.y - m_radius : 0 );
		for( ; yr <= loc.y + m_radius && yr < map->m_height; ++yr )
		{
			int xr = (int)( loc.x - m_radius >= 0 ? loc.x - m_radius : 0 );
			for( ; xr <= loc.x + m_radius && xr < map->m_width; ++xr )
			{
				int dx = abs( loc.x - xr );
				int dy = abs( loc.y - yr );
				float roundness = RandNumGen::randInRangeFloat( m_roundness, 1.0f );
				if( dx*dx + dy*dy < m_radius*m_radius*roundness*roundness )
				{
					float d = RandNumGen::randInRangeUnit();
					if( d < m_density )
					{
						int ir = map->getTileIndexFromTileCoords( TileCoords( xr, yr ) );
						if( ir != -1 )
						{
							bool canOverride = true;
							if( canOverrideRules.size() > 0 )
							{
								canOverride = false;
								const TerrainType* type = map->m_tiles[ir].m_terrainType;
								int index = findStringInList( type->getName(), canOverrideRules);
								if( index != -1 )
								{
									canOverride = true;
								}								
							}
							if( cannotOverrideRules.size() > 0 && canOverride )
							{
								const TerrainType* type = map->m_tiles[ir].m_terrainType;
								int index = findStringInList( type->getName(), cannotOverrideRules );
								if( index != -1 )
								{
									canOverride = false;
								}
							}

							

							if( map->m_tiles[ir].m_resourceType == Map::NONE && canOverride )
							{
								map->m_tiles[ir].m_terrainType = type;
								map->m_tiles[ir].m_resourceAmount = type->getResourceInfo().amount;
								map->m_tiles[ir].m_resourceType = type->getResourceInfo().resourceType;
							}
						}
					}
				}
			}
		}		
	}
}