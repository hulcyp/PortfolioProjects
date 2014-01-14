class BS_GfXHUD_SkillSelection extends GFxMoviePlayer;



enum UnitID
{
	BJORN,
	DAG,
	FREYA,
	ILIANA
};

struct AbilityPreset
{
	var array< int > BjornAbilityIndices;
	var array< int > DagAbilityIndices;
	var array< int > FreyaAbilityIndices;
	var array< int > IlianaAbilityIndices;
};

struct UnitInfo
{
	var GFxCLIKWidget InfoMC;
	var GFxObject Portrait;
	var GFxObject Name;
	var GFxObject HP;
	var GFxObject Atk;
	var GFxObject ClassName;
	var GFxObject BackgroundMC;
};

struct SkillsInfo
{
	var GFxObject ActionsInfoMC;
	var GFxObject ActionsInfoTF;
	var GFxObject PlayerActionButtonsMC;
	var GFxObject BackgroundMC;
};

struct UnitPanel
{
	var UnitInfo TheUnitsInfo;
	var SkillsInfo SkillsInfo;
	var GFxObject SkillPoolPanel;
	var GFxObject SkillPoolPanelLabel;
};

struct AbilitySlotButtonPair
{
	var GFxObject Slot;
	var GFxCLIKWidget Button;
};

struct AbilityPoolInfo
{
	var array<GFxCLIKWidget> Buttons;
	var array<GFxObject> PoolSlots;
	var GFxObject SelectionSlots[4];
};

struct HoverOverInfo
{
	var GFxObject HoverOverInfoMC;
	var GFxObject HoverOverInfoTitleTF;
	var GFxObject HoverOverInfoInfoTF;
	var GFxObject HoverOverInfoCornerTF;
	var GFxObject HoverOverInfoAPTextTF;
	var GFxObject HoverOverInfoIconMC;
};

var const int MAX_NUM_SKILLS;
var UnitPanel UnitPanels[4];
var BS_TurnManager TheTurnManager;
var BS_Pawn HeroUnits[4];


var BS_HeroAbilityPool TheAbilityPool;

var BS_Localization TheLocalizer;
var BS_GFxHUDMoviePlayer TheHUDMovie;
var BS_SaveSettings SaveSettings;

var Vector2D ItemPadding;
var Vector2D ContainerPadding;

var array< AbilityPoolInfo > UnitAbilityInfos;

var GFxCLIKWidget PresetsButtonBar;
var int timerBeforeUpdatingPreset;

var GFxCLIKWidget ShowHideScreenButton;

var HoverOverInfo TheHoverOverInfo;

var GFxObject ConfirmationScreen;
var bool bIsShowingConfirmationScreen;

// Sound
var SoundCue AbilitySelectionClickSound;
var SoundCue ButtonClickSound;
var SoundCue MenuClickSound;

var localized string BjornName;
var localized string DagName;
var localized string FreyaName;
var localized string IlianaName;

var localized string HPString;
var localized string AtkString;
var localized string SkillsString;
var localized string StartGame;
var localized string SavePresetStr;
var localized string ShowSkillScreenStr;
var localized string HideSkillScreenStr;
var localized string ConfirmationStr;
var localized string YesStr;
var localized string NoStr;

var bool bRightClickStarted;
var bool bShowingScreen;

function Init( optional LocalPlayer LocPlay )
{
	local string names[4];
	local string localizedNames[4];
	local int i;
	local int a;
	//local int a;
	local BS_PlayerController BSPC;
	local GFxCLIKWidget tempWidget;

	UnitAbilityInfos.Add( 4 );

	BSPC = BS_PlayerController( TheTurnManager.GetController( 0 ) );
	if( BSPC != none )
		TheHUDMovie = BS_HUD( BSPC.myHUD ).TheHUDMovie;

	TheLocalizer = BS_GameInfo( class'WorldInfo'.static.GetWorldInfo().Game ).TheLocalizer;
	SaveSettings = BS_GameInfo( class'WorldInfo'.static.GetWorldInfo().Game ).SaveSettings;
	Start();
	Advance( 0.f );

	names[0] = "Bjorn";
	names[1] = "Dag";
	names[2] = "Freya";
	names[3] = "Iliana";

	localizedNames[0] = TheLocalizer.GetLocalizedString( BjornName );
	localizedNames[1] = TheLocalizer.GetLocalizedString( DagName );
	localizedNames[2] = TheLocalizer.GetLocalizedString( FreyaName );
	localizedNames[3] = TheLocalizer.GetLocalizedString( IlianaName );

	for( i = 0; i < 4; ++i )
	{
		UnitPanels[ i ].TheUnitsInfo.InfoMC = GFxCLIKWidget( GetVariableObject( "_root." $names[i], class'GFxCLIKWidget' ) );
		UnitPanels[ i ].TheUnitsInfo.Portrait = GetVariableObject( "_root."$names[i]$".UnitPictures" );
		UnitPanels[ i ].TheUnitsInfo.Portrait.GotoAndStopI( i + 1 );
		UnitPanels[ i ].TheUnitsInfo.Name = GetVariableObject( "_root."$names[i]$".UnitSelected" );
	
		AutosizeTextFont( UnitPanels[ i ].TheUnitsInfo.Name.GetObject( "textField" ), localizedNames[i] );
		//UnitPanels[ i ].TheUnitsInfo.Name.SetString( "text", localizedNames[i] );
		
		UnitPanels[ i ].TheUnitsInfo.HP = GetVariableObject( "_root."$names[i]$".UnitHP.textField" );
		UnitPanels[ i ].TheUnitsInfo.Atk = GetVariableObject( "_root."$names[i]$".UnitAtk.textField" );
		UnitPanels[ i ].TheUnitsInfo.BackgroundMC = GetVariableObject( "_root."$names[i]$".UnitInfoBackground" );

		UnitPanels[ i ].SkillsInfo.ActionsInfoMC = GetVariableObject( "_root."$names[i]$"Skills" );
		UnitPanels[ i ].SkillsInfo.PlayerActionButtonsMC = GetVariableObject( "_root."$names[i]$"Skills.PlayerActionButtons" );
		UnitPanels[ i ].SkillsInfo.BackgroundMC = GetVariableObject( "_root."$names[i]$"Skills.ActionsInfoBackground" );
		UnitPanels[ i ].SkillsInfo.ActionsInfoTF = GetVariableObject( "_root."$names[i]$"Skills.ActionsInfoLabel.textField" );
		AutosizeTextFont( UnitPanels[ i ].SkillsInfo.ActionsInfoTF, TheLocalizer.GetLocalizedString( SkillsString ) );
		//UnitPanels[ i ].SkillsInfo.ActionsInfoTF.SetString( "text", TheLocalizer.GetLocalizedString( SkillsString ) );

		UnitPanels[ i ].SkillPoolPanel = GetVariableObject( "_root."$names[i]$"SkillPool" );
		UnitPanels[ i ].SkillPoolPanelLabel = GetVariableObject( "_root."$names[i]$"SkillPoolLabel" );
		UnitPanels[ i ].SkillPoolPanelLabel.GetObject( "UnitName" ).SetText( localizedNames[i] );

		for( a = 0; a < MAX_NUM_SKILLS; ++a )
		{
			UnitAbilityInfos[i].SelectionSlots[a] = UnitPanels[ i ].SkillsInfo.PlayerActionButtonsMC.GetObject( "Action"$(a+1) ).GetObject( "DragSlot" );
			UnitAbilityInfos[i].SelectionSlots[a].SetInt( "slotIdx", a );
		}

		tempWidget = GFxCLIKWidget( GetVariableObject( "_root."$names[i], class'GFxCLIKWidget' ) );
		tempWidget.AddEventListener( 'CLIK_buttonPress', ButtonClicked );
	}


	if( BSPC != none )
	{
		for( i = 0; i < BSPC.units.Length; ++i )
		{
			switch( BSPC.units[i].UnitName )
			{
			case 'Bjorn':
				HeroUnits[ BJORN - 1 ] = BSPC.units[i];
				FillUnitPanelInfo( BJORN - 1 );
				FillSkillPoolPanel( UnitPanels[ BJORN - 1 ].SkillPoolPanel, TheAbilityPool.BjornAbilityPool, BJORN - 1, HideBjornDagDescription, BjornSkillClicked ); 
				break;
			case 'Dag':
				HeroUnits[ DAG - 1 ] = BSPC.units[i];
				FillUnitPanelInfo( DAG - 1 );
				FillSkillPoolPanel( UnitPanels[ DAG - 1 ].SkillPoolPanel, TheAbilityPool.DagAbilityPool, DAG - 1, HideBjornDagDescription, DagSkillClicked );
				break;
			case 'Freya':
				HeroUnits[ FREYA - 1 ] = BSPC.units[i];
				FillSkillPoolPanel( UnitPanels[ FREYA - 1 ].SkillPoolPanel, TheAbilityPool.FreyaAbilityPool, FREYA - 1, HideFreyaIlianaDescription, FreyaSkillClicked );
				FillUnitPanelInfo( FREYA - 1 );
				break;
			case 'Iliana':
				HeroUnits[ ILIANA - 1 ] = BSPC.units[i];
				FillUnitPanelInfo( ILIANA - 1 );
				FillSkillPoolPanel( UnitPanels[ ILIANA - 1 ].SkillPoolPanel, TheAbilityPool.IlianaAbilityPool, ILIANA - 1, HideFreyaIlianaDescription, IlianaSkillClicked );
				break;
			}
		}
	}

	PresetsButtonBar = GFxCLIKWidget( GetVariableObject( "_root.LeftSideBarContainer.PresetButtonBar", class'GFxCLIKWidget' ) );
	if( PresetsButtonBar != none )
	{
		PresetsButtonBar.AddEventListener( 'CLIK_buttonPress', CurrentPresetChanged );
		PresetsButtonBar.AddEventListener( 'CLIK_buttonPress', MenuButtonClicked );
		//PresetsButtonBar.SetBool( "focusable", false );
	}


	LoadLastUsedSet();

	//Hover Over Info
	TheHoverOverInfo.HoverOverInfoTitleTF  = GetVariableObject( "_root.HoverOverInfo.HoverOverInfoTitle.textField" );
	TheHoverOverInfo.HoverOverInfoInfoTF   = GetVariableObject( "_root.HoverOverInfo.HoverOverInfoDescription.textField" );
	TheHoverOverInfo.HoverOverInfoCornerTF = GetVariableObject( "_root.HoverOverInfo.HoverOverInfoCornerText.textField" );
	TheHoverOverInfo.HoverOverInfoAPTextTF = GetVariableObject( "_root.HoverOverInfo.HoverOverInfoAPText.textField" );
	TheHoverOverInfo.HoverOverInfoIconMC   = GetVariableObject( "_root.HoverOverInfo.HoverOverInfoIcons" );
	TheHoverOverInfo.HoverOverInfoMC       = GetVariableObject( "_root.HoverOverInfo" );

	ConfirmationScreen = GetVariableObject( "_root.Confirmation" );

	

}

event bool WidgetInitialized( name WidgetName, name WidgetPath, GFxObject Widget )
{
	local GFxCLIKWidget tempCLIKWidget;
	local bool bWasHandled;
	//local string widgetPathStr;
	local BS_GameInfo BSGI;
	local BS_HeroAbilityPool BS_HAP;
	bWasHandled = false;
	//widgetPathStr = string( WidgetPath );

	if( TheAbilityPool == none )
	{
		BSGI = BS_GameInfo( class'WorldInfo'.static.GetWorldInfo().Game );

		foreach BSGI.AllActors( class'BS_HeroAbilityPool', BS_HAP )
		{
			TheAbilityPool = BS_HAP;
		}
	}

	switch( WidgetName )
	{
	case( 'UnitHPTF' ):
		Widget.SetText( TheLocalizer.GetLocalizedString( HPString ) );
		bWasHandled = true;
		break;
	case( 'UnitAtkTF' ):
		Widget.SetText( TheLocalizer.GetLocalizedString( AtkString ) );
		bWasHandled = true;
		break;
	case( 'StartGame' ):
		Widget.SetString( "label", TheLocalizer.GetLocalizedString( StartGame ) );
		AutosizeTextFont( Widget.GetObject( "textField" ), TheLocalizer.GetLocalizedString( StartGame ) );
		tempCLIKWidget = GFxCLIKWidget( Widget );
		if( tempCLIKWidget != none )
		{
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', SkillSelectionDone );
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', MenuButtonClicked );
		}
		bWasHandled = true;
		break;	
	case( 'SavePresets' ):
		Widget.SetString( "label", TheLocalizer.GetLocalizedString( SavePresetStr ) );
		AutosizeTextFont( Widget.GetObject( "textField" ), TheLocalizer.GetLocalizedString( SavePresetStr ) );
		tempCLIKWidget = GFxCLIKWidget( Widget );
		if( tempCLIKWidget != none )
		{
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', SaveCurrentPreset );
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', MenuButtonClicked );
		}
		bWasHandled = true;
		break;
	case( 'ShowHideSkillPool' ):
		Widget.SetString( "label", TheLocalizer.GetLocalizedString( HideSkillScreenStr ) );
		AutosizeTextFont( Widget.GetObject( "textField" ), TheLocalizer.GetLocalizedString( HideSkillScreenStr ) );
		tempCLIKWidget = GFxCLIKWidget( Widget );
		ShowHideScreenButton = tempCLIKWidget;
		if( tempCLIKWidget != none )
		{
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', ShowHideSkillScreen );
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', MenuButtonClicked );
		}
		bWasHandled = true;
		break;
	case( 'ConfirmationLabelMC' ):
		Widget.SetText( TheLocalizer.GetLocalizedString( ConfirmationStr ) );
		AutosizeTextFont( Widget.GetObject( "textField" ), TheLocalizer.GetLocalizedString( ConfirmationStr ) );
		bWasHandled = true;
		break;
	case( 'YesButton' ):
		Widget.SetString( "label", TheLocalizer.GetLocalizedString( YesStr ) );
		AutosizeTextFont( Widget.GetObject( "textField" ), TheLocalizer.GetLocalizedString( YesStr ) );
		tempCLIKWidget = GFxCLIKWidget( Widget );
		if( tempCLIKWidget != none )
		{
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', YesConfirmationClicked );
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', MenuButtonClicked );
		}
		bWasHandled = true;		
		break;
	case( 'NoButton' ):
		Widget.SetString( "label", TheLocalizer.GetLocalizedString( NoStr ) );
		AutosizeTextFont( Widget.GetObject( "textField" ), TheLocalizer.GetLocalizedString( NoStr ) );
		tempCLIKWidget = GFxCLIKWidget( Widget );
		if( tempCLIKWidget != none )
		{
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', NoConfirmationClicked );
			tempCLIKWidget.AddEventListener( 'CLIK_buttonPress', MenuButtonClicked );
		}
		bWasHandled = true;		
		break;
	}	
	
	if( !bWasHandled )
		bWasHandled = super.WidgetInitialized( WidgetName, WidgetPath, Widget );

	return bWasHandled;
}

function ButtonClicked( GFxCLIKWidget.EventData params )
{
	TheTurnManager.PlaySound( ButtonClickSound );
}

function MenuButtonClicked( GFxCLIKWidget.Eventdata params )
{
	TheTurnManager.PlaySound( MenuClickSound );
}


function SkillSelectionDone( GFxCLIKWidget.EventData params )
{	
	if( !bIsShowingConfirmationScreen )
	{
		bIsShowingConfirmationScreen = true;
		ConfirmationScreen.SetVisible( bIsShowingConfirmationScreen );
		ConfirmationScreen.GetObject( "YesButton" ).SetInt( "focused", 1 );
	}
	else
		YesConfirmationClicked( params );
}

function YesConfirmationClicked( GFxCLIKWidget.EventData params )
{	
	UpdateUnitStatsBasedOnAbilities();

	SaveCurrentSetToLastUsed();
	Close();
	TheTurnManager.bCanEndTurnEarly = true;
	TheTurnManager.TriggerGlobalEventClass( class'BS_SeqEvent_SkillSelectionDone',TheTurnmanager );
	MenuButtonClicked( params );
}

function NoConfirmationClicked( GFxCLIKWidget.EventData params )
{
	bIsShowingConfirmationScreen = false;
	ConfirmationScreen.SetVisible( bIsShowingConfirmationScreen );
}

function CurrentPresetChanged( GFxCLIKWidget.EventData params )
{
	timerBeforeUpdatingPreset = 10;
}

function SaveCurrentPreset( GFxCLIKWidget.EventData params )
{
	local int presetIdx;
	presetIdx = -1;
	if( PresetsButtonBar != none )
	{
		presetIdx = PresetsButtonBar.GetInt( "selectedIndex" );
		if( presetIdx > -1 )
		{
			++presetIdx;
			if( presetIdx == 10 )
				presetIdx = 0;
			SavePreset( presetIdx );
		}
	}
}

function ShowHideSkillScreen( GFxCLIKWidget.EventData params )
{
	if( bShowingScreen )
		HideSkillScreen();
	else
		ShowSkillScreen();
	bShowingScreen = !bShowingScreen;
}

function MouseOverSkillButton( GFxCLIKWidget.EventData params )
{
	params._this.SetBool( "isMouseOver", true );
}

function MouseOutSkillButton( GFxCLIKWidget.EventData params )
{
	params._this.SetBool( "isMouseOver", false );
}

function BjornSkillClicked( GFxCLIKWidget.EventData params )
{
	UnitPanels[ BJORN - 1 ].TheUnitsInfo.InfoMC.SetBool( "selected", true );
	HideAllAbilityPanels();
	UnitPanels[ BJORN - 1 ].SkillPoolPanel.SetVisible( true );
	UnitPanels[ BJORN - 1 ].SkillPoolPanelLabel.SetVisible( true );
	TheTurnManager.PlaySound( AbilitySelectionClickSound );
}

function DagSkillClicked( GFxCLIKWidget.EventData params )
{
	UnitPanels[ DAG - 1 ].TheUnitsInfo.InfoMC.SetBool( "selected", true );
	HideAllAbilityPanels();
	UnitPanels[ DAG - 1 ].SkillPoolPanel.SetVisible( true );
	UnitPanels[ DAG - 1 ].SkillPoolPanelLabel.SetVisible( true );
	TheTurnManager.PlaySound( AbilitySelectionClickSound );
}

function FreyaSkillClicked( GFxCLIKWidget.EventData params )
{
	UnitPanels[ FREYA - 1 ].TheUnitsInfo.InfoMC.SetBool( "selected", true );
	HideAllAbilityPanels();
	UnitPanels[ FREYA - 1 ].SkillPoolPanel.SetVisible( true );
	UnitPanels[ FREYA - 1 ].SkillPoolPanelLabel.SetVisible( true );
	TheTurnManager.PlaySound( AbilitySelectionClickSound );
}

function IlianaSkillClicked( GFxCLIKWidget.EventData params )
{
	UnitPanels[ ILIANA - 1 ].TheUnitsInfo.InfoMC.SetBool( "selected", true );
	HideAllAbilityPanels();
	UnitPanels[ ILIANA - 1 ].SkillPoolPanel.SetVisible( true );
	UnitPanels[ ILIANA - 1 ].SkillPoolPanelLabel.SetVisible( true );
	TheTurnManager.PlaySound( AbilitySelectionClickSound );
}

function HideAllAbilityPanels()
{	
	local int i;
	for( i = 0; i < 4; ++i )
	{
		UnitPanels[ i ].SkillPoolPanel.SetVisible( false );
		UnitPanels[ i ].SkillPoolPanelLabel.SetVisible( false );
	}
}

function ShowSkillScreen()
{
	local int i;
	ShowHideScreenButton.SetString( "label", TheLocalizer.GetLocalizedString( HideSkillScreenStr ) );
	AutosizeTextFont( ShowHideScreenButton.GetObject( "textField" ), TheLocalizer.GetLocalizedString( HideSkillScreenStr ) );
	for( i = 0; i < 4; ++i )
	{
		UnitPanels[i].TheUnitsInfo.InfoMC.SetVisible( true );
		UnitPanels[i].SkillsInfo.ActionsInfoMC.SetVisible( true );		
		if( UnitPanels[i].TheUnitsInfo.InfoMC.GetBool( "selected" ) )
		{
			UnitPanels[i].SkillPoolPanel.SetVisible( true );
			UnitPanels[ i ].SkillPoolPanelLabel.SetVisible( true );
		}
	}
}

function HideSkillScreen()
{
	local int i;
	ShowHideScreenButton.SetString( "label", TheLocalizer.GetLocalizedString( ShowSkillScreenStr ) );
	AutosizeTextFont( ShowHideScreenButton.GetObject( "textField" ), TheLocalizer.GetLocalizedString( ShowSkillScreenStr ) );
	for( i = 0; i < 4; ++i )
	{
		UnitPanels[i].TheUnitsInfo.InfoMC.SetVisible( false );
		UnitPanels[i].SkillsInfo.ActionsInfoMC.SetVisible( false );
		UnitPanels[i].SkillPoolPanel.SetVisible( false );
		UnitPanels[i].SkillPoolPanelLabel.SetVisible( false );
	}
}

function array<int> GetAbilityArrayIdx( int UnitIndex )
{
	local array<int> tempAbilityIdxArray;
	local GFxObject data;

	data = UnitAbilityInfos[UnitIndex].SelectionSlots[0].GetObject( "data" );
	if( data != none )
		tempAbilityIdxArray[0] =  data.GetInt( "abilityID" );// ActionScriptInt( "getSkillFromSlot1" );
	else
		tempAbilityIdxArray[0] = -1;
	
	data = UnitAbilityInfos[UnitIndex].SelectionSlots[1].GetObject( "data" );
	if( data != none )
		tempAbilityIdxArray[1] =  data.GetInt( "abilityID" );// ActionScriptInt( "getSkillFromSlot1" );
	else
		tempAbilityIdxArray[1] = -1;
	
	data = UnitAbilityInfos[UnitIndex].SelectionSlots[2].GetObject( "data" );
	if( data != none )
		tempAbilityIdxArray[2] =  data.GetInt( "abilityID" );// ActionScriptInt( "getSkillFromSlot1" );
	else
		tempAbilityIdxArray[2] = -1;
	
	data = UnitAbilityInfos[UnitIndex].SelectionSlots[3].GetObject( "data" );
	if( data != none )
		tempAbilityIdxArray[3] =  data.GetInt( "abilityID" );// ActionScriptInt( "getSkillFromSlot1" );
	else
		tempAbilityIdxArray[3] = -1;
	
	return tempAbilityIdxArray;
}

function InitAbilities( array<int> abilityIndexArray, array< BS_Ability > abilityPool, BS_Pawn thePawn )
{
	local int i;
	local int tempAbilityID;
	local array< BS_Ability > tempAbilitiesToAssign;
	
	for( i = 0; i < 4; ++i )
	{
		tempAbilityId = abilityIndexArray[i];
		if( tempAbilityId >= 0 )
		{
			tempAbilitiesToAssign[i] = abilityPool[ tempAbilityId ];
		}
	}	
	thePawn.AssignAbilityArchetypeArray( tempAbilitiesToAssign );
	thePawn.InitAbilities();
	
}

function FillUnitPanelInfo( int index )
{
	if( HeroUnits[ index ] != none )
	{
		UnitPanels[ index ].TheUnitsInfo.Atk.SetString( "text", ""$HeroUnits[ index ].GetAdjustedAttackPower() );
		UnitPanels[ index ].TheUnitsInfo.HP.SetString( "text", HeroUnits[ index ].GetAdjustedHealth()$"/"$HeroUnits[ index ].GetAdjustedMaxHealth() );
	}
}

function FillSkillPoolPanel( GFxObject panel, array< BS_Ability > abilities, int poolInfoIdx, delegate<GFxClikWidget.EventListener> outListener, delegate<GFxCLIKWidget.EventListener> panelListener )
{
	local GFxObject dragSlot;
	local GFxObject dragData;
	local GFxObject namesToRecieve;
	local GFxObject skillButton;
	local int i;
	local int top, bottom, left, right, posX, posY;
	local int itemHeight, itemWidth, hItemWidth, hItemHeight;
	local int contHeight, contWidth;
	local string panelName;
	local array<ASValue> dragDataConstructorParams;
	local string abilityName;
	local int abilityIcon;

	i = 0;
	panelName = panel.GetString( "name" );
	contHeight = panel.GetInt( "height" );
	contWidth = panel.GetInt( "width" );

	dragDataConstructorParams.Add(2);
	dragDataConstructorParams[0].s = panelName;
	dragDataConstructorParams[0].Type = AS_String;
	dragDataConstructorParams[1].i = 0;
	dragDataConstructorParams[1].Type = AS_Int;
	
	dragSlot = CreateObject( "BS_DragSlot" );
	itemHeight = DragSlot.GetInt( "height" );
	itemWidth = DragSlot.GetInt( "width" );
	hItemWidth = itemWidth / 2;
	hItemHeight = itemHeight / 2;
	top = ContainerPadding.Y + hItemHeight;
	bottom = contHeight - ContainerPadding.Y - hItemHeight;
	left = ContainerPadding.X + hItemWidth;
	right = contWidth - ContainerPadding.X - hItemWidth;
	posX = hItemWidth + left;
	posY = top;
		
	//namesToRecieve = CreateArray();
	//namesToRecieve.SetElementString( 0, panelName );
			
	while( posY < bottom && i < abilities.Length )
	{
		while( posX < right && i < abilities.Length )
		{
			//dragSlot = panel.AttachMovie("BS_DragSlot", panelName $ i );
			dragSlot = panel.AttachMovie("ActionSlotContainer", panelName $ i );
			dragSlot.SetInt( "x", posX );
			dragSlot.SetInt( "y", posY );			
			dragSlot = dragSlot.GetObject( "DragSlot" );
			UnitAbilityInfos[ poolInfoIdx ].PoolSlots.AddItem( dragSlot );
			dragSlot.SetInt( "slotIdx", i );
			dragSlot.SetBool( "isInPool", true );

			namesToRecieve = dragSlot.GetObject( "namesToRecieve" );
			namesToRecieve.SetElementString( 0, panelName );
			
			skillButton = CreateObject( "BS_SkillPoolItem", class'GFxCLIKWidget' );
			GFxCLIKWidget( skillButton ).AddEventListener( 'CLIK_buttonPress', panelListener );
			UnitAbilityInfos[ poolInfoIdx ].Buttons.AddItem( GFxCLIKWidget( skillButton ) );
			UnitAbilityInfos[ poolInfoIdx ].Buttons[i].SetInt( "slotIdx", i );
			UnitAbilityInfos[ poolInfoIdx ].Buttons[i].SetBool( "isInPool", true );
				
			UnitAbilityInfos[ poolInfoIdx ].Buttons[i].AddEventListener( 'CLIK_rollOut', outListener );
			UnitAbilityInfos[ poolInfoIdx ].Buttons[i].AddEventListener( 'CLIK_rollOver', MouseOverSkillButton );
			UnitAbilityInfos[ poolInfoIdx ].Buttons[i].AddEventListener( 'CLIK_rollOut', MouseOutSkillButton );
			
			abilityName = TheHUDMovie.GetLocalizedAbilityNameOrDescription( abilities[i].AbilityName );
			abilityIcon = int( TheHUDMovie.GetLocalizedAbilityNameOrDescription( abilities[i].AbilityName, 2 ) );
			
			skillButton.GetObject( "AbilityIcons" ).GotoAndStopI( abilityIcon + 1 );
			AutosizeTextFont( skillButton.GetObject( "textField" ), abilityName );
			//skillButton.GetObject( "textField" ).SetText( TheHUDMovie.GetLocalizedAbilityNameOrDescription( abilities[i].AbilityName ) );
			dragSlot.SetObject( "content", skillButton );
			dragDataConstructorParams[1].i = i;
			dragData = CreateObject( "BS_DragData",, dragDataConstructorParams );
			dragSlot.SetObject( "data", dragData );

			posX += itemWidth + ItemPadding.X;
			++i;
		}
		posY += itemHeight + ItemPadding.Y;
		posX = hItemWidth + left;
	}	
}

function SkillButtonPressed( GFxCLIKWidget.EventData params )
{
	TheTurnManager.PlaySound( AbilitySelectionClickSound );
}

function HideBjornDagDescription( GFxCLIKWidget.EventData params )
{
	//BjornDagAbilityDesc.Panel.SetVisible( false );
	HideHoverOverInfo();
}

function HideFreyaIlianaDescription( GFxCLIKWidget.EventData params )
{
	//FreyaIlianaAbilityDesc.Panel.SetVisible( false );
	HideHoverOverInfo();
}

function TickHUD()
{
	UpdateBjornHoverOverAbilityDesc();
	UpdateDagHoverOverAbilityDesc();
	UpdateFreyaHoverOverAbilityDesc();
	UpdateIlianaHoverOverAbilityDesc();
	
	//TODO If Right click is set then move the currently hovered over button to the opposite list
	UpdateRightClickMoveAbility();
	UpdateCurrentPreset();
	ResetAllButtonsToDownState();
}

function bool UpdateAbilityHoverOverInfo( GFxCLIKWidget button, BS_Ability ability )
{
	local string btnState;
	local string abilityName;
	local string abilityDesc;
	local int abilityIcon;
	local bool isOver;
	local BS_AttribBoostAbility BSABA;
	isOver = false;
	if( button != none )
	{
		btnState = button.GetString( "state" );
		if( btnState == "over" )
		{
			abilityName = TheHUDMovie.GetLocalizedAbilityNameOrDescription( ability.AbilityName);
			abilityDesc = TheHUDMovie.GetLocalizedAbilityNameOrDescription( ability.AbilityName, 1, ability );
			abilityIcon = int(TheHUDMovie.GetLocalizedAbilityNameOrDescription( ability.AbilityName, 2 ));
			TheHoverOverInfo.HoverOverInfoAPTextTF.SetString( "text", ""$ability.APCost$" AP" );

			BSABA = BS_AttribBoostAbility( ability );
			if( ability.AbilityType == ABILITY_TYPE_PASSIVE || ability.AbilityType == ABILITY_TYPE_ATTACKOVERRIDE || ( BSABA != none && BSABA.ApplyType == PASSIVE ) )
			{
				DisplayHoverOverInfo( abilityName, abilityDesc, TheLocalizer.GetLocalizedString( TheHUDMovie.PassiveAbilityName ), abilityIcon, false );
			}
			else
			{
					DisplayHoverOverInfo( abilityName, abilityDesc, TheLocalizer.GetLocalizedString( TheHUDMovie.ActiveAbilityName ), abilityIcon, true );
			}

			isOver = true;
		}
	}
	return isOver;
}

function bool UpdateBjornHoverOverAbilityDesc()
{
	local int i;
	local bool mouseOver;
	mouseOver = false;
	for( i = 0; i < UnitAbilityInfos[ BJORN - 1 ].Buttons.Length; ++i )
	{
		if( UpdateAbilityHoverOverInfo( UnitAbilityInfos[ BJORN - 1 ].Buttons[i], TheAbilityPool.BjornAbilityPool[i] ) )
		{
			mouseOver = true;
			break;
		}
	}
	return mouseOver;
}

function bool UpdateDagHoverOverAbilityDesc()
{
	local int i;
	local bool mouseOver;
	mouseOver = false;

	for( i = 0; i < UnitAbilityInfos[ DAG - 1 ].Buttons.Length; ++i )
	{
		if( UpdateAbilityHoverOverInfo( UnitAbilityInfos[ DAG - 1 ].Buttons[i], TheAbilityPool.DagAbilityPool[i] ) )
		{
			mouseOver = true;
			break;
		}
	}
	return mouseOver;
}

function bool UpdateFreyaHoverOverAbilityDesc()
{
	local int i;
	local bool mouseOver;
	mouseOver = false;
	for( i = 0; i < UnitAbilityInfos[ FREYA - 1 ].Buttons.Length; ++i )
	{
		if( UpdateAbilityHoverOverInfo( UnitAbilityInfos[ FREYA - 1 ].Buttons[i], TheAbilityPool.FreyaAbilityPool[i] ) )
		{
			mouseOver = true;
			break;
		}
	}
	return mouseOver;
}

function bool UpdateIlianaHoverOverAbilityDesc()
{
	local int i;
	local bool mouseOver;
	mouseOver = false;
	for( i = 0; i < UnitAbilityInfos[ ILIANA - 1 ].Buttons.Length; ++i )
	{
		if( UpdateAbilityHoverOverInfo( UnitAbilityInfos[ ILIANA - 1 ].Buttons[i], TheAbilityPool.IlianaAbilityPool[i] ) )
		{
			mouseOver = true;
			break;
		}
	}
	return mouseOver;
}


function bool GetSlotThatButtonIsIn( GFxCLIKWidget button, int abilityInfoIdx, optional out int poolIdx, optional out GFxObject slot )
{
	local int i;
	local bool isInPool;
	local string buttonText;
	local GFxObject content;
	local GFxObject textField;
	slot = none;
	textField = button.GetObject( "textField" );
	buttonText = textField.GetText();
	for( i = 0; i < UnitAbilityInfos[ abilityInfoIdx ].PoolSlots.Length; ++i )
	{
		content = UnitAbilityInfos[ abilityInfoIdx ].PoolSlots[i].GetObject( "content" );
		if( content != none && content.GetObject( "textField" ).GetText() == buttonText )
		{
			slot = UnitAbilityInfos[ abilityInfoIdx ].PoolSlots[i];
			poolIdx = i;
			isInPool = true;
		}
	}

	if( slot == none )
	{
		for( i = 0; i < MAX_NUM_SKILLS; ++i )
		{
			content = UnitAbilityInfos[ abilityInfoIdx ].PoolSlots[i].GetObject( "content" );
			if( content != none && content.GetString( "text" ) == button.GetString( "text" ) )
			{
				slot = UnitAbilityInfos[ abilityInfoIdx ].PoolSlots[i];
				poolIdx = i;
				isInPool = false;
			}
		}
	}

	return isInPool;
}

function MoveAbilityToNewContainer( GFxObject oldSlot, GFxObject newSlot, int pawnIdx )
{
	local array<BS_Ability> abilityPool;
	
	newSlot.SetObject( "content", oldSlot.GetObject( "content" ) );
	newSlot.SetObject( "data", oldSlot.GetObject( "data" ) );
	oldSlot.SetObject( "content", none );
	oldSlot.SetObject( "data", none );
	

	switch( pawnIdx )
	{
	case 0:
		abilityPool = TheAbilityPool.BjornAbilityPool;
		break;
	case 1:
		abilityPool = TheAbilityPool.DagAbilityPool;
		break;
	case 2:
		abilityPool = TheAbilityPool.FreyaAbilityPool;
		break;
	case 3:
		abilityPool = TheAbilityPool.IlianaAbilityPool;
		break;
	}

	InitAbilities( GetAbilityArrayIdx( pawnIdx ), abilityPool, HeroUnits[pawnIdx] );
	FillUnitPanelInfo( pawnIdx );
}

function UpdateRightClickMoveAbility()
{
	local int i;
	local int s;
	local string tempState;
	local int frameNum;
	local GFxObject slotToMoveTo;
	local int slotIdx;
	local bool itemMoved;
	local bool shouldCheckFrameNum;
	local bool shouldMove;
	itemMoved = false;
	shouldCheckFrameNum = true;
	shouldMove = false;

	if( bRightClickStarted )
	{
		bRightClickStarted = false;
		do
		{
			shouldCheckFrameNum = !shouldCheckFrameNum;
			for( i = 0; i < UnitAbilityInfos.Length && !itemMoved; ++i )
			{
				for( s = 0; s < UnitAbilityInfos[ i ].Buttons.Length && !itemMoved; ++s )
				{
					tempState = UnitAbilityInfos[ i ].Buttons[ s ].GetString( "state" );
					frameNum = UnitAbilityInfos[ i ].Buttons[ s ].GetInt( "currentFrame" );
					if( !shouldCheckFrameNum )
						shouldMove = tempState == "over";//UnitAbilityInfos[ i ].Buttons[ s ].GetBool( "isMouseOver" ) &&
					else
						shouldMove = frameNum == 20;
				
					if( shouldMove )
					{
						HideAllAbilityPanels();
						UnitPanels[ i ].SkillPoolPanel.SetVisible( true );
						UnitPanels[ i ].SkillPoolPanelLabel.SetVisible( true );
						UnitPanels[ i ].TheUnitsInfo.InfoMC.SetBool( "selected", true );

						if( UnitAbilityInfos[ i ].Buttons[ s ].GetBool( "isInPool" ) )
						{
							slotToMoveTo = GetEmptyAbilitySelectionSlot( i, slotIdx );
						}
						else
						{
							slotToMoveTo = GetEmptyPoolSlot( i, slotIdx );	
						}
						if( slotToMoveTo != none )
						{
							MoveAbilityToNewContainer( GetCurrentSlot( UnitAbilityInfos[ i ].Buttons[ s ], i ), slotToMoveTo, i );
						}
						itemMoved = true;
					}
				} 
			} 
		} until( shouldCheckFrameNum );

		TheTurnManager.PlaySound( AbilitySelectionClickSound );
	}
}

function ResetAllButtonsToDownState()
{
	local int i;
	local int s;
	for( i = 0; i < UnitAbilityInfos.Length; ++i )
	{
		for( s = 0; s < UnitAbilityInfos[ i ].Buttons.Length; ++s )
		{
			UnitAbilityInfos[ i ].Buttons[ s ].SetBool( "focused", false );
		}
	}
}

function GFxObject GetCurrentSlot( GFxObject button, int unitAbilityIdx )
{
	local int slotIdx;
	local bool isInPool;
	local GFxObject slot;
	slotIdx = button.GetInt( "slotIdx" );
	isInPool = button.GetBool( "isInPool" );
	if( isInPool )
	{
		slot = UnitAbilityInfos[ unitAbilityIdx ].PoolSlots[ slotIdx ];
	}
	else
	{
		slot = UnitAbilityInfos[ unitAbilityIdx ].SelectionSlots[ slotIdx ];
	}
	return slot;
}

function GFxObject GetEmptyAbilitySelectionSlot( int idx, optional out int slotIdx )
{
	local int i;
	local GFxObject emptySelectionSlot;
	for( i = 0; i < MAX_NUM_SKILLS && emptySelectionSlot == none; ++i )
	{
		if( UnitAbilityInfos[ idx ].SelectionSlots[ i ].GetObject( "content" ) == none )
		{
			emptySelectionSlot = UnitAbilityInfos[ idx ].SelectionSlots[ i ];
			slotIdx = i;
		}
	}
	return emptySelectionSlot;
}

function GFxObject GetEmptyPoolSlot( int idx, optional out int slotIdx )
{
	local int i;
	local GFxObject emptyPoolSlot;
	for( i = 0; i < UnitAbilityInfos[ idx ].PoolSlots.Length && emptyPoolSlot == none; ++i )
	{
		if( UnitAbilityInfos[ idx ].PoolSlots[ i ].GetObject( "content" ) == none )
		{
			emptyPoolSlot = UnitAbilityInfos[ idx ].PoolSlots[ i ];
			slotIdx = i;
		}
	}
	return emptyPoolSlot;
}


function StartRightClick()
{
	bRightClickStarted = true;
}

function DeselectAllUnitPanels()
{
	local int i;
	for( i = 0; i < 4; ++i )
	{
		UnitPanels[ i ].SkillPoolPanel.SetVisible( false );
		UnitPanels[ i ].SkillPoolPanelLabel.SetVisible( false );
		UnitPanels[ i ].TheUnitsInfo.InfoMC.GotoAndPlay( "up" );
	}
}

function AutosizeTextFont( GFxObject obj, string txt, int numOfLines = 1 )
{
	ActionScriptVoid("_root.Autosize");
}

function UpdateCurrentPreset()
{
	local int presetIdx;
	presetIdx = -1;
	if( timerBeforeUpdatingPreset != -1 )
	{
		if( timerBeforeUpdatingPreset > 0 )
		{
			--timerBeforeUpdatingPreset;
		}
		else if( PresetsButtonBar != none )
		{
			timerBeforeUpdatingPreset = -1;
			presetIdx = PresetsButtonBar.GetInt( "selectedIndex" );
			if( presetIdx > -1 )
			{
				++presetIdx;
				if( presetIdx == 9 )
					presetIdx = 0;
				LoadPreset( presetIdx );
			}
		}
	}
}

function SaveCurrentSetToLastUsed()
{
	local AbilityPreset presetToSave;
	local BS_GameInfo BSGI;
	BSGI = BS_GameInfo( class'WorldInfo'.static.GetWorldInfo().Game );

	presetToSave.BjornAbilityIndices = GetAbilityArrayIdx( BJORN - 1 );
	presetToSave.DagAbilityIndices = GetAbilityArrayIdx( DAG - 1 );
	presetToSave.FreyaAbilityIndices = GetAbilityArrayIdx( FREYA - 1 );
	presetToSave.IlianaAbilityIndices = GetAbilityArrayIdx( ILIANA - 1 );
	
	SaveSettings.SetLastAbilitySetUsed( presetToSave );
	BSGI.SaveGameUtils.SaveGameSettings( SaveSettings, BSGI.SaveGameUtils.SaveSettingsFileName );
}

function LoadLastUsedSet()
{
	local AbilityPreset presetToLoad;
	local int presetIdx;
	presetToLoad = SaveSettings.LastAbilitySetUsed;
	
	if( SaveSettings.IsLastAbilitySetUsedAPreset( presetIdx ) )
	{
		SetCurrentButtonBarIdx( presetIdx );
	}
	else
	{
		presetIdx = SaveSettings.GetFirstEmptyPresetSlot();
		if( presetIdx != -1 )
		{
			SetCurrentButtonBarIdx( presetIdx );
		}
	}

	LoadSelectionSlotsFromPreset( presetToLoad.BjornAbilityIndices, TheAbilityPool.BjornAbilityPool, BJORN );
	LoadSelectionSlotsFromPreset( presetToLoad.DagAbilityIndices, TheAbilityPool.DagAbilityPool, DAG );
	LoadSelectionSlotsFromPreset( presetToLoad.FreyaAbilityIndices, TheAbilityPool.FreyaAbilityPool, FREYA );
	LoadSelectionSlotsFromPreset( presetToLoad.IlianaAbilityIndices, TheAbilityPool.IlianaAbilityPool, ILIANA );
}

function SavePreset( int idx )
{
	local AbilityPreset presetToSave;
	local BS_GameInfo BSGI;
	
	if( idx >= 0 )
	{
		BSGI = BS_GameInfo( class'WorldInfo'.static.GetWorldInfo().Game );
		presetToSave.BjornAbilityIndices = GetAbilityArrayIdx( BJORN - 1 );
		presetToSave.DagAbilityIndices = GetAbilityArrayIdx( DAG - 1 );
		presetToSave.FreyaAbilityIndices = GetAbilityArrayIdx( FREYA - 1 );
		presetToSave.IlianaAbilityIndices = GetAbilityArrayIdx( ILIANA - 1 );
	
		SaveSettings.AddPreset( presetToSave, idx );
		BSGI.SaveGameUtils.SaveGameSettings( SaveSettings, BSGI.SaveGameUtils.SaveSettingsFileName );
	}
}


function LoadPreset( int idx )
{
	local AbilityPreset presetToLoad;
	if( idx >= 0 )
	{
		presetToLoad = SaveSettings.GetPreset( idx );
	
		LoadSelectionSlotsFromPreset( presetToLoad.BjornAbilityIndices, TheAbilityPool.BjornAbilityPool, BJORN );
		LoadSelectionSlotsFromPreset( presetToLoad.DagAbilityIndices, TheAbilityPool.DagAbilityPool, DAG );
		LoadSelectionSlotsFromPreset( presetToLoad.FreyaAbilityIndices, TheAbilityPool.FreyaAbilityPool, FREYA );
		LoadSelectionSlotsFromPreset( presetToLoad.IlianaAbilityIndices, TheAbilityPool.IlianaAbilityPool, ILIANA );
		SetCurrentButtonBarIdx( idx );		
	}
}

function SetCurrentButtonBarIdx( int idx )
{
	local int selectedIndex;
	selectedIndex = idx - 1;
	PresetsButtonBar.SetBool( "focused", true );
	if( selectedIndex == -1 )
	{
		selectedIndex = 9;
	}
	PresetsButtonBar.SetInt( "selectedIndex", selectedIndex  );
}


function LoadSelectionSlotsFromPreset( array< int > abilityIndexArray, array< BS_Ability > abilityPool, UnitID uID )
{
	local int i;
	local GFxObject currentButton;
	local GFxObject currentSlot;
	local GFxObject data;
	local int abilityID;
	local int emptyPoolSlotIdx;
	local int abilitySlotIdx;
	local int unitIdx;
	local int tempAbilityID;
	local array< BS_Ability > abilitiesToLoad;
	unitIdx =  uID;

	for( i = 0; i < 4 && i < abilityIndexArray.Length; ++i )
	{
		tempAbilityId = abilityIndexArray[i];
		if( tempAbilityId >= 0 )
		{
			abilitiesToLoad[i] = abilityPool[ tempAbilityId ];
		}
	}

	for( i = 0; i < 4; ++i )
	{
		if( i < abilitiesToLoad.Length && abilitiesToLoad[i] != none )
		{
			GetButtonAndSlotFromAbilityName( abilitiesToLoad[i].AbilityName, uID, currentButton, currentSlot, abilitySlotIdx );

			if( UnitAbilityInfos[ unitIdx ].SelectionSlots[ i ] != currentSlot )
			{
				if( UnitAbilityInfos[ unitIdx ].SelectionSlots[ i ].GetObject( "content" ) != none )
				{
					data = UnitAbilityInfos[ unitIdx ].SelectionSlots[i].GetObject( "data" ); 
					abilityID = data.GetInt( "abilityID" );
					MoveAbilityToNewContainer( UnitAbilityInfos[ uID ].SelectionSlots[i], GetEmptyPoolSlot( unitIdx, emptyPoolSlotIdx ), unitIdx );
					UnitAbilityInfos[ unitIdx ].Buttons[ abilityID ].SetInt( "slotIdx", emptyPoolSlotIdx );
					UnitAbilityInfos[ unitIdx ].Buttons[ abilityID ].SetBool( "isInPool", true );
				}
			
				MoveAbilityToNewContainer( currentSlot, UnitAbilityInfos[ unitIdx ].SelectionSlots[i], unitIdx );
				currentButton.SetInt( "slotIdx", i );
				currentButton.SetBool( "isInPool", false );
			}
		}
		else
		{
			if( UnitAbilityInfos[ unitIdx ].SelectionSlots[ i ].GetObject( "content" ) != none )
			{
				data = UnitAbilityInfos[ unitIdx ].SelectionSlots[i].GetObject( "data" ); 
				abilityID = data.GetInt( "abilityID" );
				MoveAbilityToNewContainer( UnitAbilityInfos[ uID ].SelectionSlots[i], GetEmptyPoolSlot( unitIdx, emptyPoolSlotIdx ), uID );
				UnitAbilityInfos[ unitIdx ].Buttons[ abilityID ].SetInt( "slotIdx", emptyPoolSlotIdx );
				UnitAbilityInfos[ unitIdx ].Buttons[ abilityID ].SetBool( "isInPool", true );
			}
		}
	}
}

function GetButtonAndSlotFromAbilityName( name abilityName, UnitID uID, out GFxObject button, out GFxObject slot, out int slotIdx )
{
	local bool found;
	local int i;
	local string localizedName;
	found = false;
	localizedName = TheHUDMovie.GetLocalizedAbilityNameOrDescription( abilityName );
	
	for( i = 0; i < UnitAbilityInfos[ uID ].Buttons.Length && !found; ++i )
	{
		if( UnitAbilityInfos[ uID ].Buttons[i].GetObject( "textField" ).GetText() == localizedName )
		{
			button = UnitAbilityInfos[ uID ].Buttons[i];
			slot = GetCurrentSlot( UnitAbilityInfos[ uID ].Buttons[i], uID );
			slotIdx = i;
			found = true;
		}
	}
}

function UpdateButtonSlotIdx( GFxObject button, GFxObject data, int newSlot, bool isInPool )
{
	local int prevSlotIdx;
	local bool prevIsInPool;
	local string unitName;
	local int unitIdx;
	unitIdx = -1;
	unitName = data.GetString( "unitName" );
	prevSlotIdx = button.GetInt( "slotIdx" );
	prevIsInPool = button.GetBool( "isInPool" );
	button.SetInt( "slotIdx", newSlot );
	button.SetBool( "isInPool", isInPool );

	switch( unitName )
	{
	case "BjornSkillPool":
		unitIdx = BJORN - 1;
		break;
	case "DagSkillPool":
		unitIdx = DAG - 1;
		break;
	case "FreyaSkillPool":
		unitIdx = FREYA - 1;
		break;
	case "IlianaSkillPool":
		unitIdx = ILIANA - 1;
		break;
	}
	
	if( unitIdx != -1 )
	{
		if( prevIsInPool )
		{
			UnitAbilityInfos[ unitIdx ].PoolSlots[ prevSlotIdx ].SetObject( "data", none );
			UnitAbilityInfos[ unitIdx ].PoolSlots[ prevSlotIdx ].SetObject( "content", none );
		}
		else
		{
			UnitAbilityInfos[ unitIdx ].SelectionSlots[ prevSlotIdx ].SetObject( "data", none );
			UnitAbilityInfos[ unitIdx ].SelectionSlots[ prevSlotIdx ].SetObject( "content", none );
		}
		UpdateUnitStatsBasedOnAbilities();
	}
}


function UpdateUnitStatsBasedOnAbilities()
{	
	InitAbilities( GetAbilityArrayIdx( BJORN - 1 ), TheAbilityPool.BjornAbilityPool, HeroUnits[ BJORN - 1 ] );
	FillUnitPanelInfo( BJORN - 1 );
	//
	InitAbilities( GetAbilityArrayIdx( DAG - 1 ), TheAbilityPool.DagAbilityPool, HeroUnits[ DAG - 1 ] );
	FillUnitPanelInfo( DAG - 1 );
	//
	InitAbilities( GetAbilityArrayIdx( FREYA - 1 ), TheAbilityPool.FreyaAbilityPool, HeroUnits[ FREYA - 1 ] );
	FillUnitPanelInfo( FREYA - 1 );
	//
	InitAbilities( GetAbilityArrayIdx( ILIANA - 1 ), TheAbilityPool.IlianaAbilityPool, HeroUnits[ ILIANA - 1 ] );
	FillUnitPanelInfo( ILIANA - 1 );
}

function MoveButtonToSlotThatOtherButtonWasIn( GFxCLIKWidget otherButton, GFxCLIKWidget buttonToMove )
{
	local int i;
	local GFxObject slot;
	local int slotIdx;
	slot = none;
	for( i = 0; i < UnitAbilityInfos.Length; ++i )
	{
		GetSlotThatButtonIsIn( otherButton, i, slotIdx, slot );
		if( slot != none )
			break;
	}
	if( slot != none )
	{
		MoveAbilityToNewContainer( GetCurrentSlot( buttonToMove, i ), slot, i );
		//buttonToMove.SetInt( "slotIdx", slotIdx );
		//buttonToMove.SetBool( "isInPool", isInPool );
	}
}

function DisplayHoverOverInfo( string title = "", string info = "", string corner = "", int icon = 0, bool showAPCost = false )
{
	TheHoverOverInfo.HoverOverInfoInfoTF.SetBool( "html", true );
	AutosizeTextFont( TheHoverOverInfo.HoverOverInfoTitleTF, title );
	AutosizeTextFont( TheHoverOverInfo.HoverOverInfoInfoTF, info, 5 );
	//TheHoverOverInfo.HoverOverInfoInfoTF.SetString( "htmlText", info );
	AutosizeTextFont( TheHoverOverInfo.HoverOverInfoCornerTF, corner );
	TheHoverOverInfo.HoverOverInfoIconMC.GotoAndStopI( (icon + 1) );
	
	if(!showAPCost)
		TheHoverOverInfo.HoverOverInfoAPTextTF.SetString( "text", "" );
	
		
	ActionScriptVoid("_root.DisplayHoverOverInfo");
}

function HideHoverOverInfo()
{
	TheHoverOverInfo.HoverOverInfoMC.SetVisible( false );
	ActionScriptVoid("_root.HideHoverOverInfo");
}

function CanReciveCalled( bool canRecieve )
{
	if( canRecieve )
		`log( "Can recieve succeeded" );
	else
		`log( "Can recieve failed" );
}

function DebugMessage( int msgID )
{
	switch( msgID )
	{
	case 0:
		`log( "Successfully recieved" );
		break;
	case 1:
		`log( "Attempting to set content" );		
		break;
	case 2:
		`log( "Content == this" );			
		break;
	}
}


DefaultProperties
{
	WidgetBindings.Add( ( WidgetName="StartGame", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="SavePresets", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="ShowHideSkillPool", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="YesButton", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="NoButton", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="Bjorn", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="Dag", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="Freya", WidgetClass=class'GFxCLIKWidget' ) )
	WidgetBindings.Add( ( WidgetName="Iliana", WidgetClass=class'GFxCLIKWidget' ) )
	
	
	MovieInfo=SwfMovie'BS_SkillSelectionScreen.BS_SkillSelectionScreen'
	MAX_NUM_SKILLS=4

	ItemPadding = ( X = 30, Y = 70 )
	ContainerPadding = ( X = 0, Y = 75 )
	bRightClickStarted = false

	//Sounds

	AbilitySelectionClickSound = Soundcue'bs_HudUi.SFX.BS_SFX_SkillSelectionClick_CUE'
	ButtonClickSound     = SoundCue'Sounds.UI.BS-sfx_UI_ButtonPress_Cue'
	MenuClickSound = SoundCue'bs_HudUi.SFX.BS_SFX_MenuClick_CUE'

	timerBeforeUpdatingPreset = -1;
	bShowingScreen = true
	bIsShowingConfirmationScreen = false;
}
