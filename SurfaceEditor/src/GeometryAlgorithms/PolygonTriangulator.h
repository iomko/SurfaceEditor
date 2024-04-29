#pragma once
#include <vector>
#include <glm/vec3.hpp>


class PolygonTriangulator
{
public:
	virtual ~PolygonTriangulator() {}
	virtual bool calculateTriangulation(const std::vector<int>& polygonIndices, const std::vector<glm::vec3>& polygonVertices, std::vector<glm::vec3>& triangulatedPolygonResult, glm::vec3& polygonNormalResult) = 0;
};