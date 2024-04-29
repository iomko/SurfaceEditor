#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
class RayIntersectionAlg
{
public:
    static bool intersectPlane(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& l0, const glm::vec3& l, float& t)
    {
        float denom = glm::dot(n, l);
        if (abs(denom) > 0.0001f) // your favorite epsilon
        {
            t = glm::dot((p0 - l0), n) / denom;
            if (t >= 0) return true; // you might want to allow an epsilon here too
        }
        return false;
        
    }
};
