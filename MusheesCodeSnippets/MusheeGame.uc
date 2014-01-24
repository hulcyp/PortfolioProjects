class MusheeGame extends UTCTFGame
	config(Mushees);

var array<Mushee> mushees;
var const array<UTWeapon> DefaultWeaponArchetypes;

var UTCTFNeutralFlagBase redNeutralBase[3];
var UTCTFNeutralFlagBase neutralBase[3];
var UTCTFNeutralFlagBase blueNeutralBase[3];

var SatuBase vSatuBase;		// Red base index 0

var DuaBase vDuaBase;		// Blue base index 1

var string locationIDs[3];

function InitGame( string options, out string errorMessage )
{
	super.InitGame( options, errorMessage );
}

event AddDefaultInventory(Pawn P)
{
	local int i;
	local MusheeInventoryManager InventoryMgr;

	// Ensure the pawn is valid
	if (P == None)
	{
		return;
	}
	
	    //For generic inventory items
    for (i = 0; i < DefaultInventory.Length; i++)
    {
        // Ensure we don't give duplicate items
        if (P.FindInventoryType( DefaultInventory[i] ) == None)
        {
            // Only activate the first weapon
            P.CreateInventory( DefaultInventory[i], (i > 0 ) );
        }
    }

	// Ensure that the pawn has the right inventory mananger
	InventoryMgr = MusheeInventoryManager(P.InvManager);
	if (InventoryMgr == None)
	{
		return;
	}

	for (i = 0; i < DefaultWeaponArchetypes.Length; ++i)
	{
		if (DefaultWeaponArchetypes[i] != None)
		{
			// Create the inventory from the weapon archetype
			InventoryMgr.CreateInventoryFromArchetype(DefaultWeaponArchetypes[i]);
		}
	}
	super.AddDefaultInventory(P);
}

static event class<GameInfo> SetGameType( string MapName, string Options, string Portal )
{
	return Default.Class;
}

function PostBeginPlay()
{
	
	MSCTFTeamAI( Teams[0].AI ).FriendlyFlag = mushees[0];
	MSCTFTeamAI( Teams[1].AI ).FriendlyFlag = mushees[1];	

	MSCTFTeamAI( Teams[0].AI ).mushees[0] = mushees[0];
	MSCTFTeamAI( Teams[0].AI ).mushees[1] = mushees[1];
	MSCTFTeamAI( Teams[1].AI ).mushees[0] = mushees[0];
	MSCTFTeamAI( Teams[1].AI ).mushees[1] = mushees[1];
}

function SetEndGameFocus( PlayerReplicationInfo winner )
{
	local Controller p;
	
	if( winner != None )
	{
		if( winner.Team.TeamIndex == 0 )
		{
			EndGameFocus = vSatuBase;
		}
		else
		{
			EndGameFocus = vDuaBase;
		}
	}
	
	if( EndGameFocus != none )
	{
		EndGameFocus.bAlwaysRelevant = true;
	}
	
	foreach WorldInfo.AllControllers( class'Controller', p )
	{
		p.GameHasEnded( EndGameFocus, ( p.PlayerReplicationInfo != none ) && ( p.PlayerReplicationInfo.Team == GameReplicationInfo.Winner ) );		
	}
}

function bool GetLocationFor( Pawn statusPawn, out Actor locationObject, out int messageIndex, int locationSpeechOffset )
{
	local UTPickupFactory f;
	local UDKGameObjective best;
	local UTBot b;
	
	// see if it's a bot heading for an objective or a power up
	b = UTBot( statusPawn.Controller );
	if( b != none )
	{
		f = UTPickupFactory( statusPawn.Controller.RouteGoal );
		if( ( f != none ) && f.bHasLocationSpeech )
		{
			MessageIndex = 0;
			LocationObject = f;
			return true;
		}
	}
	
	best = ( VSizeSq( statusPawn.Location - vSatuBase.Location ) < VSizeSq( statusPawn.Location - vDuaBase.Location ) )
				? vSatuBase
				: vDuaBase;

	MessageIndex = UTGameObjective( best ).GetLocationMessageIndex( b, statusPawn );
	LocationObject = best;
	return true;

	MessageIndex = 10;
	return true;	
}

function RegisterFlag( UTCTFFlag f, int teamIndex )
{
	//update this games replication info with the new flag that was added
	MusheeGameReplicationInfo( GameReplicationInfo ).replicatedMushee[ mushees.Length ] = Mushee( f );
	mushees.AddItem( Mushee( f ) );
}

function RegisterNeutralBase( UTCTFNeutralFlagBase b, string locationID, int baseGroup  )
{
	local int teamIndex;

	teamIndex = getIndexFromLocationID( locationID );

	switch( teamIndex )
	{
	case 0:
		redNeutralBase[baseGroup] = b;
		break;
	case 1:
		neutralBase[baseGroup] = b;
		break;
	case 2:
		blueNeutralBase[baseGroup] = b;
		break;
	}
}

function RegisterRedBase( SatuBase b )
{
	vSatuBase = b;
	MusheeGameReplicationInfo( GameReplicationInfo ).replicatedSatuBase = b;
	MSCTFTeamAI( Teams[0].AI ).friendlyBase = vSatuBase;
}

function RegisterBlueBase( DuaBase b )
{
	vDuaBase = b;
	MusheeGameReplicationInfo( GameReplicationinfo ).replicatedDuaBase = b;
	MSCTFTeamAI( Teams[1].AI ).friendlyBase = vDuaBase;
}

function ViewObjective( PlayerController pc )
{
	local Actor a;
	
	if( ( pc.ViewTarget != none ) &&
		( pc.ViewTarget == Flags[0] || ( pc.ViewTarget == Flags[0].Holder || pc.ViewTarget == Flags[0].HomeBase ) ) )
	{
		a = Flags[1].HomeBase.GetBestViewTarget();
		pc.SetViewTarget( a );
		if( UTPlayerController( pc ) != none )
		{
			UTPlayerController( pc ).ClientSetBehindView( true );
		}
	}
	else
	{
		a = Flags[0].HomeBase.GetBestViewTarget();
		pc.SetViewTarget( a );
		if( UTPlayerController( pc ) != none )
		{
			UTPlayerController( pc ).ClientSetBehindView( true );
		}
	}
}

function ScoreFlag( Controller scorer, UTCTFFlag theFlag )
{
	local int teamLoc;
	if( theFlag.Team != none && theFlag.Team.TeamIndex != AlienPlayerController( scorer ).PlayerReplicationInfo.Team.TeamIndex )
	{
		theFlag.Team.Score -= 1.0;
		theFlag.Team.bForceNetUpdate = true;	
	}
	
	teamLoc = getIndexFromLocationID( Mushee( theFlag ).locationID );
	if( scorer.GetTeamNum() == 0 )
	{
		teamLoc = clamp( teamLoc + 1, 0, 2 );
		
	}
	else
	{
		teamLoc = clamp( teamLoc - 1, 0, 2 );
	}
	
	switch( teamLoc )
	{
		case 0:					
			theFlag.HomeBase = redNeutralBase[ Mushee( theFlag ).baseGroupIndex ];				
			break;
		case 1:
			theFlag.HomeBase = neutralBase[ Mushee( theFlag ).baseGroupIndex ];
			break;
		case 2:
			theFlag.HomeBase = blueNeutralBase[ Mushee( theFlag ).baseGroupIndex ];
			break;
	}
	Mushee( theFlag ).locationID = locationIDs[teamLoc];
	
	super.ScoreFlag( scorer, theFlag );
	
	
	if( theFlag.Team == none )
	{
		//SetTimer( 45.0, false, 'newMushee' );

	}
}


function bool TooManyBots( Controller botToRemove )
{
	return ( NumBots + NumPlayers > DesiredPlayerCount );
}

function Killed( Controller killer, Controller killedPlayer, Pawn killedPawn, class<DamageType> damageType )
{
	super.Killed( killer, killedPlayer, killedPawn, damageType );	
}

function ScoreKill( Controller killer, Controller other )
{
	super.ScoreKill( killer, other );
}

function bool NeedPlayers()
{

	if( WorldInfo.NetMode == NM_Standalone )
	{
		return super.NeedPlayers();
	}
	return false;
}

function float RatePlayerStart( PlayerStart p, byte team, Controller player )
{
	if( bSpawnInTeamArea )
	{
		if( UTTeamPlayerStart( p ) == none )
		{
			`warn( P$" is not a team playerstart!" );
			return -9;
		}
		if( team != UTTeamPlayerStart( p ).TeamNumber )
		{
			return -9;
		}
		return Super(UTGame).RatePlayerStart( p, team, player );
	}
}

function GenericPlayerInitialization( Controller c )
{
	super(GameInfo).GenericPlayerInitialization( c );
}

function int getIndexFromLocationID( string locationID )
{
	local int teamIndex;
	local int i;
	for( i = 0; i < 3; ++i )
	{
		if( locationID ~= locationIDs[i] )
		{
			teamIndex = i;
		}
	}
	return teamIndex;
}

exec function ChangeSatuCapTime( float capTime )
{
	vSatuBase.ChangeBaseCapTime( capTime );
}

exec function ChangeDuaCapTime( float capTime )
{
	vDuaBase.ChangeBaseCapTime( capTime );
}

defaultproperties
{
	bPlayersVsBots = false;
	bCustomBots = true;
	bAutoNumBots = false;

	GameReplicationInfoClass = class'Mushees.MusheeGameReplicationInfo'
	PlayerReplicationInfoClass = class'Mushees.AlienPlayerReplicationInfo'
	DefaultPawnClass = class'Mushees.AlienPawn'
	PlayerControllerClass = class'Mushees.AlienPlayerController'
	BotClass=class'MSBot'
	
	TeamAIType(0)=class'Mushees.MSCTFTeamAI'
	TeamAIType(1)=class'Mushees.MSCTFTeamAI'

	HUDType = class'Mushees.AlienHUDBase'
	bUseClassicHUD = true;

	locationIDs(0) = "red"
	locationIDs(1) = "neutral"
	locationIDs(2) = "blue"

	/******************************************************
	 * Custom archetype inventory registration
	 * ****************************************************/
	DefaultInventory(0)=None;
	DefaultInventory(1)=None;
	DefaultInventory(2)=None;

	DefaultWeaponArchetypes(0) = MusheeAssaultRifleWeapon'MS_Weapons.Archetype.MusheeAssaultRifle';
	
}
