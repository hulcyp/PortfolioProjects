//-----------------------------------------------------------------------------------------------
// TheGame.hpp
//-----------------------------------------------------------------------------------------------
#ifndef __include_TheGame__
#define __include_TheGame__

#include "GameShared.hpp"


//-----------------------------------------------------------------------------------------------
// Forward declarations
class Scenario;


//-----------------------------------------------------------------------------------------------
// Global variables



/////////////////////////////////////////////////////////////////////////////////////////////////
class TheGame
{
public:
	TheGame();
	~TheGame();

	void Startup( const std::string& appCommandLine );
	void Shutdown();
	void RunFrame();
	void SetUpView();
	void Update();
	double AdjudicateTurn();
	void ApplyMoveOrders( Arena& arena );
	void AddMissingReports( Arena& arena );
	void RunCombatAndUpkeep( Arena& arena );
	void RunCombat( Arena& arena );
	void CheckForQueensWithHostileAntsOnThem( Arena& arena );
	void ResolveCombatGroup( Arena& arena, std::vector< Agent* >& combatGroup );
	bool PerformOneRoundOfFighting( Arena& arena, std::vector< std::vector< Agent* > >& liveAgentsByFaction );
	void KillAgent( Agent& agent, Arena& arena, ReportCode reasonForDeath );
	void CalculateVisibility( Arena& arena );
	void BuildObservations( Arena& arena );
	void CheckForDefeatedPlayers( Arena& arena );
	void Render();
	double RenderScorecard( float virtualCoordsPerSquare );
	void DrawDebugGraphics();
	bool IsGamePaused() const { return m_isPaused; }
	void SetGamePaused( bool makePaused );
	bool IsQuitting() const { return m_isQuitting; }
	bool HandleWin32Message( UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
	bool ProcessKeyDownEvent( unsigned char keyCode );
	bool ProcessKeyUpEvent( unsigned char keyCode );
	bool IsKeyDown( unsigned char keyCode );
	void LoadDataFiles();
	Vector2 GetCursorWorldPos();
	IntVector2 GetCursorSquareCoords();

public:
	int m_frameNumber;
	int m_turnNumber;
	int m_labelVerbosity;
	int m_winningPlayerIndex;
	bool m_isToAdvanceOnlyOnce;
	int m_debugGraphicsForPlayer;
	IntVector2 m_inspectedSquareCoords;
	std::map< unsigned int, std::vector< Agent* > > m_potentialPassThroughCombatGroups;

private:
	bool m_isQuitting;
	bool m_isPaused;
};


//extern TheGame* theGame;
extern bool g_showDebugOverlay;


#endif // __include_TheGame__
