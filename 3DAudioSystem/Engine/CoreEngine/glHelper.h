#pragma once
#include "StdLibraryIncludes.h"
#include "Assertion.h"
#ifdef _WIN32
#include <glew.h>
#endif
#include <Gl/GL.h>

namespace Monky
{
	std::string& getGlErrorAsString();

#define glAssertError	\
	assertion( glGetError() == GL_NO_ERROR, getGlErrorAsString().c_str() );
}