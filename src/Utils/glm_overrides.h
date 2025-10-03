#pragma once
#include <glm/glm.hpp>

namespace glm {
    bool operator<(const glm::vec3& lhs, const glm::vec3& rhs);
    bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs);

	// Custom comparison operator for glm::vec3
    //
    inline bool operator<(const glm::vec3& lhs, const glm::vec3& rhs) {
        if (lhs.x != rhs.x) return lhs.x < rhs.x;
        if (lhs.y != rhs.y) return lhs.y < rhs.y;
        if (lhs.z != rhs.z) return lhs.z < rhs.z;
        return false;
    }

	// Custom comparison operator for std::pair<glm::vec3, glm::vec3>
	inline bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs) {
		if (lhs.first < rhs.first) return true;
		if (rhs.first < lhs.first) return false;
		return lhs.second < rhs.second;
	}

    struct Vec3ApproxComparator {
        static constexpr float eps = 1e-6f;

        bool operator()(const glm::vec3& a, const glm::vec3& b) const {
            if (std::abs(a.x - b.x) > eps) return a.x < b.x;
            if (std::abs(a.y - b.y) > eps) return a.y < b.y;
            if (std::abs(a.z - b.z) > eps) return a.z < b.z;
            return false;
        }
    };

    struct Vec3PairApproxComparator {
        bool operator()(const std::pair<glm::vec3, glm::vec3>& lhs,
                        const std::pair<glm::vec3, glm::vec3>& rhs) const {
            auto l = std::minmax(lhs.first, lhs.second, Vec3ApproxComparator{});
            auto r = std::minmax(rhs.first, rhs.second, Vec3ApproxComparator{});

            if (Vec3ApproxComparator{}(l.first, r.first)) return true;
            if (Vec3ApproxComparator{}(r.first, l.first)) return false;
            return Vec3ApproxComparator{}(l.second, r.second);
        }
    };

}
