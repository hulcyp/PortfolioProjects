class MSOddballSwfWrapper extends AlienSWFWrapper;


function tickLivingHUD( float dt )
{
	local PlayerController PC;
	local AlienPawnOddball APO;
	local UTPlayerReplicationInfo UTPRI;
	
	super.tickLivingHUD( dt );
	
	PC = getPC();

	if( PC != none )
	{
		UTPRI = UTPlayerReplicationInfo( PC.PlayerReplicationInfo );
		APO = AlienPawnOddball( PC.Pawn );

		if( APO == none )
			return;
		if( UTPRI == none )
			return;

		if( bReticlesEnabled && UTPRI.bHasFlag )
		{
			bReticlesEnabled = false;
			enableReticles( bReticlesEnabled );
		}
		else if( !APO.bFlagStatus && !bReticlesEnabled )
		{			
			bReticlesEnabled = true;
			enableReticles( bReticlesEnabled );
		}
		else if( APO.bFlagStatus && !UTPRI.bHasFlag )
		{
			bReticlesEnabled = true;
			enableReticles( bReticlesEnabled );
		}
	}
}

function updateMusheesBeingCapped( MusheeGameReplicationInfo gameRepInfo, float dt )
{
	local Vector vel;

	if( gameRepInfo.replicatedMushee[0] == none )
		return;

	if( gameRepInfo.replicatedMushee[0].bPlayerIsMushee && !gameRepInfo.bMatchIsOver )
	{
		if( gameRepInfo.replicatedMushee[0].playerTeamNum == 0 )
		{
			vel = flagIcons[0].capVel;
			
			if( !flagicons[0].isRunning  )
				flagIcons[0].flagMC.ActionScriptVoid( "startRunningRight" );

			flagIcons[0].isRunning = true;
			
			gameRepInfo.replicatedMushee[0].posOnHUDRedForOddball = gameRepInfo.replicatedMushee[0].posOnHUDRedForOddball + vel * dt;
		}
		else
		{
			vel = flagIcons[1].capVel * -1.0f;
			if( !flagicons[1].isRunning  )
				flagIcons[1].flagMC.ActionScriptVoid( "startRunningLeft" );

			flagIcons[1].isRunning = true;	
			
			gameRepInfo.replicatedMushee[0].posOnHUDBlueForOddball = gameRepInfo.replicatedMushee[0].posOnHUDBlueForOddball + vel * dt;
		}

	}
	else
	{
		if( flagIcons[0].isRunning || gameRepInfo.bMatchIsOver )
		{
			flagIcons[0].flagMC.ActionScriptVoid( "stopRunning" );
			flagIcons[0].isRunning = false;
		}
		if( flagIcons[1].isRunning || gameRepInfo.bMatchIsOver )
		{
			flagIcons[1].flagMC.ActionScriptVoid( "stopRunning" );
			flagIcons[1].isRunning = false;
		}
	}
	
	flagIcons[0].currentPos = gameRepInfo.replicatedMushee[0].posOnHUDRedForOddball;
	flagIcons[0].flagMC.SetPosition( flagIcons[0].currentPos.X, flagIcons[0].currentPos.Y );
	flagIcons[1].currentPos = gameRepInfo.replicatedMushee[0].posOnHUDBlueForOddball;
	flagIcons[1].flagMC.SetPosition( flagIcons[1].currentPos.X, flagIcons[1].currentPos.Y );
}

function enableReticles( bool enable )
{
	if( !enable )
	{
		ActionScriptVoid( "_root.WeaponReticles.clearReticles" );
	}
	else
	{
		lastWeaponType = none;
	}
}

DefaultProperties
{
	MovieInfo = SwfMovie'MS_HUD_RedTeam.MS_HUDRedTeam'
	flagCapIconVel=(X=2.5)
	bReticlesEnabled=true
}
