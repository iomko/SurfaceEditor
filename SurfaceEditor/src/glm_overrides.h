#pragma once
#include <glm/glm.hpp>

namespace glm {
    bool operator<(const glm::vec3& lhs, const glm::vec3& rhs);
    bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs);

	// Custom comparison operator for glm::vec3
	inline bool operator<(const glm::vec3& lhs, const glm::vec3& rhs) {
		if (lhs.x < rhs.x) return true;
		if (lhs.x > rhs.x) return false;
		if (lhs.y < rhs.y) return true;
		if (lhs.y > rhs.y) return false;
		return lhs.z < rhs.z;
	}

	// Custom comparison operator for std::pair<glm::vec3, glm::vec3>
	inline bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs) {
		if (lhs.first < rhs.first) return true;
		if (rhs.first < lhs.first) return false;
		return lhs.second < rhs.second;
	}
}
