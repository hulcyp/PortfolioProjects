function StartDialogue( LevelIndex lvlIndex, int dialogueIndex, bool deselectPawn = true, bool disableInputWhenInDialogue = true )
{
	switch( lvlIndex )
	{
	case LEVEL_1:
		StartSpecifiedDialogue( DialogueLevel1, dialogueIndex, deselectPawn, disableInputWhenInDialogue );
		break;
	case LEVEL_2:
		StartSpecifiedDialogue( DialogueLevel2, dialogueIndex, deselectPawn, disableInputWhenInDialogue );
		break;
	case LEVEL_3:
		StartSpecifiedDialogue( DialogueLevel3, dialogueIndex, deselectPawn, disableInputWhenInDialogue );
		break;
	case LEVEL_4:
		StartSpecifiedDialogue( DialogueLevel4, dialogueIndex, deselectPawn, disableInputWhenInDialogue );
		break;
	case LEVEL_5:
		StartSpecifiedDialogue( DialogueLevel5, dialogueIndex, deselectPawn, disableInputWhenInDialogue );
		break;
	case LEVEL_6:
		StartSpecifiedDialogue( DialogueLevel6, dialogueIndex, deselectPawn, disableInputWhenInDialogue );
		break;
	}
	CurrentDialogueLevelIndex = lvlIndex;
}

function StartSpecifiedDialogue( array<string> lvlDialogues, int dialogueIndex, bool deselectPawn = true, bool disableInputWhenInDialogue = true )
{
	if( dialogueIndex < lvlDialogues.Length )
	{
		ParseDialogue( lvlDialogues[ dialogueIndex ], FirstPersonCurrentDialogue, SecondPersonCurrentDialogue, CurrentDialogueOrder, CharWhoseFirstAndSecond );
		ShowDialoguePanel( SecondPersonCurrentDialogue.Length > 0, deselectPawn, disableInputWhenInDialogue );
		CurrentDialogueIndex = dialogueIndex;
	}
}

function ShowDialoguePanel( bool hasSecondSpeaker, bool deselectPawn = true, bool disableInputWhenInDialogue = true )
{
	bDisableInputWhenInDialogue = disableInputWhenInDialogue;
	bIsInDialogueMode = true;
	DialoguePanel.DialogueMC.SetVisible( true );
	DialoguePanel.LeftSpeakerMC.SetVisible( true );

	if( hasSecondSpeaker )
	{
		DialoguePanel.RightSpeakerMC.SetVisible( true );
		DialoguePanel.RightDialogueTextMC.SetVisible( false );
	}
	//DisablePlayerHUD( deselectPawn );

//	ShowLeftSpeaker( FirstPersonCurrentDialogue[CurrentDialogueOrder[0]] );
	if( CurrentDialogueOrder.Length > 0 )
	{
		if( CharWhoseFirstAndSecond[0] == CurrentDialogueOrder[0] )
		{
			ShowLeftSpeaker( FirstPersonCurrentDialogue[0] );
			FirstPersonCurrentDialogue.Remove( 0, 1 );
			HideRightSpeaker();
		}
		else if( CharWhoseFirstAndSecond[1] == CurrentDialogueOrder[0] )
		{
			ShowRightSpeaker( SecondPersonCurrentDialogue[0] );
			SecondPersonCurrentDialogue.Remove( 0, 1 );
			HideLeftSpeaker();
		}
	}
	else
	{
		DisableDialoguePanel();
	}
}

function DisableDialoguePanel( bool triggerEvent = true )
{
	if( bIsInDialogueMode )
	{
		DialoguePanel.DialogueMC.SetVisible( false );
		DialoguePanel.RightSpeakerMC.SetVisible( false );
		DialoguePanel.LeftSpeakerMC.SetVisible( false );

		bIsInDialogueMode = false;
		//bDisableInputWhenInDialogue = true;

		//EnablePlayerHUD();

		FirstPersonCurrentDialogue.Length = 0;
		SecondPersonCurrentDialogue.Length = 0;
		CurrentDialogueOrder.Length = 0;

		CharWhoseFirstAndSecond[0] = NO_UNIT;
		CharWhoseFirstAndSecond[1] = NO_UNIT;
		PrevDialogueLevelIndex = CurrentDialogueLevelIndex;
		PrevDialogueIndex = CurrentDialogueIndex;
		if( triggerEvent )
			TheTurnManager.TriggerGlobalEventClass( class'BS_SeqEvent_DialogueEnded', TheTurnManager, 0 );	
	}
}

function ShowLeftSpeaker( string DialogueStr )
{
	DialoguePanel.LeftDialogueTextMC.SetVisible( true );
	DialoguePanel.LeftPictureCoverMC.SetVisible( false );
	AutosizeTextFont( DialoguePanel.LeftDialogueTextTF, DialogueStr, 2 );
	bIsShowingLeftSpeaker = true;
}

function HideLeftSpeaker()
{
	DialoguePanel.LeftDialogueTextMC.SetVisible( false );
	DialoguePanel.LeftPictureCoverMC.SetVisible( true );
	bIsShowingLeftSpeaker = false;
}

function ShowRightSpeaker( string DialogueStr )
{
	DialoguePanel.RightDialogueTextMC.SetVisible( true );
	DialoguePanel.RightPictureCoverMC.SetVisible( false );
	AutosizeTextFont( DialoguePanel.RightDialogueTextTF, DialogueStr, 2 );
	bIsShowingRightSpeaker = true;
}

function HideRightSpeaker()
{
	DialoguePanel.RightDialogueTextMC.SetVisible( false );
	DialoguePanel.RightPictureCoverMC.SetVisible( true );
	bIsShowingRightSpeaker = false;
}

function AdvanceCurrentDialogue( GFxCLIKWidget.EventData params )
{
	TheTurnManager.PlaySound( MenuClickSound );

	DialoguePanel.DialogueButtonMC.SetBool( "focused", false );

	CurrentDialogueOrder.Remove( 0, 1 );
	if( CurrentDialogueOrder.Length > 0 )
	{
		if( CharWhoseFirstAndSecond[0] == CurrentDialogueOrder[0] )
		{
			ShowLeftSpeaker( FirstPersonCurrentDialogue[0] );
			FirstPersonCurrentDialogue.Remove( 0, 1 );
			HideRightSpeaker();
		}
		else if( CharWhoseFirstAndSecond[1] != NO_UNIT && CharWhoseFirstAndSecond[1] == CurrentDialogueOrder[0] )
		{
			ShowRightSpeaker( SecondPersonCurrentDialogue[0] );
			SecondPersonCurrentDialogue.Remove( 0, 1 );
			HideLeftSpeaker();
		}
	}
	else
	{
		DisableDialoguePanel();
	}
}

function EndDialogueEarly()
{
	CurrentDialogueOrder.Length = 0;
	DisableDialoguePanel( false );
}

function ParseDialogue( string DialogueStr, out array<string> FirstPersonDialogue, out array<string> SecondPersonDialogue, out array<UnitID> Order, out UnitID WhoseFirstAndSecond[2] )
{
	local string charInConvoStr;
	local array<string> charInConvo;
	local UnitID firstUnitID;
	local UnitID secondUnitID;
	
	local string modifiedDialogue;
	local string firstCharTagStr;
	local string secCharTagStr;

	local array<string> firstSplit;
	local array<string> secSplit;

	local string tempStrOne;
	local int i;

	DialogueStr = TheLocalizer.GetLocalizedString( DialogueStr );

	FirstPersonCurrentDialogue.Length = 0;
	SecondPersonCurrentDialogue.Length = 0;
	Order.Length = 0;

	firstUnitID = NO_UNIT;
	secondUnitID = NO_UNIT;

	charInConvoStr = Left( DialogueStr, InStr( DialogueStr, ">" ) );
	CharInConvo = SplitString( charInConvoStr, "," );
	firstUnitID = GetUnitIDFromString( charInConvo[0] );
	firstCharTagStr = "<" $ charInConvo[0] $">";
	if( CharInConvo.Length > 1 )
	{
		secondUnitID = GetUnitIDFromString( charInConvo[1] );
		secCharTagStr = "<" $ charInConvo[1] $">";
	}
	
	modifiedDialogue = Mid( DialogueStr, InStr( DialogueStr, ">" ) + 1 );
	
	firstSplit = SplitString( modifiedDialogue, firstCharTagStr, true );
	
	foreach firstSplit( tempStrOne )
	{
		if( secondUnitID != NO_UNIT && secCharTagStr != "" && left( tempStrOne, InStr( tempSTrOne, ">" ) ) == secCharTagStr )
		{
			SecondPersonDialogue.AddItem( right( tempStrOne, len( tempStrOne ) - len( secCharTagSTr ) ) );
			Order.AddItem( secondUnitID );
		}
		else
		{
			if( secCharTagStr != "" )
				secSplit = SplitString( tempStrOne, secCharTagStr, true );
			else
				secSplit.AddItem( tempStrOne );
			FirstPersonDialogue.AddItem( secSplit[0] );
			Order.AddItem( firstUnitID );
			if( secondUnitID != NO_UNIT )
			{
				for( i = 1; i < secSplit.Length; ++i )
				{
					SecondPersonDialogue.AddItem( secSplit[i] );
					Order.AddItem( secondUnitID );
				}
			}
		}
		while( secSplit.Length > 0 )
			secSplit.Remove( 0, 1 );
	}
	WhoseFirstAndSecond[0] = firstUnitID;
	WhoseFirstAndSecond[1] = secondUnitID;

	SetLeftSpeakerNameAndPicture( firstUnitID );
	SetRightSpeakerNameAndPicture( secondUnitID );
}