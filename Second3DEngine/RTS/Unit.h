#pragma once
#include <string>
#include "Entity.h"
#include "Map.h"
#include <EventSystemInterface.h>

namespace pdh
{
	class UnitBlueprint;
	class Controller;
	class MoveOrders;
	class GotoResource;
	class CollectResource;
	class ReturnResource;
	class BuildOrders;
	class UnitOrders;
	class AttackOrders;
	class AttackMoveOrders;

	class Unit : public Entity
	{
	public:

		friend UnitBlueprint;
		friend MoveOrders;
		friend GotoResource;
		friend CollectResource;
		friend ReturnResource;
		friend BuildOrders;
		friend UnitOrders;
		friend AttackOrders;
		friend AttackMoveOrders;

		struct Attack
		{
			float damage;
			float speed;
			float range;
		};

		struct Movement
		{
			float speed;
			float vision;
		};

		struct Resource
		{
			Resource()
				:	currentAmount( 0 )
				,	resourceType( Map::NONE )
			{}
			int currentAmount;
			Map::ResourceType resourceType;
		};
		
		Unit( const UnitBlueprint* blueprint, RTSWorld* world );
		~Unit();

		virtual void update( float deltaTime );
		virtual void render();
		virtual void debugRender();
		virtual void renderOrders();

		virtual aabb2f getAABB2() const;

		float getMovementSpeed() const { return m_movement.speed; }	
		float getVisionRadius() const { return m_movement.vision; }	
		float getWidth() const;
		float getHeight() const;
		virtual const std::string& getBlueprintName() const;
		bool canMoveToLocation( const WorldCoords& worldCoords ) const;
		bool canMoveOnTileWithWorldCoords( const WorldCoords& worldCoords ) const;
		bool canMoveOnTile( const std::string& terrainType ) const;

		const UnitBlueprint* getUnitBlueprint() const { return m_blueprint; }

		bool getBuildingNameFromHotkey( char hotkey, std::string& buildingName ) const;
		const std::string& getRandomBuildingName() const;
		
		void assignOrders( Orders* orders );
		void pushOrders( Orders* orders );

		const Orders* getCurrentOrders() const;
		const Orders* getLastOrderInQueue() const;

		void toggleAStarRender( NamedProperties& params );


	protected:
		void searchForTarget();


	private:
		const UnitBlueprint* m_blueprint;
		
		Attack m_attack;
		Movement m_movement;
		Resource m_resource;
		bool m_renderAStarPath;

	};
}