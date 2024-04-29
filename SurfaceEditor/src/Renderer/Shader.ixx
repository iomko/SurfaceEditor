module;
#include <glm/glm.hpp>
#include "glad/glad.h"
#include <string>
export module Renderer.Shader;

export class Shader
{
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);
	void bind();
	void unbind();
	void setMat4(const std::string& name, glm::mat4 val);
	void setVec4(const std::string& name, const glm::vec4& val);
	void setVec3(const std::string& name, const glm::vec3& val);
	void setFloat(const std::string& name, float val);
	void setInt(const std::string& name, int val);
private:
	void createAndlinkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID);
	void createAndlinkProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID, unsigned int geometryShaderID);
	std::string loadShaderSrc(const std::string& shaderPath);
	unsigned int compileShader(const std::string& shaderPath, GLenum type);
	unsigned int id;
};
