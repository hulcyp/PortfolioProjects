#pragma once
#include <iostream>
#include <Wincon.h>
#include "ConsoleWindowColors.h"

#define ConsolePrint( msg )	\
{	\
	std::cout << msg << std::endl; \
}

#define  ConsolePrintColor( msg, clr )								\
{																		\
	//HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );					\
	//SetConsoleTextAttribute( hstdout, color | C_BG_BLACK );				\	
	std::cout << msg << std::endl;										\
	//SetConsoleTextAttribute( hstdout, C_FG_WHITE | C_BG_BLACK );	
}