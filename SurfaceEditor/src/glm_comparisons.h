#pragma once
#include <glm/glm.hpp>

namespace glm {
    bool operator<(const glm::vec3& lhs, const glm::vec3& rhs);
    bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs);
}
