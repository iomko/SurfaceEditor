#pragma once
#include <vector>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include "PolygonTriangulator.h"

class TriangleTriangulator : public PolygonTriangulator {
public:
	virtual bool calculateTriangulation(const std::vector<int>& triangleIndices, const std::vector<glm::vec3>& triangleVertices, std::vector<glm::vec3>& triangulatedTriangleResult, glm::vec3& triangleNormalResult) override {
		glm::vec3 triangleNormal;
		if (calculateNormal(triangleIndices, triangleVertices, triangleNormal))
		{
			for (const auto& triangleIndex : triangleIndices)
			{
				triangulatedTriangleResult.push_back(triangleVertices.at(triangleIndex));
			}

			triangleNormalResult = triangleNormal;
			return true;
		}
		return false;
	}
private:
	bool calculateNormal(const std::vector<int>& triangleIndices, const std::vector<glm::vec3>& triangleVertices, glm::vec3& returnedNormal) {
		if (triangleIndices.size() != 3) {
			return false;
		}

		glm::vec3 edge1 = triangleVertices[triangleIndices.at(1)] - triangleVertices[triangleIndices.at(0)];
		glm::vec3 edge2 = triangleVertices[triangleIndices.at(2)] - triangleVertices[triangleIndices.at(0)];

		glm::vec3 triangleNormal = glm::normalize(glm::cross(edge1, edge2));

		if (glm::length(triangleNormal) < std::numeric_limits<float>::epsilon()) {
			return false;
		}

		returnedNormal = triangleNormal;
		return true;
	}
};