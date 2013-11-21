#pragma once
#include <GameApp.h>
#include <vector>
#include <string>
#include <EventSystemInterface.h>

namespace pdh
{
	class Map;
	class RTSWorld;

	class RTSGame : public GameApp
	{
	public:
		RTSGame( double deltaTime, int width, int height, float fov,  const char* windowTitle );
		virtual ~RTSGame();

		virtual bool onKeyDown( unsigned char keyCode, int x, int y );
		virtual bool onSpecialKeyDown( int keyCode, int x, int y );

		void loadWorld( int numHumans, int numAI, const std::string& mapType );

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();
		virtual void renderDebugHUD();
		virtual void idle();

		void loadDataFiles();
		void loadUnitBP( const char* file );
		void loadTerrainBP( const char* file );
		void loadTechBP( const char* file );
		void loadBuildingBP( const char* file );
		void loadMapTypeBP( const char* file );

		//Map* createMap( const std::string& mapType );
		

	private:
		std::vector< std::string > m_mapTypes;
		int m_currentMapWidth;
		int m_currentMapHeight;
		int m_currentNumPlayers;
		std::string m_currentMapType;

		RTSWorld* m_currentWorld;
	};
}