#pragma once
#include <string>
#include "Assertion.h"
#ifdef _WIN32
#include <glew.h>
#endif
#include <glut.h>

namespace pdh
{
	std::string& getGlErrorAsString();

#define glAssertError	\
	assertion( glGetError() == GL_NO_ERROR, getGlErrorAsString().c_str() );
}