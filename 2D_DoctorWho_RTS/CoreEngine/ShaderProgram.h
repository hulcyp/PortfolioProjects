#pragma once
#include <memory>
#include "Shader.h"

namespace pdh
{
	class ShaderProgram
	{
	public:
		ShaderProgram( const std::string& shaderName, Shader* vertex, Shader* frag);
		~ShaderProgram();

		GLuint getProgramID() { return m_programID; }
		const std::string getShaderProgramName() { return m_shaderProgramName; } 

	protected:
		void handleProgramLinkError( GLuint program );

	private:
		GLuint m_programID;
		const std::string m_shaderProgramName;
		Shader* m_vertex;
		Shader* m_fragment;
	};
}