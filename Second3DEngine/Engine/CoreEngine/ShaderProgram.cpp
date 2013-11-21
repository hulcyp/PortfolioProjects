#include "ShaderProgram.h"
#include "CommonEngineIncludes.h"
#include "MonkyException.h"

namespace Monky
{
	std::map< std::string, ShaderProgram* > ShaderProgram::sm_programs;

	ShaderProgram::ShaderProgram( const std::string& shaderName, Shader* vertex, Shader* frag )
		:	m_shaderProgramName( shaderName )
		,	m_vertex( vertex )
		,	m_fragment( frag )
	{
		if( vertex != nullptr && frag != nullptr )
		{
			m_programID = glCreateProgram();

			glAttachShader( m_programID, vertex->getShaderID() );
			glAttachShader( m_programID, frag->getShaderID() );
		
			glBindAttribLocation( m_programID, 0, "aPosition" );
			glBindAttribLocation( m_programID, 1, "aNormal" );
			glBindAttribLocation( m_programID, 2, "aColor" );
			glBindAttribLocation( m_programID, 3, "aTexCoord0" );
			glBindAttribLocation( m_programID, 4, "aTangent" );
			glBindAttribLocation( m_programID, 5, "aBitangent" );

			glLinkProgram( m_programID );

			GLint linked = GL_FALSE;
			glGetProgramiv( m_programID, GL_LINK_STATUS, &linked );
			if( linked != GL_TRUE )
			{
				handleProgramLinkError( m_programID );
			}
		}

	}
	//--------------------------------------------------------------------------------
	ShaderProgram::~ShaderProgram()
	{
		if( m_programID )
		{
			glDeleteShader( m_programID );
			m_programID = 0;
		}
	}
	//--------------------------------------------------------------------------------
	ShaderProgram* ShaderProgram::createOrGetShaderProgram( const std::string& shaderName, const std::string& vertexFile, const std::string& fragFile )
	{
		Shader* vertex = Shader::createOrGetShader( vertexFile, GL_VERTEX_SHADER );
		Shader* frag = Shader::createOrGetShader( fragFile, GL_FRAGMENT_SHADER );

		auto iter = sm_programs.find( shaderName );
		//ShaderProgram*& program = sm_programs[ shaderName ];

		if( iter == sm_programs.end() )
		{
			iter = sm_programs.insert( std::pair< std::string, ShaderProgram* >( shaderName, new ShaderProgram( shaderName, vertex, frag ) ) ).first;
		}
		return nullptr;
	}
	//--------------------------------------------------------------------------------
	void ShaderProgram::createShaderProgram( const std::string& shaderName, const std::string& vertexFile, const std::string& fragFile )
	{
		createOrGetShaderProgram( shaderName, vertexFile, fragFile );
	}
	//--------------------------------------------------------------------------------
	ShaderProgram* ShaderProgram::getShaderProgram( const std::string& shaderName )
	{
		auto iter = sm_programs.find( shaderName );
		if( iter != sm_programs.end() )
			return iter->second;
		else
			return nullptr;
	}
	//--------------------------------------------------------------------------------
	void ShaderProgram::cleanupShaderPrograms()
	{
		MAP_CLEANUP( sm_programs );
	}
	//--------------------------------------------------------------------------------
	void ShaderProgram::handleProgramLinkError( GLuint program )
	{
		GLint length = 0;
		GLchar* log;
		glGetProgramiv( m_programID, GL_INFO_LOG_LENGTH, &length );
		log = new char[ length + 1 ];
		glGetProgramInfoLog( m_programID, length, &length, log );
		MonkyException::fatalErrorMessageBox( "Program Link Error", log );
	}
}