//-----------------------------------------------------------------------------------------------
// PluginManager.hpp
//-----------------------------------------------------------------------------------------------
#ifndef __include_PluginManager__
#define __include_PluginManager__

#include "GameShared.hpp"
#include <windows.h>


//-----------------------------------------------------------------------------------------------
// Exported DLL function types (for GetProcAddress)
//
typedef int ( *GetInterfaceVersionFunctionType )();
typedef const char* ( *GetPlayerNameFunctionType )();
typedef const char* ( *GetAuthorNameFunctionType )();
typedef void ( *GiveColorsFunctionType )( unsigned int playerColorRGBABytes );
typedef void ( *PrepareForBattleFunctionType )( int playerID, const ArenaInfo& arenaInfo );
typedef void ( *DrawDebugOverlayFunctionType )( short inspectSquareX, short inspectSquareY );
typedef void ( *BeginWorkFunctionType )();
typedef void ( *FetchNewOrdersFunctionType )( int turnNumber, Orders& newOrders_out );
typedef void ( *SendUpdateFunctionType )( int turnNumber, int currentNutrients, const AgentReports& agentReports, const ObservedEntities& observedEntities, const ArenaSquares& observedSquares );


//-----------------------------------------------------------------------------------------------
// Forward declarations
//


//-----------------------------------------------------------------------------------------------
// Global variables
//


/////////////////////////////////////////////////////////////////////////////////////////////////
class Player
{
public:
	Player( const std::string& dllFileName, int playerIndex );
	~Player();
	void ClearReportStructures( ArenaSquares& arenaSquares );
	void Validate();
	void ValidateInterfaceVersion();
	void ReadPlayerName();
	void GetDLLFunctionPointers();
	void GetDrawDebugOverlayFunctionPointer();
	void InitializeForArena( Arena& arena, const IntVector2& startingPosition );
	int CalcNumLivingAgents() const;
	bool HasLivingAgents() const;
	void MarkSquaresInManhattanRadiusVisible( const IntVector2& center, int manhattanRadius );
	void CalculateVisibility( Arena& arena );
	void BuildObservations( Arena& arena );
	void StarveAntIfNoneSuicidedThisTurn( int turnNumber, Arena& arena );
	int ComputeUpkeepCost( Arena& arena );
	void KillAgent( Agent& agent, Arena& arena, ReportCode reasonForDeath );
	void AddAgentReport( AgentReport& agentReport )
	{
		_ASSERT( m_agentReports.m_numberAgentReports < MAX_REPORTS_PER_PLAYER );
		m_agentReports.m_agentReports[ m_agentReports.m_numberAgentReports ] = agentReport;
		++ m_agentReports.m_numberAgentReports;
	}

public:
	HMODULE							m_dllHandle;
	HANDLE							m_workThreadHandle;
	unsigned int					m_workThreadID;

	BeginWorkFunctionType			m_beginWorkFunc;
	FetchNewOrdersFunctionType		m_fetchNewOrdersFunc;
	SendUpdateFunctionType			m_sendUpdateFunc;

	DrawDebugOverlayFunctionType	m_drawDebugOverlayFunc;
	bool							m_isDebugDrawEnabled;
	int								m_playerIndex;
	std::string						m_playerName;
	std::string						m_authorName;
	Rgba							m_playerColor;
	std::string						m_playerColorName;
	std::string						m_dllFileName;
	std::string						m_dllFilePath;
	IntVector2						m_startingPosition;
	int								m_numberOfPenalties;
	std::vector< Agent* >			m_agents;
	Orders							m_moveOrders;
	AgentReports					m_agentReports;
	ObservedEntities				m_observedEntities;
	ArenaSquares					m_observedArenaSquares;
	char							m_isVisibleToPlayer[ MAX_ARENA_SQUARES ]; // 0 if not currently visible to the player, 1 if currently visible
	int								m_currentNutrients;
	double							m_timeQueenWasLastAssaulted;
	int								m_turnQueenWasLastAssaulted;
	bool							m_isDefeated;
};


/////////////////////////////////////////////////////////////////////////////////////////////////
class PluginManager
{
public:
	PluginManager();
	~PluginManager();
	bool LoadAndTestLibraryByName( const std::string& dllFileName );
	void LoadAndTestPlugins();
	void LoadPlayers();
	void ValidatePlayers();
	void AnnouncePlayers();
	void PreparePlayersForBattle( Arena& arena );
	double FetchNewOrders( int turnNumber, int& totalOrderCount );
	double SendUpdates( int turnNumber, int& totalReportCount, int& totalObservedEntityCount );

	void StartPlayerWorkThread( int playerIndex );
	void KickPlayerWorkThreadsOffMainCPU();
	void LetPlayerWorkThreadsBackOnMainCPU();
	void SuspendAllPlayerWorkThreads();
	void ResumeAllPlayerWorkThreads();

public:
	std::vector< Player* > m_players;
};


// Standalone functions
unsigned __stdcall PlayerWorkThreadEntryFunction( void* playerPointer );



//-----------------------------------------------------------------------------------------------
inline void Player::MarkSquaresInManhattanRadiusVisible( const IntVector2& center, int manhattanRadius )
{
	IntVector2 arenaSize( m_observedArenaSquares.m_width, m_observedArenaSquares.m_height );

	if( center.x < 0 || center.y < 0 || center.x >= arenaSize.x || center.y >= arenaSize.y )
		return;

	const int centerIndex = center.x + (m_observedArenaSquares.m_width * center.y);

	// Mark center as visible
	m_isVisibleToPlayer[ centerIndex ] = 1;

	// Do radial axes away from center, cloning to all 4 quadrants as we go
	int radius, perpendicularRadius, perpendicularRadiusCeiling;
	int eastOnAxisX, westOnAxisX, northOnAxisY, southOnAxisY;
	for( radius = 1; radius <= manhattanRadius; ++ radius )
	{
		eastOnAxisX = center.x + radius;
		northOnAxisY = center.y - radius;
		westOnAxisX = center.x - radius;
		southOnAxisY = center.y + radius;

		int radiusTimesArenaWidth = radius * arenaSize.x;
		if( eastOnAxisX < arenaSize.x )
			m_isVisibleToPlayer[ centerIndex + radius ] = 1;
		if( westOnAxisX >= 0 )
			m_isVisibleToPlayer[ centerIndex - radius ] = 1;
		if( southOnAxisY < arenaSize.y )
			m_isVisibleToPlayer[ centerIndex + radiusTimesArenaWidth ] = 1;
		if( northOnAxisY >= 0 )
			m_isVisibleToPlayer[ centerIndex - radiusTimesArenaWidth ] = 1;

		perpendicularRadiusCeiling = 1 + manhattanRadius - radius;
		for( perpendicularRadius = 1; perpendicularRadius < perpendicularRadiusCeiling; ++ perpendicularRadius )
		{
			int quadrant1And2Y = center.y - perpendicularRadius;
			int quadrant3And4Y = center.y + perpendicularRadius;
			
			if( eastOnAxisX < arenaSize.x )
			{
				// Quadrants 1 and 4
				if( quadrant1And2Y >= 0 )
					m_isVisibleToPlayer[ eastOnAxisX + (quadrant1And2Y * arenaSize.x) ] = 1;
				if( quadrant3And4Y < arenaSize.y )
					m_isVisibleToPlayer[ eastOnAxisX + (quadrant3And4Y * arenaSize.x) ] = 1;
			}

			if( westOnAxisX >= 0 )
			{
				// Quadrants 2 and 3
				if( quadrant1And2Y >= 0 )
					m_isVisibleToPlayer[ westOnAxisX + (quadrant1And2Y * arenaSize.x) ] = 1;
				if( quadrant3And4Y < arenaSize.y )
					m_isVisibleToPlayer[ westOnAxisX + (quadrant3And4Y * arenaSize.x) ] = 1;
			}
		}
	}
}




#endif // __include_PluginManager__
