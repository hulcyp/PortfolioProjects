#include "ResourceManager.h"
#include "CommonEngineIncludes.h"
#include "ErrorHandlingUtils.h"
#include "tinyxml2.h"

namespace pdh
{

	std::map< std::string, std::string > ResourceManager::ms_ResourcePool;

	ResourceManager::ResourceManager()
	{}
	//////////////////////////////////////////////////
	ResourceManager::~ResourceManager()
	{}
	//////////////////////////////////////////////////
	bool ResourceManager::addResource( const std::string& resourceName, const std::string& filePath )
	{
		auto iter = ms_ResourcePool.find( resourceName );
		bool found = false;
		if( iter != ms_ResourcePool.end() )
		{
			consolePrintColorf( "The resource: %s, already exists", color::RED, resourceName.c_str() );
		}
		else
		{
			ms_ResourcePool[ resourceName ]  = filePath;
			found = true;			
		}
		return found;
	}
	//////////////////////////////////////////////////
	const std::string& ResourceManager::getResource( const std::string& resourceName )
	{
		auto iter = ms_ResourcePool.find( resourceName );
		if( iter == ms_ResourcePool.end() )
		{
			fatalError( "Resource does not exist", "The resource: %s, does not exist", resourceName.c_str() ); 
		}
		return iter->second;
	}
	// Not implemented do not use
	void ResourceManager::loadResourcesFromFile( const char* file )
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile( file );

		if( !doc.Error() )
		{
			tinyxml2::XMLElement* resourceFolder;
			for( resourceFolder = doc.FirstChildElement( "ResourceFolder" ); resourceFolder != 0; resourceFolder->NextSiblingElement( "ResourceFolder" ) )
			{
				const char* folder = resourceFolder->GetText();
				
				tinyxml2::XMLElement* resourceFile;
				
			}
		}
		else
		{
			reportError( "File could not be opened", "Failed to load file: %s", file );
		}
	}
}