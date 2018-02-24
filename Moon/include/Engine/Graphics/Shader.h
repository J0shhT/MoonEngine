#pragma once

#include "include/Common.h"

#include <GL/glew.h>
#include <GL/glut.h>

namespace Moon::Graphics {

	GLuint LinkShaders(
		GLuint vertexShader = 0,
		GLuint fragmentShader = 0,
		GLuint controlShader = 0,
		GLuint evaluationShader = 0,
		GLuint geometryShader = 0
	);
	GLuint LoadVertexShader(std::string path);
	GLuint LoadFragmentShader(std::string path);
	GLuint LoadControlShader(std::string path);
	GLuint LoadEvaluationShader(std::string path);
	GLuint LoadGeometryShader(std::string path);

}