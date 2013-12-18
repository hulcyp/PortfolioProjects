#include "CommonEngineIncludes.h"
#include "ConsoleSystem.h"
#include <stdio.h>
#include "GDOManager.h"
#include "ActorManager.h"
#include "GDOPoint.h"
#include "GDOActor.h"
#include "GDOSphere.h"
#include "GDOOrientation.h"
#include "GDOArrow.h"
#include "GDOText.h"

namespace pdh
{
	ConsoleSystem * g_ConsoleSystem = nullptr;
	GDOManager* g_GDOManager = nullptr; 

	void createActorGDOCmd( const std::string& args );
	void listActorsCmd( const std::string& args );
	void clearAllGDOsCmd( const std::string& args );
	void createTextGDOCmd( const std::string& args );

	ConsoleSystem * initConsole( int width, int height, int bufferSize, Renderer* renderer )
	{
		if( !g_ConsoleSystem )
		{
			g_ConsoleSystem = new ConsoleSystem( width, height, bufferSize, renderer );
			g_ConsoleSystem->registerCommand( "tagactor", "Used to tag an actor for debug", createActorGDOCmd );
			g_ConsoleSystem->registerCommand( "listactors", "Lists all actors currently in the level", listActorsCmd );
			g_ConsoleSystem->registerCommand( "clearGDOs", "Clears all active GDOs", clearAllGDOsCmd );
			g_ConsoleSystem->registerCommand( "tagactortext", "Tag an actor to display text info", createTextGDOCmd );
			return g_ConsoleSystem;
		}
		return nullptr;
	}

	void consolePrintf( const char* format, ... )
	{
		va_list args;
		va_start( args, format );
		vConsolePrintf( format, color::WHITE, args );
	}

	void consolePrintColorf( const char* format, Color4f color, ... )
	{
		va_list args;
		va_start( args, color );
		vConsolePrintf( format, color, args );
	}

	void vConsolePrintf( const char* format, Color4f color, va_list args )
	{
		char text[256];
		vsprintf_s( text, format, args );
		va_end( args );
		if( g_ConsoleSystem )
		{
			g_ConsoleSystem->printToConsole( text, color );
		}
	}

	void executeConsoleCommand( const std::string& cmd, const std::string& args )
	{
		g_ConsoleSystem->executeCommand( cmd, args );
	}

	void registerConsoleCommand( const std::string& cmd, const std::string& helpText, std::function< void( const std::string& ) > cmdFunc )
	{
		g_ConsoleSystem->registerCommand( cmd, helpText, cmdFunc );
	}
	
	GDOManager* initGDOManager( ActorManager* aManager )
	{
		if( !g_GDOManager )
		{
			g_GDOManager = new GDOManager( aManager );
			return g_GDOManager;
		}
		return nullptr;
	}

	void createPointGDO( float radius, const vec3f& pos, float showUntilTime, Color4f color )
	{
		g_GDOManager->addGDO( new GDOPoint( radius, pos, showUntilTime, color ) );
	}

	void createSphereGDO( float radius, int rings, int sectors, const vec3f& pos, float showUntilTime, Color4f color )
	{
		g_GDOManager->addGDO( new GDOSphere( radius, rings, sectors, pos, showUntilTime, color ) );
	}

	void createOrientationGDO( float size, const vec3f& pos, const mat3f& rotation, Color4f color, float showUntilTime )
	{
		g_GDOManager->addGDO( new GDOOrientation( size, pos, rotation, color, showUntilTime ) );
	}

	void createOrientationGDO( float size, const vec3f& pos, const mat3f& rotation, float showUntilTime )
	{
		g_GDOManager->addGDO( new GDOOrientation( size, pos, rotation, showUntilTime ) );
	}

	void createArrowGDO( const vec3f& dir, float length, const vec3f& pos, float showUntilTime )
	{
		g_GDOManager->addGDO( new GDOArrow( dir, length, pos, showUntilTime ) );
	}

	void createActorGDO( const std::string& id, float showUntilTime, Color4f color )
	{
		Actor* actor = g_GDOManager->getActorManager()->getActor( id );
		if( actor )
		{
			g_GDOManager->addGDO( new GDOActor( actor, showUntilTime, "DebugMaterial", color ) );
		}
	}

	void createTextGDO( const std::string& actorID, const std::string& type, float showUntilTime, Color4f color, float fontHeight )
	{
		Actor* actor = g_GDOManager->getActorManager()->getActor( actorID );
		if( actor )
		{
			g_GDOManager->addGDO( new GDOText( actor, type, showUntilTime, color, fontHeight ) );
		}
	}

	void createActorGDOCmd( const std::string& args )
	{
		std::vector< std::string > tokens;
		stringTokenizer( args, tokens, " " );
		Actor* actor = g_GDOManager->getActorManager()->getActor( tokens[0] );
		if( actor )
		{
			float showUntilTime = 0.0f; 
			if( tokens.size() > 1 )
			{
				if( stringToType( tokens[1], &showUntilTime ) )
				{
					float time = showUntilTime ? g_GDOManager->getCurrentTime() + showUntilTime : 0.0f;
					g_GDOManager->addGDO( new GDOActor( actor, time, "DebugMaterial" ) );
				}
				else
				{
					consolePrintf( "The value %s is not a valid value please see usage.", color::RED, tokens[1].c_str() );	
				}
			}
			else
			{
				g_GDOManager->addGDO( new GDOActor( actor, showUntilTime, "DebugMaterial" ) );
			}
		}
	}

	void listActorsCmd( const std::string& args )
	{
		g_GDOManager->getActorManager()->listActors();
	}

	void clearAllGDOsCmd( const std::string& args )
	{
		g_GDOManager->clearAllGDOs();
	}

	void createTextGDOCmd( const std::string& args )
	{
		std::vector< std::string > tokens;
		stringTokenizer( args, tokens, " " );
		Actor* actor = g_GDOManager->getActorManager()->getActor( tokens[0] );
		if( actor )
		{
			float showUntilTime = 0.0f;
			if( tokens.size() > 1 )
			{
				std::string type = tokens[1];
				if( tokens.size() > 2 )
				{
					if( stringToType( tokens[2], &showUntilTime ) )
					{
						showUntilTime += g_GDOManager->getCurrentTime();
					}
					else
					{
						consolePrintf( "The value %s is not a valid value please see usage.", color::RED, tokens[2].c_str() );	
					}
				}
				g_GDOManager->addGDO( new GDOText( actor, type, showUntilTime ) );
			}
			else
			{
				consolePrintf( "Not enough arguments, see usage", color::RED );
			}
			
		}
	}
}