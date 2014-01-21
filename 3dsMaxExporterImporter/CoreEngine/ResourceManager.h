#pragma once
#include <string>
#include <map>
namespace pdh
{
	class ResourceManager
	{
	public:
		static bool addResource( const std::string& resourceName, const std::string& filePath );
		static const std::string& getResource( const std::string& resourceName );
		static void loadResourcesFromFile( const char* file );
		

	private:
		static std::map< std::string, std::string > ms_ResourcePool;

		ResourceManager();
		~ResourceManager();

	};
}