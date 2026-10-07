#pragma once

#include <glm/glm.hpp>

struct Segment2D {
    glm::vec2 p0;
    glm::vec2 p1;
};

struct Ray2D {
    bool intersectSegment(const Segment2D& seg, glm::vec2& intersectionPoint) {
        const glm::vec2 p = origin;
        const glm::vec2 r = direction;
        const glm::vec2 q = seg.p0;
        const glm::vec2 s = seg.p1 - seg.p0;

        float rxs = r.x * s.y - r.y * s.x;
        glm::vec2 q_p = q - p;

        if (fabs(rxs) < 1e-6f) {
            // Parallel or collinear
            float qpxr = q_p.x * r.y - q_p.y * r.x;
            if (fabs(qpxr) < 1e-6f) {
                float rDotR = glm::dot(r, r);
                if (rDotR < 1e-12f) return false;

                float t0 = glm::dot(q - p, r) / rDotR;
                float t1 = glm::dot(seg.p1 - p, r) / rDotR;
                if (t0 > t1) std::swap(t0, t1);

                if (t1 >= 0.0f) {
                    float t = std::max(t0, 0.0f);
                    intersectionPoint = p + r * t;
                    return true;
                }
            }
            return false;
        }

        float t = (q_p.x * s.y - q_p.y * s.x) / rxs;
        float u = (q_p.x * r.y - q_p.y * r.x) / rxs;

        if (t >= 0.0f && u >= 0.0f && u <= 1.0f) {
            intersectionPoint = p + r * t;
            return true;
        }

        return false;
    }

	glm::vec2 origin{};
	glm::vec2 direction{};
};
