#include "include/Engine/Graphics/Shader.h"

#include "include/Engine/StandardOut.h"

using namespace Moon;

GLuint Graphics::LoadShaders(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	//Read files
	///Vertex Shader
	std::ifstream vertexShaderFile(vertexShaderPath);
	if (!vertexShaderFile.good()) {
		throw std::runtime_error("Graphics::LoadShaders() - Error reading file \"" + vertexShaderPath + "\"");
	}
	std::stringstream vertexShaderBuffer;
	vertexShaderBuffer << vertexShaderFile.rdbuf();
	std::string vertexShaderSourceStr = vertexShaderBuffer.str();
	const GLchar* vertexShaderSource = (const GLchar*)vertexShaderSourceStr.c_str();
	///Fragment Shader
	std::ifstream fragmentShaderFile(fragmentShaderPath);
	if (!fragmentShaderFile.good()) {
		throw std::runtime_error("Graphics::LoadShaders() - Error reading file \"" + fragmentShaderPath + "\"");
	}
	std::stringstream fragmentShaderBuffer;
	fragmentShaderBuffer << fragmentShaderFile.rdbuf();
	std::string fragmentShaderSourceStr = fragmentShaderBuffer.str();
	const GLchar* fragmentShaderSource = (const GLchar*)fragmentShaderSourceStr.c_str();

	//Compile shaders
	///Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint vertexShaderStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);
	if (vertexShaderStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "Graphics::LoadShaders() - Vertex shader compiled");
	}
	else
	{
		GLint maxLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteShader(vertexShader);
		throw std::runtime_error("Graphics::LoadShaders() - Failed to compile vertex shader: " + errorLogStream.str());
	}
	///Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	GLint fragmentShaderStatus;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderStatus);
	if (fragmentShaderStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "Graphics::LoadShaders() - Fragment shader compiled");
	}
	else
	{
		GLint maxLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteShader(fragmentShader);
		throw std::runtime_error("Graphics::LoadShaders() - Failed to compile fragment shader: " + errorLogStream.str());
	}

	//Link shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Debug, "Graphics::LoadShaders() - Shader program linked");
	}
	else
	{
		GLint maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteProgram(program);
		throw std::runtime_error("Graphics::LoadShaders() - Failed to link shader program: " + errorLogStream.str());
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
