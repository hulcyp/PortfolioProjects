#pragma once
#include "BuildUnitOrders.h"
#include "UnitBlueprint.h"
#include "Building.h"
#include "RTSWorld.h"
#include <Font.h>
#include "Controller.h"
#include "Unit.h"
#include "Orders.h"
#include "UnitOrders.h"
#include "MoveOrders.h"
#include "GotoResource.h"

namespace pdh
{
	BuildUnitOrders::BuildUnitOrders( Building* building, const std::string& unitType )
		:	BuildingOrders( building )
		,	m_currentBuildTime( 0.0f )
		,	m_startedBuilding( false )
	{
		m_unitBlueprint = UnitBlueprint::getBluePrint( unitType );
	}
	//------------------------------------------------------------------------
	BuildUnitOrders::BuildUnitOrders( Building* building, const UnitBlueprint* unitBP )
		:	BuildingOrders( building )
		,	m_unitBlueprint( unitBP )
		,	m_currentBuildTime( 0.0f )
		,	m_startedBuilding( false )
	{}
	//------------------------------------------------------------------------
	void BuildUnitOrders::followOrders( float deltaTime )
	{
		if( !m_startedBuilding )
		{
			Controller::ResourceRequestError requestResourceError = 
				m_building->m_owner->requestResource( m_unitBlueprint->getUnitCost().fishFingers, m_unitBlueprint->getUnitCost().custard, m_unitBlueprint->getUnitCost().timeEnergy );
			
			if( requestResourceError == Controller::NOT_ENOUGH_FISH_FINGERS )
			{
				NamedProperties params;
				m_ordersComplete = true;
				std::string text = "Not enough Fish Fingers";
				params.set( "text", text );
				params.set( "owner", m_building->m_orders );
				fireEvent( "NotEnoughResources", params );
			}
			else if( requestResourceError == Controller::NOT_ENOUGH_CUSTARD )
			{
				NamedProperties params;
				m_ordersComplete = true;
				std::string text = "Not enough Custard";
				params.set( "text", text );
				params.set( "owner", m_building->m_orders );
				fireEvent( "NotEnoughResources", params );
			}
			else if( requestResourceError == Controller::NOT_ENOUGH_TIME_ENERGY )
			{
				NamedProperties params;
				m_ordersComplete = true;
				std::string text = "Not enough Time Energy";
				params.set( "text", text );
				params.set( "owner", m_building->m_orders );
				fireEvent( "NotEnoughResources", params );
			}
			else
			{
				m_startedBuilding = true;
				if( m_building->m_owner != nullptr )
					m_building->m_owner->addToCurrentSupplyCount( m_unitBlueprint->getUnitCost().supply );
			}
		}

		if( m_currentBuildTime >= m_unitBlueprint->getUnitCost().buildTime )
		{
			Unit* unit = m_building->getWorld()->spawnUnit( m_unitBlueprint, m_building->getPosition(), m_building->getOwner() );
			for( size_t i = 0; i < m_building->m_initialOrdersForUnits.size(); ++i )
			{
				if( m_building->m_initialOrdersForUnits[i]->getType() == Orders::UNIT )
				{
					UnitOrders* unitOrders = reinterpret_cast< UnitOrders* >( m_building->m_initialOrdersForUnits[i] );
					Orders* orders = nullptr;
					MoveOrders* moveOrders = nullptr;
					GotoResource* gotoResource = nullptr;
					switch( unitOrders->getUnitOrderType() )
					{
					case UnitOrders::MOVE:
						moveOrders = reinterpret_cast< MoveOrders* >( m_building->m_initialOrdersForUnits[i] );
						orders = new MoveOrders( *moveOrders );
						break;
					case UnitOrders::GO_TO_RESOURCE:
						gotoResource = reinterpret_cast< GotoResource* >( m_building->m_initialOrdersForUnits[i] );
						if( unit->getUnitBlueprint()->isPeon() )
						{
							orders = new GotoResource( *gotoResource );
						}
						else
						{
							TileCoords tileCoords = m_building->m_world->getTileCoordsForCollectingResource( unit, gotoResource->getMapTile() );
							orders = new MoveOrders( m_building->m_world->getMap()->getWorldCoordsFromTileCoords( tileCoords ) );
						}
						break;
					}
					unit->pushOrders( orders );
				}
			}

			m_ordersComplete = true;
		}
		else
		{
			m_currentBuildTime += deltaTime;
		}
	}
	void BuildUnitOrders::renderOrders()
	{		
		if( m_startedBuilding )
		{
			std::string fontName = "mainFont_72";
			std::string text = toString( (int)m_currentBuildTime ) + "/" + toString( m_unitBlueprint->getUnitCost().buildTime );
			vec3f pos = vec3f( m_building->getPosition(), 0.3f );
			pos.y += m_building->getHeight() * 0.5f;
			float fontHeight = 1.0f;
			Color4f color = color::WHITE;
			vec2f posRel = Font::BOTCENTER;
			NamedProperties textParams;
			textParams.set( "fontName", fontName );
			textParams.set( "text", text );
			textParams.set( "pos", pos );
			textParams.set( "fontHeight", fontHeight );
			textParams.set( "color", color );
			textParams.set( "posRel", posRel );
			fireEvent( "renderText", textParams );
		}
	}
}