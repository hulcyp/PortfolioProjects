class AlienSWFWrapper extends GFxMinimapHUD;

var bool bReticlesEnabled;

struct FlagIcon
{
	var GFxObject flagMC;
	var Vector defaultPos;
	var Vector currentPos;
	var Vector capVel;
	var bool isRunning;
};

//var float lastHealth;
//var int lastAmmoCount;
var int currentWeaponEquipped;
var float messageCoolDown;

var bool bIsZoomed;

// MC: Movie clip, TF: Text field
var GFxObject healthMC;
var GFxObject ammoMC;
var float ammoMCScale;
var GFxObject ammoTF;
var GFxObject redScoreTF;
var GFxObject blueScoreTF;
var GFxObject messageTF;
var GFxObject weaponsPanelMC;
var GFxObject majorHUDMessageTF;

var GFxObject scoreboardPanelMC;
var GFxObject reticleMC;
var GFxObject healthPanelMC;
var GFxObject weaponsPaneMC;
var GFxObject majorHUDMessageMC;
var GFxObject flagMessageTF;
var GFxObject flagHeldAlertMessageTF;
var GFxObject msCenterTextTF;
var GFxObject gameTimeTF;
var GFxObject sniperScopeAmmoTF;

var FlagIcon flagIcons[2];
var class<UTWeapon> lastWeaponType;
var Vector flagCapIconVel;

var bool livingHUD;

/////////////////////////////////////////////////////////////
// Colors
////////////////////////////////////////////////////////////
var LinearColor blue;
var LinearColor red;
var LinearColor yellow;
var LinearColor white;


function registerMiniMapView( GFxMinimap mc, float r )
{
	super.registerMiniMapView( mc, r );
}

function int roundNum( float num )
{
	local int iNum;
	local float fNum;

	fNum = num;
	iNum = int( fNum );
	fNum -= iNum;

	if( fNum >= 0.5f )
	{
		iNum += 1;
	}

	return iNum;
}

function int getPercentage( int val, int max )
{
	return roundNum( ( float( val ) / float( max ) ) * 100.0f );
}

function Init( optional LocalPlayer LP )
{
	super.Init( LP );

	//Load and start the movie
	
	super.Start();
	Advance( 0.0f );

	lastHealth = -1;
	lastAmmoCount = -1;
	
	healthMC        =	GetVariableObject( "_root.HealthPanel.HealthBarMC" );
	gameTimeTF              = GetVariableObject( "_root.HealthPanel.GameTime" );
	ammoMC          =	GetVariableObject( "_root.Weapons.AmmoBarMC" );
	ammoTF          =	GetVariableObject( "_root.Weapons.ammoTF" );
	redScoreTF      =   GetVariableObject( "_root.Scoreboard.redScoreTF" );
	blueScoreTF     =   GetVariableObject( "_root.Scoreboard.blueScoreTF" );
	majorHUDMessageTF		= GetVariableObject( "_root.majorHUDMessage.messageTF" );
	majorHUDMessageMC		= GetVariableObject( "_root.majorHUDMessage" );
	flagMessageTF			= GetVariableObject( "_root.FlagMessageTF" );
	flagHeldAlertMessageTF  = GetVariableObject( "_root.AlertMessageTF" );
	msCenterTextTF          = GetVariableObject( "_root.CenterTextTF" );
	sniperScopeAmmoTF       = GetVariableObject( "_root.WeaponReticles.SniperScopeAmmo" );
	
	ammoMCScale = ammoMC.GetFloat( "scaleX" );

	scoreboardPanelMC   = GetVariableObject( "_root.Scoreboard" );
	reticleMC           = GetVariableObject( "_root.WeaponReticles" );
	healthPanelMC       = GetVariableObject( "_root.HealthPanel" );
	weaponsPaneMC       = GetVariableObject( "_root.Weapons" );

	flagIcons[0].flagMC = GetVariableObject( "_root.Scoreboard.MusheePurpleMC" );
	flagIcons[0].flagMC.GetPosition( flagIcons[0].defaultPos.X, flagIcons[0].defaultPos.Y );
	flagIcons[0].currentPos = flagIcons[0].defaultPos;
	flagIcons[0].capVel = flagCapIconVel;
	flagIcons[0].isRunning = false;

	flagIcons[1].flagMC = GetVariableObject( "_root.Scoreboard.MusheeGreenMC" );
	flagIcons[1].flagMC.GetPosition( flagIcons[1].defaultPos.X, flagIcons[1].defaultPos.Y );
	flagIcons[1].currentPos = flagIcons[1].defaultPos;
	flagIcons[1].capVel = flagCapIconVel;
	flagIcons[1].isRunning = false;
}

function TickHUD( float dt )
{
	tickLivingHUD( dt );
}

function tickLivingHUD( float dt )
{
	local UTPawn UTP;
	local UTWeapon UTW;
	local MusheeGameReplicationInfo gameRepInfo;
	local PlayerController PC;
	local float ammoPercentage;
	local Inventory inventoryItem;
	local MusheeSniperRifleWeapon sniperWeapon;

	PC = GetPC();
	gameRepInfo = MusheeGameReplicationInfo( PC.WorldInfo.GRI );
	
	if( gameRepInfo == none )
		return;
	
	if( PC == none )
		return;

	UTP = UTPawn( PC.Pawn );
	if( UTP == none )
		return;
	
	UTW = UTWeapon( UTP.Weapon );

	inventoryItem = UTP.FindInventoryType( class'MusheeSniperRifleWeapon' );
	sniperWeapon = MusheeSniperRifleWeapon( inventoryItem );
	
	if( !bIsZoomed )
	{
		// Set flag scores
		redScoreTF.SetString( "text", int( gameRepInfo.Teams[0].Score ) $"" );
		blueScoreTF.SetString( "text", int( gameRepInfo.Teams[1].Score ) $"" );
	
		updatePlayerHealth( UTP );
		
		if( UTW != none )
			updateWeaponAmmo( UTW );
		
		updateMusheesBeingCapped( gameRepInfo, dt );

		// Update game time
		gameTimeTF.SetText( class'GFxUIScoreboard'.static.FormatTime( gameRepInfo.TimeLimit != 0 ? gameRepInfo.RemainingTime : gameRepInfo.ElapsedTime ) );
	
	}
	else
	{
		// If zoomed in set the sniper scope ammo count
		if( UTW != none )
		{
			if( lastAmmoCount != UTW.AmmoCount )
			{
				lastAmmoCount = UTW.AmmoCount;
				ammoPercentage = getPercentage( lastAmmoCount, UTW.MaxAmmoCount );
				sniperScopeAmmoTF.SetString( "text", string( lastAmmoCount ) );
				if( ammoMC != none )
				{
					ammoMC.SetFloat( "scaleX", ( ammoPercentage > 100 ) ? 1.0f * ammoMCScale : ( ammoPercentage/100.0f ) * ammoMCScale );
				}
			}
		}
	}
	
	if( UTW != none )
		updateSniperWeapon( sniperWeapon, UTW );
	
	swapWeapon( UTP );
}

function updatePlayerHealth( UTPawn UTP )
{
	local float healthPercentage;
	
	if( UTP == none )
		return;

	healthPercentage = getPercentage( UTP.Health, UTP.HealthMax ); 
	if( lastHealth !=  healthPercentage )
	{
		lastHealth = getPercentage( UTP.Health, UTP.HealthMax );

		if( healthMC != none )
		{
			healthMC.SetFloat( "scaleX", ( lastHealth > 100 ) ? 1.0f : lastHealth/100.0f );
		}
	}
}

function updateWeaponAmmo( UTWeapon UTW )
{	
	local float ammoPercentage;

	if( UTW == none )
		return;

	if( lastAmmoCount != UTW.AmmoCount )
	{
		lastAmmoCount = UTW.AmmoCount;
		ammoPercentage = getPercentage( lastAmmoCount, UTW.MaxAmmoCount );
		ammoTF.SetString( "text", string( lastAmmoCount ) );
		if( ammoMC != none )
		{
			ammoMC.SetFloat( "scaleX", ( ammoPercentage > 100 ) ? 1.0f*ammoMCScale : ( ammoPercentage/100.0f ) * ammoMCScale );
		}
	}
}

function updateMusheesBeingCapped( MusheeGameReplicationInfo gameRepInfo, float dt )
{
	local Vector vel;
	if( gameRepInfo.replicatedMushee[0] == none )
		return;

	if( gameRepInfo.replicatedMushee[0].isBeingCapped )
	{
		if( gameRepInfo.replicatedMushee[0].baseBeingCappedAt.GetTeamNum() == 0 )
		{
			vel = flagIcons[0].capVel;
			
			if( !flagicons[0].isRunning  )
				flagIcons[0].flagMC.ActionScriptVoid( "startRunningRight" );
		}
		else
		{
			vel = flagIcons[0].capVel * -1.0f;
			if( !flagicons[0].isRunning  )
				flagIcons[0].flagMC.ActionScriptVoid( "startRunningLeft" );
		}
		flagIcons[0].isRunning = true;
	
		flagIcons[0].currentPos = flagIcons[0].currentPos + vel * dt;

		flagIcons[0].flagMC.SetPosition( flagIcons[0].currentPos.X, flagIcons[0].currentPos.Y );

	}
	else
	{
		flagIcons[0].currentPos = flagIcons[0].defaultPos;
		flagIcons[0].flagMC.SetPosition( flagIcons[0].defaultPos.X, flagIcons[0].defaultPos.Y );
		if( flagIcons[0].isRunning )
		{
			flagIcons[0].flagMC.ActionScriptVoid( "stopRunning" );
			flagIcons[0].isRunning = false;
		}
	}

	if( gameRepInfo.replicatedMushee[1].isBeingCapped )
	{
		if( gameRepInfo.replicatedMushee[1].baseBeingCappedAt.GetTeamNum() == 0 )
		{
			vel = flagIcons[1].capVel;
			if( !flagicons[1].isRunning  )
				flagIcons[1].flagMC.ActionScriptVoid( "startRunningRight" );
		}
		else
		{
			vel = flagIcons[1].capVel * -1.0f;
			if( !flagicons[1].isRunning  )
				flagIcons[1].flagMC.ActionScriptVoid( "startRunningLeft" );
		}

		
		flagIcons[1].isRunning = true;	
		flagIcons[1].currentPos = flagIcons[1].currentPos + vel * dt;
		flagIcons[1].flagMC.SetPosition( flagIcons[1].currentPos.X, flagIcons[1].currentPos.Y );
	
	}
	else
	{
		flagIcons[1].currentPos = flagIcons[1].defaultPos;
		flagIcons[1].flagMC.SetPosition( flagIcons[1].defaultPos.X, flagIcons[1].defaultPos.Y );
		if( flagIcons[1].isRunning )
		{
			flagIcons[1].flagMC.ActionScriptVoid( "stopRunning" );
			flagIcons[1].isRunning = false;
		}
	}
}

function updateSniperWeapon( MusheeSniperRifleWeapon sniperWeapon, UTWeapon UTW )
{	
	local EZoomState zoomState;
	
	if( sniperWeapon != none )
	{
		if( sniperWeapon.IsInState( 'Active' )  || sniperWeapon.IsInState( 'WeaponFiring' ) )
		{
			zoomState = sniperWeapon.GetZoomedState();
			if( zoomState == ZST_Zoomed || zoomState == ZST_ZoomingIn || zoomState == ZST_ZoomingOut || sniperWeapon.CurrentFireMode == 1 )
			{
				if( !bIsZoomed )
				{
					ActionScriptVoid( "_root.enableSniperScope" );
					lastAmmoCount = UTW.AmmoCount;
					sniperScopeAmmoTF.SetString( "text", string( lastAmmoCount ) );			
					bIsZoomed = true;
				}
			}
			else
			{
				ActionScriptVoid( "_root.disableSniperScope" );
				bIsZoomed = false;
				lastAmmoCount = UTW.AmmoCount;
				ammoTF.SetString( "text", string( lastAmmoCount ) );
			}
		}
	}
}

function swapWeapon( UTPawn UTP )
{
	if( lastWeaponType != UTP.Weapon.Class )
	{
		if( UTP.Weapon.Class == class'MusheeShotgunWeapon' )
		{
			lastWeaponType = class'MusheeShotgunWeapon';
			ActionScriptVoid( "_root.disableSniperScope" );
			ActionScriptVoid( "_root.WeaponReticles.shotgunReticle" );
			ActionScriptVoid( "_root.Weapons.shotgunEquipped" );
			bIsZoomed = false;
			bReticlesEnabled = true;
		}
		else if( UTP.Weapon.Class == class'MusheeAssaultRifleWeapon' )
		{
			lastWeaponType = class'MusheeAssaultRifleWeapon';
			ActionScriptVoid( "_root.disableSniperScope" );
			ActionScriptVoid( "_root.WeaponReticles.blastorPistolReticle" );
			ActionScriptVoid( "_root.Weapons.blasterPistolEquipped" );
			bIsZoomed = false;
			bReticlesEnabled = true;
		}
		else if( UTP.Weapon.Class == class'MusheeSniperRifleWeapon' )
		{
			lastWeaponType = class'MusheeSniperRifleWeapon';
			ActionScriptVoid( "_root.WeaponReticles.sniperRifleReticle" );
			ActionScriptVoid( "_root.Weapons.sniperRifleEquipped" );
			bReticlesEnabled = true;
		}
	}
}

function displayMajorHUDMessage( string message )
{
	majorHUDMessageMC.SetBool( "visible", true );
	majorHUDMessageTF.SetString( "text", message );

}

function setLivingHUD( bool enabled )
{
	if( !bIsZoomed )
	{
		scoreboardPanelMC.SetBool( "visible", enabled );
		reticleMC.SetBool( "visible", enabled ); 
		healthPanelMC.SetBool( "visible", enabled );
		weaponsPaneMC.SetBool( "visible", enabled );
		gameTimeTF.SetBool( "visible", enabled );
	}
}

function setMajorHUDMessage( bool enabled )
{
	majorHUDMessageMC.SetBool( "visible", enabled );
}

function ShowFlagCapped( int n, string msg, int teamIndex )
{
	local ASColorTransform asColor;

	if( teamIndex == 0 )
	{
		asColor.multiply = red;		
	}
	else
	{
		asColor.multiply = blue;	
	}
	flagMessageTF.SetColorTransform( asColor );
	flagMessageTF.SetString( "text", msg );
	flagMessageTF.GotoAndPlay( "play" );
}

function displayFlagHeldWarning( string youHaveTheFlag, string enemyHasTheFlag )
{
	local ASColorTransform asColor;
	local MusheeGameReplicationInfo gameRepInfo;
	local PlayerController PC;
	
	PC = GetPC();

	if( PC == none )
		return;

	gameRepInfo = MusheeGameReplicationInfo( PC.WorldInfo.GRI );	

	if( gameRepInfo.replicatedMushee[0].IsInState( 'Held' ) || gameRepInfo.replicatedMushee[1].IsInState( 'Held' ) )
	{

		if( ( gameRepInfo.replicatedMushee[0].HolderPRI != none && gameRepInfo.replicatedMushee[0].HolderPRI == PC.PlayerReplicationInfo ) ||
			( gameRepInfo.replicatedMushee[1].HolderPRI != none && gameRepInfo.replicatedMushee[1].HolderPRI == PC.PlayerReplicationInfo ) )
		{
			asColor.multiply = yellow;
			flagHeldAlertMessageTF.SetColorTransform( asColor );
			flagHeldAlertMessageTF.SetString( "text", youHaveTheFlag );
			flagHeldAlertMessageTF.GotoAndPlay( "play" );
		}
		else if( ( gameRepInfo.replicatedMushee[0].HolderPRI != none && gameRepInfo.replicatedMushee[0].HolderPRI.GetTeamNum() != PC.GetTeamNum() ) ||
				 ( gameRepInfo.replicatedMushee[1].HolderPRI != none &&	gameRepInfo.replicatedMushee[1].HolderPRI.GetTeamNum() != PC.GetTeamNum() ) )
		{
			asColor.multiply = red;
			flagHeldAlertMessageTF.SetColorTransform( asColor );
			flagHeldAlertMessageTF.SetString( "text", enemyHasTheFlag );
			flagHeldAlertMessageTF.GotoAndPlay( "play" );
		}
	}
}

function setFlagHeldWarningEnabled( bool enabled )
{
	if( !enabled )
	{
		flagHeldAlertMessageTF.GotoAndPlay( "stop" );
	}
}

function SetCenterText(string text)
{
	msCenterTextTF.SetString( "text", text );
	msCenterTextTF.GotoAndPlay( "play" );
}


DefaultProperties
{
	bIsZoomed = false;

	bDisplayWithHudOff = false
	MovieInfo = SwfMovie'MS_HUD_RedTeam.MS_HUDRedTeam'
	//blueMovieInfo = SwfMovie'MS_HUD_BlueTeam.MS_HUDBlueTeam'
	//redMovieInfo = SwfMovie'MS_HUD_RedTeam.MS_HUDRedTeam'

	blue=( R=0, G=0.6f, B=1.0f )
	red=( R=1.0f, G=0, B=0 )
	yellow=( R=1.0f, G=1.0f, B=0 )
	white=( R=1.0f, G=1.0f, B=1.0f )
	
	flagCapIconVel=(X=20)
}
