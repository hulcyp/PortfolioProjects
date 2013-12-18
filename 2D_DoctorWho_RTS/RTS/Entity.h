#pragma once
#include <CommonEngineIncludes.h>
#include <deque>
#include "Map.h"

namespace pdh
{
	class Renderer;
	class Orders;
	class Controller;
	class RTSWorld;
	class Mesh;
	class AttackOrders;

	class Entity
	{
	public:

		friend AttackOrders;

		enum EntityType
		{
			NONE,
			UNIT,
			BUILDING
		};

		struct Cost
		{
			int buildTime;
			int fishFingers;
			int custard;
			int timeEnergy;
			int supply;
		};

		Entity( RTSWorld* world );
		virtual ~Entity();

		virtual void update( float deltaTime ){}
		virtual void render(){}
		virtual void debugRender();
		virtual void renderOrders();
		

		void setPosition( const vec2f& pos );
		const vec2f& getPosition() const { return m_position; }
		virtual aabb2f getAABB2() const { return aabb2f(); };
		EntityType getType() const { return m_entityType; }
		Mesh* getSelectionMesh() const { return m_selectionMesh; }
		virtual const std::string& getBlueprintName() const = 0;//{ return std::string(); }
		void setHealth( float health );

		void setSelected( bool selected, bool allySelected );

		Map* getMap() const;
		RTSWorld* getWorld() const;
		Map::MapTile& getMapTileCurrentlyOn() const;
		

		virtual void setOwner( Controller* owner );
		Controller* getOwner() const;

		void assignOrders( Orders* orders );
		void pushOrders( Orders* orders );
		void cancelOrders();
		void cancelAllQueuedOrders();
		void popOrder();

		void dealDamage( float damage );

		bool hasOrders() const { return m_orders.size() > 0; }

		void getVisibleTiles( float visionRadius, std::vector< TileCoords >& tiles );

	protected:
		vec2f m_position;
		vec2f m_prevPosition;
		Cost m_cost;
		float m_currentHealth;
		float m_maxHealth;
		std::deque< Orders* > m_orders;
		Controller* m_owner;
		RTSWorld* m_world;
		Mesh* m_selectionMesh;
		bool m_isSelected;
		bool m_isAllySelected;
		EntityType m_entityType;

		std::vector< TileCoords > m_visibleTiles;
		std::vector< Entity* > m_visibleEntities;

		Mesh* m_healthBar;
	};
}