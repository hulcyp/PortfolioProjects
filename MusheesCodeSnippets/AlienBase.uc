class AlienBase extends UTCTFBase_Content
	placeable;

struct PendingMushee
{
	var Mushee flag;
	var Controller scorer;
};

var array<PendingMushee> pendingMusheesToScore;
var bool bHasPendingMushees;

var() float flagCapTime;
var() AudioComponent flagCapturedSound;
var() AudioComponent flagBeingCapturedSound;

var() StaticMeshActor tractorBeam;

var int baseTeamID;

replication
{
	if( bNetDirty )
		tractorBeam, bHasPendingMushees;
}

simulated function PostBeginPlay()
{
	if( Role < ROLE_Authority )
	{
		return;
	}
}

exec function changeBaseCapTime( int capTime )
{
	flagCapTime = capTime;
}

simulated function Tick( float deltaTime )
{
	local Mushee flag;
	local Pawn p;
	local float radius;
	local float distSqrd;
	local Vector xyPlanePawnLoc;
	local Vector xyPlaneBaseLoc;
	local UTPlayerReplicationInfo UTPRI;
	local PendingMushee pendMushee;
	local PendingMushee iPendMushee;
	local array<PendingMushee> musheesToScore;
	local int index;
	radius = 100;

	xyPlaneBaseLoc.X = Location.X;
	xyPlaneBaseLoc.Y = Location.Y;
	xyPlaneBaseLoc.Z = 0.0;

	foreach AllActors( class'Pawn', p )
	{
		xyPlanePawnLoc.X = p.Location.X;
		xyPlanePawnLoc.Y = p.Location.Y;
		xyPlanePawnLoc.Z = 0.0;

		distSqrd = VSizeSq( xyPlanePawnLoc - xyPlaneBaseLoc );
		if( distSqrd <= radius*radius )
		{
			if( p.Controller != none )
			{
				UTPRI = UTPlayerReplicationInfo( p.Controller.PlayerReplicationInfo );
				if( UTPRI != none )
				{	
					if( p.GetTeamNum() == baseTeamID )
					{
						if( UTPRI.bHasFlag )
						{
							flag = Mushee( UTPRI.GetFlag() );
							if( flag.Team == none || flag.Team.TeamIndex == baseTeamID )
							{
								pendMushee.flag = flag;
								pendMushee.flag.currentCapTime = flagCapTime;
								pendMushee.flag.isBeingCapped = true;
								pendMushee.flag.baseBeingCappedAt = self;
								pendMushee.scorer = p.Controller;
								pendingMusheesToScore.AddItem( pendMushee );
								UTPRI.SetFlag( None );
								pendMushee.flag.GotoState( 'BeingCapped' );
							}
						}				
					}
					else
					{
						if( !UTPRI.bHasFlag )
						{
							if( pendingMusheesToScore.Length != 0 )
							{
								pendingMusheesToScore[0].flag.isBeingCapped = false;
								pendingMusheesToScore[0].flag.SetHolder( p.Controller );
								UTPRI.SetFlag( pendingMusheesToScore[0].flag );
								pendingMusheesToScore.Remove( 0, 1 );
							}
						}
					}
				}
			}
		}
	}

	if( pendingMusheesToScore.Length != 0 )
	{
		bHasPendingMushees = true;
		if( flagBeingCapturedSound != none && flagBeingCapturedSound.IsPlaying() == false )
		{
			PlaySound( flagBeingCapturedSound.SoundCue, false, false, false, self.Location );
		}
		index = 0;
		foreach pendingMusheesToScore( iPendMushee, index )
		{
			if( iPendMushee.flag.currentCapTime <= 0.0f )
			{
				MusheeGame( WorldInfo.Game ).ScoreFlag( iPendMushee.scorer, iPendMushee.flag );
				iPendMushee.flag.MusheeScore( self, iPendMushee.scorer.PlayerReplicationInfo );
				musheesToScore.AddItem( iPendMushee );
				
			}
			else
			{
				pendingMusheesToScore[index].flag.currentCapTime -= deltaTime;
			}
		}
		foreach musheesToScore( iPendMushee, index )
		{
			if( flagCapturedSound != none )
			{
				PlaySound( flagCapturedSound.SoundCue, false, false, false, self.Location );
			}
			pendingMusheesToScore.RemoveItem( iPendMushee );
			iPendMushee.flag.isBeingCapped = false;
		}
		bHasPendingMushees = pendingMusheesToScore.Length != 0;
		tractorBeam.SetHidden( false );
	}
	else if( bHasPendingMushees )
	{
		tractorBeam.SetHidden( false );
	}
	else
	{
		if( flagBeingCapturedSound != none && flagBeingCapturedSound.IsPlaying() == true )
		{
			flagBeingCapturedSound.Stop();
		}
		tractorBeam.SetHidden( true );		
		bHasPendingMushees = false;
	}
}



DefaultProperties
{
	baseTeamID = 1
	flagCapTime = 15

	//SupportedEvents.Add( class'MSSeqEvent_FlagBeingCapped' )
}
