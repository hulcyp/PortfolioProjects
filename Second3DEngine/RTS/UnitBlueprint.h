#pragma once
#include <XMLParser.h>
#include <string>
#include <map>
#include <vector>
#include <RandItemPool.h>

namespace pdh
{
	class Unit;
	class Mesh;
	class Map;
	class RTSWorld;

	class UnitBlueprint
	{
	public:

		struct CostBP
		{
			int buildTime;
			int fishFingers;
			int custard;
			int timeEnergy;
			int supply;
		};

		struct AttackBP
		{
			float damage;
			float speed;
			float range;
		};

		struct MovementBP
		{
			float speed;
			std::vector< std::string > canMoveOn;
			float vision;
		};

		UnitBlueprint( XMLParser& parser, const XMLNode* unitBlueprintNode );
		~UnitBlueprint();
		
		Unit* createUnit( RTSWorld* world ) const;
		Mesh* getMesh() const;
		
		const std::string& getName() const { return m_name; }		
		float getHeight() const { return m_height; }
		float getWidth() const { return m_width; }
		const std::vector< std::string >& getCanMoveOnList() const { return m_movement.canMoveOn; }

		const CostBP& getUnitCost() const { return m_cost; }
		const std::map< char, std::string >& getBuildingsCanProduce() const { return m_buildingsCanProduce; }
		const RandItemPool<char>& getHotkeyPool() const { return m_hotkeyPool; }
		bool isPeon() const { return m_isPeon; }


		static UnitBlueprint* getBluePrint( const std::string& name );

	protected:
		void registerBP( const std::string& name );
	
	private:
		std::string m_name;
		CostBP m_cost;
		float m_maxHealth;
		AttackBP m_attack;
		MovementBP m_movement;

		Mesh* m_mesh;
		Mesh* m_selectionMesh;		
		float m_width;
		float m_height;
		Mesh* m_healthBar;

		bool m_isPeon;

		std::map< char, std::string > m_buildingsCanProduce;
		RandItemPool<char> m_hotkeyPool;

		static std::map< std::string, UnitBlueprint* > sm_blueprints;
	};
}