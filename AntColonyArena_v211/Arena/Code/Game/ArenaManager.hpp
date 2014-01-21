//-----------------------------------------------------------------------------------------------
// ArenaManager.hpp
//-----------------------------------------------------------------------------------------------
#ifndef __include_ArenaManager__
#define __include_ArenaManager__

#include "GameShared.hpp"
#include <algorithm>


//-----------------------------------------------------------------------------------------------
// Forward declarations
//


//-----------------------------------------------------------------------------------------------
// Global variables
//



/////////////////////////////////////////////////////////////////////////////////////////////////
class Agent
{
public:
	Agent( Arena& arena, int playerIndex, int newEntityID, EntityType agentType );

public:
	int								m_entityID;
	int								m_playerIndex;
	EntityType						m_agentType;
	IntVector2						m_position;
	IntVector2						m_previousPosition;
	OrderCode						m_latestOrderCode;
	int								m_turnNumberLastOrdered;
	bool							m_isDead;
	double							m_timeBornOrDied;
	double							m_timeLastChangedPosition;
	double							m_timeLastAssaulted;
	double							m_timeLastFed;
	EntitySpecialStatus				m_specialStatus;
	int								m_strength;
};



/////////////////////////////////////////////////////////////////////////////////////////////////
class Arena
{
public:
	Arena( const ResourceStream& xmlFile, XMLNode arenaNode, int numPlayers );
	Arena( const Arena& copy );
	~Arena() {}
	void InitializeAndBegin();
	void PopulateSquares();
	double Render( float virtualCoordsPerSquare );
	void InitializePlayers();
	Agent* CreateNewAgentForPlayerIndex( int playerIndex, const IntVector2& agentBirthLocation, EntityType agentType, bool displayConsoleMessage=true );
	static Rgba GetColorForSquareType( ArenaSquareType squareType );
	Agent* GetAgentForEntityID( int entityID );
	void ApplyMoveOrderForPlayer( AgentOrder& order, Player& player );
	void SpawnNewFoodMaybe( int turnNumber );
	void Validate();

	int GetNumEntitiesAtLocation( const IntVector2& location )
	{
		int index = location.x + (m_arenaInfo.width * location.y);
		return (int) m_arenaEntitiesByLocation[ index ].size();
	}

	void AddEntityToLocation( Agent* agent, const IntVector2& location )
	{
		int index = location.x + (m_arenaInfo.width * location.y);
		m_arenaEntitiesByLocation[ index ].push_back( agent );
	}

	void RemoveEntityFromLocation( Agent* agent, const IntVector2& location )
	{
		int index = location.x + (m_arenaInfo.width * location.y);
		std::vector< Agent* >& agentsAtLocation = m_arenaEntitiesByLocation[ index ];
		std::vector< Agent* >::iterator found = std::find( agentsAtLocation.begin(), agentsAtLocation.end(), agent );
		if( found != agentsAtLocation.end() )
		{
			agentsAtLocation.erase( found );
		}
	}

public:
	std::string						m_name;
	ArenaInfo						m_arenaInfo;
	IntVector2						m_dimensions;
	int								m_startingNumberOfWorkersPerPlayer;
	int								m_startingNumberOfSoldiersPerPlayer;
	int								m_startingNumberOfScoutsPerPlayer;
	int								m_startingNumberOfQueensPerPlayer;
	int								m_maxNumberOfAgentsPerPlayer;
	float							m_fractionOfMapThatIsStone;
	float							m_fractionOfMapThatIsDirt;
	float							m_fractionOfMapThatIsFood;
	int								m_numStonesPerClump;
	int								m_numDirtsPerClump;
	int								m_turnsPerFoodSpawn;
	ArenaSquares					m_arenaSquares;
	std::vector< Agent* >			m_arenaEntitiesByLocation[ MAX_ARENA_SQUARES ];
	std::vector< Agent* >			m_allAgentReferences;
};



/////////////////////////////////////////////////////////////////////////////////////////////////
class ArenaManager
{
public:
	ArenaManager();
	~ArenaManager();

//	void StartArenaWithPlayers( const std::string& arenaName, std::vector< 
public:
	std::vector< Arena* >			m_arenas;
	Arena*							m_currentArenaCopy;
};


#endif // __include_ArenaManager__
