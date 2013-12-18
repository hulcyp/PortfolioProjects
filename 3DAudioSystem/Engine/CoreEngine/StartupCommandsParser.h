#pragma once
#include "CommonEngineIncludes.h"

namespace Monky
{	
	class StartupCommandsParser
	{
	public:
		StartupCommandsParser( int argc, char** argv );
		
		void issueStartupCommands();
		bool shouldStartEngine() const;

	private:
		std::map< std::string, std::vector< std::string > > m_commandsAndArgs;
		bool m_shouldStartEngine;
	};
}