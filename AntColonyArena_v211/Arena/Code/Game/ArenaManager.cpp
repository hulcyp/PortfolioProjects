//-----------------------------------------------------------------------------------------------
// ArenaManager.cpp
//-----------------------------------------------------------------------------------------------
#include "ArenaManager.hpp"
#include "PluginManager.hpp"
#include "TheGame.hpp"


//-----------------------------------------------------------------------------------------------
// Globals and constants
//
const int MANHATTAN_ANNIHILATION_DISTANCE = 2;
const float DEATH_ANIM_SECONDS = 1.0f;
const float BIRTH_ANIM_SECONDS = 1.0f;


/////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
Agent::Agent( Arena& arena, int playerIndex, int newEntityID, EntityType agentType )
	: m_entityID( newEntityID )
	, m_playerIndex( playerIndex )
	, m_agentType( agentType )
	, m_position( IntVector2::ZERO )
	, m_previousPosition( IntVector2::ZERO )
	, m_latestOrderCode( ORDER_HOLD )
	, m_isDead( false )
	, m_timeBornOrDied( 0.0 )
	, m_timeLastChangedPosition( 0.0 )
	, m_timeLastAssaulted( 0.0 )
	, m_timeLastFed( 0.0 )
	, m_turnNumberLastOrdered( 0 )
	, m_specialStatus( ENTITY_SPECIAL_STATUS_NONE )
	, m_strength( 0 )
{
	static int strengthPerAntType[ NUM_AGENT_TYPES ] = // NOTE: make this non-static if these strengths could ever change at runtime
	{
		arena.m_arenaInfo.combatStrengthSoldier,
		arena.m_arenaInfo.combatStrengthScout,
		arena.m_arenaInfo.combatStrengthWorker,
		0 // Queens have effectively zero combat strength (they fight last, and always "lose")
	};

	m_strength = strengthPerAntType[ m_agentType ];
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
Arena::Arena( const ResourceStream& xmlFile, XMLNode arenaNode, int numPlayers )
	: m_name( "UNNAMED MAP" )
	, m_dimensions( IntVector2( 100, 100 ) )
	, m_startingNumberOfWorkersPerPlayer( 3 )
	, m_startingNumberOfSoldiersPerPlayer( 3 )
	, m_startingNumberOfScoutsPerPlayer( 3 )
	, m_startingNumberOfQueensPerPlayer( 1 )
	, m_maxNumberOfAgentsPerPlayer( 100 )
	, m_fractionOfMapThatIsStone( 0.10f )
	, m_fractionOfMapThatIsDirt( 0.20f )
	, m_fractionOfMapThatIsFood( 0.05f )
	, m_numStonesPerClump( 5 )
	, m_numDirtsPerClump( 20 )
	, m_turnsPerFoodSpawn( 7 )
	, m_arenaSquares( 0, 0 )
{
	ValidateXMLElementsForNode( xmlFile, arenaNode, "", "" );
	ValidateXMLAttributesForNode( xmlFile, arenaNode, "name", "size,rockPercent,dirtPercent,foodPercent,rockClumpSize,dirtClumpSize,turnsPerFoodSpawn,maxAgents,initialWorkers,initialSoldiers,initialScouts,initialQueens,startingNutrients" );

	m_name = GetStringProperty( arenaNode, "name", m_name );
	int arenaSize = GetIntProperty( arenaNode, "size", 100 );
	m_arenaSquares.m_width = (short) arenaSize;
	m_arenaSquares.m_height = (short) arenaSize;
	m_dimensions.SetXY( arenaSize, arenaSize );
	m_fractionOfMapThatIsStone	= 0.01f * GetFloatProperty( arenaNode, "rockPercent", 0.10f );
	m_fractionOfMapThatIsDirt	= 0.01f * GetFloatProperty( arenaNode, "dirtPercent", 0.20f );
	m_fractionOfMapThatIsFood	= 0.01f * GetFloatProperty( arenaNode, "foodPercent", 0.05f );
	m_numStonesPerClump			= GetIntProperty( arenaNode, "rockClumpSize", 5 );
	m_numDirtsPerClump			= GetIntProperty( arenaNode, "dirtClumpSize", 20 );
	m_turnsPerFoodSpawn			= GetIntProperty( arenaNode, "turnsPerFoodSpawn", 7 );

	m_startingNumberOfWorkersPerPlayer = GetIntProperty( arenaNode, "initialWorkers", m_startingNumberOfWorkersPerPlayer );
	m_startingNumberOfSoldiersPerPlayer = GetIntProperty( arenaNode, "initialSoldiers", m_startingNumberOfSoldiersPerPlayer );
	m_startingNumberOfScoutsPerPlayer = GetIntProperty( arenaNode, "initialScouts", m_startingNumberOfScoutsPerPlayer );
	m_startingNumberOfQueensPerPlayer = GetIntProperty( arenaNode, "initialQueens", m_startingNumberOfQueensPerPlayer );

	m_arenaInfo.numPlayers								= (char) numPlayers;
	m_arenaInfo.width									= m_arenaSquares.m_width;
	m_arenaInfo.height									= m_arenaSquares.m_height;
	m_arenaInfo.startingNutrients						= GetIntProperty( arenaNode, "startingNutrients", 2000 );
	m_arenaInfo.nutrientsEarnedPerFoodEatenByQueen		= 1000;
	m_arenaInfo.upkeepNutrientsPerTurnForQueen			= 8;
	m_arenaInfo.upkeepNutrientsPerTurnForWorker			= 1;
	m_arenaInfo.upkeepNutrientsPerTurnForScout			= 1;
	m_arenaInfo.upkeepNutrientsPerTurnForSoldier		= 2;
	m_arenaInfo.nutrientCostToBirthWorker				= 500;
	m_arenaInfo.nutrientCostToBirthScout				= 500;
	m_arenaInfo.nutrientCostToBirthSoldier				= 500;
	m_arenaInfo.visibilityRangeQueen					= 6;
	m_arenaInfo.visibilityRangeWorker					= 4;
	m_arenaInfo.visibilityRangeScout					= 8;
	m_arenaInfo.visibilityRangeSoldier					= 4;
	m_arenaInfo.combatStrengthWorker					= 1;
	m_arenaInfo.combatStrengthScout						= 1;
	m_arenaInfo.combatStrengthSoldier					= 2;
	m_arenaInfo.combatStrengthBonusFromNearbyQueen		= 0;
	m_arenaInfo.combatStrengthBonusMaximumDistance		= 0;
	m_arenaInfo.numTurnsBeforeSuddenDeath				= 1000;
	m_arenaInfo.nutrientLossPerAttackerStrength			= 1;
	m_arenaInfo.colonyMaxPopulation						= 100;
}


//-----------------------------------------------------------------------------------------------
Arena::Arena( const Arena& copy )
	: m_name( copy.m_name )
	, m_dimensions( copy.m_dimensions )
	, m_arenaSquares( copy.m_arenaSquares )
	, m_startingNumberOfWorkersPerPlayer( copy.m_startingNumberOfWorkersPerPlayer )
	, m_startingNumberOfSoldiersPerPlayer( copy.m_startingNumberOfSoldiersPerPlayer )
	, m_startingNumberOfScoutsPerPlayer( copy.m_startingNumberOfScoutsPerPlayer )
	, m_startingNumberOfQueensPerPlayer( copy.m_startingNumberOfQueensPerPlayer )
	, m_maxNumberOfAgentsPerPlayer( copy.m_maxNumberOfAgentsPerPlayer )
	, m_fractionOfMapThatIsStone( copy.m_fractionOfMapThatIsStone )
	, m_fractionOfMapThatIsDirt( copy.m_fractionOfMapThatIsDirt )
	, m_fractionOfMapThatIsFood( copy.m_fractionOfMapThatIsFood )
	, m_numStonesPerClump( copy.m_numStonesPerClump )
	, m_numDirtsPerClump( copy.m_numDirtsPerClump )
	, m_turnsPerFoodSpawn( copy.m_turnsPerFoodSpawn )
	, m_arenaInfo( copy.m_arenaInfo )
{}


//-----------------------------------------------------------------------------------------------
void Arena::InitializeAndBegin()
{
	PopulateSquares();
	InitializePlayers();
}


//-----------------------------------------------------------------------------------------------
void Arena::PopulateSquares()
{
	short x,y;
	short highestX = (short) m_dimensions.x - 1;
	short highestY = (short) m_dimensions.y - 1;

	float numInteriorSquares = (float)( (m_dimensions.x - 2) * (m_dimensions.y - 2) );
	int numStoneSquares = (int)( m_fractionOfMapThatIsStone * numInteriorSquares );
	int numDirtSquares = (int)( m_fractionOfMapThatIsDirt * numInteriorSquares );
	int numFoodSquares = (int)( m_fractionOfMapThatIsFood * numInteriorSquares );

	int i, c;
	for( i = 0; i < (numStoneSquares / m_numStonesPerClump); ++ i )
	{
		x = (short) RandomIntInRangeInclusive( 1, highestX - 1 );
		y = (short) RandomIntInRangeInclusive( 1, highestY - 1 );
		for( c = 0; c < m_numStonesPerClump; ++ c )
		{
			m_arenaSquares.SetSquareTypeAtCoords( x, y, ARENA_SQUARE_TYPE_STONE );
			CardinalDirection randomDirection = (CardinalDirection)( 1 + rand() % 4 );
			IntVector2 offset = GetDisplacementForCardinalDirection( randomDirection );
			x += (short) offset.x;
			y += (short) offset.y;
		}
	}

	for( i = 0; i < (numDirtSquares / m_numDirtsPerClump); ++ i )
	{
		x = (short) RandomIntInRangeInclusive( 1, highestX - 1 );
		y = (short) RandomIntInRangeInclusive( 1, highestY - 1 );
		for( c = 0; c < m_numDirtsPerClump; ++ c )
		{
			m_arenaSquares.SetSquareTypeAtCoords( x, y, ARENA_SQUARE_TYPE_DIRT );
			CardinalDirection randomDirection = (CardinalDirection)( 1 + rand() % 4 );
			IntVector2 offset = GetDisplacementForCardinalDirection( randomDirection );
			x += (short) offset.x;
			y += (short) offset.y;
			x = (short) ClampInt( x, 1, highestX-1 );
			y = (short) ClampInt( y, 1, highestY-1 );
		}
	}

	for( i = 0; i < numFoodSquares; ++ i )
	{
		x = (short) RandomIntInRangeInclusive( 1, highestX - 1 );
		y = (short) RandomIntInRangeInclusive( 1, highestY - 1 );
		ArenaSquareType squareType = m_arenaSquares.GetSquareTypeAtCoords( x, y );
		if( squareType == ARENA_SQUARE_TYPE_STONE )
		{
			// Try again; we allow Food embedded within Dirt, but not Stone
			-- i;
			continue;
		}

		m_arenaSquares.SetSquareTypeAtCoords( x, y, ARENA_SQUARE_TYPE_FOOD );
	}


	// Add stone border
	for( x = 0; x < m_dimensions.x; ++ x )
	{
		m_arenaSquares.SetSquareTypeAtCoords( x, 0, ARENA_SQUARE_TYPE_STONE );
		m_arenaSquares.SetSquareTypeAtCoords( x, highestY, ARENA_SQUARE_TYPE_STONE );
	}

	for( y = 0; y < m_dimensions.y; ++ y )
	{
		m_arenaSquares.SetSquareTypeAtCoords( 0, y, ARENA_SQUARE_TYPE_STONE );
		m_arenaSquares.SetSquareTypeAtCoords( highestX, y, ARENA_SQUARE_TYPE_STONE );
	}

	// Clear each player's start square and its immediate neighbors
	for( i = 0; i < (int) thePluginManager->m_players.size(); ++ i )
	{
		Player& player = *thePluginManager->m_players[ i ];
		IntVector2 startPos = player.m_startingPosition;
		m_arenaSquares.SetSquareTypeAtCoords( (short) startPos.x, (short) startPos.y, ARENA_SQUARE_TYPE_AIR );
		m_arenaSquares.SetSquareTypeAtCoords( (short) startPos.x+1, (short) startPos.y, ARENA_SQUARE_TYPE_AIR );
		m_arenaSquares.SetSquareTypeAtCoords( (short) startPos.x-1, (short) startPos.y, ARENA_SQUARE_TYPE_AIR );
		m_arenaSquares.SetSquareTypeAtCoords( (short) startPos.x, (short) startPos.y+1, ARENA_SQUARE_TYPE_AIR );
		m_arenaSquares.SetSquareTypeAtCoords( (short) startPos.x, (short) startPos.y-1, ARENA_SQUARE_TYPE_AIR );
	}
}


//-----------------------------------------------------------------------------------------------
double Arena::Render( float virtualCoordsPerSquare )
{
	double startSeconds = Clock::GetAbsoluteTimeSeconds();
	double timeNow = startSeconds;

	AABB2 area;
	for( short x = 0; x < (short) m_dimensions.x; ++ x )
	for( short y = 0; y < (short) m_dimensions.y; ++ y )
	{
		ArenaSquareType squareType = m_arenaSquares.GetSquareTypeAtCoords( x, y );
		Rgba color = GetColorForSquareType( squareType );

		if( color.a > 0 )
		{
			area.mins.x = virtualCoordsPerSquare * (float) x;
			area.mins.y = virtualCoordsPerSquare * (float) y;
			area.maxs.x = area.mins.x + virtualCoordsPerSquare;
			area.maxs.y = area.mins.y + virtualCoordsPerSquare;
			DrawFilledArea( area, color );
		}

		if( theGame->m_debugGraphicsForPlayer != -1 )
		{
			if( color.a <= 0 )
			{
				area.mins.x = virtualCoordsPerSquare * (float) x;
				area.mins.y = virtualCoordsPerSquare * (float) y;
				area.maxs.x = area.mins.x + virtualCoordsPerSquare;
				area.maxs.y = area.mins.y + virtualCoordsPerSquare;
			}

			if( thePluginManager->m_players[ theGame->m_debugGraphicsForPlayer ]->m_isVisibleToPlayer[ x + (y * m_dimensions.x) ] == 0 )
				DrawFilledArea( area, Rgba( 128, 128, 128, 128 ) );
		}
	}

	for( int i = 0; i < (int) m_allAgentReferences.size(); ++ i )
	{
		Agent* agent = m_allAgentReferences[ i ];
		if( !agent )
			continue;

		float alphaValue = 1.f;
		float scaleBias = 0.f;
		float dyingFraction = 0.f;
		float birthingFraction = 0.f;
		if( agent->m_isDead )
		{
			float maxScaleBias = 5.f;
			float deathAnimSeconds = DEATH_ANIM_SECONDS;
			if( agent->m_agentType == ENTITY_TYPE_QUEEN )
			{
				deathAnimSeconds = 3.f;
				maxScaleBias = 25.f;
			}

			double secondsSinceDied = Clock::GetAbsoluteTimeSeconds() - agent->m_timeBornOrDied;
			dyingFraction = (float)( secondsSinceDied / deathAnimSeconds );
			if( dyingFraction >= 1.f )
				continue; // don't draw old-dead dudes

			alphaValue = (1.f - dyingFraction) * (1.f - dyingFraction);
			scaleBias = (maxScaleBias * (1.f - ((1.f - dyingFraction) * (1.f - dyingFraction))) );
		}
		else
		{
			float maxScaleBias = 0.5f;
			float birthAnimSeconds = BIRTH_ANIM_SECONDS;
			if( agent->m_agentType == ENTITY_TYPE_QUEEN )
			{
				birthAnimSeconds = 1.f;
				maxScaleBias = 5.f;
			}

			double secondsSinceBorn = Clock::GetAbsoluteTimeSeconds() - agent->m_timeBornOrDied;
			birthingFraction = (float)( secondsSinceBorn / birthAnimSeconds );
			if( birthingFraction < 1.f )
			{
				scaleBias = maxScaleBias * ClampFloat( (1.f - birthingFraction) * (1.f - birthingFraction), 0.f, 1.f );
				alphaValue = 1.f - ((1.f - birthingFraction) * (1.f - birthingFraction));
			}
		}

		Player& player = *thePluginManager->m_players[ agent->m_playerIndex ];
		Rgba& color = player.m_playerColor;
		Vector2 agentCenter( 0.5f + (float)agent->m_position.x, 0.5f + (float)agent->m_position.y );
		float agentRadius = 0.4f + scaleBias;
		agentCenter *= virtualCoordsPerSquare;
		agentRadius *= virtualCoordsPerSquare;
		EntityType agentType = agent->m_agentType;
		Vector2 entityCenter( 0.5f + (float)agent->m_position.x, 0.5f + (float)agent->m_position.y );
		Vector2 entityPrevCenter( 0.5f + (float)agent->m_previousPosition.x, 0.5f + (float)agent->m_previousPosition.y );
//		float entityRadius = 0.4f + scaleBias;
		double secondsSinceMoved = timeNow - agent->m_timeLastChangedPosition;
		float fractionMoved = ClampFloat( (float)( secondsSinceMoved / MOVE_ANIMATION_SECONDS ), 0.f, 1.f );
		Vector2 entityVisualCenter = virtualCoordsPerSquare * entityPrevCenter.InterpolateTo( entityCenter, fractionMoved );

		//float radius = 0.5;
		//Vector2 location( 20.f, 2.f );
		//radius *= virtualCoordsPerSquare;
		//location *= virtualCoordsPerSquare;
		//DrawOutlinedCircle( agentCenter, radius, Rgba::PURPLE, 1.f );

		glPushMatrix();
		if( agent->m_isDead )
		{
			float baseDegreesToRotate = 2.f * (float) agent->m_playerIndex;
			float animatedDegreesToRotate = agent->m_agentType == ENTITY_TYPE_QUEEN ? 200.f : ENTITY_TYPE_SOLDIER ? 0.f : 10.f;
			glTranslatef( entityVisualCenter.x, entityVisualCenter.y, 0.f );
			glRotatef( baseDegreesToRotate + dyingFraction * animatedDegreesToRotate, 0.f, 0.f, 1.f );
			glTranslatef( -entityVisualCenter.x, -entityVisualCenter.y, 0.f );
		}

		if( agentType == ENTITY_TYPE_QUEEN )
		{
			float innerRadius = (0.45f + scaleBias) * virtualCoordsPerSquare;
			float outerRadius = (0.7f + scaleBias) * virtualCoordsPerSquare;

			float recentnessOfHavingBeenAssaulted = 1.f - ClampFloat( (float)((timeNow - agent->m_timeLastAssaulted) / QUEEN_DAMAGE_FLASH_SECONDS), 0.f, 1.f );
			float recentnessOfHavingEaten = 1.f - ClampFloat( (float)((timeNow - agent->m_timeLastFed) / QUEEN_ATE_FLASH_SECONDS), 0.f, 1.f );
			if( recentnessOfHavingEaten > 0.f || recentnessOfHavingBeenAssaulted > 0.f )
			{
				Rgba queenRingFillColor( 0, 0, 0, 0 );
				// FIXME TODO: Finish implementing this (space between queen's inner and outer rings is filled with green (for eating) and/or red (for being hurt) that fades with time
			}

			DrawOutlinedCircle( entityVisualCenter, innerRadius, color, alphaValue );
			DrawOutlinedCircle( entityVisualCenter, outerRadius, color, alphaValue );

			if( theGame->m_labelVerbosity > 0 )
			{
//				float alpha = 1.f;
//				if( theGame->m_labelVerbosity == 1 )
//					alpha = 0.5f;
				float alpha = 0.5f;

				alpha *= (1.f - dyingFraction);
				IntVector2 queenLabelScreenCoords( entityVisualCenter + Vector2( 0.9f * virtualCoordsPerSquare, 0.0f * virtualCoordsPerSquare ) );
				Draw2DText( player.m_playerName, queenLabelScreenCoords + IntVector2::ONE, Rgba::BLACK, alpha );
				Draw2DText( player.m_playerName, queenLabelScreenCoords, color, alpha );
			}
		}
		else if( agentType == ENTITY_TYPE_WORKER )
		{
			if( agent->m_specialStatus == ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
			{
				DrawFilledDiamond( entityVisualCenter, agentRadius, Rgba::BLACK, alphaValue );
				DrawFilledDiamond( entityVisualCenter, 0.5f * agentRadius, Rgba::DARKGREEN, alphaValue );
			}
			DrawOutlinedDiamond( entityVisualCenter, agentRadius, color, alphaValue );
		}
		else if( agentType == ENTITY_TYPE_SOLDIER )
		{
			DrawX( entityVisualCenter, agentRadius, color, alphaValue );
		}
		else if( agentType == ENTITY_TYPE_SCOUT )
		{
			DrawPlus( entityVisualCenter, agentRadius, color, alphaValue );
		}
		glPopMatrix();
	}

	double elapsedSeconds = Clock::GetAbsoluteTimeSeconds() - startSeconds;
	return elapsedSeconds;
}


//-----------------------------------------------------------------------------------------------
void Arena::InitializePlayers()
{
	// Initialize
	int playerIndex;
	for( playerIndex = 0; playerIndex < (int) thePluginManager->m_players.size(); ++ playerIndex )
	{
		thePluginManager->m_players[ playerIndex ]->m_currentNutrients = m_arenaInfo.startingNutrients;
		thePluginManager->m_players[ playerIndex ]->m_agents.clear();
	}

	for( int globalAgentIndex = 0; globalAgentIndex < (int) m_allAgentReferences.size(); ++ globalAgentIndex )
	{
		if( m_allAgentReferences[ globalAgentIndex ] != NULL )
		{
			delete m_allAgentReferences[ globalAgentIndex ];
		}
	}
	m_allAgentReferences.clear();

	// Spawn starting ants
	for( playerIndex = 0; playerIndex < (int) thePluginManager->m_players.size(); ++ playerIndex )
	{
		int spawnCounter;
		for( spawnCounter = 0; spawnCounter < m_startingNumberOfWorkersPerPlayer; ++ spawnCounter )
		{
			IntVector2 playerStartingPosition = thePluginManager->m_players[ playerIndex ]->m_startingPosition;
			CreateNewAgentForPlayerIndex( playerIndex, playerStartingPosition, ENTITY_TYPE_WORKER, false );
		}
		for( spawnCounter = 0; spawnCounter < m_startingNumberOfSoldiersPerPlayer; ++ spawnCounter )
		{
			IntVector2 playerStartingPosition = thePluginManager->m_players[ playerIndex ]->m_startingPosition;
			CreateNewAgentForPlayerIndex( playerIndex, playerStartingPosition, ENTITY_TYPE_SOLDIER, false );
		}
		for( spawnCounter = 0; spawnCounter < m_startingNumberOfScoutsPerPlayer; ++ spawnCounter )
		{
			IntVector2 playerStartingPosition = thePluginManager->m_players[ playerIndex ]->m_startingPosition;
			CreateNewAgentForPlayerIndex( playerIndex, playerStartingPosition, ENTITY_TYPE_SCOUT, false );
		}
		for( spawnCounter = 0; spawnCounter < m_startingNumberOfQueensPerPlayer; ++ spawnCounter )
		{
			IntVector2 playerStartingPosition = thePluginManager->m_players[ playerIndex ]->m_startingPosition;
			CreateNewAgentForPlayerIndex( playerIndex, playerStartingPosition, ENTITY_TYPE_QUEEN, false );
		}
	}

}


//-----------------------------------------------------------------------------------------------
Agent* Arena::CreateNewAgentForPlayerIndex( int playerIndex, const IntVector2& agentBirthLocation, EntityType agentType, bool displayConsoleMessage )
{
	Player& player = *thePluginManager->m_players[ playerIndex ];
	int numLivingAgents = player.CalcNumLivingAgents();
	if( numLivingAgents >= m_maxNumberOfAgentsPerPlayer )
		return NULL;

	int numAgents = (int) m_allAgentReferences.size();
	int newAgentID = (1000000 * playerIndex) + numAgents;
	Agent* newAgent = new Agent( *this, playerIndex, newAgentID, agentType );
	newAgent->m_position = agentBirthLocation;
	newAgent->m_previousPosition = agentBirthLocation;
	newAgent->m_timeBornOrDied = Clock::GetAbsoluteTimeSeconds();

	m_allAgentReferences.push_back( newAgent );
	thePluginManager->m_players[ playerIndex ]->m_agents.push_back( newAgent );

	AgentReport newBirthAnnouncement;
	newBirthAnnouncement.m_entityID = newAgentID;
	newBirthAnnouncement.m_agentType = (char) agentType;
	newBirthAnnouncement.m_reportCode = REPORT_WAS_CREATED;
	newBirthAnnouncement.m_newPositionX = (short) newAgent->m_position.x;
	newBirthAnnouncement.m_newPositionY = (short) newAgent->m_position.y;
	newBirthAnnouncement.m_specialStatus = ENTITY_SPECIAL_STATUS_NONE;
	player.AddAgentReport( newBirthAnnouncement );
	AddEntityToLocation( newAgent, agentBirthLocation );

	if( displayConsoleMessage )
		DebuggerPrintf( "Created new %s (#%d) for player #%d at (%d,%d)\n", GetNameForEntityType( agentType ), newAgentID, playerIndex, newAgent->m_position.x, newAgent->m_position.y );

	return newAgent;
}


//-----------------------------------------------------------------------------------------------
STATIC Rgba Arena::GetColorForSquareType( ArenaSquareType squareType )
{
	switch( squareType )
	{
		case ARENA_SQUARE_TYPE_AIR:			return Rgba::TRANSPARENTBLACK; // Rgba( (unsigned char)(255.f * (0.5f + 0.5f * cos( Clock::GetAbsoluteTimeSeconds() ))), 0, 0, 255 );
		case ARENA_SQUARE_TYPE_DIRT:		return Rgba( 100, 60, 40, 255 );
		case ARENA_SQUARE_TYPE_STONE:		return Rgba::LIGHTGREY;
		case ARENA_SQUARE_TYPE_FOOD:		return Rgba::DARKGREEN;
		default:							return Rgba::TRANSPARENTBLACK;
	}
}


//-----------------------------------------------------------------------------------------------
Agent* Arena::GetAgentForEntityID( int entityID )
{
	for( int i = 0; i < (int) m_allAgentReferences.size(); ++ i )
	{
		Agent* checkAgent = m_allAgentReferences[ i ];
		if( checkAgent && checkAgent->m_entityID == entityID )
		{
			return checkAgent;
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------------------------
void Arena::ApplyMoveOrderForPlayer( AgentOrder& order, Player& player )
{
	double timeNow = Clock::GetAbsoluteTimeSeconds();
	AgentReport moveResult;
	int entityID = order.m_entityID;
	moveResult.m_entityID = entityID;
	moveResult.m_reportCode = REPORT_HOLD_SUCCESSFUL;
	moveResult.m_agentType = ENTITY_TYPE_WORKER;
	moveResult.m_newPositionX = 0;
	moveResult.m_newPositionY = 0;
	moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_NONE;

	Agent* agent = GetAgentForEntityID( entityID );
	if( !agent )
	{
		moveResult.m_reportCode = REPORT_ERROR_BAD_ANT_ID;
		moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_DEAD;
		player.AddAgentReport( moveResult );
		return;
	}

	moveResult.m_newPositionX = (short) agent->m_position.x;
	moveResult.m_newPositionY = (short) agent->m_position.y;
	moveResult.m_agentType = (char) agent->m_agentType;
	moveResult.m_specialStatus = (char) agent->m_specialStatus;

	if( agent->m_isDead || agent->m_playerIndex != player.m_playerIndex )
	{
		moveResult.m_reportCode = REPORT_ERROR_BAD_ANT_ID;
		moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_DEAD;
		player.AddAgentReport( moveResult );
		return;
	}

	int lastFrameNumberOrdered = agent->m_turnNumberLastOrdered;
	agent->m_turnNumberLastOrdered = theGame->m_turnNumber;

	if( lastFrameNumberOrdered == theGame->m_turnNumber )
	{
		moveResult.m_reportCode = REPORT_ERROR_ANT_MOVED_TWICE;
		player.AddAgentReport( moveResult );
		return;
	}

	EntityType agentType = agent->m_agentType;

	if( order.m_orderCode == ORDER_SUICIDE )
	{
		moveResult.m_reportCode = REPORT_SUICIDE_SUCCESSFUL;
		moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_DEAD;
		player.AddAgentReport( moveResult );
		player.KillAgent( *agent, *this, REPORT_SUICIDE_SUCCESSFUL );
		return;
	}

	// Check and update Queen exhaustion status
	if( agentType == ENTITY_TYPE_QUEEN )
	{
		if( agent->m_specialStatus == ENTITY_SPECIAL_STATUS_EXHAUSTED )
		{
			// The queen spent this turn being exhausted from her non-hold move last turn;
			// reject her order (whatever it was, doesn't matter) and report her as being exhausted
			// (but reset her exhaustion flag internally since she's now "served her time".
			agent->m_specialStatus = ENTITY_SPECIAL_STATUS_NONE;
			moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_NONE;
			moveResult.m_reportCode = REPORT_ERROR_QUEEN_WAS_EXHAUSTED;
			player.AddAgentReport( moveResult );
			return;
		}

		if( order.m_orderCode != ORDER_HOLD )
		{
			agent->m_specialStatus = ENTITY_SPECIAL_STATUS_EXHAUSTED;
			moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_EXHAUSTED;
		}
	}

	// Queens (only) giving birth to new ant
	if( order.m_orderCode == ORDER_CREATE_SCOUT || order.m_orderCode == ORDER_CREATE_SOLDIER || order.m_orderCode == ORDER_CREATE_WORKER )
	{
		if( agentType == ENTITY_TYPE_QUEEN )
		{
			if( player.CalcNumLivingAgents() >= m_arenaInfo.colonyMaxPopulation )
			{
				moveResult.m_reportCode = REPORT_ERROR_MAXIMUM_POPULATION_REACHED;
				moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_EXHAUSTED;
				player.AddAgentReport( moveResult );
				return;
			}

			if( order.m_orderCode == ORDER_CREATE_SCOUT && player.m_currentNutrients >= m_arenaInfo.nutrientCostToBirthScout )
			{
				player.m_currentNutrients -= m_arenaInfo.nutrientCostToBirthScout;
				CreateNewAgentForPlayerIndex( player.m_playerIndex, agent->m_position, ENTITY_TYPE_SCOUT );
				moveResult.m_reportCode = REPORT_CREATE_SUCCESSFUL;
			}
			else if( order.m_orderCode == ORDER_CREATE_WORKER && player.m_currentNutrients >= m_arenaInfo.nutrientCostToBirthWorker )
			{
				player.m_currentNutrients -= m_arenaInfo.nutrientCostToBirthWorker;
				CreateNewAgentForPlayerIndex( player.m_playerIndex, agent->m_position, ENTITY_TYPE_WORKER );
				moveResult.m_reportCode = REPORT_CREATE_SUCCESSFUL;
			}
			else if( order.m_orderCode == ORDER_CREATE_SOLDIER && player.m_currentNutrients >= m_arenaInfo.nutrientCostToBirthSoldier )
			{
				player.m_currentNutrients -= m_arenaInfo.nutrientCostToBirthSoldier;
				CreateNewAgentForPlayerIndex( player.m_playerIndex, agent->m_position, ENTITY_TYPE_SOLDIER );
				moveResult.m_reportCode = REPORT_CREATE_SUCCESSFUL;
			}
		}
		else
		{
			moveResult.m_reportCode = REPORT_ERROR_WRONG_ANT_TYPE;
			player.AddAgentReport( moveResult );
			return;
		}
	}

	// Workers (only) taking or dropping food morsels
	if( order.m_orderCode == ORDER_TAKE_FOOD || order.m_orderCode == ORDER_DROP_FOOD )
	{
		ArenaSquareType arenaSquareType = m_arenaSquares.GetSquareTypeAtCoords( moveResult.m_newPositionX, moveResult.m_newPositionY );
		if( agentType == ENTITY_TYPE_WORKER )
		{
			if( order.m_orderCode == ORDER_TAKE_FOOD )
			{
				if( arenaSquareType != ARENA_SQUARE_TYPE_FOOD )
				{
					moveResult.m_reportCode = REPORT_ERROR_NO_FOOD_PRESENT;
					player.AddAgentReport( moveResult );
					return;
				}
				else if( agent->m_specialStatus == ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
				{
					moveResult.m_reportCode = REPORT_ERROR_ALREADY_CARRYING_FOOD;
					player.AddAgentReport( moveResult );
					return;
				}
				else
				{
					// Successfully took food
					m_arenaSquares.SetSquareTypeAtCoords( moveResult.m_newPositionX, moveResult.m_newPositionY, ARENA_SQUARE_TYPE_AIR );
					agent->m_specialStatus = ENTITY_SPECIAL_STATUS_CARRYING_FOOD;
					moveResult.m_reportCode = REPORT_TAKE_SUCCESSFUL;
					moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_CARRYING_FOOD;
				}
			}
			else if( order.m_orderCode == ORDER_DROP_FOOD )
			{
				if( agent->m_specialStatus != ENTITY_SPECIAL_STATUS_CARRYING_FOOD )
				{
					moveResult.m_reportCode = REPORT_ERROR_NOT_CARRYING_FOOD;
					player.AddAgentReport( moveResult );
					return;
				}

				int agentSquareIndex = agent->m_position.x + (agent->m_position.y * m_arenaSquares.m_width);
				std::vector< Agent* >& agentsHere = m_arenaEntitiesByLocation[ agentSquareIndex ];
				Agent* firstAlliedQueenFoundInThisSquare = NULL;
				for( int agentsHereIndex = 0; agentsHereIndex < (int) agentsHere.size(); ++ agentsHereIndex )
				{
					Agent* eachAgentHere = agentsHere[ agentsHereIndex ];
					if( eachAgentHere && !eachAgentHere->m_isDead && eachAgentHere->m_playerIndex == agent->m_playerIndex && eachAgentHere->m_agentType == ENTITY_TYPE_QUEEN )
					{
						firstAlliedQueenFoundInThisSquare = eachAgentHere;
						break;
					}
				}

				if( firstAlliedQueenFoundInThisSquare )
				{
					// Successfully dropped food to directly feed allied queen
					thePluginManager->m_players[ firstAlliedQueenFoundInThisSquare->m_playerIndex ]->m_currentNutrients += m_arenaInfo.nutrientsEarnedPerFoodEatenByQueen;
					agent->m_specialStatus = ENTITY_SPECIAL_STATUS_NONE; // not carrying food anymore
					moveResult.m_reportCode = REPORT_QUEEN_WAS_FED;
					moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_NONE;
					player.AddAgentReport( moveResult );
					return;
				}

				if( arenaSquareType != ARENA_SQUARE_TYPE_AIR )
				{
					moveResult.m_reportCode = REPORT_ERROR_SQUARE_NOT_EMPTY;
					player.AddAgentReport( moveResult );
					return;
				}
				else
				{
					// Successfully dropped food to arena square
					m_arenaSquares.SetSquareTypeAtCoords( moveResult.m_newPositionX, moveResult.m_newPositionY, ARENA_SQUARE_TYPE_FOOD );
					agent->m_specialStatus = ENTITY_SPECIAL_STATUS_NONE;
					moveResult.m_reportCode = REPORT_DROP_SUCCESSFUL;
					moveResult.m_specialStatus = ENTITY_SPECIAL_STATUS_NONE;
				}
			}
		}
		else
		{
			moveResult.m_reportCode = REPORT_ERROR_WRONG_ANT_TYPE;
			player.AddAgentReport( moveResult );
			return;
		}
	}

	CardinalDirection direction = CARDINAL_DIRECTION_NONE;
	if( order.m_orderCode == ORDER_MOVE_EAST )
		direction = CARDINAL_DIRECTION_EAST;
	else if( order.m_orderCode == ORDER_MOVE_NORTH )
		direction = CARDINAL_DIRECTION_NORTH;
	else if( order.m_orderCode == ORDER_MOVE_WEST )
		direction = CARDINAL_DIRECTION_WEST;
	else if( order.m_orderCode == ORDER_MOVE_SOUTH )
		direction = CARDINAL_DIRECTION_SOUTH;

	if( direction != CARDINAL_DIRECTION_NONE )
	{
		IntVector2 currentPosition( agent->m_position );
		IntVector2 moveDisplacement = GetDisplacementForCardinalDirection( direction );
		IntVector2 proposedPosition = currentPosition + moveDisplacement;
		ArenaSquareType proposedSquareType = m_arenaSquares.GetSquareTypeAtCoords( (short) proposedPosition.x, (short) proposedPosition.y );
		if( proposedSquareType == ARENA_SQUARE_TYPE_STONE )
		{
			moveResult.m_reportCode = REPORT_ERROR_BLOCKED_BY_ROCK;
		}
		else if( proposedSquareType == ARENA_SQUARE_TYPE_DIRT )
		{
			m_arenaSquares.SetSquareTypeAtCoords( (short) proposedPosition.x, (short) proposedPosition.y, ARENA_SQUARE_TYPE_AIR );
			moveResult.m_reportCode = REPORT_DIG_SUCCESSFUL;
			if( agentType == ENTITY_TYPE_SCOUT )
			{
				currentPosition = proposedPosition; // scouts dig automatically when they move through dirt
				moveResult.m_reportCode = REPORT_MOVE_SUCCESSFUL;
			}
		}
		else
		{
			moveResult.m_reportCode = REPORT_MOVE_SUCCESSFUL;
			currentPosition = proposedPosition;
		}

		// Move the agent (if they moved)
		moveResult.m_newPositionX = (short) currentPosition.x;
		moveResult.m_newPositionY = (short) currentPosition.y;

		if( currentPosition != agent->m_position )
		{
			RemoveEntityFromLocation( agent, agent->m_position );
			AddEntityToLocation( agent, currentPosition );
			unsigned int indexForOldLocation = (unsigned int)( agent->m_position.x + (agent->m_position.y * m_arenaSquares.m_width) );
			unsigned int indexForNewLocation = (unsigned int)( currentPosition.x + (currentPosition.y * m_arenaSquares.m_width) );
			unsigned int dualIndex = indexForNewLocation < indexForOldLocation ?
				(indexForNewLocation << 16) + indexForOldLocation :
				(indexForOldLocation << 16) + indexForNewLocation;
			theGame->m_potentialPassThroughCombatGroups[ dualIndex ].push_back( agent );
			agent->m_timeLastChangedPosition = timeNow;
		}

		agent->m_previousPosition = agent->m_position;
		agent->m_position = currentPosition;
	}

	moveResult.m_specialStatus = (char) agent->m_specialStatus;
	player.m_agentReports.AddReport( moveResult );
}


//-----------------------------------------------------------------------------------------------
void Arena::SpawnNewFoodMaybe( int turnNumber )
{
	static int numCallsSinceLastSpawn = 0;
	if( turnNumber > m_arenaInfo.numTurnsBeforeSuddenDeath )
		return;

	++ numCallsSinceLastSpawn;
	if( numCallsSinceLastSpawn < m_turnsPerFoodSpawn )
		return;

	m_turnsPerFoodSpawn = 0;

	short highestX = (short) m_dimensions.x - 1;
	short highestY = (short) m_dimensions.y - 1;

	const int maxNumberOfAttempts = 5;
//	float foodChanceRoll = RandomNonNegativeFloatLessThanOne();
//	if( foodChanceRoll <= m_chanceOfNewFoodPerTurn )
	{
		for( int attemptNumber = 0; attemptNumber < maxNumberOfAttempts; ++ attemptNumber )
		{
			short x = (short) RandomIntInRangeInclusive( 1, highestX - 1 );
			short y = (short) RandomIntInRangeInclusive( 1, highestY - 1 );

			ArenaSquareType squareType = m_arenaSquares.GetSquareTypeAtCoords( x, y );
			int numEntitiesAtSquare = GetNumEntitiesAtLocation( IntVector2( x, y ) );
			if( squareType == ARENA_SQUARE_TYPE_AIR && numEntitiesAtSquare == 0 )
			{
				m_arenaSquares.SetSquareTypeAtCoords( x, y, ARENA_SQUARE_TYPE_FOOD );
//				DebuggerPrintf( "New food spawned at %d,%d\n", x, y );
				break;
			}
		}
	}
}


void Assertion( bool shouldBeTrue )
{
	if( shouldBeTrue == false )
	{
		FatalError( "oops", "assertion failure" );
	}
}


//-----------------------------------------------------------------------------------------------
void Arena::Validate()
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
ArenaManager::ArenaManager()
	: m_currentArenaCopy( NULL )
{
}


//-----------------------------------------------------------------------------------------------
ArenaManager::~ArenaManager()
{
}


