module;
#include <vector>
#include <glm/vec3.hpp>
#include "../Geometry/Mesh2.h"
export module Geometry.Primitive.Cube;
import Geometry.Primitive;

export struct CubeVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

export class Cube : public Mesh2
{
public:
	glm::vec3 m_centerPos;
	glm::vec3 m_color;
	float m_size;
public:
	//miesto toho aby Mesh brala color, tak neskor bude brat Material ako parameter
	Cube(glm::vec3 pos, glm::vec3 color, float size)
		: Mesh2(color), m_centerPos(pos), m_color(color), m_size(size)
	{
	}

	void setPosition(glm::vec3 pos)
	{
		m_centerPos = pos;
	}

	void setColor(glm::vec3 color)
	{
		m_color = color;
	}

	void setSize(float size)
	{
		m_size = size;
	}

	void calculateData() override
	{
		clearTriangles();
		clearVertexBufferData();
		//left side
		glm::vec3 bottomUpLeft = glm::vec3(m_centerPos.x - (m_size / 2), m_centerPos.y - (m_size / 2), m_centerPos.z - (m_size / 2));
		glm::vec3 bottomDownLeft = glm::vec3(m_centerPos.x - (m_size / 2), m_centerPos.y - (m_size / 2), m_centerPos.z + (m_size / 2));
		glm::vec3 upperDownLeft = glm::vec3(m_centerPos.x - (m_size / 2), m_centerPos.y + (m_size / 2), m_centerPos.z + (m_size / 2));
		glm::vec3 upperUpLeft = glm::vec3(m_centerPos.x - (m_size / 2), m_centerPos.y + (m_size / 2), m_centerPos.z - (m_size / 2));
		//right side
		glm::vec3 bottomUpRight = glm::vec3(m_centerPos.x + (m_size / 2), m_centerPos.y - (m_size / 2), m_centerPos.z - (m_size / 2));
		glm::vec3 bottomDownRight = glm::vec3(m_centerPos.x + (m_size / 2), m_centerPos.y - (m_size / 2), m_centerPos.z + (m_size / 2));
		glm::vec3 upperDownRight = glm::vec3(m_centerPos.x + (m_size / 2), m_centerPos.y + (m_size / 2), m_centerPos.z + (m_size / 2));
		glm::vec3 upperUpRight = glm::vec3(m_centerPos.x + (m_size / 2), m_centerPos.y + (m_size / 2), m_centerPos.z - (m_size / 2));

		addTriangle({ bottomUpLeft, bottomDownLeft, upperDownLeft });
		addTriangle({ upperDownLeft, upperUpLeft, bottomUpLeft });

		addTriangle({ upperUpLeft, upperDownLeft, upperDownRight });
		addTriangle({ upperDownRight, upperUpRight, upperUpLeft });

		addTriangle({ upperUpRight, upperDownRight, bottomDownRight });
		addTriangle({ bottomDownRight, bottomUpRight, upperUpRight });

		addTriangle({ bottomUpRight, bottomDownRight, bottomDownLeft });
		addTriangle({ bottomDownLeft, bottomUpLeft, bottomUpRight });

		addTriangle({ upperUpLeft, upperUpRight, bottomUpRight });
		addTriangle({ bottomUpRight, bottomUpLeft, upperUpLeft });

		addTriangle({ upperDownLeft, upperDownRight, bottomDownRight });
		addTriangle({ bottomDownRight, bottomDownLeft, upperDownLeft });
	}
};