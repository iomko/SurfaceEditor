#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "TrianglePP/tpp_interface.hpp"

class ConversionUtils
{
public:
	static std::vector<tpp::Delaunay::Point> toDelaunayPoints(const std::vector<glm::vec2>& vertices) {
		std::vector<tpp::Delaunay::Point> result;
		result.reserve(vertices.size());
		for (const auto& vertex : vertices) {
			result.emplace_back(vertex.x, vertex.y);
		}
		return result;
	}
};
