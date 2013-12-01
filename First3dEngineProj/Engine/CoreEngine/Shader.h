#pragma once
#include <glew.h>
#include <glut.h>
#include <string>

namespace pdh
{
	class Shader
	{
	public:
		Shader( const std::string& fileName, GLenum shaderType );
		~Shader();

		GLuint getShaderID() { return m_shaderID; }
	protected:
		void loadShaderFromFile( const char* fileName , GLenum shaderType );
		void handleShaderCompilerError( GLuint shaderID, const char* fileName, const std::string& shaderBuffer );
		
	private:
		GLuint m_shaderID;
	};
}