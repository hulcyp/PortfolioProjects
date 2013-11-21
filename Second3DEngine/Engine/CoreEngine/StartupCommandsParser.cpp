#include "StartupCommandsParser.h"
#include "EventSystemInterface.h"

namespace Monky
{
	StartupCommandsParser::StartupCommandsParser( int argc, char** argv )
		:	m_shouldStartEngine( true )
	{
		std::string currentCommand;
		for( int i = 1; i < argc; ++i )
		{
			std::string currentArg = argv[i];
			if( currentArg[0] == '-' )
			{
				currentArg = currentArg.substr( 1, currentArg.npos );
				m_commandsAndArgs[ currentArg ];
				currentCommand = currentArg;
			}
			else
			{
				if( currentCommand.size() > 0 )
					m_commandsAndArgs[ currentCommand ].push_back( currentArg );
			}
		}
	}
	//-----------------------------------------------------------
	void StartupCommandsParser::issueStartupCommands()
	{
		NamedProperties params;
		
		for( auto iter = m_commandsAndArgs.begin(); iter != m_commandsAndArgs.end(); ++iter )
		{
			params.set( "cmdArgs", iter->second );
			fireEvent( iter->first, params );
			bool shouldStartEngine = true;
			if( params.get( "shouldStartEngine", shouldStartEngine ) == NamedProperties::SUCCESS )
			{
				if( !shouldStartEngine )
					m_shouldStartEngine = false;
			}	
		}
	}
	//-----------------------------------------------------------
	bool StartupCommandsParser::shouldStartEngine() const
	{
		return m_shouldStartEngine;
	}
}