#pragma once
#include "RTSGame.h"
#include <Mesh.h>
#include <Renderer.h>
#include <Material.h>
#include <CommonEngineIncludes.h>
#include <Model.h>
#include <ShaderProgram.h>
#include <Font.h>
#include <XMLParser.h>
#include "UnitBlueprint.h"
#include "BuildingBlueprint.h"
#include "TechnologyBlueprint.h"
#include "TerrainTypeBlueprint.h"
#include "Unit.h"
#include "Technology.h"
#include "TerrainType.h"
#include "Building.h"
#include "MapTypeBlueprint.h"
#include "Map.h"
#include "RTSWorld.h"
#include "Building.h"
#include "RTSDebuggingStats.h"

namespace pdh
{
	RTSGame::RTSGame( double deltaTime, int width, int height, float fov,  const char* windowTitle )
		:	GameWorld( deltaTime, width, height, fov, windowTitle )
		,	m_currentMapWidth( 100 )
		,	m_currentMapHeight( 100 )
		,	m_currentNumPlayers( 8 )
		,	m_currentMapType( "Plains" )
		,	m_currentWorld( nullptr )
	{
		m_freeMouse = true;
		loadDataFiles();
		loadWorld( 1, 3, m_currentMapType );
		
		registerConsoleCommand( "event", "Fire event. Usage \"event [eventName]\"", 
			[&]( const std::string& args ) 
			{
				fireEvent( args ); 
			} );

		registerConsoleCommand( "World", "Create new world. Usage \"world [map type] [width] [height] [optional numPlayers]", 
			[&]( const std::string& args ) 
			{
				std::vector< std::string > tokens;
				stringTokenizer( args, tokens, " " );
				if( tokens.size() < 3 )
				{
					consolePrintColorf( "Not enough arguments, see usage", color::RED );
					return;
				}

				int width = 0;
				int height = 0;

				if( !stringToType( tokens[1], &width ) )
				{
					consolePrintColorf( "Invalid width: %s", color::RED, tokens[0].c_str() );
					return;
				}
				if( !stringToType( tokens[2], &height ) )
				{
					consolePrintColorf( "Invalid height: %s", color::RED, tokens[1].c_str() );
					return;
				}
				if( tokens.size() >= 4 )
				{
					int numPlayers = 0;
					if( !stringToType( tokens[3], &numPlayers ) )
					{
						consolePrintColorf( "Invalid Number of Players: %s", color::RED, tokens[3].c_str() );
						return;
					}	
					m_currentNumPlayers = numPlayers;
				}
				m_currentMapWidth = width;
				m_currentMapHeight = height;
				loadWorld( 1, m_currentNumPlayers - 1, tokens[0] );
			} );

		//m_renderer->pushProjectionMatrix( mat4f::ortho( 0.0f, (float)m_currentMapWidth*0.4f, (float)m_currentMapHeight*0.4f, 0.0f, 1.0f, -1.0f ) );
		GameWorld::executeStartUpCmds( "../data/xml/StartupCmds.xml" );
	}
	//----------------------------------------------------------------------------
	RTSGame::~RTSGame()
	{
		SAFE_DELETE( m_currentWorld );
	}
	//----------------------------------------------------------------------------
	bool RTSGame::onKeyDown( unsigned char keyCode, int x, int y )
	{
		bool used = false;
		used = GameWorld::onKeyDown( keyCode, x, y );
		return used;
	}
	//----------------------------------------------------------------------------
	bool RTSGame::onSpecialKeyDown( int keyCode, int x, int y )
	{
		bool used = false;
		used = GameWorld::onSpecialKeyDown( keyCode, x, y );
		if( !used )
		{
			switch( keyCode )
			{
			case GLUT_KEY_F5:
				loadDataFiles();
				loadWorld( 1, m_currentNumPlayers - 1, m_currentMapType );
				used = true;
				break;
			}
		}
		return used;
	}
	//----------------------------------------------------------------------------
	void RTSGame::loadWorld( int numHumans, int numAI, const std::string& mapType )
	{
		SAFE_DELETE( m_currentWorld ); 
		//Map* map = createMap( mapType );
		
		m_currentWorld = new RTSWorld( numAI, mapType, m_currentMapWidth, m_currentMapHeight, m_renderer );		
	}
	//----------------------------------------------------------------------------
	// Protected member functions
	//----------------------------------------------------------------------------
	void RTSGame::updateSimulation()
	{
		GameWorld::updateSimulation();
		if( m_currentWorld )
		{
			m_currentWorld->updateSimulation( static_cast<float>( DELTA_TIME ) );
		}
	}
	//----------------------------------------------------------------------------
	void RTSGame::updateDisplay()
	{		
		m_freeMouse = false;
		GameWorld::updateDisplay();
		m_renderer->renderAxis( 10.0f );
		
		m_renderer->setProjectionMode( Renderer::PH_ORTHOGRAPHIC );
		if( m_currentWorld )
		{
			m_currentWorld->updateDisplay();
		}
		m_renderer->setWireFrame( false );
		GameWorld::renderConsole();
		renderDebugHUD();
		glutSwapBuffers();
	}
	//----------------------------------------------------------------------------
	void RTSGame::renderDebugHUD()
	{
		m_renderer->pushCamera( m_debugCamera );
		GameWorld::renderDebugHUD();
		if( m_renderDebugHUD )
		{
			matStackf& modelStack = m_renderer->getModelStack();
			modelStack.push();
			modelStack.loadIdentity();
			m_font->renderText( "Current map seed: " + toString( m_currentWorld->getMap()->getSeed() ), vec3f( 0.0f, (float)m_screenHeight, 0.0f ), 24.0f, color::GREEN, Font::TOPLEFT );
			m_font->renderText( "A* Paths Gen: " + toString( RTSDebuggingStats::ASTAR_PATHS_GEN ), vec3f( (float)m_screenWidth, (float)( m_screenHeight - 48.0f ) ), 12.0f, color::WHITE, Font::TOPRIGHT );
			m_font->renderText( "A* Paths Gen time: " + toString( RTSDebuggingStats::ASTAR_PATH_GEN_TIME ), vec3f( (float)m_screenWidth, (float)( m_screenHeight - 60.0f ) ), 12.0f, color::WHITE, Font::TOPRIGHT );
			float avgPathGenTime = RTSDebuggingStats::ASTAR_PATH_GEN_TIME / (float)RTSDebuggingStats::ASTAR_PATHS_GEN;
			m_font->renderText( "Avg A* Path Gen time: " + toString( avgPathGenTime ), vec3f( (float)m_screenWidth, (float)( m_screenHeight - 72.0f ) ), 12.0f, color::WHITE, Font::TOPRIGHT );

			modelStack.pop();
		}
		m_renderer->popCamera();
	}
	//----------------------------------------------------------------------------
	void RTSGame::idle()
	{
		GameWorld::idle();
		//RTSDebuggingStats::clearStats();
	}
	//----------------------------------------------------------------------------
	void RTSGame::loadDataFiles()
	{
		loadUnitBP( "../data/xml/RTS/Units.xml" );
		loadTerrainBP( "../data/xml/RTS/Terrain.xml" );
		loadTechBP( "../data/xml/RTS/Technologies.xml" );
		loadBuildingBP( "../data/xml/RTS/Buildings.xml" );
		loadMapTypeBP( "../data/xml/RTS/MapTypes.xml" );
	}
	//-----------------------------------------------------------
	void RTSGame::loadUnitBP( const char* file )
	{
		XMLParser parser( file );
		XMLNode* root = parser.getDocument().FirstChildElement( "UnitTypes" );

		for( XMLNode* unit = root->FirstChildElement( "UnitType" ); unit != nullptr; unit = unit->NextSiblingElement( "UnitType" ) )
		{
			new UnitBlueprint( parser, unit );
		}
	}
	//-----------------------------------------------------------
	void RTSGame::loadTerrainBP( const char* file )
	{
		XMLParser parser( file );
		XMLNode* root = parser.getDocument().FirstChildElement( "TerrainTypes" );

		for( XMLNode* terrain = root->FirstChildElement( "TerrainType" ); terrain != nullptr; terrain = terrain->NextSiblingElement( "TerrainType" ) )
		{
			new TerrainTypeBlueprint( parser, terrain );
		}
	}
	//-----------------------------------------------------------
	void RTSGame::loadTechBP( const char* file )
	{
		XMLParser parser( file );
		XMLNode* root = parser.getDocument().FirstChildElement( "Technologies" );

		for( XMLNode* tech = root->FirstChildElement( "Technology" ); tech != nullptr; tech = tech->NextSiblingElement( "Technology" ) )
		{
			new TechnologyBlueprint( parser, tech );
		}
	}
	//-----------------------------------------------------------
	void RTSGame::loadBuildingBP( const char* file )
	{
		XMLParser parser( file );
		XMLNode* root = parser.getDocument().FirstChildElement( "BuildingTypes" );

		for( XMLNode* buidling = root->FirstChildElement( "BuildingType" ); buidling != nullptr; buidling = buidling->NextSiblingElement( "BuildingType" ) )
		{
			new BuildingBlueprint( parser, buidling );
		}
	}
	//-----------------------------------------------------------
	void RTSGame::loadMapTypeBP( const char* file )
	{
		XMLParser parser( file );
		XMLNode* root = parser.getDocument().FirstChildElement( "MapTypes" );

		for( XMLNode* map = root->FirstChildElement( "MapType" ); map != nullptr; map = map->NextSiblingElement( "MapType" ) )
		{
			m_mapTypes.push_back( (new MapTypeBlueprint( parser, map ))->getName() );
		}		
	}
	//-----------------------------------------------------------
	//Map* RTSGame::createMap( const std::string& mapType )
	//{
	//	MapTypeBlueprint* blueprint = MapTypeBlueprint::getBlueprint( mapType ); 
	//	Map* map;
	//	if( blueprint == nullptr )
	//	{
	//		consolePrintColorf( "Failed to create map. Invalid map type: %s", color::RED, mapType.c_str() );
	//		consolePrintColorf( "Valid map types:", color::RED );
	//		for( size_t i = 0; i < m_mapTypes.size(); ++i )
	//		{
	//			consolePrintColorf( "%s", color::RED, m_mapTypes[i].c_str() );
	//		}
	//		return nullptr;
	//	}
	//	map = blueprint->createMap( m_currentMapWidth, m_currentMapHeight, m_currentNumPlayers );
	//	m_currentMapType = mapType;
	//
	//	return map;
	//}
}