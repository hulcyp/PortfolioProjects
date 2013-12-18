#include "MonkyException.h"
#include "GameApp.h"
#include <windows.h>
#include <stdio.h>

namespace Monky
{
	MonkyException::MonkyException( const char* message, const char* filePath, int lineNum )
		:	m_message( message )
		,	m_filePath( filePath )
		,	m_lineNum( lineNum )
	{}
	//------------------------------------------------------------
	const char* MonkyException::what() const
	{
		return m_message;
	}
	//------------------------------------------------------------
	const char* MonkyException::getFilePath() const
	{
		return m_filePath;
	}
	//------------------------------------------------------------
	int MonkyException::getLineNum() const
	{
		return m_lineNum;
	}
	//------------------------------------------------------------
	void MonkyException::printWhat() const
	{
		fatalErrorMessageBox( "Monky Exception", "%s \nException occured at: %s(%d)", m_message, m_filePath, m_lineNum );
	}
	
	//------------------------------------------------------------
	// Static member functions
	//------------------------------------------------------------
	void MonkyException::simpleErrorMessageBox( const char* title, const char* format, ... )
	{
		char msg[1024];
		va_list args;
		va_start( args, format );
		vsprintf_s( msg, format, args );
		va_end( args );
		MessageBoxA( NULL, msg, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
	}
	//------------------------------------------------------------
	void MonkyException::fatalErrorMessageBox( const char* title, const char* format, ... )
	{
		char msg[1024];
		va_list args;
		va_start( args, format );
		vsprintf_s( msg, format, args );
		va_end( args );
		MessageBoxA( NULL, msg, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
		GameApp::getInstance()->exitProgram( -1 );
	}
	//------------------------------------------------------------
	void MonkyException::printToCompilerOutputConsole( const char* error )
	{
		OutputDebugStringA( error );		
	}
	//------------------------------------------------------------
	bool MonkyException::yesNoMessageBox( const char* title, const char* msg )
	{
		bool yesSelected = false;
		int selected = MessageBoxA( NULL, msg, title, MB_YESNO | MB_ICONWARNING | MB_SETFOREGROUND );
		switch( selected )
		{
		case IDYES:
			yesSelected = true;
			break;
		case IDNO:
			yesSelected = false;
			break;
		}

		return yesSelected;
	}
	void MonkyException::debuggerBreakPoint()
	{
#ifdef MONKY_DEBUG
		__debugbreak();	
#endif
	}
}