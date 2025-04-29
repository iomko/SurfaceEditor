#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

enum ProjectionAxis
{
	ZY,
	ZX,
	XY
};

class ProjectionUtils
{
public:
	static std::vector<glm::vec2> projectVertices(const std::vector<glm::vec3>& vertices, const ProjectionAxis& projectionAxis)
	{
		std::vector<glm::vec2> projectedVertices;
		if(projectionAxis == ProjectionAxis::ZY)
		{
			//X needs to be zero
			for (const glm::vec3& vertex : vertices)
			{
				projectedVertices.emplace_back(glm::vec2(vertex.z, vertex.y));
			}
		} else if(projectionAxis == ProjectionAxis::ZX)
		{
			//Y needs to be zero
			for (const glm::vec3& vertex : vertices)
			{
				projectedVertices.emplace_back(glm::vec2(vertex.x, vertex.z));
			}
		} else if(projectionAxis == ProjectionAxis::XY)
		{
			//Z needs to be zero
			for (const glm::vec3& vertex : vertices)
			{
				projectedVertices.emplace_back(glm::vec2(vertex.x, vertex.y));
			}
		}

		return projectedVertices;
	}
};
