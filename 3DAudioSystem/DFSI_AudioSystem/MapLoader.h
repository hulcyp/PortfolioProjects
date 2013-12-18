#pragma once
#include <string>

namespace Monky
{
	class GameApp;
	class Mesh;

	class MapLoader
	{
	public:
		MapLoader( const std::string& filePath, GameApp* game );

		void loadMap();

	private:
		std::string m_filePath;
		GameApp* m_theGame;
		const float WALL_WIDTH;
		const float WALL_DEPTH;
		Mesh* m_wallMesh;
		Mesh* m_floorMesh;
	};
}