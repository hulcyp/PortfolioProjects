//-----------------------------------------------------------------------------------------------
// PluginManager.cpp
//-----------------------------------------------------------------------------------------------
#include "PluginManager.hpp"
#include "ArenaManager.hpp"
#include <process.h>
#include <windows.h>


//-----------------------------------------------------------------------------------------------
// Globals and constants
//
const int MANHATTAN_VISIBILITY_DISTANCE = 5;


/////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
Player::Player( const std::string& dllFileName, int playerIndex )
	: m_dllHandle( NULL )
	, m_beginWorkFunc( NULL )
	, m_fetchNewOrdersFunc( NULL )
	, m_sendUpdateFunc( NULL )
	, m_drawDebugOverlayFunc( NULL )
	, m_workThreadHandle( NULL )
	, m_isDebugDrawEnabled( false )
	, m_playerIndex( playerIndex )
	, m_playerName( "UNNAMED CONTESTANT" )
	, m_playerColor( Rgba::ORANGE )
	, m_numberOfPenalties( 0 )
	, m_dllFileName( dllFileName )
	, m_dllFilePath( "Players/" + dllFileName )
	, m_startingPosition( IntVector2::ZERO )
	, m_currentNutrients( 0 )
	, m_timeQueenWasLastAssaulted( 0.0 )
	, m_turnQueenWasLastAssaulted( -1 )
	, m_isDefeated( false )
{
	m_dllHandle = LoadLibraryA( m_dllFilePath.c_str() );
	if( !m_dllHandle )
	{
		FatalError( "DLL Load Error", Stringf( "Failed to LoadLibrary for DLL \"%s\"!\n", m_dllFilePath.c_str() ) );
	}
}


//-----------------------------------------------------------------------------------------------
Player::~Player()
{
}


//-----------------------------------------------------------------------------------------------
void Player::ClearReportStructures( ArenaSquares& arenaSquares )
{
	UNUSED( arenaSquares );

	m_agentReports.m_numberAgentReports = 0;
	m_observedEntities.m_numberOfObservedEntities = 0;
//	m_observedArenaSquares = arenaSquares;
}


//-----------------------------------------------------------------------------------------------
void Player::Validate()
{
	ValidateInterfaceVersion();
	ReadPlayerName();
	GetDrawDebugOverlayFunctionPointer();
	GetDLLFunctionPointers();
}


//-----------------------------------------------------------------------------------------------
void Player::ValidateInterfaceVersion()
{
	// Check if we are using the same version of the common interface
	GetInterfaceVersionFunctionType getInterfaceVersionFunc = reinterpret_cast< GetInterfaceVersionFunctionType >( GetProcAddress( m_dllHandle, "GetInterfaceVersion" ) );

	if( getInterfaceVersionFunc )
	{
		int pluginVersion = getInterfaceVersionFunc();
		if( pluginVersion != COMMON_INTERFACE_VERSION_NUMBER )
		{
			FatalError( "DLL Data Exchange Error",
				Stringf( "Error: Arena and DLL \"%s\" are using different CommonInterface.hpp versions;\n - Arena is using version %d\n - %s is using version %d\n\nPlease build %s with version %d of CommonInterface.hpp (the version being used by the Arena).",
				m_dllFilePath.c_str(), COMMON_INTERFACE_VERSION_NUMBER, m_dllFileName.c_str(), pluginVersion, m_dllFileName.c_str(), COMMON_INTERFACE_VERSION_NUMBER ) );
		}
	}
	else
	{
		FatalError( "DLL Data Exchange Error", Stringf( "GetProcAddress(\"GetInterfaceVersion\") FAILED for DLL \"%s\"!\n", m_dllFilePath.c_str() ) );
	}
}


//-----------------------------------------------------------------------------------------------
void Player::ReadPlayerName()
{
	// Get player name from DLL
	GetPlayerNameFunctionType getPlayerNameFunc = reinterpret_cast< GetPlayerNameFunctionType >( GetProcAddress( m_dllHandle, "GetPlayerName" ) );
	if( getPlayerNameFunc )
	{
		m_playerName = getPlayerNameFunc();
	}
	else
	{
		FatalError( "DLL Data Exchange Error", Stringf( "GetProcAddress(\"GetPlayerName\") FAILED for DLL \"%s\"!\n", m_dllFilePath.c_str() ) );
	}

	// Get author name from DLL (optional, for backward compatibility with C16 DLLs)
	GetAuthorNameFunctionType getAuthorNameFunc = reinterpret_cast< GetAuthorNameFunctionType >( GetProcAddress( m_dllHandle, "GetAuthorName" ) );
	if( getAuthorNameFunc )
	{
		m_authorName = getAuthorNameFunc();
	}
	else
	{
		m_authorName = "???";
	}
}


//-----------------------------------------------------------------------------------------------
void Player::GetDLLFunctionPointers()
{
	m_beginWorkFunc			= reinterpret_cast< BeginWorkFunctionType >( GetProcAddress( m_dllHandle, "BeginWork" ) );
	m_fetchNewOrdersFunc	= reinterpret_cast< FetchNewOrdersFunctionType >( GetProcAddress( m_dllHandle, "FetchNewOrders" ) );
	m_sendUpdateFunc		= reinterpret_cast< SendUpdateFunctionType >( GetProcAddress( m_dllHandle, "SendUpdate" ) );

	if( !m_beginWorkFunc )
		FatalError( "DLL Data Exchange Error", Stringf( "Error: GetProcAddress(\"BeginWork\") FAILED for DLL \"%s\"!\n", m_dllFilePath.c_str() ) );

	if( !m_fetchNewOrdersFunc )
		FatalError( "DLL Data Exchange Error", Stringf( "Error: GetProcAddress(\"FetchNewOrders\") FAILED for DLL \"%s\"!\n", m_dllFilePath.c_str() ) );

	if( !m_sendUpdateFunc )
		FatalError( "DLL Data Exchange Error", Stringf( "Error: GetProcAddress(\"SendUpdate\") FAILED for DLL \"%s\"!\n", m_dllFilePath.c_str() ) );
}


//-----------------------------------------------------------------------------------------------
void Player::GetDrawDebugOverlayFunctionPointer()
{
	m_drawDebugOverlayFunc = reinterpret_cast< DrawDebugOverlayFunctionType >( GetProcAddress( m_dllHandle, "DrawDebugOverlay" ) );
	// okay to fail; this one is optional
}


//-----------------------------------------------------------------------------------------------
void Player::InitializeForArena( Arena& arena, const IntVector2& startingPosition )
{
	m_startingPosition = startingPosition;
	PrepareForBattleFunctionType prepareForBattleFunc = reinterpret_cast< PrepareForBattleFunctionType >( GetProcAddress( m_dllHandle, "PrepareForBattle" ) );
	GiveColorsFunctionType giveColorsFunc = reinterpret_cast< GiveColorsFunctionType >( GetProcAddress( m_dllHandle, "GiveColors" ) );

	ClearReportStructures( arena.m_arenaSquares );

	if( giveColorsFunc )
	{
		giveColorsFunc( m_playerColor.GetAsDwordRGBA() );
	}

	if( prepareForBattleFunc )
	{
		prepareForBattleFunc( m_playerIndex, arena.m_arenaInfo );
	}
	else
	{
		FatalError( "DLL Data Exchange Error", Stringf( "GetProcAddress(\"PrepareForBattle\") FAILED for DLL \"%s\"!\n", m_dllFilePath.c_str() ) );
	}
}


//-----------------------------------------------------------------------------------------------
int Player::CalcNumLivingAgents() const
{
	int numLivingAgents = 0;

	for( int i = 0; i < (int) m_agents.size(); ++ i )
	{
		Agent* agent = m_agents[ i ];
		if( agent && !agent->m_isDead )
		{
			++ numLivingAgents;
		}
	}

	return numLivingAgents;
}


//-----------------------------------------------------------------------------------------------
bool Player::HasLivingAgents() const
{
	for( int i = 0; i < (int) m_agents.size(); ++ i )
	{
		Agent* agent = m_agents[ i ];
		if( agent && !agent->m_isDead )
		{
			return true;
		}
	}

	return false;
}




//-----------------------------------------------------------------------------------------------
void Player::CalculateVisibility( Arena& arena )
{
	m_observedArenaSquares = arena.m_arenaSquares;
	memset( m_isVisibleToPlayer, 0, sizeof( m_isVisibleToPlayer ) );

	int visibilityRangePerAntType[ NUM_AGENT_TYPES ] =
	{
		arena.m_arenaInfo.visibilityRangeSoldier,
		arena.m_arenaInfo.visibilityRangeScout,
		arena.m_arenaInfo.visibilityRangeWorker,
		arena.m_arenaInfo.visibilityRangeQueen,
	};

	for( int i = 0; i < (int) m_agents.size(); ++ i )
	{
		Agent* agentPtr = m_agents[ i ];
		if( agentPtr && !agentPtr->m_isDead )
		{
			Agent& agent = *agentPtr;
			int visibilityRange = visibilityRangePerAntType[ agent.m_agentType ];
			MarkSquaresInManhattanRadiusVisible( agent.m_position, visibilityRange );
		}
	}
}


//-----------------------------------------------------------------------------------------------
void Player::BuildObservations( Arena& arena )
{
	for( int x = 0; x < arena.m_arenaSquares.m_width; ++ x )
	for( int y = 0; y < arena.m_arenaSquares.m_height; ++ y )
	{
		int squareIndex = x + (y * arena.m_arenaSquares.m_width);
		if( m_isVisibleToPlayer[ squareIndex ] == 0 )
		{
			m_observedArenaSquares.m_arenaSquareMaterials[ squareIndex ] = (char) ARENA_SQUARE_TYPE_UNKNOWN;
		}
		else
		{
			m_observedArenaSquares.m_arenaSquareMaterials[ squareIndex ] = arena.m_arenaSquares.m_arenaSquareMaterials[ squareIndex ];
		}
	}

	std::vector< Agent* >::iterator endOfAgents = arena.m_allAgentReferences.end();
	for( std::vector< Agent* >::iterator agentIter = arena.m_allAgentReferences.begin(); agentIter != endOfAgents; ++ agentIter )
	{
		Agent* agentPtr = *agentIter;
		if( !agentPtr )
			continue;

		Agent& agent = *agentPtr;
		if( agent.m_isDead || agent.m_playerIndex == m_playerIndex )
			continue;

		int agentSquareIndex = agent.m_position.x + (agent.m_position.y * arena.m_arenaSquares.m_width);
		if( m_isVisibleToPlayer[ agentSquareIndex ] == 1 )
		{
			m_observedEntities.AddObservedEntity( agent.m_entityID, (char) agent.m_playerIndex, agent.m_agentType, agent.m_specialStatus, agent.m_latestOrderCode, (short) agent.m_position.x, (short) agent.m_position.y );
		}
	}
}


//-----------------------------------------------------------------------------------------------
int Player::ComputeUpkeepCost( Arena& arena )
{
	int upkeepPerAntType[ NUM_AGENT_TYPES ] =
	{
		arena.m_arenaInfo.upkeepNutrientsPerTurnForSoldier,
		arena.m_arenaInfo.upkeepNutrientsPerTurnForScout,
		arena.m_arenaInfo.upkeepNutrientsPerTurnForWorker,
		arena.m_arenaInfo.upkeepNutrientsPerTurnForQueen
	};

	int upkeepCost = 0;
	for( std::vector< Agent* >::iterator agentIter = m_agents.begin(); agentIter != m_agents.end(); ++ agentIter )
	{
		Agent* agent = *agentIter;
		if( agent && !agent->m_isDead )
		{
			upkeepCost += upkeepPerAntType[ agent->m_agentType ];
		}
	}

	return upkeepCost;
}


//-----------------------------------------------------------------------------------------------
void Player::KillAgent( Agent& agent, Arena& arena, ReportCode reasonForDeath )
{
	if( agent.m_specialStatus == ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
	{
		arena.m_arenaSquares.SetSquareTypeAtCoords( (short) agent.m_position.x, (short) agent.m_position.y,  ARENA_SQUARE_TYPE_FOOD );
	}

	agent.m_isDead = true;
	agent.m_specialStatus = ENTITY_SPECIAL_STATUS_DEAD;
	agent.m_timeBornOrDied = Clock::GetAbsoluteTimeSeconds();
	m_agentReports.AddReport( agent.m_entityID, agent.m_agentType, reasonForDeath, (short) agent.m_position.x, (short) agent.m_position.y, agent.m_specialStatus, true );
	const char* reasonForDeathString = GetNameForReportCode( reasonForDeath );
	DebuggerPrintf( "KILLED AGENT #%d (player %d), was %s at (%d,%d) - %s\n", agent.m_entityID, agent.m_playerIndex, GetNameForEntityType( agent.m_agentType ), agent.m_position.x, agent.m_position.y, reasonForDeathString );
}


//-----------------------------------------------------------------------------------------------
void Player::StarveAntIfNoneSuicidedThisTurn( int turnNumber, Arena& arena )
{
	Agent* agentToStarve = NULL;
	int numAgentsOfThisType = 0;
	for( std::vector< Agent* >::iterator agentIter = m_agents.begin(); agentIter != m_agents.end(); ++ agentIter )
	{
		Agent* agent = *agentIter;
		if( !agent )
			continue;

		if( agent->m_latestOrderCode == ORDER_SUICIDE && agent->m_turnNumberLastOrdered == turnNumber )
		{
			return;
		}

		if( agent->m_isDead )
			continue;

		if( !agentToStarve )
		{
			agentToStarve = agent;
			numAgentsOfThisType = 1;
		}
		else
		{
			if( agent->m_agentType < agentToStarve->m_agentType )
			{
				// always starve lower-class agents (soldiers, then scouts) before higher-class agents (workers, then queens)
				agentToStarve = agent;
				numAgentsOfThisType = 1;
			}
			else if( agent->m_agentType == agentToStarve->m_agentType )
			{
				// This agent is the same "class" as the one we are planning to starve; give it a 1/N chance to be the one we switch to
				++ numAgentsOfThisType;
				if( RandomIntLessThan( numAgentsOfThisType ) == 0 )
				{
					agentToStarve = agent;
				}
			}
		}
	}

	if( agentToStarve )
	{
		DebuggerPrintf( "%s ANT #%d (player #%d) starved to death!  Colony has %d nutrients (upkeep is %d).\n", GetNameForEntityType( agentToStarve->m_agentType ), agentToStarve->m_entityID, agentToStarve->m_playerIndex, m_currentNutrients, ComputeUpkeepCost( arena ) );
//		m_agentReports.AddReport( agentToStarve->m_entityID, agentToStarve->m_agentType, REPORT_STARVED_TO_DEATH, (short) agentToStarve->m_position.x, (short) agentToStarve->m_position.y, agentToStarve->m_specialStatus, true );
		KillAgent( *agentToStarve, arena, REPORT_STARVED_TO_DEATH );
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
PluginManager::PluginManager()
{
}


//-----------------------------------------------------------------------------------------------
void PluginManager::LoadPlayers()
{
	std::vector< Rgba > reservedPlayerColors;
	std::vector< std::string > reservedPlayerColorNames;
	reservedPlayerColors.push_back( Rgba( 255, 128, 0 ) );		reservedPlayerColorNames.push_back( "Orange" );
	reservedPlayerColors.push_back( Rgba( 0, 255, 255 ) );		reservedPlayerColorNames.push_back( "Cyan" );
	reservedPlayerColors.push_back( Rgba( 255, 48, 255 ) );		reservedPlayerColorNames.push_back( "Pink" );
	reservedPlayerColors.push_back( Rgba( 255, 255, 0 ) );		reservedPlayerColorNames.push_back( "Yellow" );
	reservedPlayerColors.push_back( Rgba( 0, 255, 0 ) );		reservedPlayerColorNames.push_back( "Green" );
	reservedPlayerColors.push_back( Rgba( 70, 90, 255 ) );		reservedPlayerColorNames.push_back( "Blue" );
	reservedPlayerColors.push_back( Rgba( 153, 51, 255 ) );		reservedPlayerColorNames.push_back( "Purple" );
	reservedPlayerColors.push_back( Rgba( 0, 128, 0 ) );		reservedPlayerColorNames.push_back( "DarkGreen" );
	reservedPlayerColors.push_back( Rgba( 255, 255, 255 ) );	reservedPlayerColorNames.push_back( "White" );
	reservedPlayerColors.push_back( Rgba( 255, 0, 0 ) );		reservedPlayerColorNames.push_back( "Red" );

	std::vector< std::string > dllFileNames = EnumerateFilesInFolder( "Players/*.dll" );
	int numPlayers = (int) dllFileNames.size();
	if( numPlayers == 0 )
	{
		FatalError( "Data / Working Folder Error", "ERROR: Found no player DLLs in the Players/ folder!\n\nNOTE TO DEVELOPERS: If running Arena from the debugger, make sure you set your working folder\n to the folder directly above Players/ and Data/.\n\n In MS Visual Studio, set Arena Project -> Properties -> Configuration Properties -> Debugging -> Working Directory to  $(ProjectDir)../" );
		return;
	}

	DebuggerPrintf( "Loading each of %d DLL(s) found in Players/ folder...\n", dllFileNames.size() );
	for( int dllFileNameIndex = 0; dllFileNameIndex < numPlayers; ++ dllFileNameIndex )
	{
		std::string& dllFileName = dllFileNames[ dllFileNameIndex ];
		DebuggerPrintf( "  > Loading player #%d from DLL \"%s\"...\n", dllFileNameIndex, dllFileName.c_str() );
		Player* player = new Player( dllFileName, dllFileNameIndex );
		if( dllFileNameIndex < (int) reservedPlayerColors.size() )
		{
			player->m_playerColor = reservedPlayerColors[ dllFileNameIndex ];
			player->m_playerColorName = reservedPlayerColorNames[ dllFileNameIndex ];
		}
		m_players.push_back( player );
	}
}


//-----------------------------------------------------------------------------------------------
void PluginManager::ValidatePlayers()
{
	for( int playerIndex = 0; playerIndex < (int) m_players.size(); ++ playerIndex )
	{
		Player* player = m_players[ playerIndex ];
		DebuggerPrintf( "  > Validating player #%d (from %s)...\n", playerIndex, player->m_dllFilePath.c_str() );
		player->Validate();
	}
}


//-----------------------------------------------------------------------------------------------
void PluginManager::AnnouncePlayers()
{
	DebuggerPrintf( "\n************************************************************\n" );
	DebuggerPrintf( "There are a total of %d player(s):\n", m_players.size() );
	for( int playerIndex = 0; playerIndex < (int) m_players.size(); ++ playerIndex )
	{
		Player* player = m_players[ playerIndex ];
		DebuggerPrintf( " %2d> %10.10s: %-25.25s (%s, written by %s)\n", playerIndex, player->m_playerColorName.c_str(), player->m_playerName.c_str(), player->m_dllFileName.c_str(), player->m_authorName.c_str() );
	}
	DebuggerPrintf( "************************************************************\n\n" );
}


//-----------------------------------------------------------------------------------------------
void PluginManager::PreparePlayersForBattle( Arena& arena )
{
	int numPlayers = (int) m_players.size();
	DebuggerPrintf( "Initializing players for new %dx%d arena \"%s\"...\n", arena.m_dimensions.x, arena.m_dimensions.y, arena.m_name.c_str() );

	float degreesPerPlayer = 360.0f / (float) numPlayers;
	float degreesForThisPlayer = 0.f;

	std::vector< int > randomOrderPlayerIndexes;
	int playerIndex;
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		randomOrderPlayerIndexes.push_back( playerIndex );
		int randomSlot = RandomIntLessThan( 1+playerIndex );
		int valueFromRandomSlot = randomOrderPlayerIndexes[ randomSlot ];
		randomOrderPlayerIndexes[ playerIndex ] = valueFromRandomSlot;
		randomOrderPlayerIndexes[ randomSlot ] = playerIndex;
	}

	for( int randomizedIndex = 0; randomizedIndex < numPlayers; ++ randomizedIndex )
	{
		int playerIndex = randomOrderPlayerIndexes[ randomizedIndex ];

		Vector2 arenaCenter( 0.5f * (float) arena.m_dimensions.x, 0.5f * (float) arena.m_dimensions.y );
		float circleRadius = arenaCenter.y * 0.67f;
		Vector2 displacement;
		displacement.SetLengthAndYawDegrees( circleRadius, degreesForThisPlayer );
		Vector2 playerStart = arenaCenter + displacement;

		Player* player = m_players[ playerIndex ];
		IntVector2 startingPosition( (int)playerStart.x, (int)playerStart.y );
		startingPosition.x += RandomIntInRangeInclusive( -3, 3 );
		startingPosition.y += RandomIntInRangeInclusive( -3, 3 );
		startingPosition.x = ClampInt( startingPosition.x, 3, arena.m_dimensions.x-4 );
		startingPosition.y = ClampInt( startingPosition.y, 3, arena.m_dimensions.y-4 );

//		DebuggerPrintf( "  > Player #%d: \"%s\" (%s) - color is %d,%d,%d\n", playerIndex, player->m_playerName.c_str(), player->m_dllFileName.c_str(), player->m_playerColor.r, player->m_playerColor.g, player->m_playerColor.b );
		player->InitializeForArena( arena, startingPosition );

		degreesForThisPlayer += degreesPerPlayer;
	}
//	printf( "\n" );

	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		StartPlayerWorkThread( playerIndex );
	}
}


//-----------------------------------------------------------------------------------------------
double PluginManager::FetchNewOrders( int turnNumber, int& totalOrderCount )
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();

	totalOrderCount = 0;
	int numPlayers = (int) m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *m_players[ playerIndex ];
		Orders newOrdersForPlayer;
		newOrdersForPlayer.m_numberOfOrders = 0;

		if( !player.m_isDefeated )
		{
			player.m_fetchNewOrdersFunc( turnNumber, newOrdersForPlayer );
		}
		player.m_moveOrders = newOrdersForPlayer; // save off a server-side copy for thread/module safety
		totalOrderCount += newOrdersForPlayer.m_numberOfOrders;

		player.ClearReportStructures( theArenaManager->m_currentArenaCopy->m_arenaSquares );
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	return elapsedSeconds;
}


//-----------------------------------------------------------------------------------------------
double PluginManager::SendUpdates( int turnNumber, int& totalReportCount, int& totalObservedEntityCount )
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();

	totalReportCount = 0;
	totalObservedEntityCount = 0;
	int numPlayers = (int) m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *m_players[ playerIndex ];
		if( !player.m_isDefeated )
		{
			player.m_sendUpdateFunc( turnNumber, player.m_currentNutrients, player.m_agentReports, player.m_observedEntities, player.m_observedArenaSquares );
		}
		totalReportCount += player.m_agentReports.m_numberAgentReports;
		totalObservedEntityCount += player.m_observedEntities.m_numberOfObservedEntities;
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	return elapsedSeconds;
}


//-----------------------------------------------------------------------------------------------
void PluginManager::StartPlayerWorkThread( int playerIndex )
{
	Player& player = *m_players[ playerIndex ];
	unsigned int threadID;

	HANDLE threadHandle = (HANDLE) _beginthreadex( NULL, 0, &PlayerWorkThreadEntryFunction, &player, 0, &threadID );
//	SetThreadPriority( threadHandle, THREAD_PRIORITY_BELOW_NORMAL );

	player.m_workThreadHandle = threadHandle;
	player.m_workThreadID = threadID;
}


//-----------------------------------------------------------------------------------------------
void PluginManager::KickPlayerWorkThreadsOffMainCPU()
{
	for( int playerIndex = 0; playerIndex < (int) m_players.size(); ++ playerIndex )
	{
		Player& player = *m_players[ playerIndex ];
		HANDLE threadHandle = player.m_workThreadHandle;
		SetThreadAffinityMask( threadHandle, 0xfffffffe );
	}	
}


//-----------------------------------------------------------------------------------------------
void PluginManager::LetPlayerWorkThreadsBackOnMainCPU()
{
	for( int playerIndex = 0; playerIndex < (int) m_players.size(); ++ playerIndex )
	{
		Player& player = *m_players[ playerIndex ];
		HANDLE threadHandle = player.m_workThreadHandle;
		SetThreadAffinityMask( threadHandle, 0xffffffff );
	}	
}


//-----------------------------------------------------------------------------------------------
void PluginManager::SuspendAllPlayerWorkThreads()
{
	for( int playerIndex = 0; playerIndex < (int) m_players.size(); ++ playerIndex )
	{
		Player& player = *m_players[ playerIndex ];
		HANDLE threadHandle = player.m_workThreadHandle;
		SuspendThread( threadHandle );
	}	
}


//-----------------------------------------------------------------------------------------------
void PluginManager::ResumeAllPlayerWorkThreads()
{
	for( int playerIndex = 0; playerIndex < (int) m_players.size(); ++ playerIndex )
	{
		Player& player = *m_players[ playerIndex ];
		HANDLE threadHandle = player.m_workThreadHandle;
		ResumeThread( threadHandle );
	}
}


//-----------------------------------------------------------------------------------------------
unsigned __stdcall PlayerWorkThreadEntryFunction( void* playerPointer )
{
	Player* player = (Player*)( playerPointer );

	player->m_beginWorkFunc();

	_endthreadex( 0 );
	return 0;
}


