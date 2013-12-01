#include "ShaderProgram.h"
#include <ErrorHandlingUtils.h>

namespace pdh
{
	ShaderProgram::ShaderProgram( const std::string& shaderName, Shader* vertex, Shader* frag )
		:	m_shaderProgramName( shaderName )
		,	m_vertex( vertex )
		,	m_fragment( frag )
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

	ShaderProgram::~ShaderProgram()
	{
		if( m_programID )
		{
			glDeleteShader( m_programID );
			m_programID = 0;
		}
	}

	void ShaderProgram::handleProgramLinkError( GLuint program )
	{
		GLint length = 0;
		GLchar* log;
		glGetProgramiv( m_programID, GL_INFO_LOG_LENGTH, &length );
		log = new char[ length + 1 ];
		glGetProgramInfoLog( m_programID, length, &length, log );
		fatalError( "Program Link Error", log );
	}
}