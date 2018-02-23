#pragma once

#include "include/Common.h"

#include <GL/glew.h>
#include <GL/glut.h>

namespace Moon::Graphics {

	GLuint LoadShaders(std::string vertexShader, std::string fragmentShader);

}