#pragma once
#include "StdLibraryIncludes.h"
#include "Assertion.h"

namespace Monky
{
	class MonkyException : public std::exception
	{
	public:
		MonkyException( const char* message, const char* filePath, int lineNum );

		const char* what() const;
		const char* getFilePath() const;
		int getLineNum() const;

		void printWhat() const;

		static void simpleErrorMessageBox( const char* title, const char* format, ... );
		static void fatalErrorMessageBox( const char* title, const char* format, ... );
		static void printToCompilerOutputConsole( const char* error );
		static bool yesNoMessageBox( const char* title, const char* msg );
		static void debuggerBreakPoint();

	private:
		const char* m_message;
		const char* m_filePath;
		int m_lineNum;
	};
}

#ifdef MONKY_DEBUG
#define MONKY_EXCEPTION( msg ) {		\
	MonkyException::printToCompilerOutputConsole( msg );	\
	MonkyException::debuggerBreakPoint();					\
	throw MonkyException( msg, __FILE__, __LINE__ );	}
#else
#define MONKY_EXCEPTION( msg )		\
	throw MonkyException( msg, __FILE__, __LINE__ );
#endif