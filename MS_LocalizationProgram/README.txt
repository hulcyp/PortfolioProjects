******************************************************
* Midgard Saga localization system
******************************************************

Description: The localization program uses two seperate programs to assist designers in the process of localizing the game to various languages. The first program LocalParser.bat converts the UDK generated localization file (Bjeardsoft.int) into a tab seperated file that can be opened in Excel. After adding a column for each language into the Excel file, the Excel file is saved as a tab seperated .txt file( BS_Localization.txt). Then the Localizer.bat is run to generate the final UDK localization file(Bjeardsoft.int) from the tab seperated file generated using Excel.

Usage:
	1. Copy the "Bjeardsoft.int" file into the same directory as the "LocalParser.bat" and "BS_LocalFileParser.exe"
	2. Run "LocalParser.bat"
	3. Open the generated file "BS_Localization.txt" using Excel
	4. Insert a new column to the right of the current set of columns. Naming the column based on the language that the column represents.
	5. Translate all strings in the English column to the given language.
	6. Save the Excel file as a tab seperate file (.txt) with the name BS_Localization.txt in the same directory as the "Localizer.bat" and "BS_Localizer.exe"
	7. Run "Localizer.bat"
	8. Copy the generated "Bjeardsoft.int" file back in to the directory where UDK stores it's localization files and you are done. 