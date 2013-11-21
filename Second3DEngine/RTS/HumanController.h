#pragma once
#include "Controller.h"
#include <EventSystemInterface.h>
#include <Timer.h>
#include <map>

namespace pdh
{
	class Mesh;
	class Camera;
	class Entity;

	class HumanController : public Controller
	{
	public:
		enum CurrentOrder
		{
			NONE,
			ATTACK,
			ATTACK_MOVE,
			MOVE,
			BUILD_BUILDING,
			BUILD_UNIT,
			HOLD,
			HARVEST,
			SPAWN
		};
		
		HumanController( RTSWorld* world, int playerNum, Camera* HUDCamera, const Color4f& teamColor );
		virtual ~HumanController();

		void render();

		void onKeyDown( NamedProperties& params );
		void onKeyUp( NamedProperties& params );
		void onMouseButtonDown( NamedProperties& params );
		void onMouseButtonUp( NamedProperties& params );
		void onMouseMove( NamedProperties& params );
		
		void queueMoveOrders( const vec2f& loc );
		void queueBuildOrders( const std::string& buildingType, const vec2f& loc );		
		void queueHoldOrders();

		void notEnoughtResources( NamedProperties& params );
		void notEnoughSupply();
		
		virtual void issueBuildOrders( const std::string& blueprint, const vec2f& loc );
		virtual void issueBuildUnitOrder( const std::string& unitType );

	protected:
		void deselectAll();
		Entity* getFirstUnitEntity( const std::vector< Entity* > entities ) const;
		Entity* getFirstBuildingEntity( const std::vector< Entity* > entities ) const;

		void popBackBuildUnitOrder();
		void createControlGroup( char ctrlGroup );
		void addToControlGroup( char ctrlGroup );
		void selectControlGroup( char ctrlGroup );
		char getNumericalCharFromShiftedChar( char keyCode );
		

	private:
		aabb2i m_currentSelection;
		bool m_isSelecting;
		CurrentOrder m_currentOrder;
		vec2i m_currentCursorPos;
		Camera* m_hudCamera;
		float m_resourceIconWidth;

		std::string m_currentBuildingSelectedToBuild;
		bool m_validBuildingSelected;
		
		std::map< char, std::vector< Unit* > > m_unitControlGroups;
		std::map< char, std::vector< Building* > > m_buildingControlGroups;

		Mesh* m_fishFingersMesh;
		Mesh* m_custardMesh;
		Mesh* m_timeEnergyMesh;
		Mesh* m_supplyMesh;
		
		NamedProperties m_resourcesTextParams;
		vec3f m_fishFingersIconPos;
		vec3f m_custardIconPos;
		vec3f m_timeEnergyIconPos;
		vec3f m_fishFingersResourcePos;
		vec3f m_custardResourcePos;
		vec3f m_timeEnergyResourcePos;

		vec3f m_supplyIconPos;
		vec3f m_supplyPos;

		Timer m_outOfResourceMsgTimer;
		NamedProperties m_outOfResourceMsgParams;

		Timer m_notEnoughSupplyMsgTimer;
		NamedProperties m_notEnoughSupplyMsgParams;

	};
}