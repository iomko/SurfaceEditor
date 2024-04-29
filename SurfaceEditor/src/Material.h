#pragma once
#include <string>
#include <glm/vec4.hpp>
import Renderer.Shader;
class Material
{
public:
	Shader* m_shader = nullptr;
	std::string m_id;
public:
	Material(const std::string& id)
	{
		m_id = id;
	}
	Material(Shader* shader)
	{
		m_shader = shader;
	}
	void setColorValue(const std::string& name, const glm::vec4& color)
	{
		m_shader->setVec4(name, color);
	}
	Shader* getShader()
	{
		return m_shader;
	}
};
