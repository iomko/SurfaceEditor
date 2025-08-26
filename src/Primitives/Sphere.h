#pragma once
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>

struct Sphere
{
	glm::vec3 position;
	float radius;

	bool containsPoint(const glm::vec3& point) const {
		float distanceSquared = glm::distance2(point, position);
		return distanceSquared <= (radius * radius);
	}
};
