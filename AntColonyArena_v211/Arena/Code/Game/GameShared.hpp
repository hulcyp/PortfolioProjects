//-----------------------------------------------------------------------------------------------
// GameShared.hpp
//
// Stuff that is globally visible throughout game code.
//-----------------------------------------------------------------------------------------------
#ifndef __include_GameShared__
#define __include_GameShared__

#include "../Engine/Common.hpp"

#define ARENA // excludes certain DLL-only portions of the CommonInterface from us
#include "../../include/CommonInterface.hpp"


//-----------------------------------------------------------------------------------------------
// Forward declarations
//
class TheGame;
class PluginManager;
class ArenaManager;
class Arena;
class Agent;
class Player;


//-----------------------------------------------------------------------------------------------
// Global variables
//
extern TheGame* theGame;
extern PluginManager* thePluginManager;
extern ArenaManager* theArenaManager;
extern const double MOVE_ANIMATION_SECONDS;
extern const double QUEEN_SCORECARD_DAMAGE_FLASH_SECONDS;
extern const double QUEEN_DAMAGE_FLASH_SECONDS;
extern const double QUEEN_ATE_FLASH_SECONDS;


//-----------------------------------------------------------------------------------------------
// Utility functions
//
inline IntVector2 GetDisplacementForCardinalDirection( CardinalDirection direction )
{
	switch( direction )
	{
		case CARDINAL_DIRECTION_EAST:
			return IntVector2( 1, 0 );
		case CARDINAL_DIRECTION_SOUTH:
			return IntVector2( 0, 1 );
		case CARDINAL_DIRECTION_WEST:
			return IntVector2( -1, 0 );
		case CARDINAL_DIRECTION_NORTH:
			return IntVector2( 0, -1 );
		case CARDINAL_DIRECTION_NONE:
		default:
			return IntVector2::ZERO;
	}
}


#endif // __include_GameShared__
