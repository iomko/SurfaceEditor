#ifndef TRIANGLEDATA_H
#define TRIANGLEDATA_H

#include <glm/vec3.hpp>
#include <array>

struct TriangleData {
public:
    std::array<glm::vec3, 3> positions;
    glm::vec3 color;

    TriangleData(const std::array<glm::vec3, 3>& pos, const glm::vec3& col)
        : positions(pos), color(col) {}
};

#endif