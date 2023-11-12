#ifndef RAY_H
#define RA_H

#include <glm/glm.hpp>

class Ray {
public:
	Ray(const glm::vec3& origin, const glm::vec3& direction) {
		this->origin = origin;
		this->direction = direction;
		this->inverseDirection = 1.0f / this->direction;
		sign[0] = (this->inverseDirection.x < 0);
		sign[1] = (this->inverseDirection.y < 0);
		sign[2] = (this->inverseDirection.z < 0);
	}

public:
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 inverseDirection;
	int sign[3];
};
#endif
