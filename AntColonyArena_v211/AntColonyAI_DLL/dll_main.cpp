#include <stdio.h>
#include <cassert>
#define _ASSERT( arg ) assert( arg );
#include "CommonInterface.hpp"
#include "AIController.h"
#include "TimeUtils.h"

#pragma comment( lib, "opengl32" )
#pragma comment( lib, "Glu32" )

int g_playerID = 0;
unsigned int g_playerColor = 0;
bool g_isQuitting = 0;
bool g_areNewUpdatesReadyToProcess = false;
bool g_needToUpdateMap = false;

AgentReports g_resultForPrevOrders;
ObservedEntities g_observedEntities;
ArenaSquares g_observedArenaSquares;
Orders g_tentativeOrders;

int g_turnNumber = 0;
int g_currentNutrients = 0;

AIController g_aiController;
ArenaMap g_arenaMap;

CRITICAL_SECTION g_ordersAndResultsCS;
CRITICAL_SECTION g_arenaMapCS;
CRITICAL_SECTION g_aiControllerCS;

//-----------------------------------------------------------------------------------------------
int GetInterfaceVersion()
{
//	DebuggerPrintf( "[?] GetInterfaceVersion called.\n" );
	return COMMON_INTERFACE_VERSION_NUMBER;
};


//-----------------------------------------------------------------------------------------------
const char* GetPlayerName()
{
//	DebuggerPrintf( "[?] GetPlayerName called.\n" );
	return "SG-1";
};


//-----------------------------------------------------------------------------------------------
const char* GetAuthorName()
{
//	DebuggerPrintf( "[?] GetAuthorName called.\n" );
	return "Preston Hulcy";
};
//-----------------------------------------------------------------------------------------------
void GiveColors( unsigned int playerColorRGBABytes )
{
	g_playerColor = playerColorRGBABytes;
}
//-----------------------------------------------------------------------------------------------
void PrepareForBattle( int playerID, const ArenaInfo& arenaInfo )
{
	InitializeCriticalSection( &g_ordersAndResultsCS );
	InitializeCriticalSection( &g_arenaMapCS );
	InitializeCriticalSection( &g_aiControllerCS );

	g_aiController.initialize( playerID, arenaInfo );
	g_arenaMap.setArenaInfo( arenaInfo );
}
//-----------------------------------------------------------------------------------------------
void BeginWork()
{
//	DebuggerPrintf( "[%d] WORKER THREAD: BeginWork() called.\n", g_playerID );
	while( !g_isQuitting )
	{
		EnterCriticalSection( &g_ordersAndResultsCS );
		bool areTentativeOrdersEmpty = g_tentativeOrders.m_numberOfOrders == 0;
		bool areResultsEmpty = g_resultForPrevOrders.m_numberAgentReports == 0;
		LeaveCriticalSection( &g_ordersAndResultsCS );

		EnterCriticalSection( &g_arenaMapCS );
		EnterCriticalSection( &g_ordersAndResultsCS );
		if( g_needToUpdateMap )
		{
//			double updateMapTime = Monky::TimeUtils::GetAbsoluteTimeSeconds();
			g_arenaMap.updateMap( g_observedArenaSquares, g_turnNumber );
//			double endTime = Monky::TimeUtils::GetAbsoluteTimeSeconds();
//			DebuggerPrintf( "Update map time: %fms", Monky::TimeUtils::ConvertTimeFromSecondsTo( endTime - updateMapTime, Monky::TimeUtils::MILLI ) );
			g_needToUpdateMap = false;
		}
		LeaveCriticalSection( &g_ordersAndResultsCS );
		LeaveCriticalSection( &g_arenaMapCS );

		if( !( areTentativeOrdersEmpty && !areResultsEmpty ) )
		{
			EnterCriticalSection( &g_ordersAndResultsCS );
			g_areNewUpdatesReadyToProcess = true;
			LeaveCriticalSection( &g_ordersAndResultsCS );
			Sleep( 0 );
		}
		else
		{
			EnterCriticalSection( &g_ordersAndResultsCS );
			AgentReports tempResultsForPrevOrders = g_resultForPrevOrders;
			ObservedEntities tempObservedEntities = g_observedEntities;
			g_areNewUpdatesReadyToProcess = true;
			LeaveCriticalSection( &g_ordersAndResultsCS );			
			
			// Do ai logic
			Orders tempTentativeOrders;
//			double startTime = Monky::TimeUtils::GetAbsoluteTimeSeconds();
//			double endTime = 0.0f;

			EnterCriticalSection( &g_ordersAndResultsCS );
			int turnNumber = g_turnNumber;
			int currentNutrients = g_currentNutrients;
			LeaveCriticalSection( &g_ordersAndResultsCS );
			
			EnterCriticalSection( &g_aiControllerCS );
			g_aiController.updateControllerState( turnNumber, currentNutrients );
			LeaveCriticalSection( &g_aiControllerCS );

			EnterCriticalSection( &g_arenaMapCS );
			EnterCriticalSection( &g_aiControllerCS );
			g_arenaMap.updateEnemySightingsHeatMap( tempObservedEntities );
			g_aiController.handleAgentReports( g_arenaMap, tempResultsForPrevOrders, tempObservedEntities, tempTentativeOrders );			
			LeaveCriticalSection( &g_aiControllerCS );
			LeaveCriticalSection( &g_arenaMapCS );


			EnterCriticalSection( &g_ordersAndResultsCS );
			g_resultForPrevOrders.m_numberAgentReports = 0;
			g_tentativeOrders = tempTentativeOrders;
			LeaveCriticalSection( &g_ordersAndResultsCS );
			
			EnterCriticalSection( &g_arenaMapCS );
			EnterCriticalSection( &g_aiControllerCS );
			g_aiController.processAgentPathOrders( g_arenaMap, tempTentativeOrders );
			g_aiController.generateNewOrders( g_arenaMap, tempResultsForPrevOrders, tempObservedEntities, tempTentativeOrders );
			LeaveCriticalSection( &g_aiControllerCS );
			LeaveCriticalSection( &g_arenaMapCS );
			
			EnterCriticalSection( &g_arenaMapCS );
			EnterCriticalSection( &g_aiControllerCS );
			g_aiController.processAStarPathRequests( g_arenaMap, tempObservedEntities );
			LeaveCriticalSection( &g_aiControllerCS );
			LeaveCriticalSection( &g_arenaMapCS );



//			endTime = Monky::TimeUtils::GetAbsoluteTimeSeconds();
//			DebuggerPrintf( "Total update time: %fms\n", Monky::TimeUtils::ConvertTimeFromSecondsTo( endTime - startTime, Monky::TimeUtils::MILLI ) );

			EnterCriticalSection( &g_ordersAndResultsCS );
			g_resultForPrevOrders.m_numberAgentReports = 0;
			g_tentativeOrders = tempTentativeOrders;
			LeaveCriticalSection( &g_ordersAndResultsCS );	
		}
	}
}
//-----------------------------------------------------------------------------------------------
void EndWork()
{
	g_isQuitting = true;
}
//-----------------------------------------------------------------------------------------------
// Update the colony's current state and get the results of the previous turn's orders
// Store the colony's current nutrient supply and its map of the arena
//-----------------------------------------------------------------------------------------------
void SendUpdate( int turnNumber, int currentNutrients, const AgentReports& agentReports, const ObservedEntities& observedEntities, const ArenaSquares& observedSquares )
{
 	EnterCriticalSection( &g_ordersAndResultsCS );
	g_resultForPrevOrders = agentReports;
	g_observedEntities = observedEntities; 
	g_observedArenaSquares = observedSquares;

	g_needToUpdateMap = true;
	g_areNewUpdatesReadyToProcess = false;
	LeaveCriticalSection( &g_ordersAndResultsCS );

	EnterCriticalSection( &g_ordersAndResultsCS );
	g_currentNutrients = currentNutrients;
	g_turnNumber = turnNumber;
	LeaveCriticalSection( &g_ordersAndResultsCS );
}
//-----------------------------------------------------------------------------------------------
// Dispatch new orders to the Arena
//-----------------------------------------------------------------------------------------------
void FetchNewOrders( int turnNumber, Orders& newOrders_out )
{	
   	EnterCriticalSection( &g_ordersAndResultsCS );
	bool areNewUpatesReady = g_areNewUpdatesReadyToProcess;
	LeaveCriticalSection( &g_ordersAndResultsCS );

	while( !areNewUpatesReady )
	{
		Sleep( 0 );
		EnterCriticalSection( &g_ordersAndResultsCS );
		areNewUpatesReady = g_areNewUpdatesReadyToProcess;
		LeaveCriticalSection( &g_ordersAndResultsCS );
	}

	EnterCriticalSection( &g_ordersAndResultsCS );
	newOrders_out = g_tentativeOrders;
	g_tentativeOrders.m_numberOfOrders = 0;
	LeaveCriticalSection( &g_ordersAndResultsCS );
}
//-----------------------------------------------------------------------------------------------
void DrawDebugOverlay( short inspectSquareX, short inspectSquareY )
{
	EnterCriticalSection( &g_aiControllerCS );
	g_aiController.renderPathsForAgents();
	LeaveCriticalSection( &g_aiControllerCS );
	
	EnterCriticalSection( &g_arenaMapCS );
	g_arenaMap.renderMapOverlay( g_playerColor, inspectSquareX, inspectSquareY );
	LeaveCriticalSection( &g_arenaMapCS );
}