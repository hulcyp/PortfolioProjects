//-----------------------------------------------------------------------------------------------
// TheGame.cpp
//-----------------------------------------------------------------------------------------------
#include "TheGame.hpp"
#include "PluginManager.hpp"
#include "ArenaManager.hpp"
#include "../Engine/Main_Win32.hpp"


//-----------------------------------------------------------------------------------------------
// Global variables
//
TheGame* theGame = NULL;
PluginManager* thePluginManager = NULL;
ArenaManager* theArenaManager = NULL;
bool g_keyDownStates[ 256 ];
bool g_showDebugOverlay = false;
bool g_showPerformanceMessages = false;

const float VIRTUAL_SCREEN_WIDTH = 1024.f;
const float VIRTUAL_SCREEN_HEIGHT = 768.f;
const double MOVE_ANIMATION_SECONDS = 8.0 * MAXIMUM_SECONDS_PER_DLL_CALL;
const double QUEEN_SCORECARD_DAMAGE_FLASH_SECONDS = 0.5;
const double QUEEN_DAMAGE_FLASH_SECONDS = 0.5;
const double QUEEN_ATE_FLASH_SECONDS = 0.25;


//-----------------------------------------------------------------------------------------------
TheGame::TheGame()
	: m_isQuitting( false )
	, m_isPaused( true )
	, m_isToAdvanceOnlyOnce( false )
	, m_frameNumber( 1 )
	, m_turnNumber( 1 )
	, m_labelVerbosity( 2 )
	, m_winningPlayerIndex( -1 )
	, m_debugGraphicsForPlayer( -1 )
	, m_inspectedSquareCoords( IntVector2::ZERO )
{
}


//-----------------------------------------------------------------------------------------------
TheGame::~TheGame()
{
}


//-----------------------------------------------------------------------------------------------
void TheGame::Startup( const std::string& appCommandLine )
{
	UNUSED( appCommandLine );
	DebuggerPrintf( "TheGame::Startup...\n" );

	Clock::InitializeClockSystem();
	InitGraphics( g_displayDeviceContext );
	for( unsigned int i = 0; i < 256; ++ i )
	{
		g_keyDownStates[ i ] = false;
	}

	thePluginManager = new PluginManager;
	theArenaManager = new ArenaManager;

	thePluginManager->LoadPlayers();
	thePluginManager->ValidatePlayers();
	thePluginManager->AnnouncePlayers();
	LoadDataFiles();

	int currentMicroseconds = (int)( 1000000.0 * Clock::GetAbsoluteTimeSeconds() );
	srand( currentMicroseconds );

	if( theArenaManager->m_currentArenaCopy )
	{
		Arena& arena = *theArenaManager->m_currentArenaCopy;
		m_turnNumber = 0;
		thePluginManager->PreparePlayersForBattle( arena );
		arena.InitializeAndBegin();

		CalculateVisibility( arena );
		BuildObservations( arena );

		int totalReportCount = 0;
		int totalObservedEntityCount = 0;
		thePluginManager->SendUpdates( m_turnNumber, totalReportCount, totalObservedEntityCount ); // send "0th" turn updates, to provide DLLs with initial starting conditions
	}
}


//-----------------------------------------------------------------------------------------------
void TheGame::Shutdown()
{
	DebuggerPrintf( "TheGame::Shutdown...\n" );
}


//-----------------------------------------------------------------------------------------------
// Returns true if the app is ready to exit.
//
void TheGame::RunFrame()
{
	++ m_frameNumber;
	SetUpView();

	static double timeLastFrameBegan = Clock::GetAbsoluteTimeSeconds();
	static double timeLastUpdateEnded = Clock::GetAbsoluteTimeSeconds();

	// Delay until minimum time has elapsed
	const double minimumNonUpdateFrameSeconds = 0.020;
	double timeFrameBegan = Clock::GetAbsoluteTimeSeconds();
	if( g_showPerformanceMessages ) DebuggerPrintf( "ARENA {%d @ %.3f}: Starting turn #%d (previous turn took %.3f seconds)\n", m_turnNumber, Clock::GetAbsoluteTimeSeconds(), m_turnNumber, timeFrameBegan - timeLastFrameBegan );
	double deltaSecondsSinceLastUpdateEnded = timeFrameBegan - timeLastUpdateEnded;

	// Spin in place - mostly idle up until the last millisecond - until our minimum non-update frame time has elapsed
	double secondsLeftToBurn = minimumNonUpdateFrameSeconds - deltaSecondsSinceLastUpdateEnded;
	while( secondsLeftToBurn > 0.000 )
	{
		secondsLeftToBurn = minimumNonUpdateFrameSeconds - deltaSecondsSinceLastUpdateEnded;
		if( secondsLeftToBurn > 0.002 )
		{
			int millisecondsToBurn = (int)( 1000.f * secondsLeftToBurn );
			Sleep( millisecondsToBurn );
// 			static int playerNumToSuspend = 0;
// 			++ playerNumToSuspend;
// 			playerNumToSuspend %= (int) thePluginManager->m_players.size();
//			SuspendThread( thePluginManager->m_players[ playerNumToSuspend ]->m_workThreadHandle );
//			Sleep( 1 );
//			ResumeThread( thePluginManager->m_players[ playerNumToSuspend ]->m_workThreadHandle );
		}
		else if( secondsLeftToBurn > 0.001 )
		{
			Sleep( 0 );
		}

		double timeNow = Clock::GetAbsoluteTimeSeconds();
		deltaSecondsSinceLastUpdateEnded = timeNow - timeLastUpdateEnded;
	}

	timeLastFrameBegan = timeFrameBegan;

	Update();

	timeLastUpdateEnded = Clock::GetAbsoluteTimeSeconds();

	Render();
	DrawDebugGraphics();
	SwapBuffers( g_displayDeviceContext );
}


//-----------------------------------------------------------------------------------------------
void TheGame::Update()
{
	if( !IsGamePaused() )
	{
		Arena* arena = theArenaManager->m_currentArenaCopy;

		++ m_turnNumber;
		if( arena )
		{
			if( arena->m_arenaInfo.numTurnsBeforeSuddenDeath == m_turnNumber )
			{
				DebuggerPrintf( "************************************************************\n" );
				DebuggerPrintf( " Turn #%d: SUDDEN DEATH BEGINS!  No more food will spawn.\n", m_turnNumber );
				DebuggerPrintf( "************************************************************\n" );
			}
			else if( m_turnNumber % 100 == 0 )
			{
				DebuggerPrintf( "Entering turn #%d%s.\n", m_turnNumber, m_turnNumber >= arena->m_arenaInfo.numTurnsBeforeSuddenDeath ? " (SUDDEN DEATH!)" : "" );
			}
		}

		int totalOrderCount = 0;
		int totalReportCount = 0;
		int totalObservedEntityCount = 0;
		double elapsedSeconds = 0;

		if( g_showPerformanceMessages ) DebuggerPrintf( "ARENA {%d @ %.3f}: Fetching new orders...\n", m_turnNumber, Clock::GetAbsoluteTimeSeconds() );
		elapsedSeconds = thePluginManager->FetchNewOrders( m_turnNumber, totalOrderCount );

		if( g_showPerformanceMessages ) DebuggerPrintf( "ARENA {%d @ %.3f}: Adjudicating... (fetched %d orders in %.3f ms)\n", m_turnNumber, Clock::GetAbsoluteTimeSeconds(), totalOrderCount, elapsedSeconds * 1000.0 );
		elapsedSeconds = AdjudicateTurn();

		if( g_showPerformanceMessages ) DebuggerPrintf( "ARENA {%d @ %.3f}: Sending updates... (adjudicate took %.3f ms)\n", m_turnNumber, Clock::GetAbsoluteTimeSeconds(), elapsedSeconds * 1000.0 );
		elapsedSeconds = thePluginManager->SendUpdates( m_turnNumber, totalReportCount, totalObservedEntityCount );

		if( g_showPerformanceMessages ) DebuggerPrintf( "ARENA {%d @ %.3f}: ...end of turn %d (sent %d reports, %d observed entities in %.3f ms)\n", m_turnNumber, Clock::GetAbsoluteTimeSeconds(), m_turnNumber, totalReportCount, totalObservedEntityCount, elapsedSeconds * 1000.0 );

		if( m_isToAdvanceOnlyOnce )
		{
			m_isToAdvanceOnlyOnce = false;
			SetGamePaused( true );
		}
	}
}


//-----------------------------------------------------------------------------------------------
void TheGame::SetUpView()
{
	glLoadIdentity();
	glOrtho( 0.f, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT, 0.f, -1.f, 1.f );
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//-----------------------------------------------------------------------------------------------
void TheGame::Render()
{
	if( theArenaManager->m_currentArenaCopy )
	{
		double elapsedSeconds = 0.0;
		Arena& arena = *theArenaManager->m_currentArenaCopy;
		Vector2 fDimensions( (float)arena.m_dimensions.x, (float)arena.m_dimensions.y );
		Vector2 virtualScreenSize( VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT );
		float scale = fDimensions.CalcScaleRequiredToFitInsideBox( virtualScreenSize );

		elapsedSeconds = arena.Render( scale );

		Vector2 cursorWorldPos = GetCursorWorldPos();
		m_inspectedSquareCoords.SetXY( (int)cursorWorldPos.x, (int)cursorWorldPos.y );

		glPushMatrix();
		glScalef( scale, scale, 1.f );
		glTranslatef( 0.5f, 0.5f, 0.f );

//		Vector2 drawCenter( RoundToNearestInt( m_inspectedSquareCoords.x ), RoundToNearestInt( m_inspectedSquareCoords.y ) );
//		DrawOutlinedArea( drawCenter, 1.f, Rgba::PURPLE );

		for( int playerIndex = 0; playerIndex < (int) thePluginManager->m_players.size(); ++ playerIndex )
		{
			Player& player = *thePluginManager->m_players[ playerIndex ];
			if( m_debugGraphicsForPlayer == player.m_playerIndex && player.m_isDebugDrawEnabled && player.m_drawDebugOverlayFunc )
			{
				player.m_drawDebugOverlayFunc( (short) m_inspectedSquareCoords.x, (short) m_inspectedSquareCoords.y );
			}
		}
		glPopMatrix();

		elapsedSeconds = RenderScorecard( scale );
		UNUSED( elapsedSeconds );
	}
}


//-----------------------------------------------------------------------------------------------
double TheGame::RenderScorecard( float virtualCoordsPerSquare )
{
	const int numPlayers = (int) thePluginManager->m_players.size();
	const int numColumnsPerPlayer = 2;

	int playerIndex;
	int numPlayersAlive = 0;
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		if( !player.m_isDefeated )
			++ numPlayersAlive;
	}

	Arena& arena = *theArenaManager->m_currentArenaCopy;
	UNUSED( virtualCoordsPerSquare );
	double timeNow = Clock::GetAbsoluteTimeSeconds();
	double startSeconds = timeNow;

	const float entitiesAreaTopPadding = 2.f * MAIN_FONT_HEIGHT + 4.f;
	AABB2 scorecardBounds( VIRTUAL_SCREEN_HEIGHT, 0.f, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT );
	Vector2 scorecardSize = scorecardBounds.CalcSize();
	Vector2 pieGraphSize( scorecardSize.x, scorecardSize.x );
	Vector2 namesAreaSize( VIRTUAL_SCREEN_HEIGHT, (float)( MAIN_FONT_HEIGHT * thePluginManager->m_players.size() ) );
	AABB2 entitiesAreaBounds( scorecardBounds.mins.x, scorecardBounds.mins.y + entitiesAreaTopPadding, scorecardBounds.maxs.x, scorecardBounds.maxs.y - scorecardSize.x - namesAreaSize.y );
	AABB2 pieGraphBounds( scorecardBounds.mins.x, scorecardBounds.maxs.y - pieGraphSize.y, scorecardBounds.maxs.x, scorecardBounds.maxs.y );
	AABB2 namesAreaBounds( entitiesAreaBounds.mins.x, pieGraphBounds.mins.y - namesAreaSize.y, entitiesAreaBounds.maxs.x, pieGraphBounds.mins.y );
	Vector2 entitiesAreaSize = entitiesAreaBounds.CalcSize();

	const int numEntitiesPerColumn = MAX_AGENTS_PER_PLAYER / numColumnsPerPlayer;
	const float entitiestrideSpacing = entitiesAreaSize.y / (float)( numEntitiesPerColumn );
	const float entityDiameter = 0.8f * entitiestrideSpacing;
	const float entityRadius = 0.5f * entityDiameter;

	Rgba shadowColor( 0, 0, 0, 255 );
	Vector2 shadowOffset( 2.f, 2.f );

	const float strideBetweenEntitiesY = entitiestrideSpacing;
	float strideBetweenEntitiesX = entitiestrideSpacing;
	const float maxStrideBetweenEntitiesX = entitiesAreaSize.x / (float)(((1 + numColumnsPerPlayer) * numPlayers) );
	if( strideBetweenEntitiesX > maxStrideBetweenEntitiesX )
		strideBetweenEntitiesX = maxStrideBetweenEntitiesX;

	const float strideBetweenPlayersX = entitiesAreaSize.x / (float)(1 + numPlayers);

	const int MAX_NUTRIENTS_SHOWN = 500000; // this "maxes out" the nutrients meter
	const int SHOW_NUTRIENTS_LINE_EVERY = 10000;

//	int playerIndex;
//	int numPlayersAlive = 0;
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
//		if( !player.m_isDefeated )
//			++ numPlayersAlive;

		Rgba& color = player.m_playerColor;
		Rgba deadColor( color );
		unsigned char lum = deadColor.CalcLuminance();
		deadColor.SetLuminance( lum / 2 );

		if( m_labelVerbosity >= 1 )
		{
			std::string nameToPrint = Stringf( "#%d - %s", player.m_playerIndex, player.m_playerName.c_str() );
			Rgba nameColor = player.m_isDefeated ? deadColor : color;
			IntVector2 namePosition( 10 + (int) namesAreaBounds.mins.x, 15 + ( MAIN_FONT_HEIGHT * playerIndex ) + (int) namesAreaBounds.mins.y );
			if( player.m_isDefeated )
			{
				namePosition.x += 20;
				nameToPrint += " (dead)";
			}

			Draw2DText( nameToPrint, namePosition, nameColor, 1.f );
		}

		int numAliveEntities = player.CalcNumLivingAgents();
		float fractionOfMaxNutrientsOwned = (float) player.m_currentNutrients / (float) MAX_NUTRIENTS_SHOWN;
		fractionOfMaxNutrientsOwned = ClampFloat( fractionOfMaxNutrientsOwned, 0.f, 1.f );
		float logarithmicFraction = 1.f - (1.f - fractionOfMaxNutrientsOwned) * (1.f - fractionOfMaxNutrientsOwned) * (1.f - fractionOfMaxNutrientsOwned) * (1.f - fractionOfMaxNutrientsOwned);
		float widthOfPlayerEntitiesVisibleColumns = strideBetweenEntitiesX * numColumnsPerPlayer;
		float offsetWithinEntitiesAreaX = (strideBetweenPlayersX * (float)( 1 + playerIndex )) - (0.5f * widthOfPlayerEntitiesVisibleColumns); // strideBetweenEntitiesX * (float) (1 + ((1 + numColumnsPerPlayer) * playerIndex)); // strideBetweenPlayersX * (float)( playerIndex + 1 );

		float offsetWithinEntitiesAreaY = entitiesAreaSize.y; // strideBetweenEntitiesY * (float)(numAliveEntities/2 - 1);
		Vector2 lineLeft = entitiesAreaBounds.mins + Vector2( offsetWithinEntitiesAreaX, offsetWithinEntitiesAreaY );
		Vector2 lineRight = lineLeft + Vector2( widthOfPlayerEntitiesVisibleColumns, 0.f );
		const float hashRadius = (0.25f * entitiestrideSpacing);

		Rgba playerFoodMeterBarFillColor( 10, 50, 10 );
		Rgba playerFoodMeterBarEdgeColor( 30, 80, 30 );
		Rgba playerFoodMeterBarHashColor( 80, 120, 80 );

		AABB2 playerFoodMeterBounds;
		playerFoodMeterBounds.mins.x = lineLeft.x - hashRadius;
		playerFoodMeterBounds.maxs.x = lineRight.x + hashRadius;
		playerFoodMeterBounds.mins.y = entitiesAreaBounds.mins.y;
		playerFoodMeterBounds.maxs.y = playerFoodMeterBounds.mins.y + (logarithmicFraction * entitiesAreaSize.y);

		if( m_labelVerbosity >= 1 && !player.m_isDefeated )
		{
			std::string nameToPrint = Stringf( "#%d", player.m_playerIndex );
			IntVector2 namePosition( 3 + (int) playerFoodMeterBounds.mins.x, MAIN_FONT_HEIGHT - 2 );
			Draw2DText( nameToPrint, namePosition, color, 1.f );

			if( m_labelVerbosity >= 2 )
			{
				std::string nutrientsString;
				if( player.m_currentNutrients > 999 )
				{
					nutrientsString = Stringf( "%dk", player.m_currentNutrients / 1000 );
				}
				else
				{
					nutrientsString = Stringf( "%d", player.m_currentNutrients );
				}

				int leftPadding = nutrientsString.length() > 3 ? 2 : 5;
//				IntVector2 stringPosition( 3 + (int) playerFoodMeterBounds.mins.x, (int) playerFoodMeterBounds.maxs.y + MAIN_FONT_HEIGHT - 2 );
				IntVector2 stringPosition( leftPadding + (int) playerFoodMeterBounds.mins.x, 2 * MAIN_FONT_HEIGHT - 2 );
				Draw2DText( nutrientsString, stringPosition, playerFoodMeterBarHashColor, 1.f );
			}
		}

		double secondsSinceLastDamaged = timeNow - player.m_timeQueenWasLastAssaulted;
		float fractionOfQueenDamageFlash = 1.f - ClampFloat( (float)( secondsSinceLastDamaged / QUEEN_SCORECARD_DAMAGE_FLASH_SECONDS ), 0.f, 1.f );
		if( fractionOfQueenDamageFlash > 0.f )
		{
			fractionOfQueenDamageFlash *= fractionOfQueenDamageFlash;
			playerFoodMeterBarFillColor.InterpolateTo( Rgba( 180, 0, 0 ), fractionOfQueenDamageFlash );
			playerFoodMeterBarEdgeColor.InterpolateTo( Rgba( 255, 0, 0 ), fractionOfQueenDamageFlash );
		}

		DrawFilledArea( playerFoodMeterBounds, playerFoodMeterBarFillColor );
		DrawOutlinedArea( playerFoodMeterBounds, playerFoodMeterBarEdgeColor );

		const int maximumToDisplay = MinInt( MAX_NUTRIENTS_SHOWN, player.m_currentNutrients );
		for( int nutrientsBarHashAmount = SHOW_NUTRIENTS_LINE_EVERY; nutrientsBarHashAmount <= maximumToDisplay; nutrientsBarHashAmount += SHOW_NUTRIENTS_LINE_EVERY )
		{
			const float nutrientsBarFraction = (float) nutrientsBarHashAmount / (float) MAX_NUTRIENTS_SHOWN;
			const float nutrientsBarLogFraction = 1.f - (1.f - nutrientsBarFraction) * (1.f - nutrientsBarFraction) * (1.f - nutrientsBarFraction) * (1.f - nutrientsBarFraction);
			const float hashY = playerFoodMeterBounds.mins.y + (nutrientsBarLogFraction * entitiesAreaSize.y);
			Vector2 leftHashMarkLeft( playerFoodMeterBounds.mins.x - hashRadius, hashY );
			Vector2 leftHashMarkRight( lineLeft.x, hashY );
			Vector2 rightHashMarkLeft( lineRight.x, hashY );
			Vector2 rightHashMarkRight( playerFoodMeterBounds.maxs.x + hashRadius, hashY );
			DrawShadedLineSegment( leftHashMarkLeft, rightHashMarkRight, playerFoodMeterBarHashColor, playerFoodMeterBarHashColor );
			DrawShadedLineSegment( leftHashMarkLeft, leftHashMarkRight, playerFoodMeterBarEdgeColor, playerFoodMeterBarEdgeColor );
			DrawShadedLineSegment( rightHashMarkLeft, rightHashMarkRight, playerFoodMeterBarEdgeColor, playerFoodMeterBarEdgeColor );
		}

		if( numAliveEntities == MAX_AGENTS_PER_PLAYER )
		{
			DrawShadedLineSegment( lineLeft, lineRight, color, color );
		}

		int numEntitiesOfEachType[ NUM_ENTITY_TYPES ] = {0, 0, 0, 0};
		int numEntitiesCarryingFood = 0;
		for( int entityIndex = 0; entityIndex < (int) player.m_agents.size(); ++ entityIndex )
		{
			Agent* agent = player.m_agents[ entityIndex ];
			if( agent && !agent->m_isDead )
			{
				++ numEntitiesOfEachType[ agent->m_agentType ];
				if( agent->m_specialStatus == ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
					++ numEntitiesCarryingFood;
			}
		}

		const Vector2 shadowOfs1( 1.f, 0.f );
		const Vector2 shadowOfs2( 0.f, 1.f );

		int drawCounter = 0;
		for( int entityType = NUM_ENTITY_TYPES-1; entityType >= 0; -- entityType )
		{
			for( int typedDrawCounter = 0; typedDrawCounter < numEntitiesOfEachType[ entityType ]; ++ typedDrawCounter, ++ drawCounter )
			{
				float offsetWithinEntitiesAreaY = strideBetweenEntitiesY * (float)( drawCounter / numColumnsPerPlayer );
				Vector2 entityMins = entitiesAreaBounds.mins + Vector2( offsetWithinEntitiesAreaX, offsetWithinEntitiesAreaY );
				int columnNumber = drawCounter % numColumnsPerPlayer;
				entityMins.x += entitiestrideSpacing * (float)( columnNumber );
				Vector2 entityCenter = entityMins + Vector2( entityRadius, entityRadius );

				if( entityType == ENTITY_TYPE_QUEEN )
				{
					DrawFilledCircle( entityCenter + shadowOfs1, entityRadius, Rgba::BLACK );
					DrawFilledCircle( entityCenter + shadowOfs2, entityRadius, Rgba::BLACK );
					DrawFilledCircle( entityCenter, entityRadius, color);
				}
				else if( entityType == ENTITY_TYPE_WORKER )
				{
					DrawOutlinedDiamond( entityCenter + shadowOfs1, entityRadius, Rgba::BLACK );
					DrawOutlinedDiamond( entityCenter + shadowOfs2, entityRadius, Rgba::BLACK );
					if( typedDrawCounter < numEntitiesCarryingFood )
					{
						DrawFilledDiamond( entityCenter, 0.5f * entityRadius, Rgba( 0, 200, 0 ) );
					}
					else
					{
						DrawFilledDiamond( entityCenter, entityRadius, Rgba::BLACK );
					}
					DrawOutlinedDiamond( entityCenter, entityRadius, color );
				}
				else if( entityType == ENTITY_TYPE_SOLDIER )
				{
					DrawX( entityCenter + shadowOfs1, entityRadius, Rgba::BLACK );
					DrawX( entityCenter + shadowOfs2, entityRadius, Rgba::BLACK );
					DrawX( entityCenter, entityRadius, color );
				}
				else if( entityType == ENTITY_TYPE_SCOUT )
				{
					DrawPlus( entityCenter + shadowOfs1, entityRadius, Rgba::BLACK );
					DrawPlus( entityCenter + shadowOfs2, entityRadius, Rgba::BLACK );
					DrawPlus( entityCenter, entityRadius, color );
				}
			}
		}
	}

	// RENDER PIE CHART for NUTRIENTS
	Vector2 pieGraphCenter = pieGraphBounds.CalcCenter();
	float pieRadius = 0.9f * (pieGraphCenter.y - pieGraphBounds.mins.y);

	int totalNutrientsForAllPlayers = 0;
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		totalNutrientsForAllPlayers += player.m_currentNutrients;
	}

	float startDegrees = 0.f;
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		Rgba& color = player.m_playerColor;
		int nutrients = player.m_currentNutrients;
		if( playerIndex == m_winningPlayerIndex || (numPlayersAlive == 1 && !player.m_isDefeated) )
		{
//			DrawFilledDiamond( pieGraphCenter, pieRadius, color );
//			Rgba darkColor( color );
//			darkColor.InterpolateTo( Rgba::BLACK, 0.7f );
			Rgba darkColor( Rgba::BLACK );
			DrawFilledCircle( pieGraphCenter, pieRadius, color );
			DrawFilledCircle( pieGraphCenter, pieRadius * 0.92f, darkColor );
			DrawFilledCircle( pieGraphCenter, pieRadius * 0.6f, color );
			DrawFilledCircle( pieGraphCenter, pieRadius * 0.52f, darkColor );
			if( m_winningPlayerIndex == -1 && numPlayers > 1 )
			{
				m_isPaused = true;
				m_winningPlayerIndex = playerIndex;
//				if( m_debugGraphicsForPlayer == -1 )
//					m_debugGraphicsForPlayer = m_winningPlayerIndex;

				DebuggerPrintf( "==============================================================================\n" );
				DebuggerPrintf( "VICTORY for \"%s\" (%s) on Turn %d - %s, written by %s\n", player.m_playerName.c_str(), player.m_playerColorName.c_str(), m_turnNumber, player.m_dllFileName.c_str(), player.m_authorName.c_str() );
				DebuggerPrintf( "Map: %s (%dx%d, %d players)\n", arena.m_name.c_str(), arena.m_arenaInfo.width, arena.m_arenaInfo.height, numPlayers );
				DebuggerPrintf( "==============================================================================\n" );
			}
		}
		else if( nutrients > 0 )
		{
			float fractionOfPopulation = ((float) nutrients) / ((float) totalNutrientsForAllPlayers);
			float endDegrees = startDegrees + (360.f * fractionOfPopulation);
			DrawFilledPiePiece( pieGraphCenter, pieRadius, startDegrees, endDegrees, 0.01f * pieRadius, color );
			startDegrees = endDegrees;
		}
	}

	AABB2 squareArea( pieGraphCenter );
	squareArea.AddPadding( 0.15f * pieRadius, 0.15f * pieRadius );
	DrawFilledCircle( pieGraphCenter, 0.30f * pieRadius, Rgba::BLACK, 1.f );
	if( m_turnNumber > arena.m_arenaInfo.numTurnsBeforeSuddenDeath )
	{
		DrawFilledDiamond( pieGraphCenter, 0.20f * pieRadius, Rgba::RED, 1.f );
		DrawFilledArea( squareArea, Rgba::RED, 1.f );
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	return elapsedSeconds;
}


//-----------------------------------------------------------------------------------------------
void TheGame::DrawDebugGraphics()
{
}


//-----------------------------------------------------------------------------------------------
void TheGame::SetGamePaused( bool makePaused )
{
	m_isPaused = makePaused;
	DebuggerPrintf( "Game is now %s.\n", m_isPaused ? "PAUSED" : "RUNNING" );
}



//-----------------------------------------------------------------------------------------------
double TheGame::AdjudicateTurn()
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();
	if( !theArenaManager->m_currentArenaCopy )
		return 0.0;

	Arena& arena = *theArenaManager->m_currentArenaCopy;
	thePluginManager->KickPlayerWorkThreadsOffMainCPU();

	arena.SpawnNewFoodMaybe( m_turnNumber );
	ApplyMoveOrders( arena );
	AddMissingReports( arena );
	RunCombatAndUpkeep( arena );
	CalculateVisibility( arena );
	BuildObservations( arena );
	CheckForDefeatedPlayers( arena );

	thePluginManager->LetPlayerWorkThreadsBackOnMainCPU();
	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	return elapsedSeconds;
}


//-----------------------------------------------------------------------------------------------
void TheGame::ApplyMoveOrders( Arena& arena )
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();

	m_potentialPassThroughCombatGroups.clear();
	int orderCount = 0;
	const int numPlayers = (int) thePluginManager->m_players.size();
	int playerIndex;

	// First pass through all agents simultaneously: DROP FOOD
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		for( int i = 0; i < (int) player.m_moveOrders.m_numberOfOrders; ++ i )
		{
			AgentOrder& order = player.m_moveOrders.m_orders[ i ];
			if( order.m_orderCode == ORDER_DROP_FOOD )
			{
				arena.ApplyMoveOrderForPlayer( order, player );
			}
		}
	}

	// Second pass through all agents simultaneously: HOLD, MOVE, and SUICIDE orders only
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		for( int i = 0; i < (int) player.m_moveOrders.m_numberOfOrders; ++ i )
		{
			++ orderCount;
			AgentOrder& order = player.m_moveOrders.m_orders[ i ];
			if( order.m_orderCode == ORDER_HOLD ||
				order.m_orderCode == ORDER_MOVE_EAST ||
				order.m_orderCode == ORDER_MOVE_NORTH ||
				order.m_orderCode == ORDER_MOVE_WEST ||
				order.m_orderCode == ORDER_MOVE_SOUTH ||
				order.m_orderCode == ORDER_SUICIDE )
			{
				arena.ApplyMoveOrderForPlayer( order, player );
			}
		}
	}

	// Third pass; allow queens to eat food in their square
	for( int allAgentsIndex = 0; allAgentsIndex < (int) arena.m_allAgentReferences.size(); ++ allAgentsIndex )
	{
		Agent* thisAgent = arena.m_allAgentReferences[ allAgentsIndex ];
		if( thisAgent && !thisAgent->m_isDead && thisAgent->m_agentType == ENTITY_TYPE_QUEEN )
		{
			short x = (short) thisAgent->m_position.x;
			short y = (short) thisAgent->m_position.y;
			ArenaSquareType squareType = arena.m_arenaSquares.GetSquareTypeAtCoords( x, y );
			if( squareType == ARENA_SQUARE_TYPE_FOOD )
			{
				arena.m_arenaSquares.SetSquareTypeAtCoords( x, y, ARENA_SQUARE_TYPE_AIR );
				Player& queenFactionPlayer = *thePluginManager->m_players[ thisAgent->m_playerIndex ];
				queenFactionPlayer.m_currentNutrients += arena.m_arenaInfo.nutrientsEarnedPerFoodEatenByQueen;

				AgentReport queenAteFoodReport;
				queenAteFoodReport.m_agentType = (char) thisAgent->m_agentType;
				queenAteFoodReport.m_entityID = thisAgent->m_entityID;
				queenAteFoodReport.m_newPositionX = (short) thisAgent->m_position.x;
				queenAteFoodReport.m_newPositionY = (short) thisAgent->m_position.y;
				queenAteFoodReport.m_reportCode = REPORT_QUEEN_WAS_FED;
				queenAteFoodReport.m_specialStatus = (char) thisAgent->m_specialStatus;
				queenFactionPlayer.AddAgentReport( queenAteFoodReport );
			}
		}		
	}

	// Fourth pass through all agents simultaneously: TAKE FOOD, and CREATE_XXX
	for( playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		for( int i = 0; i < (int) player.m_moveOrders.m_numberOfOrders; ++ i )
		{
			AgentOrder& order = player.m_moveOrders.m_orders[ i ];
			if( order.m_orderCode == ORDER_TAKE_FOOD ||
				order.m_orderCode == ORDER_CREATE_SCOUT ||
				order.m_orderCode == ORDER_CREATE_WORKER ||
				order.m_orderCode == ORDER_CREATE_SOLDIER )
			{
				arena.ApplyMoveOrderForPlayer( order, player );
			}
		}
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	if( g_showPerformanceMessages ) DebuggerPrintf( "  Adjudicate: Applied %d orders in %.0f ms.\n", orderCount, 1000.f * (float) elapsedSeconds );
}


//-----------------------------------------------------------------------------------------------
void TheGame::AddMissingReports( Arena& arena )
{
	UNUSED( arena );

	int numPlayers = (int) thePluginManager->m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		for( int agentIndex = 0; agentIndex < (int) player.m_agents.size(); ++ agentIndex )
		{
			Agent* agent = player.m_agents[ agentIndex ];
			if( agent && !agent->m_isDead )
			{
				if( agent->m_turnNumberLastOrdered != m_turnNumber )
				{
					AgentReport agentStillAliveButDidNothingReport;
					agentStillAliveButDidNothingReport.m_agentType = (char) agent->m_agentType;
					agentStillAliveButDidNothingReport.m_entityID = agent->m_entityID;
					agentStillAliveButDidNothingReport.m_newPositionX = (short) agent->m_position.x;
					agentStillAliveButDidNothingReport.m_newPositionY = (short) agent->m_position.y;
					agentStillAliveButDidNothingReport.m_reportCode = REPORT_HOLD_SUCCESSFUL;
					agentStillAliveButDidNothingReport.m_specialStatus = (char) agent->m_specialStatus;
					agent->m_turnNumberLastOrdered = m_turnNumber;
					player.AddAgentReport( agentStillAliveButDidNothingReport );
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------
void TheGame::RunCombatAndUpkeep( Arena& arena )
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();

	RunCombat( arena );
	CheckForQueensWithHostileAntsOnThem( arena );

	int numPlayers = (int) thePluginManager->m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		int upkeepNutrientCostThisTurn = player.ComputeUpkeepCost( arena );
		player.m_currentNutrients -= upkeepNutrientCostThisTurn;
		if( player.m_currentNutrients <= 0 )
		{
			player.m_currentNutrients = 0;
			player.StarveAntIfNoneSuicidedThisTurn( m_turnNumber, arena );
		}
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	if( g_showPerformanceMessages ) DebuggerPrintf( "  Adjudicate: Ran combat and upkeep in %.0f ms.\n", 1000.f * (float) elapsedSeconds );
}


//-----------------------------------------------------------------------------------------------
void TheGame::CheckForQueensWithHostileAntsOnThem( Arena& arena )
{
	double timeNow = Clock::GetAbsoluteTimeSeconds();
	int numPlayers = (int) thePluginManager->m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		for( int agentIndex = 0; agentIndex < (int) player.m_agents.size(); ++ agentIndex )
		{
			Agent* queen = player.m_agents[ agentIndex ];
			if( queen && !queen->m_isDead && queen->m_agentType == ENTITY_TYPE_QUEEN )
			{
				bool wasQueenAssaulted = false;
				int squareIndex = queen->m_position.x + (queen->m_position.y * arena.m_arenaInfo.width);
				std::vector< Agent* >& agentsHere = arena.m_arenaEntitiesByLocation[ squareIndex ];
				for( int agentsHereIndex = 0; agentsHereIndex < (int) agentsHere.size(); ++ agentsHereIndex )
				{
					Agent* possibleEnemy = agentsHere[ agentsHereIndex ];
					if( possibleEnemy && !possibleEnemy->m_isDead && possibleEnemy->m_playerIndex != queen->m_playerIndex )
					{
						player.m_currentNutrients -= (5 * possibleEnemy->m_strength);
						wasQueenAssaulted = true;
					}
				}

				if( wasQueenAssaulted )
				{
					AgentReport queenWasAssaultedReport;
					queenWasAssaultedReport.m_agentType = ENTITY_TYPE_QUEEN;
					queenWasAssaultedReport.m_entityID = queen->m_entityID;
					queenWasAssaultedReport.m_reportCode = REPORT_QUEEN_WAS_ASSAULTED;
					queenWasAssaultedReport.m_newPositionX = (short) queen->m_position.x;
					queenWasAssaultedReport.m_newPositionY = (short) queen->m_position.y;
					queenWasAssaultedReport.m_specialStatus = (char) queen->m_specialStatus;
					player.m_agentReports.AddReport( queenWasAssaultedReport );
					player.m_timeQueenWasLastAssaulted = Clock::GetAbsoluteTimeSeconds();
					queen->m_timeLastAssaulted = timeNow;
					player.m_turnQueenWasLastAssaulted = m_turnNumber;
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------
void TheGame::RunCombat( Arena& arena )
{
	std::vector< std::vector< Agent* > > combatGroups;

	// Resolve any "in passing" combats that might have happened due to enemy agents exchanging positions
	std::map< unsigned int, std::vector< Agent* > >::iterator passThroughSpecialCombatIter = m_potentialPassThroughCombatGroups.begin();
	for( ; passThroughSpecialCombatIter != m_potentialPassThroughCombatGroups.end(); ++ passThroughSpecialCombatIter )
	{
		std::vector< Agent* >& involvedActors = passThroughSpecialCombatIter->second;
		ResolveCombatGroup( arena, involvedActors );
	}

	// Check for agents of mixed faction cohabiting in the same square
	combatGroups.clear();
	std::vector< Agent* >::iterator agentIter = arena.m_allAgentReferences.begin();
	std::vector< Agent* >::iterator endOfAgents = arena.m_allAgentReferences.end();
	for( ; agentIter != endOfAgents; ++ agentIter )
	{
		Agent* agent = *agentIter;
		if( !agent || agent->m_isDead )
			continue;

		int squareIndex = agent->m_position.x + (agent->m_position.y * arena.m_arenaSquares.m_width);
		std::vector< Agent* >& agentsHere = arena.m_arenaEntitiesByLocation[ squareIndex ];

		int numAgentsHere = (int) agentsHere.size();
		int playerIndexAlreadySeen = agent->m_playerIndex;
		bool isLegitUniqueCombatGroup = false;
		for( int agentHereIndex = 0; agentHereIndex < numAgentsHere; ++ agentHereIndex )
		{
			Agent* agentHere = agentsHere[ agentHereIndex ];
			if( !agentHere || agentHere == agent || agentHere->m_isDead )
				continue;

			if( agentHere->m_entityID < agent->m_entityID )
			{
				isLegitUniqueCombatGroup = false; // only allow this group to be considered once (and "owned" by the lowest-ID living agent present)
				break;
			}

			if( !isLegitUniqueCombatGroup && agentHere->m_playerIndex != playerIndexAlreadySeen )
			{
				isLegitUniqueCombatGroup = true; // must see at least one agent who is an enemy of the owner
			}
		}

		if( isLegitUniqueCombatGroup )
		{
			combatGroups.push_back( agentsHere );
		}
	}

	for( int combatGroupIndex = 0; combatGroupIndex < (int) combatGroups.size(); ++ combatGroupIndex )
	{
		std::vector< Agent* >& combatGroup = combatGroups[ combatGroupIndex ];
		ResolveCombatGroup( arena, combatGroup );
	}
}


//-----------------------------------------------------------------------------------------------
void TheGame::ResolveCombatGroup( Arena& arena, std::vector< Agent* >& combatGroup )
{
	// First, check if everyone here is from the same faction (common case, and early out)
	bool isValidPotentialCombat = false;
	for( int i = 1; i < (int) combatGroup.size(); ++ i )
	{
		if( combatGroup[ i ]->m_playerIndex != combatGroup[ 0 ]->m_playerIndex )
		{
			isValidPotentialCombat = true;
			break;
		}
	}

	if( !isValidPotentialCombat )
		return;

	// Sort into by-faction groups, leaving out any dead agents
	int numPlayers = (int) thePluginManager->m_players.size();
	std::vector< std::vector< Agent* > > liveAgentsByFaction;
	liveAgentsByFaction.resize( numPlayers );
	int numAgentsInGroup = (int) combatGroup.size();
	for( int agentIndex = 0; agentIndex < numAgentsInGroup; ++ agentIndex )
	{
		Agent* agent = combatGroup[ agentIndex ];
		if( agent && !agent->m_isDead )
		{
			// Insert this agent into his faction's list of live combatants for this combat
			bool wasInserted = false;
			std::vector< Agent* >& agentsInThisFaction = liveAgentsByFaction[ agent->m_playerIndex ];
			std::vector< Agent* >::iterator agentInThisFactionIter = agentsInThisFaction.begin();
			for( ; agentInThisFactionIter != agentsInThisFaction.end(); ++ agentInThisFactionIter )
			{
				Agent* compareAgainstFriendlyAgent = *agentInThisFactionIter;
				if( agent->m_strength <= compareAgainstFriendlyAgent->m_strength )
				{
					agentsInThisFaction.insert( agentInThisFactionIter, agent );
					wasInserted = true;
					break;
				}
			}

			if( !wasInserted )
			{
				// This must be the strongest (or first) ant of this faction yet seen so far in this combat group
				agentsInThisFaction.push_back( agent );
			}
		}
	}

	// Collapse the factions list down to only those with at least one living ant in them
	int numFactionsRemainingInCombat = numPlayers;
	int factionPlayerIndex;
	for( factionPlayerIndex = 0; factionPlayerIndex < numFactionsRemainingInCombat; ++ factionPlayerIndex )
	{
		if( liveAgentsByFaction[ factionPlayerIndex ].empty() )
		{
			if( factionPlayerIndex != numFactionsRemainingInCombat-1 )
			{
				liveAgentsByFaction[ factionPlayerIndex ] = liveAgentsByFaction[ numFactionsRemainingInCombat-1 ];
				-- numFactionsRemainingInCombat;
				liveAgentsByFaction.pop_back();
			}
		}
	}

	std::string combatReport = "COMBAT:";
	for( factionPlayerIndex = 0; factionPlayerIndex < numFactionsRemainingInCombat; ++ factionPlayerIndex )
	{
		if( !liveAgentsByFaction[ factionPlayerIndex ].empty() )
		{
			combatReport += Stringf( " (player #%d:%d ants)", liveAgentsByFaction[ factionPlayerIndex ].back()->m_playerIndex, liveAgentsByFaction[ factionPlayerIndex ].size() );
		}
	}
	DebuggerPrintf( "%s\n", combatReport.c_str() );

	// Have the strongest ants from each faction fight until all non-queen ants remaining are from a single faction
	bool isFinishedKillingNonQueens = false;
	while( !isFinishedKillingNonQueens )
	{
		isFinishedKillingNonQueens = PerformOneRoundOfFighting( arena, liveAgentsByFaction );
	}
}


//-----------------------------------------------------------------------------------------------
// Returns true if we are done with all agent-to-agent fighting in this combat and are ready to resolve agent-vs-queen conflicts, if any exist
//
bool TheGame::PerformOneRoundOfFighting( Arena& arena, std::vector< std::vector< Agent* > >& liveAgentsByFaction )
{
	bool didAtLeastOneAgentDie = false;

	// Compute the best agent strength remaining
	int factionPlayerIndex;
	int bestStrength = -1;
	int numFactionsRemainingInCombat = (int) liveAgentsByFaction.size();
	for( factionPlayerIndex = 0; factionPlayerIndex < numFactionsRemainingInCombat; ++ factionPlayerIndex )
	{
		if( liveAgentsByFaction[ factionPlayerIndex ].empty() )
			continue;

		Agent* bestAgentForThisFaction = liveAgentsByFaction[ factionPlayerIndex ].back();
		int bestStrengthFromThisFaction = bestAgentForThisFaction->m_strength;
		if( bestStrengthFromThisFaction > bestStrength )
			bestStrength = bestStrengthFromThisFaction;
	}

	bool areAllBestAgentsEqualOrQueens = true;
	int numberOfFactionsWithNonQueenAntsLeft = 0;
	for( factionPlayerIndex = 0; factionPlayerIndex < numFactionsRemainingInCombat; ++ factionPlayerIndex )
	{
		if( liveAgentsByFaction[ factionPlayerIndex ].empty() )
			continue;

		Agent* bestAgentForThisFaction = liveAgentsByFaction[ factionPlayerIndex ].back();
		if( bestAgentForThisFaction->m_agentType != ENTITY_TYPE_QUEEN )
		{
			++ numberOfFactionsWithNonQueenAntsLeft;
			if( bestAgentForThisFaction->m_strength < bestStrength )
			{
				areAllBestAgentsEqualOrQueens = false;
			}
		}
	}

	if( numberOfFactionsWithNonQueenAntsLeft <= 1 )
		return true;

	for( factionPlayerIndex = 0; factionPlayerIndex < numFactionsRemainingInCombat; ++ factionPlayerIndex )
	{
		if( liveAgentsByFaction[ factionPlayerIndex ].empty() )
			continue;

		Agent* bestAgentForThisFaction = liveAgentsByFaction[ factionPlayerIndex ].back();
		if( bestAgentForThisFaction->m_agentType != ENTITY_TYPE_QUEEN )
		{
			if( bestAgentForThisFaction->m_strength < bestStrength || areAllBestAgentsEqualOrQueens )
			{
//				DebuggerPrintf( "$$$$ Best agent for player %d was %s with strength=%d (bestStrength=%d); areAllBlahBlah=%s\n",
//					bestAgentForThisFaction->m_playerIndex, GetNameForEntityType( bestAgentForThisFaction->m_agentType ), bestAgentForThisFaction->m_strength, bestStrength, areAllBestAgentsEqualOrQueens ? "true" : "false" );

				didAtLeastOneAgentDie = true;
				KillAgent( *bestAgentForThisFaction, arena, REPORT_WAS_KILLED_IN_COMBAT );
				liveAgentsByFaction[ factionPlayerIndex ].pop_back();
			}
		}
	}

	return !didAtLeastOneAgentDie;
}


//-----------------------------------------------------------------------------------------------
void TheGame::KillAgent( Agent& agent, Arena& arena, ReportCode reasonForDeath )
{
	thePluginManager->m_players[ agent.m_playerIndex ]->KillAgent( agent, arena, reasonForDeath );
}


//-----------------------------------------------------------------------------------------------
void TheGame::CalculateVisibility( Arena& arena )
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();

	int numPlayers = (int) thePluginManager->m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		player.CalculateVisibility( arena );
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	if( g_showPerformanceMessages ) DebuggerPrintf( "  Adjudicate: Computed visibility in %.0f ms.\n", 1000.f * (float) elapsedSeconds );
}


//-----------------------------------------------------------------------------------------------
void TheGame::BuildObservations( Arena& arena )
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();

	int numPlayers = (int) thePluginManager->m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		player.BuildObservations( arena );
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	if( g_showPerformanceMessages ) DebuggerPrintf( "  Adjudicate: Built observations in %.0f ms.\n", 1000.f * (float) elapsedSeconds );
}


//-----------------------------------------------------------------------------------------------
void TheGame::CheckForDefeatedPlayers( Arena& arena )
{
	UNUSED( arena );
	int numPlayers = (int) thePluginManager->m_players.size();
	for( int playerIndex = 0; playerIndex < numPlayers; ++ playerIndex )
	{
		Player& player = *thePluginManager->m_players[ playerIndex ];
		if( player.m_isDefeated )
			continue;

		player.m_isDefeated = !player.HasLivingAgents();
		if( player.m_isDefeated )
		{
			// We were JUST defeated this past turn!
			DebuggerPrintf( "==============================================================================\n" );
			DebuggerPrintf( "DEATH for \"%s\" (%s) on Turn %d - %s, written by %s\n", player.m_playerName.c_str(), player.m_playerColorName.c_str(), m_turnNumber, player.m_dllFileName.c_str(), player.m_authorName.c_str() );
			DebuggerPrintf( "==============================================================================\n" );

			// Pause the game if no more colonies are alive
			bool arePlayersAlive = false;
			for( int i = 0; i < (int) thePluginManager->m_players.size(); ++ i )
			{
				if( !thePluginManager->m_players[ i ]->m_isDefeated )
				{
					arePlayersAlive = true;
					break;
				}
			}

			if( !arePlayersAlive )
			{
				// The last living player just died.  Pause the game.
				m_isPaused = true;
			}
		}
	}

}


//-----------------------------------------------------------------------------------------------
bool TheGame::HandleWin32Message( UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	UNUSED( lParam );

	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			m_isQuitting = true;
			return true;

		case WM_KEYDOWN:			
			return ProcessKeyDownEvent( asKey );

		case WM_KEYUP:
			return ProcessKeyUpEvent( asKey );

		default:
			return false;
	};
}


//-----------------------------------------------------------------------------------------------
bool TheGame::ProcessKeyDownEvent( unsigned char keyCode )
{
	bool wasHandled = false;
	g_keyDownStates[ keyCode ] = true;

	int keyCodeAsNumberKey = (keyCode - '0');

	if( keyCode == VK_F3 )
	{
		g_showDebugOverlay = !g_showDebugOverlay;
		wasHandled = true;
	}
	else if( keyCode == 'P' )
	{
		bool pauseGame = !IsGamePaused();
		SetGamePaused( pauseGame );
	}
	else if( keyCode == 'N' )
	{
		m_labelVerbosity = (++m_labelVerbosity) % 3;
	}
	else if( keyCode == 'D' )
	{
		g_showPerformanceMessages = !g_showPerformanceMessages;
	}
	else if( keyCode == VK_SPACE )
	{
		SetGamePaused( false );
		m_isToAdvanceOnlyOnce = true;
	}
	else if( keyCode == VK_ESCAPE )
	{
		m_isQuitting = true;
		wasHandled = true;
	}
	else if( keyCodeAsNumberKey >= 0 )
	{
		if( keyCodeAsNumberKey < (int)thePluginManager->m_players.size() )
		{
			Player& player = *thePluginManager->m_players[ keyCodeAsNumberKey ];
			if( m_debugGraphicsForPlayer == keyCodeAsNumberKey )
			{
				if( player.m_drawDebugOverlayFunc )
				{
					player.m_isDebugDrawEnabled = !player.m_isDebugDrawEnabled;
				}
				else
				{
					player.m_isDebugDrawEnabled = false;
				}
			}
			else
			{
				m_debugGraphicsForPlayer = keyCodeAsNumberKey;
			}
		}
		else
		{
			m_debugGraphicsForPlayer = -1;
		}
	}

	return wasHandled;
}


//-----------------------------------------------------------------------------------------------
bool TheGame::ProcessKeyUpEvent( unsigned char keyCode )
{
	g_keyDownStates[ keyCode ] = false;
	return false;
}



//-----------------------------------------------------------------------------------------------
void TheGame::LoadDataFiles()
{
	JazzPath arenaDefinitionsJazzPath( "Data/ArenaDefinitions.xml" );
	ResourceStream resourceStream( arenaDefinitionsJazzPath );
	XMLNode arenaDefinitionsNode = CreateXMLDocumentFromResourceStream( resourceStream, "ArenaDefinitions" );
	for( int childElementIndex = 0; childElementIndex < arenaDefinitionsNode.nElement(); )
	{
		XMLNode arenaNode;
		bool success = GetChildNodeAtPosition( arenaDefinitionsNode, childElementIndex, arenaNode );
		if( !success )
			break;

		Arena* newArena = new Arena( resourceStream, arenaNode, (int) thePluginManager->m_players.size() );
		theArenaManager->m_arenas.push_back( newArena );
	}

	theArenaManager->m_currentArenaCopy = new Arena( *theArenaManager->m_arenas.front() );
}


//-----------------------------------------------------------------------------------------------
Vector2 TheGame::GetCursorWorldPos()
{
	RECT clientRect;
	GetClientRect( g_windowHandle, &clientRect );

	RECT windowRect;
	GetWindowRect( g_windowHandle, &windowRect );

	POINT iCursorDesktopPos;
	GetCursorPos( &iCursorDesktopPos );

	Vector2 cursorDesktopPos( (float)iCursorDesktopPos.x, (float)iCursorDesktopPos.y );
	AABB2 clientBounds( (float)min( clientRect.left, clientRect.right ), (float)min( clientRect.top, clientRect.bottom ), (float)max( clientRect.left, clientRect.right ), (float)max( clientRect.top, clientRect.bottom ) );
	AABB2 windowBounds( (float)min( windowRect.left, windowRect.right ), (float)min( windowRect.top, windowRect.bottom ), (float)max( windowRect.left, windowRect.right ), (float)max( windowRect.top, windowRect.bottom ) );

	Vector2 clientRectPosMaybe = cursorDesktopPos - windowBounds.mins - g_clientRectBorderOffset;

	Arena& arena = *theArenaManager->m_currentArenaCopy;
	Vector2 fDimensions( (float)arena.m_dimensions.x, (float)arena.m_dimensions.y );
	Vector2 virtualScreenSize( VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT );
	float scale = fDimensions.CalcScaleRequiredToFitInsideBox( virtualScreenSize );

	clientRectPosMaybe /= scale;
	Vector2 physicalToVirtualScalingFactor2D( VIRTUAL_SCREEN_WIDTH / g_clientPhysicalSize.x, VIRTUAL_SCREEN_HEIGHT / g_clientPhysicalSize.y );
	clientRectPosMaybe.MultiplyComponents( physicalToVirtualScalingFactor2D );
//	clientRectPosMaybe -= Vector2( 0.5f, 0.5f );

	Vector2 cursorWorldPos = clientRectPosMaybe;
//	DebuggerPrintf( "CursorPos = %5.01f, %5.01f\n", cursorWorldPos.x, cursorWorldPos.y );
	return cursorWorldPos;
}


//-----------------------------------------------------------------------------------------------
IntVector2 TheGame::GetCursorSquareCoords()
{
	return IntVector2::ZERO;
}

