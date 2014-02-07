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
