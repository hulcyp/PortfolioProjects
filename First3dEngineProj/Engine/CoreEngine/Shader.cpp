#include "Shader.h"
#include <fstream>
#include <ErrorHandlingUtils.h>
#include "StringUtils.h"
#include <vector>
#include <sstream>

namespace pdh
{
	Shader::Shader( const std::string& fileName, GLenum shaderType )
	{
		loadShaderFromFile( fileName.c_str(), shaderType );
	}

	Shader::~Shader()
	{
		if( m_shaderID )
		{
			glDeleteShader( m_shaderID );
			m_shaderID = 0;
		}
	}

	void Shader::loadShaderFromFile( const char* fileName, GLenum shaderType )
	{
		GLuint shaderID = 0;
		std::string shaderString;
		std::ifstream shaderFile( fileName );
		
		if( shaderFile.is_open() )
		{
			shaderString.assign( std::istreambuf_iterator< char >( shaderFile ), std::istreambuf_iterator< char >() );
			shaderID = glCreateShader( shaderType );

			const GLchar* shaderSource = shaderString.c_str();
			glShaderSource( shaderID, 1, reinterpret_cast< const GLchar** >( &shaderSource ), nullptr );

			glCompileShader( shaderID );

			GLint compiled = GL_FALSE;
			glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compiled );

			if( compiled != GL_TRUE )
			{
				handleShaderCompilerError( shaderID, fileName, shaderString );
			}

			m_shaderID = shaderID;
		}
		else
		{
			std::stringstream ss;
			ss << "Coult not open shader file: " << fileName;
			std::string errorMessage;
			ss >> errorMessage;
			fatalError( "Shader file open error", errorMessage.c_str() );
		}
	}

	void Shader::handleShaderCompilerError( GLuint shaderID, const char* fileName, const std::string& shaderBuffer )
	{
		GLint length = 0;
		GLchar* log;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &length );
		log = new char[ length + 1 ];
		glGetShaderInfoLog( shaderID, length, &length, log );
		
		std::string sLog( log );
		std::vector< std::string > errorTokens;
		stringTokenizer( sLog, errorTokens, "\n" );

		std::string popupOutputMessage;
		for( size_t i = 0; i < errorTokens.size() && errorTokens[i].size() > 0; ++i )
		{

			size_t lineIndexStart = errorTokens[i].find( "(" );
			size_t lineIndexEnd = errorTokens[i].find( ")" );

			std::string lineNumber = 
				errorTokens[i].substr( lineIndexStart + 1, lineIndexEnd - lineIndexStart - 1 );

			std::string errorMessage( "Error on line: " + lineNumber );
		
			std::vector< std::string > lineTokens;
			stringTokenizer( shaderBuffer, lineTokens, "\n" );

			int lineNum = atoi( lineNumber.c_str() );
			errorMessage += "\n>";
			if( lineNum != 0 )
			{
				errorMessage += lineTokens[ lineNum - 1 ] + "\n";
			}
			else
			{
				errorMessage += lineTokens[ 0 ] + "\n";
			}

			errorMessage += errorTokens[i];
		
			std::string consoleOutput( "./" );
			consoleOutput += fileName;
			consoleOutput += "(" + lineNumber + "): " + errorTokens[i] + "\n";

			printErrorToOutputConsole( consoleOutput.c_str() );
			popupOutputMessage += errorMessage + "\n\n";
		}
		fatalError( "Shader Error", popupOutputMessage.c_str() );
	}
}