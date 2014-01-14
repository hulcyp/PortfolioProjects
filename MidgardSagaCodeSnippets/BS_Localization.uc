class BS_Localization extends Object;

enum LanguageCode
{
	EN,
	PT,
	SPN
};

var array<string> Languages;

var LanguageCode CurrentLanguage;

function string GetLocalizedString( string listOfLocalizedStringsStr )
{
	local array<string> listOfLocalizedStringsArray;
	local string localizedString;

	listOfLocalizedStringsArray = SplitString( listOfLocalizedStringsStr, "^" );
	if( listOfLocalizedStringsArray.Length >= CurrentLanguage + 1 )
	{
		localizedString = listOfLocalizedStringsArray[ CurrentLanguage ];
	}
	else if( listOfLocalizedStringsArray.Length >= 1 )
	{
		localizedString = listOfLocalizedStringsArray[0];
	}
	else
	{
		localizedString = listOfLocalizedStringsStr;
	}

	return localizedString;
}

function SetCurrentLanguageFromString( string language )
{
	local int i;
	for( i = 0; i < Languages.Length; ++i )
	{
		if( Languages[i] == language )
			CurrentLanguage = LanguageCode( EN + i );
	}
}

function int GetSelectedIndx()
{
	return int( CurrentLanguage );
}

DefaultProperties
{
	CurrentLanguage = EN
	Languages(0)="English";
	Languages(1)="Português";
	Languages(2)="Español";
}
