#pragma once
#include <XMLParser.h>
#include <vector>
#include <string>
#include "Entity.h"
#include <Colors.h>

namespace pdh
{
	class BuildingBlueprint;
	class RTSWorld;
	class BuildUnitOrders;
	class UnitOrders;
	class BuildOrders;

	class Building : public Entity
	{
	public:
		friend BuildingBlueprint;
		friend BuildUnitOrders;
		friend BuildOrders;
		

		Building( const BuildingBlueprint* blueprint, RTSWorld* world  );
		~Building();

		
		virtual void update( float deltaTime );
		virtual void render();
		void renderInitialUnitOrders();
		virtual aabb2f getAABB2() const;

		void addToCurrentBuildTime( float timeToAdd );
		void setCurrentBuildTime( float currentBuildTime );
		float getCurrentBuildTime() const { return m_currentBuildTime; }

		bool isComplete() { return m_isComplete; }
		int getTimeToBuild();
		float getWidth() const;
		float getHeight() const;
		virtual const std::string& getBlueprintName() const;
		const BuildingBlueprint* getBlueprint() const { return m_blueprint; }

		float getCurrentBuildRateFactor() const;
		float getCurrentHealthIncreaseFactor() const;

		void addUnitWorkingOnBuilding();
		void removeUnitWorkdingOnBuilding();
		
		bool getUnitNameFromHotkey( char hotkey, std::string& unitName ) const;
		bool getUnitNameBasedOnResources( int fishFingers, int custard, int timeEnergy, std::string& unitType ) const;
		
		void assignInitialOrderForUnits( Orders* initialOrder );
		void pushInitialOrderForUnits( Orders* initialOrder );

	private:
		const BuildingBlueprint* m_blueprint;
		float m_currentBuildTime;
		bool m_isComplete;
		Color4f m_colorTint;
		float m_visionRange;

		int m_currentNumUnitsWorkingOnThisBuilding;

		std::vector< Orders* > m_initialOrdersForUnits;
	};
}
