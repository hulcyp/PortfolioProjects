//-----------------------------------------------------------------------------------------------
// Common.hpp
//
// Commonly included engine helper/utility headers that we'll want from most everywhere.
//-----------------------------------------------------------------------------------------------
#ifndef __include_Common__
#define __include_Common__

#include "Shared.hpp"
#include "AABB2.hpp"
#include "Vector2.hpp"
#include "HashedCaseInsensitiveString.hpp"
#include "Utilities.hpp"
#include "Clock.hpp"
#include "Rgba.hpp"
#include "ResourceStream.hpp"
#include "ParsingSupport.hpp"
#include "Graphics.hpp"


//-----------------------------------------------------------------------------------------------
// Global variables
extern bool g_keyDownStates[ 256 ];


__declspec(noreturn) void FatalError( const std::string& messageTitle, const std::string& messageText );


//-----------------------------------------------------------------------------------------------
inline bool IsKeyDown( unsigned char keyCode )
{
	return g_keyDownStates[ keyCode ];
}


#endif // __include_Common__
