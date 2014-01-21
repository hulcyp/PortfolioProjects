//-----------------------------------------------------------------------------------------------
// Utilities.cpp
//
// Copyright 2009 Jazz Game Technologies.  See Utilities/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "Utilities.hpp"
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <io.h>


//-----------------------------------------------------------------------------------------------
void Sprintf( std::string& destination, const char* format, ... )
{
	const int MESSAGE_MAX_LENGTH = 2048;
	char textLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	destination = textLiteral;
}


//-----------------------------------------------------------------------------------------------
void Sprintf( const int maxLength, std::string& destination, const char* format, ... )
{
	char* textLiteral = new char[ maxLength ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	destination = textLiteral;
	delete[] textLiteral;
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const char* format, ... )
{
	const int MESSAGE_MAX_LENGTH = 2048;
	char textLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const int maxLength, const char* format, ... )
{
	char* textLiteral = new char[ maxLength ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	delete[] textLiteral;
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
const std::string GetTrimmedString( const std::string& originalString )
{
	const int stringLength = (int) originalString.length();
	if( stringLength < 1 )
		return "";

	int lastNonSpaceCharIndex = stringLength - 1;
	while( lastNonSpaceCharIndex >= 0 && isspace( originalString[ lastNonSpaceCharIndex ] ) )
		-- lastNonSpaceCharIndex;

	if( lastNonSpaceCharIndex < 0 )
		return "";

	int firstNonSpaceCharIndex = 0;
	while( firstNonSpaceCharIndex <= lastNonSpaceCharIndex && isspace( originalString[ firstNonSpaceCharIndex ] ) )
		++ firstNonSpaceCharIndex;

	const int trimmedStringLength = 1 + lastNonSpaceCharIndex - firstNonSpaceCharIndex;
	std::string trimmedString( originalString, firstNonSpaceCharIndex, trimmedStringLength );
	return trimmedString;
}


//-----------------------------------------------------------------------------------------------
void GetTrimmedSubstring( std::string& substring_out, const std::string& sourceString, int startPos, unsigned int size )
{
	const int sourceStringLen = (int) sourceString.length();
	if( sourceStringLen == 0 )
	{
		substring_out.clear();
		return;
	}

	int trueStartPos = startPos;
	while( trueStartPos < sourceStringLen && isspace( sourceString[ trueStartPos ] ) )
		++ trueStartPos;

	if( size == std::string::npos )
	{
		substring_out = sourceString.substr( trueStartPos, std::string::npos );
		return;
	}
	else if( size <= 0 )
	{
		substring_out.clear();
		return;
	}
	else
	{
		int trueEndPos = startPos + (((int)size)-1);
		if( trueEndPos >= sourceStringLen )
			trueEndPos = sourceStringLen-1;

		while( trueEndPos >= 0 && isspace( sourceString[ trueEndPos ] ) )
			-- trueEndPos;

		if( trueEndPos < trueStartPos )
		{
			substring_out.clear();
			return;
		}

		const int trueSize = 1 + trueEndPos - trueStartPos;
		substring_out = sourceString.substr( trueStartPos, trueSize );
		return;
	}
}


/*
//-----------------------------------------------------------------------------------------------
void DebuggerPrintf( const char* messageFormat, ... )
{
	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, messageFormat );
	vsnprintf_s( messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList );
	va_end( variableArgumentList );
	messageLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)
	OutputDebugStringA( messageLiteral );
}
*/


//-----------------------------------------------------------------------------------------------
void SysCreateFolder( const JazzPath& filePath )
{
	const std::string newFolderPath = filePath;
#if defined( JAZZ_PLATFORM_WIN32 )
	CreateDirectoryA( newFolderPath.c_str(), NULL );
#else
//	JAZZ_ERROR( "SysCreateFolder", Stringf( "This method is not yet implemented for the current platform (%s)", JAZZ_PLATFORM_NAME ) );
#endif
}

//-----------------------------------------------------------------------------------------------
void SysDeleteFile( const JazzPath& filePath )
{
#if defined( JAZZ_PLATFORM_WIN32 )
	const std::string folderPathToDelete = filePath;
	DeleteFileA( folderPathToDelete.c_str() );
#else
//	JAZZ_ERROR( "SysDeleteFile", Stringf( "This method is not yet implemented for the current platform (%s)", JAZZ_PLATFORM_NAME ) );
#endif
}


//-----------------------------------------------------------------------------------------------
int LoadBinaryFileToNewBuffer( const JazzPath& filePath, OUTPUT unsigned char*& buffer, bool isText )
{
	FILE* file = NULL;

	const std::string filePathString = filePath;
	const errno_t errorCode = fopen_s( &file, filePathString.c_str(), "rb" );
	if( errorCode )
	{
		buffer = NULL;
		return -1;
	}

	fseek( file, 0, SEEK_END );
	int fileSizeInBytes = (int)ftell( file );
	rewind( file );

	if ( isText )
		fileSizeInBytes += 4 ;

	buffer = new unsigned char[ fileSizeInBytes ];

	if ( isText )
		memset( buffer, 0, fileSizeInBytes );	

	const size_t bytesRead = fread( buffer, 1, fileSizeInBytes, file );
	fclose( file );

	return (int) bytesRead;
}


//-----------------------------------------------------------------------------------------------
int WriteBufferToBinaryFile( const JazzPath& filePath, const unsigned char* sourceBuffer, int bufferSize )
{
	const std::string filePathString = filePath;
	FILE* file = NULL;

	//	filePath.CreateFolder();

	const errno_t errorCode = fopen_s( &file, filePathString.c_str(), "wb" );
	if( errorCode )
	{
		return -1;
	}

	const size_t bytesWritten = fwrite( sourceBuffer, sizeof( unsigned char ), bufferSize, file );

	fclose( file );
	return (int) bytesWritten;
}


//-----------------------------------------------------------------------------------------------
std::vector< std::string > EnumerateFilesInFolder( const std::string& filePattern )
{
	std::vector< std::string > foundFiles;

	int error = 0;
	_finddata_t fileInfo;
	intptr_t searchHandle = _findfirst( filePattern.c_str(), &fileInfo );
	while( searchHandle != -1 && !error )
	{
		const char* fileName = fileInfo.name;
		std::string fileNameString( fileName );
		foundFiles.push_back( fileNameString );
		error = _findnext( searchHandle, &fileInfo );
	}

	return foundFiles;
}


//-----------------------------------------------------------------------------------------------
int RandomIntLessThan( int ceiling )
{
	if( ceiling <= 1000 )
	{
		return rand() % ceiling;
	}
	else if( ceiling <= 10000000 )
	{
		const unsigned int largeRandomNumber = (rand() << 15) & rand();
		return largeRandomNumber % ceiling;
	}
	else
	{
		const unsigned int largeRandomNumber = (rand() << 15) & rand();
		const unsigned int maxLargeRandom = 1 + ((unsigned int)RAND_MAX << 15) & (unsigned int)RAND_MAX; // the "1 +" is so preciseRandomFloat is always less than 1.0f
		const float preciseRandomFloat = (float)largeRandomNumber / (float)maxLargeRandom;
		return (int)( preciseRandomFloat * (float)ceiling );
	}
}


