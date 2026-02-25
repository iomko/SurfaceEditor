#include "Material.h"
#include <string>
#include <glm/vec4.hpp>
#include "Shader.h"


class Material
{
public:
	Shader* m_shader = nullptr;
	std::string m_id;
    std::string m_name;
public:
	Material(const std::string& id)
	{
		m_id = id;
	}
	Material(const std::string name, Shader* shader)
	{
        m_name = name;
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

    std::string getName() {
        return m_name;
    }
};
