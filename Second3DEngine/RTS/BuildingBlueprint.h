#pragma once
#include <string>
#include <vector>
#include <map>
#include <XMLParser.h>

namespace pdh
{
	class Building;
	class RTSWorld;
	class Mesh;

	class BuildingBlueprint
	{
	public:
		struct CostBP
		{
			int time;
			int fishFingers;
			int custard;
			int timeEnergy;
		};

		struct ConstructionRules
		{
			std::vector< std::string > canBeBuiltOn;
			std::vector< std::string > mustBeBuiltAdjTo;
		};
		
		BuildingBlueprint( XMLParser& parser, const XMLNode* buildingBlueprintNode );
		~BuildingBlueprint();

		Building* createBuilding( RTSWorld* world ) const;
		const std::string& getName() const { return m_name; }
		Mesh* getMesh() const { return m_mesh; }
		float getWidth() const { return m_width; }
		float getHeight() const { return m_height; }
		int getBuildTime() const { return m_cost.time; }
		const ConstructionRules getConstructionRules() const { return m_constructionRules; }
		const CostBP& getBuildingCost() const { return m_cost; }
		float getMaxHealth() const { return m_maxHealth; }

		int getSupplyProvided() const { return m_supplyProvided; }

		const std::map< char, std::string >& getUnitsProduced() const { return m_unitsProduced; }

		static BuildingBlueprint* getBlueprint( const std::string& name );

	protected:
		void registerBP( const std::string& name );

	private:
		std::string m_name;
		CostBP m_cost;
		ConstructionRules m_constructionRules;
		float m_maxHealth;
		std::map< char, std::string > m_unitsProduced;

		Mesh* m_mesh;
		float m_width;
		float m_height;
		Mesh* m_selectionMesh;
		Mesh* m_healthBar;
		float m_visionRange;

		int m_supplyProvided;

		static std::map< std::string, BuildingBlueprint* > sm_blueprints;
	};
}

