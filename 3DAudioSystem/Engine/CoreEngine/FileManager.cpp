#include "FileManager.h"
#include <zip.h>
#include <unzip.h>


namespace Monky
{
	FileManager g_fileManager;
	MONKY_CRITICAL_SECTION g_fileManagerCS;
	//--------------------------------------------------------------
	FileManager& getFileManager()
	{
		return g_fileManager;
	}
	//--------------------------------------------------------------
	FileManager::FileManager()
	{}
	//--------------------------------------------------------------
	FileManager::~FileManager()
	{}
	//--------------------------------------------------------------
	void FileManager::initialize( const std::string& rootDirectory, const std::string& zipArchiveFile, ReadMode readMode, const std::string& password )
	{
		if( !m_isInitialized )
		{
			m_rootDirectory = rootDirectory;
			m_zipArchiveFile = zipArchiveFile;
			m_readMode = readMode;
			m_password = password;
			Monky::initializeCriticalSection( g_fileManagerCS );
			m_isInitialized = true;
		}
	}
	//--------------------------------------------------------------
	char* FileManager::readDataFromFile( const std::string& filePathRelToRootDir, int& bufferSize ) const
	{
		char* buffer = nullptr;

		if( m_isInitialized )
		{
			switch( m_readMode )
			{
			case DATA_FOLDER_ONLY:
				buffer = readFileFromUnZipFile( filePathRelToRootDir, bufferSize );			
				break;
			case DATA_ARCHIVE_ONLY:
				buffer = readFileFromZipFile( filePathRelToRootDir, bufferSize );
				break;
			case DATA_FOLDER_THEN_ARCHIVE:
				buffer = readFileFromUnZipFile( filePathRelToRootDir, bufferSize );	
				if( buffer == nullptr )
					buffer = readFileFromZipFile( filePathRelToRootDir, bufferSize );
				break;
			case DATA_ARCHIVE_THEN_FOLDER:
				buffer = readFileFromZipFile( filePathRelToRootDir, bufferSize );	
				if( buffer == nullptr )
					buffer = readFileFromUnZipFile( filePathRelToRootDir, bufferSize );
				break;
			}
		}
		return buffer;
	}
	//--------------------------------------------------------------
	void FileManager::setReadMode( ReadMode readMode )
	{
		m_readMode = readMode;
	}
	//--------------------------------------------------------------
	void FileManager::writeDataToFile( const std::string& filePathRelToRootDir, char* data, int size ) const
	{
		if( size > 0 && m_isInitialized )
		{
			std::fstream file( m_rootDirectory + filePathRelToRootDir );
			if( !file.is_open() )
				file.open( m_rootDirectory + filePathRelToRootDir, std::fstream::in | std::fstream::out | std::fstream::trunc );
			file.write( data, size );
			file.close();
		}
	}
	//--------------------------------------------------------------
	const std::string& FileManager::getRootDir() const
	{
		return m_rootDirectory;
	}
	//--------------------------------------------------------------
	char* FileManager::readFileFromZipFile( const std::string& filePathRelToRootDir, int& bufferSize ) const
	{
		unzFile zFile = unzOpen( m_zipArchiveFile.c_str() );
		char* buffer = nullptr;
		int errorID = unzLocateFile( zFile, filePathRelToRootDir.c_str(), 0 );
		
		if( errorID == UNZ_OK )
		{
			unz_file_info file_infos;
			unzGetCurrentFileInfo( zFile, &file_infos, NULL, 0, NULL, 0, NULL, 0 );
			bufferSize = file_infos.uncompressed_size;
			if( m_password.empty() )
				errorID = unzOpenCurrentFile( zFile );
			else
				errorID = unzOpenCurrentFilePassword( zFile, m_password.c_str() );
			
			if( bufferSize > 0 && errorID == UNZ_OK )
			{
				buffer = new( FILE_IO ) char[ bufferSize ];
				unzReadCurrentFile( zFile, buffer, bufferSize );
			}
			else
				bufferSize = 0;
		}
		unzCloseCurrentFile(zFile);
		unzClose( zFile );

		return buffer;	
	}
	//--------------------------------------------------------------
	char* FileManager::readFileFromUnZipFile( const std::string& filePathRelToRootDir, int& bufferSize ) const
	{
		char* buffer = nullptr;
		std::string filePath = m_rootDirectory + filePathRelToRootDir;
		std::fstream file( filePath, std::ios::in | std::ios::binary | std::ios::ate );
		int size = (int)file.tellg();
		if( size != -1 )
		{
			buffer = new( FILE_IO ) char[ size ];
			
			file.seekg( 0, std::ios::beg );
			file.read( buffer, size );
			file.close();
		}
		bufferSize = size;
		return buffer;
	}
#pragma warning( default:4355 )
}