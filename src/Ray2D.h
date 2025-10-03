#pragma once

#include <glm/glm.hpp>

struct Segment2D {
    glm::vec2 p0;
    glm::vec2 p1;
};

struct Ray2D {
    bool intersectSegment(const Segment2D& seg, glm::vec2& intersectionPoint) {
        glm::vec2 v1 = origin - seg.p0;
        glm::vec2 v2 = seg.p1 - seg.p0;
        glm::vec2 v3(-direction.y, direction.x);

        float dot = glm::dot(v2, v3);
        if (fabs(dot) < 1e-6f) // parallel
            return false;

        float t1 = (v2.x * v1.y - v2.y * v1.x) / dot;
        float t2 = glm::dot(v1, v3) / dot;

        if (t1 >= 0.0f && t2 >= 0.0f && t2 <= 1.0f) {
            intersectionPoint = origin + direction * t1;
            return true;
        }

        return false;
    }

	glm::vec2 origin{};
	glm::vec2 direction{};
};
