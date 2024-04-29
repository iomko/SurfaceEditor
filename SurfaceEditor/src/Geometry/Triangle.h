#pragma once
#include <functional>
#include <glm/vec3.hpp>

class Triangle
{
public:
	glm::vec3 m_positions[3]{};
	Triangle()
	{}

	Triangle(const glm::vec3* positionsArray)
	{
		for (int i = 0; i < 3; ++i) {
			m_positions[i] = positionsArray[i];
		}
	}

	Triangle(const glm::vec3 vert1, const glm::vec3 vert2, const glm::vec3 vert3)
	{
		m_positions[0] = vert1;
		m_positions[1] = vert2;
		m_positions[2] = vert3;
	}

	Triangle(std::initializer_list<const glm::vec3*> positionsArray)
	{
		int i = 0;
		for (const auto& pos : positionsArray) {
			if (i < 3) {
				m_positions[i++] = *pos;
			}
		}
	}

	bool operator<(const Triangle& other) const {
		for (int i = 0; i < 3; ++i) {
			if (m_positions[i].x < other.m_positions[i].x && m_positions[i].y < other.m_positions[i].y && m_positions[i].z < other.m_positions[i].z) return true;
			if (other.m_positions[i].x < m_positions[i].x && other.m_positions[i].y < m_positions[i].y && other.m_positions[i].z < m_positions[i].z) return false;
		}
		return false;
	}
};