#include "ArenaMap.h"
#include <cmath>
#include "MathFuncs.h"
#include <algorithm>
#include <gl/GL.h>
#include <gl/GLU.h>


ArenaMap::MapTile::MapTile()
	:	lastAIPathClosedOn( -1 )
	,	lastAIPathOpenedOn( -1 )
	,	gCostToEnterTile( 1.5f )
	,	gCostToEnterTileForScout( 1.5f )
	,	gCostToEnterTileForWorker( 1.5f )
	,	gCostToEnterTileForSoldier( 1.5f )
	,	gCostToEnterTileForQueen( 1.5f )
	,	isClaimed( false )
	,	numAgentsKilledInTile( 0 )
	,	enemyAgentsSeenInThisTile( 0 )
{}

ArenaMap::ArenaMap()
	:	m_width( MAX_ARENA_WIDTH )
	,	m_height( MAX_ARENA_HEIGHT )
	,	m_hasNewMapInfo( false )
	,	TOLERANCE_FOR_TILE_FRESHNESS( 50 )
	,	m_maxAgentKillCountInSingleTile( 0 )
	,	m_maxEnemySeenCountInSingleTile( 0 )
{
	init();
}
//------------------------------------------------------------
void ArenaMap::init()
{
	m_currentTurn = 0;
	for( int  y = 0; y < m_height; ++y )
	{
		for( int x = 0; x < m_width; ++x )
		{
			int index = x + ( y * m_width );
			if( index >= 0 && index < ( m_width * m_height ) )
			{
				m_mapTiles[ index ].tileMaterial = (char)ARENA_SQUARE_TYPE_UNKNOWN;
				m_tilesToScout.addItem( TileCoords( x, y ) );
			}
		}
	}
}
//------------------------------------------------------------
void ArenaMap::updateMap( const ArenaSquares& currentViewOfArena, int currentTurn )
{
	m_width = currentViewOfArena.m_width;
	m_height = currentViewOfArena.m_height;
	
	m_hasNewMapInfo = false;
	m_hasNoMoreResources = true;
	m_currentTurn = currentTurn;
	m_tilesToScout.clearPool();
	for( int  y = 0; y < m_height; ++y )
	{
		for( int x = 0; x < m_width; ++x )
		{
			int index = x + ( y * m_width );
			if( index >= 0 && index < ( m_width * m_height ) )
			{
				if( currentViewOfArena.m_arenaSquareMaterials[ index ] != ARENA_SQUARE_TYPE_UNKNOWN )
				{
					if( m_mapTiles[ index ].tileMaterial != (char)ARENA_SQUARE_TYPE_STONE )
					{
						if( currentViewOfArena.m_arenaSquareMaterials[ index ] == ARENA_SQUARE_TYPE_STONE )
							m_hasNewMapInfo = true;	
						if( m_mapTiles[ index ].tileMaterial == ARENA_SQUARE_TYPE_FOOD || currentViewOfArena.m_arenaSquareMaterials[ index ]  == ARENA_SQUARE_TYPE_FOOD )
							m_hasNoMoreResources = false;
							
						m_mapTiles[ index ].isClaimed = false;

						m_mapTiles[ index ].tileMaterial = (char)currentViewOfArena.m_arenaSquareMaterials[ index ];
						m_mapTiles[ index ].gCostToEnterTileForQueen = getGCostToEnterTileForEntityType( (ArenaSquareType)m_mapTiles[ index ].tileMaterial, ENTITY_TYPE_QUEEN );
						m_mapTiles[ index ].gCostToEnterTileForScout = getGCostToEnterTileForEntityType( (ArenaSquareType)m_mapTiles[ index ].tileMaterial, ENTITY_TYPE_SCOUT );
						m_mapTiles[ index ].gCostToEnterTileForSoldier = getGCostToEnterTileForEntityType( (ArenaSquareType)m_mapTiles[ index ].tileMaterial, ENTITY_TYPE_SOLDIER );
						m_mapTiles[ index ].gCostToEnterTileForWorker = getGCostToEnterTileForEntityType( (ArenaSquareType)m_mapTiles[ index ].tileMaterial, ENTITY_TYPE_WORKER );
					}
					m_mapTiles[ index ].lastTurnUpdated = m_currentTurn;
				}
				else if( m_currentTurn - m_mapTiles[ index ].lastTurnUpdated > TOLERANCE_FOR_TILE_FRESHNESS )
					m_tilesToScout.addItem( TileCoords( x, y ) );
			}
		}
	}
}
//------------------------------------------------------------
void ArenaMap::updateEnemySightingsHeatMap( const ObservedEntities& entities )
{
	for( int i = 0; i < entities.m_numberOfObservedEntities; ++i )
	{
		TileCoords entityLoc = TileCoords( entities.m_entities[i].m_positionX, entities.m_entities[i].m_positionY );
		MapTile& tile = getMapTile( entityLoc.x, entityLoc.y );
		++tile.enemyAgentsSeenInThisTile;
		if( tile.enemyAgentsSeenInThisTile > m_maxEnemySeenCountInSingleTile )
			m_maxEnemySeenCountInSingleTile = tile.enemyAgentsSeenInThisTile;
	}
}
//------------------------------------------------------------
char ArenaMap::getMapTileType( int x, int y )
{
	int index = x + ( y * m_width );
	if( isInMap( x, y ) )
		return m_mapTiles[ index ].tileMaterial;
	else
		return ARENA_SQUARE_TYPE_UNKNOWN;
}
char ArenaMap::getMapTileType( int x, int y ) const
{
	int index = x + ( y * m_width );
	if( isInMap( x, y ) )
		return m_mapTiles[ index ].tileMaterial;
	else
		return ARENA_SQUARE_TYPE_UNKNOWN;
}
//------------------------------------------------------------
ArenaMap::MapTile& ArenaMap::getMapTile( int x, int y )
{
	int index = x + ( y * m_width );
	_ASSERT( isInMap( x, y ) );		
	return m_mapTiles[ index ];
}
const ArenaMap::MapTile& ArenaMap::getMapTile( int x, int y ) const
{
	int index = x + ( y * m_width );
	_ASSERT( isInMap( x, y ) );		
	return m_mapTiles[ index ];
}
//------------------------------------------------------------
int ArenaMap::getMapWidth() const
{
	return m_width;
}
//------------------------------------------------------------
int ArenaMap::getMapHeight() const
{
	return m_height;
}
//------------------------------------------------------------
bool ArenaMap::isInMap( int x, int y ) const
{
	return  (	x >= 0 && x < m_width &&
				y >= 0 && y < m_height );
}
//------------------------------------------------------------
void ArenaMap::setArenaInfo( const ArenaInfo& arenaInfo )
{
	m_arenaInfo = arenaInfo;
}
//------------------------------------------------------------
const ArenaInfo& ArenaMap::getArenaInfo() const
{
	return m_arenaInfo;
}
//------------------------------------------------------------
float ArenaMap::getGCostToEnterTile( EntityType entityType, int x, int y ) const
{
	MapTile tile = getMapTile( x, y );
	float gCost = tile.gCostToEnterTile;
	float battleCost = 0.0f;
	float enemySightingCost = 0.0f;
	if( m_maxAgentKillCountInSingleTile > 0 )
		battleCost = (float)tile.numAgentsKilledInTile / m_maxAgentKillCountInSingleTile;
	if( m_maxEnemySeenCountInSingleTile > 0 )
		enemySightingCost = (float)tile.numAgentsKilledInTile / m_maxEnemySeenCountInSingleTile;
	switch( entityType )
	{
	case ENTITY_TYPE_QUEEN:
		gCost = tile.gCostToEnterTileForQueen + 10.0f*battleCost + 10.0f*enemySightingCost;
		break;
	case ENTITY_TYPE_SCOUT:
		if( m_currentTurn > 0 )
			gCost = tile.gCostToEnterTileForScout - ( 1.0f - tile.lastTurnUpdated / m_currentTurn ) + battleCost;
		break;
	case ENTITY_TYPE_SOLDIER:
		gCost = tile.gCostToEnterTileForSoldier - battleCost - enemySightingCost;
		break;
	case ENTITY_TYPE_WORKER:
		gCost = tile.gCostToEnterTileForWorker + battleCost + enemySightingCost;
		break;
	}
	return gCost;
}
//------------------------------------------------------------
float ArenaMap::getCurrentGValueForTile( int x, int y ) const
{
	return getMapTile( x, y ).currentGCost;
}
//------------------------------------------------------------
void ArenaMap::setCurrentGValueForTile( int x, int y, float g )
{
	getMapTile( x, y ).currentGCost = g;
}
//------------------------------------------------------------
int ArenaMap::getLastAIPathClosedOn( int x, int y ) const
{
	return getMapTile( x, y ).lastAIPathClosedOn;
}
//------------------------------------------------------------
void ArenaMap::setLastAIPathClosedOn( int x, int y, int lastAIPath )
{
	getMapTile( x, y ).lastAIPathClosedOn = lastAIPath;
}
//------------------------------------------------------------
int ArenaMap::getLastAIPathOpenedOn( int x, int y ) const
{
	return getMapTile( x, y ).lastAIPathOpenedOn;
}
//------------------------------------------------------------
void ArenaMap::setLastAIPathOpenedOn( int x, int y, int lastAIPath )
{
	getMapTile( x, y ).lastAIPathOpenedOn = lastAIPath;
}
//------------------------------------------------------------
float ArenaMap::getCurrentFValue( int x, int y ) const
{
	return getMapTile( x, y ).currentF;
}
//------------------------------------------------------------
void ArenaMap::setCurrentFValue( int x, int y, float f )
{
	getMapTile( x, y ).currentF = f;
}
//------------------------------------------------------------
int ArenaMap::getHCost( int currentX, int currentY, int targetX, int targetY )
{
	return std::abs( targetX - currentX ) + std::abs( targetY - currentY );
}
//------------------------------------------------------------
bool ArenaMap::canMoveOnTile( int x, int y )
{
	return ( getMapTileType( x, y ) != (char)ARENA_SQUARE_TYPE_STONE );
}
//------------------------------------------------------------
bool ArenaMap::canMoveOnTile( int x, int y ) const
{
	return ( getMapTileType( x, y ) != (char)ARENA_SQUARE_TYPE_STONE );
}
//------------------------------------------------------------
void ArenaMap::setMoveOrderToGetToTileForCurrentPath( int x, int y, OrderCode order )
{
	getMapTile( x, y ).moveOrderToGetToTileForCurrentPath = order;	
}
//------------------------------------------------------------
OrderCode ArenaMap::getMoveOrderToGetToTileForCurentPath( int x, int y ) const
{
	return getMapTile( x, y ).moveOrderToGetToTileForCurrentPath;
}
//------------------------------------------------------------
void ArenaMap::addAgentKilledToTile( int x, int y )
{
	MapTile tile = getMapTile( x, y );
	++tile.numAgentsKilledInTile;
	if( tile.numAgentsKilledInTile > m_maxAgentKillCountInSingleTile )
		m_maxAgentKillCountInSingleTile = tile.numAgentsKilledInTile;
}
//------------------------------------------------------------
bool ArenaMap::getClosestNutrient( int startingRadius, const TileCoords& current, int& nutrientX, int& nutrientY ) const
{
	bool found = false;
	TileCoords nutrientLoc( -1, -1 );
	found = searchInDirectionForNutrient( current, TileCoords( 1, 0 ), startingRadius, nutrientLoc );

	if( !found )	
		found = searchInDirectionForNutrient( current, TileCoords( -1, 0 ), startingRadius, nutrientLoc );

	TileCoords hCircleDir = TileCoords( 0, 1 );
	--startingRadius;
	if( !found )
		found = searchHalfCircleUsingManhattenDistForNutrient( current + hCircleDir, hCircleDir, startingRadius, nutrientLoc );
			
	if( !found )
		found = searchHalfCircleUsingManhattenDistForNutrient( current - hCircleDir, -hCircleDir, startingRadius, nutrientLoc );

	nutrientX = nutrientLoc.x;
	nutrientY = nutrientLoc.y;

	return found;
}
//------------------------------------------------------------
void ArenaMap::requestAStarPath( EntityType entityType, const TileCoords& start, TileCoords& goal, std::vector< OrderCode >& pathOut )
{
	m_aStarPathGen.getAStarPathToGoal( *this, entityType, start, goal, pathOut );
}
//----------------------------------------------------------------------
float ArenaMap::getGCostToEnterTileForEntityType( ArenaSquareType squareType, EntityType entityType ) const
{
	float gCost = 1.0f;
	switch( squareType )
	{
	case ARENA_SQUARE_TYPE_AIR:
		break;
	case ARENA_SQUARE_TYPE_DIRT:
		switch( entityType )
		{
		case ENTITY_TYPE_SOLDIER:		
		case ENTITY_TYPE_WORKER:
		case ENTITY_TYPE_QUEEN:
			gCost = 2.0f;
			break;
		}
		break;
	case ARENA_SQUARE_TYPE_STONE:
		break;
	case ARENA_SQUARE_TYPE_FOOD:
		break;
	case ARENA_SQUARE_TYPE_UNKNOWN:
		if( entityType == ENTITY_TYPE_SCOUT )
			gCost = 0.8f;
		else
		{
			gCost = 1.5f;
		}
		break;
	}
	return gCost;
}
//----------------------------------------------------------------------
#define MONKY_FRESHNESS_MAP
void ArenaMap::renderMapOverlay( unsigned int color, short inspectSquareX, short inspectSquareY ) const
{
	unsigned char r = (unsigned char)( ( color & 0xff000000 ) >> 24 );
	unsigned char g = (unsigned char)( ( color & 0x00ff0000 ) >> 16 );
	unsigned char b = (unsigned char)( ( color & 0x0000ff00 ) >> 8 );

	for( int y = 0; y < m_height; ++y )
	{
		for( int x = 0; x < m_width; ++x )
		{	
			float alpha = 1.0f;
#ifdef MONKY_FRESHNESS_MAP
			alpha = 1.0f - (float)getMapTile( x, y ).lastTurnUpdated / (float)m_currentTurn;
#endif
#ifdef MONKY_DEATH_MAP
			alpha = 1.0f - (float)getMapTile( x, y ).numAgentsKilledInTile / (float)m_maxAgentKillCountInSingleTile;
#endif
#ifdef MONKY_ENEMY_SEEN_MAP
			alpha = 1.0f - (float)getMapTile( x, y ).enemyAgentsSeenInThisTile / (float)m_maxEnemySeenCountInSingleTile;
#endif
			glColor4f( r/255.0f, g/255.0f, b/255.0f, alpha );
			if( alpha < 1.0f )
				int x = 5 + 3;
			glBegin( GL_QUADS );
			{
				glVertex2f( x - 0.5f, y - 0.5f );
				glVertex2f( x - 0.5f, y + 0.5f );
				glVertex2f( x + 0.5f, y + 0.5f );
				glVertex2f( x + 0.5f, y - 0.5f );
			}
			glEnd();
			
			if( getMapTile( x, y ).isClaimed )
			{
				glColor4f( 0, 0, 1.0f, 1.0f );
				glBegin( GL_LINE_LOOP );
				{
					glVertex2f( x - 0.5f, y - 0.5f );
					glVertex2f( x - 0.5f, y + 0.5f );
					glVertex2f( x + 0.5f, y + 0.5f );
					glVertex2f( x + 0.5f, y - 0.5f );
					glVertex2f( x - 0.5f, y - 0.5f );
				}
				glEnd();
			}
		}
	}
}
//----------------------------------------------------------------------
bool ArenaMap::shouldMoveSucceed( EntityType entityType, OrderCode orderCode, const TileCoords& currentLoc ) const
{
	TileCoords newTile = currentLoc + AStarPathGenerator::getTileCoordsDirFromOrderCode( orderCode );
	bool shouldSucceed = true;
	if( isInMap( newTile.x, newTile.y ) )
	{
		EntityType tileType = (EntityType)getMapTileType( newTile.x, newTile.y );
		if( entityType == ENTITY_TYPE_SOLDIER || entityType == ENTITY_TYPE_WORKER )
		{
			shouldSucceed = ( tileType != ARENA_SQUARE_TYPE_DIRT && tileType != ARENA_SQUARE_TYPE_STONE );
		}
		else if( entityType == ENTITY_TYPE_SCOUT )
		{
			shouldSucceed = tileType != ARENA_SQUARE_TYPE_STONE;					
		}
	}
	return shouldSucceed;
}
//----------------------------------------------------------------------
TileCoords ArenaMap::getLocationForScoutToGoTo() const
{
	return m_tilesToScout.getRandItem();
}
//----------------------------------------------------------------------
TileCoords ArenaMap::getRandomLocationInArea( const TileCoords& pos, int radius ) const
{
	TileCoords minXY( pos.x - radius, pos.y - radius );
	TileCoords maxXY( pos.x + radius, pos.y + radius );
	minXY.x = Monky::MathFuncs<int>::clamp( minXY.x, 0, m_arenaInfo.width - 1 );
	minXY.y = Monky::MathFuncs<int>::clamp( minXY.y, 0, m_arenaInfo.height - 1 );
	maxXY.x = Monky::MathFuncs<int>::clamp( maxXY.x, 0, m_arenaInfo.width - 1 );
	maxXY.y = Monky::MathFuncs<int>::clamp( maxXY.y, 0, m_arenaInfo.height - 1 );

	return TileCoords( Monky::RandNumGen::randInRangeInt( minXY.x, maxXY.x ), Monky::RandNumGen::randInRangeInt( minXY.y, maxXY.y ) ); 
	
}
//----------------------------------------------------------------------
bool ArenaMap::searchHalfCircleUsingManhattenDistForNutrient( const TileCoords& pos, const TileCoords& dir, int radius, TileCoords& nutrientLoc ) const
{
	TileCoords currentPos = pos;
	int currRadius = radius;
	bool found = false;
	for( int i = 0; i < radius && !found; ++i )
	{
		found = searchInDirectionForNutrient( currentPos, TileCoords( 1, 0 ), currRadius, nutrientLoc );
		if( !found )
			found = searchInDirectionForNutrient( currentPos, TileCoords( -1, 0 ), currRadius, nutrientLoc );
		currentPos += dir;
		--currRadius;
	}
	return found;
}
//----------------------------------------------------------------------
bool ArenaMap::searchInDirectionForNutrient( const TileCoords& pos, const TileCoords& dir, int radius, TileCoords& nutrientLoc ) const
{
	TileCoords currPos = pos;
	bool found = false;
	for( int i = 0; i < radius && !found; ++i )
	{
		found = checkTileForNutrient( currPos );
		if( found )
			nutrientLoc = currPos;
		currPos += dir;
	}
	return found;
}
//----------------------------------------------------------------------
bool ArenaMap::checkTileForNutrient( const TileCoords& pos ) const
{
	bool found = false;
	if( isInMap( pos.x, pos.y ) )
	{
		const MapTile& tile = getMapTile( pos.x, pos.y );
		if( tile.tileMaterial == ARENA_SQUARE_TYPE_FOOD && !tile.isClaimed )
		{
			found = true;
		}
	}
	return found;
}
