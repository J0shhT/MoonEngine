#include "include/Engine/Graphics/Shader.h"

#include "include/Engine/StandardOut.h"

using namespace Moon;

GLuint Graphics::LinkShaders(GLuint vertexShader, GLuint fragmentShader, GLuint controlShader, GLuint evaluationShader, GLuint geometryShader)
{
	GLuint program = glCreateProgram();

	if (vertexShader > 0)
		glAttachShader(program, vertexShader);
	if (controlShader > 0)
		glAttachShader(program, controlShader);
	if (evaluationShader > 0)
		glAttachShader(program, evaluationShader);
	if (geometryShader > 0)
		glAttachShader(program, geometryShader);
	if (fragmentShader > 0)
		glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	//Check errors
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Graphics::LinkShaders() - Linked shaders");
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
		throw std::runtime_error("Graphics::LinkShaders() - Failed to link shaders: " + errorLogStream.str());
	}

	if (vertexShader > 0)
		glDeleteShader(vertexShader);
	if (controlShader > 0)
		glDeleteShader(controlShader);
	if (evaluationShader > 0)
		glDeleteShader(evaluationShader);
	if (geometryShader > 0)
		glDeleteShader(geometryShader);
	if (fragmentShader > 0)
		glDeleteShader(fragmentShader);

	return program;
}

GLuint Graphics::LoadVertexShader(std::string path)
{
	#ifdef _DEBUG
		path = DEBUG_DIR + path;
	#endif
	//Read file
	std::ifstream shaderFile(path);
	if (!shaderFile.good()) {
		throw std::runtime_error("Graphics::LoadVertexShader() - Error reading file \"" + path + "\"");
	}
	std::stringstream shaderBuffer;
	shaderBuffer << shaderFile.rdbuf();
	std::string shaderSourceStr = shaderBuffer.str();
	const GLchar* shaderSource = (const GLchar*)shaderSourceStr.c_str();

	//Compile shader
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//Check errors
	GLint shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Graphics::LoadVertexShader() - Vertex shader compiled");
	}
	else
	{
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteShader(shader);
		throw std::runtime_error("Graphics::LoadVertexShader() - Failed to compile vertex shader: " + errorLogStream.str());
	}

	return shader;
}

GLuint Graphics::LoadFragmentShader(std::string path)
{
	#ifdef _DEBUG
		path = DEBUG_DIR + path;
	#endif
	//Read file
	std::ifstream shaderFile(path);
	if (!shaderFile.good()) {
		throw std::runtime_error("Graphics::LoadFragmentShader() - Error reading file \"" + path + "\"");
	}
	std::stringstream shaderBuffer;
	shaderBuffer << shaderFile.rdbuf();
	std::string shaderSourceStr = shaderBuffer.str();
	const GLchar* shaderSource = (const GLchar*)shaderSourceStr.c_str();

	//Compile shader
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//Check errors
	GLint shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Graphics::LoadFragmentShader() - Fragment shader compiled");
	}
	else
	{
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteShader(shader);
		throw std::runtime_error("Graphics::LoadFragmentShader() - Failed to compile fragment shader: " + errorLogStream.str());
	}

	return shader;
}

GLuint Graphics::LoadControlShader(std::string path)
{
	#ifdef _DEBUG
		path = DEBUG_DIR + path;
	#endif
	//Read file
	std::ifstream shaderFile(path);
	if (!shaderFile.good()) {
		throw std::runtime_error("Graphics::LoadControlShader() - Error reading file \"" + path + "\"");
	}
	std::stringstream shaderBuffer;
	shaderBuffer << shaderFile.rdbuf();
	std::string shaderSourceStr = shaderBuffer.str();
	const GLchar* shaderSource = (const GLchar*)shaderSourceStr.c_str();

	//Compile shader
	GLuint shader = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//Check errors
	GLint shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Graphics::LoadControlShader() - Tesselation control shader compiled");
	}
	else
	{
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteShader(shader);
		throw std::runtime_error("Graphics::LoadControlShader() - Failed to compile tesselation control shader: " + errorLogStream.str());
	}

	return shader;
}

GLuint Graphics::LoadEvaluationShader(std::string path)
{
	#ifdef _DEBUG
		path = DEBUG_DIR + path;
	#endif
	//Read file
	std::ifstream shaderFile(path);
	if (!shaderFile.good()) {
		throw std::runtime_error("Graphics::LoadEvaluationShader() - Error reading file \"" + path + "\"");
	}
	std::stringstream shaderBuffer;
	shaderBuffer << shaderFile.rdbuf();
	std::string shaderSourceStr = shaderBuffer.str();
	const GLchar* shaderSource = (const GLchar*)shaderSourceStr.c_str();

	//Compile shader
	GLuint shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//Check errors
	GLint shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Graphics::LoadEvaluationShader() - Tesselation evaluation shader compiled");
	}
	else
	{
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteShader(shader);
		throw std::runtime_error("Graphics::LoadEvaluationShader() - Failed to compile tesselation evaluation shader: " + errorLogStream.str());
	}

	return shader;
}

GLuint Graphics::LoadGeometryShader(std::string path)
{
	#ifdef _DEBUG
		path = DEBUG_DIR + path;
	#endif
	//Read file
	std::ifstream shaderFile(path);
	if (!shaderFile.good()) {
		throw std::runtime_error("Graphics::LoadGeometryShader() - Error reading file \"" + path + "\"");
	}
	std::stringstream shaderBuffer;
	shaderBuffer << shaderFile.rdbuf();
	std::string shaderSourceStr = shaderBuffer.str();
	const GLchar* shaderSource = (const GLchar*)shaderSourceStr.c_str();

	//Compile shader
	GLuint shader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//Check errors
	GLint shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_TRUE)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Graphics::LoadGeometryShader() - Geometry shader compiled");
	}
	else
	{
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		std::stringstream errorLogStream;
		for (auto iter = errorLog.begin(); iter != errorLog.end(); iter++)
		{
			errorLogStream << *iter;
		}
		glDeleteShader(shader);
		throw std::runtime_error("Graphics::LoadGeometryShader() - Failed to compile geometry shader: " + errorLogStream.str());
	}

	return shader;
}