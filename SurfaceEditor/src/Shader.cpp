#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	unsigned int vertexShaderID = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	createAndlinkProgram(vertexShaderID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	bind();
}

void Shader::bind()
{
	glUseProgram(id);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::createAndlinkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID)
{
	id = glCreateProgram();

	glAttachShader(id, vertexShaderID);
	glAttachShader(id, fragmentShaderID);
	glLinkProgram(id);

	int success;
	char infoLog[512];

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
	}
}

std::string Shader::loadShaderSrc(const std::string& shaderPath)
{
	std::ifstream file;
	std::stringstream buffer;

	std::string ret = "";

	file.open(shaderPath);

	if(file.is_open())
	{
		buffer << file.rdbuf();
		ret = buffer.str();
	} else
	{
		std::cout << "Couldn't open " << shaderPath << std::endl;
	}
	file.close();
	return ret;
}

unsigned int Shader::compileShader(const std::string& shaderPath ,GLenum type)
{
	unsigned int shaderType;
	shaderType = glCreateShader(type);

	std::string shaderTypeSrc = loadShaderSrc(shaderPath);
	const char* shaderTypeSrcPtr = shaderTypeSrc.c_str();

	glShaderSource(shaderType, 1, &shaderTypeSrcPtr, nullptr);
	glCompileShader(shaderType);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderType, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderType, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
	}
	return shaderType;
}

void Shader::setMat4(const std::string& name, glm::mat4 val)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}