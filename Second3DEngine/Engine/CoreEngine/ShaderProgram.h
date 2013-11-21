#pragma once
#include "Shader.h"
#include "StdLibraryIncludes.h"

namespace Monky
{
	class Renderer;

	class ShaderProgram
	{
	public:
		friend Renderer;

		ShaderProgram( const std::string& shaderName, Shader* vertex, Shader* frag);
		~ShaderProgram();

		GLuint getProgramID() { return m_programID; }
		const std::string getShaderProgramName() { return m_shaderProgramName; } 

		static ShaderProgram* createOrGetShaderProgram( const std::string& shaderName, const std::string& vertexFile, const std::string& fragFile );
		static void createShaderProgram( const std::string& shaderName, const std::string& vertexFile, const std::string& fragFile );
		static ShaderProgram* getShaderProgram( const std::string& shaderName );

	protected:
		static void cleanupShaderPrograms();
		void handleProgramLinkError( GLuint program );

	private:
		GLuint m_programID;
		const std::string m_shaderProgramName;
		Shader* m_vertex;
		Shader* m_fragment;

		static std::map< std::string, ShaderProgram* > sm_programs;
	};
}