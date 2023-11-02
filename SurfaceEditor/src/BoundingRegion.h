#ifndef BOUNDINGREGION_H
#define BOUNDINGREGION_H

#include <iostream>
#include <array>
#include <glm/glm.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

// Function to create an AABB from a std::array of points
inline AABB createAABB(const std::vector<glm::vec3>& pointList) {
    AABB aabb{};
    aabb.min = aabb.max = pointList[0];

    for (const auto& point : pointList) {
        aabb.min = glm::min(aabb.min, point);
        aabb.max = glm::max(aabb.max, point);
    }

    return aabb;
}
#endif