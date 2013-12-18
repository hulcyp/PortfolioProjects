#include "glHelper.h"
#include <map>

#ifdef _WIN32
#include <glew.h>
#endif
#include <Gl/GL.h>

namespace Monky
{
	std::string& getGlErrorAsString()
	{
		static bool init = false;
		static std::map< int, std::string > glErrorStrings;
		if( !init )
		{
			glErrorStrings[ GL_NO_ERROR ] = "GL_NO_ERROR";
			glErrorStrings[ GL_INVALID_ENUM ] = "GL_INVALID_ENUM";
			glErrorStrings[ GL_INVALID_VALUE ] = "GL_INVALID_VALUE";
			glErrorStrings[ GL_INVALID_OPERATION ] = "GL_INVALID_OPERATION";
			glErrorStrings[ GL_INVALID_FRAMEBUFFER_OPERATION ]  = "GL_INVALID_FRAMEBUFFER_OPERATION";
			glErrorStrings[ GL_OUT_OF_MEMORY ] = "GL_OUT_OF_MEMORY";
			glErrorStrings[ GL_STACK_UNDERFLOW ] = "GL_STACK_UNDERFLOW";
			glErrorStrings[ GL_STACK_OVERFLOW ] = "GL_STACK_OVERFLOW";
			init = true;
		}

		return std::string( "Open GL Error: " ).append( glErrorStrings[ glGetError() ] );
	}
}