#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

class Shader
{
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void bind();
	void unbind();
	void setMat4(const std::string& name, glm::mat4 val);
private:
	void createAndlinkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);
	std::string loadShaderSrc(const std::string& shaderPath);
	unsigned int compileShader(const std::string& shaderPath, GLenum type);
	unsigned int id;
};
#endif