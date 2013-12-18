#pragma once
#include "Controller.h"

namespace pdh
{
	class AIController : public Controller
	{
	public:
		AIController( RTSWorld* world, int teamNum, const WorldCoords& startLoc, const Color4f& teamColor );

		void update( float deltaTime );

	private:
		void selectPeon();
		void selectIdalPeon();
		void tryToIssueBuildUnitOrders();
		TileCoords findLocationToBuildBuilding( const std::string& buildingName );

		WorldCoords m_startingLocation;
	};
}