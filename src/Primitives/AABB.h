#ifndef AABBBOUNDINGREGION_H
#define AABBBOUNDINGREGION_H

#include "../Ray.h"
#include "Primitives/Plane.h"

#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "../Primitives/Sphere.h"

class AABBBoundingRegion
{
public:
	template <typename It, typename PointExtractor>
	AABBBoundingRegion(It begin, It end, PointExtractor extractor)
	{
		bounds[0] = glm::vec3(std::numeric_limits<float>::max());
		bounds[1] = glm::vec3(std::numeric_limits<float>::lowest());
		for (It it = begin; it != end; ++it) {
			glm::vec3 point = extractor(*it);
			
			bounds[0].x = glm::min(bounds[0].x, point.x);
			bounds[0].y = glm::min(bounds[0].y, point.y);
			bounds[0].z = glm::min(bounds[0].z, point.z);
			
			bounds[1].x = glm::max(bounds[1].x, point.x);
			bounds[1].y = glm::max(bounds[1].y, point.y);
			bounds[1].z = glm::max(bounds[1].z, point.z);
		}
	}

	AABBBoundingRegion(const glm::vec3& min, const glm::vec3& max)
	{
		this->bounds[0].x = glm::min(min.x, max.x);
		this->bounds[0].y = glm::min(min.y, max.y);
		this->bounds[0].z = glm::min(min.z, max.z);

		this->bounds[1].x = glm::max(min.x, max.x);
		this->bounds[1].y = glm::max(min.y, max.y);
		this->bounds[1].z = glm::max(min.z, max.z);
	}

    bool insersectsPlane(const Plane& plane) const
    {
        glm::vec3 vp, vn;
        for (int i = 0; i < 3; i++) {
            if (plane.normal[i] >= 0) {
                vp[i] = bounds[1][i];
                vn[i] = bounds[0][i];
            } else {
                vp[i] = bounds[0][i];
                vn[i] = bounds[1][i];
            }
        }
        float dp = dot(plane.normal, vp - plane.point);
        float dn = dot(plane.normal, vn - plane.point);

        return (dp >= 0 && dn <= 0);
    }

	bool intersectsRay(const Ray& ray) const
	{
		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (bounds[ray.sign[0]].x - ray.origin.x) * ray.inverseDirection.x;
		tmax = (bounds[1 - ray.sign[0]].x - ray.origin.x) * ray.inverseDirection.x;
		tymin = (bounds[ray.sign[1]].y - ray.origin.y) * ray.inverseDirection.y;
		tymax = (bounds[1 - ray.sign[1]].y - ray.origin.y) * ray.inverseDirection.y;

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (bounds[ray.sign[2]].z - ray.origin.z) * ray.inverseDirection.z;
		tzmax = (bounds[1 - ray.sign[2]].z - ray.origin.z) * ray.inverseDirection.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}

	bool intersectsSphere(const Sphere& sphere) const {
		glm::vec3 closestPoint;
		const glm::vec3& min = this->getMinBoundsPos();
		const glm::vec3& max = this->getMaxBoundsPos();

		// Clamp each coordinate to the AABB bounds
		closestPoint.x = glm::clamp(sphere.position.x, min.x, max.x);
		closestPoint.y = glm::clamp(sphere.position.y, min.y, max.y);
		closestPoint.z = glm::clamp(sphere.position.z, min.z, max.z);

		// Compute squared distance from sphere center to closest position
		float distanceSquared = glm::distance2(closestPoint, sphere.position);

		// Check if it's within the radius squared
		return distanceSquared <= (sphere.radius * sphere.radius);
	}

	bool intersectsAABB(const AABBBoundingRegion& aabb) const
	{
		return (this->bounds[1].x >= aabb.getMinBoundsPos().x &&
			this->bounds[0].x <= aabb.getMaxBoundsPos().x &&
			this->bounds[1].y >= aabb.getMinBoundsPos().y &&
			this->bounds[0].y <= aabb.getMaxBoundsPos().y &&
			this->bounds[1].z >= aabb.getMinBoundsPos().z &&
			this->bounds[0].z <= aabb.getMaxBoundsPos().z);
	}

	bool containsPoint(const glm::vec3& point) const {
		return point.x >= this->getMinBoundsPos().x &&
			point.x <= this->getMaxBoundsPos().x &&
			point.y >= this->getMinBoundsPos().y &&
			point.y <= this->getMaxBoundsPos().y &&
			point.z >= this->getMinBoundsPos().z &&
			point.z <= this->getMaxBoundsPos().z;
	}

	bool containsAABB(const AABBBoundingRegion& aabb) const {
		return containsPoint(aabb.getMinBoundsPos()) &&
			containsPoint(aabb.getMaxBoundsPos());
	}

	const glm::vec3& getMinBoundsPos() const
	{
		return this->bounds[0];
	}
	const glm::vec3& getMaxBoundsPos() const
	{
		return this->bounds[1];
	}

	AABBBoundingRegion(const AABBBoundingRegion& other)
	{
		this->bounds[0] = other.bounds[0];
		this->bounds[1] = other.bounds[1];
	}

	AABBBoundingRegion& operator=(const AABBBoundingRegion& other)
	{
		if (this != &other) {
			this->bounds[0] = other.bounds[0];
			this->bounds[1] = other.bounds[1];
		}
		return *this;
	}

	bool operator<(const AABBBoundingRegion& other) const {
		if (getMinBoundsPos().x != other.getMinBoundsPos().x) return getMinBoundsPos().x < other.getMinBoundsPos().x;
		if (getMinBoundsPos().y != other.getMinBoundsPos().y) return getMinBoundsPos().y < other.getMinBoundsPos().y;
		if (getMinBoundsPos().z != other.getMinBoundsPos().z) return getMinBoundsPos().z < other.getMinBoundsPos().z;
		if (getMaxBoundsPos().x != other.getMaxBoundsPos().x) return getMaxBoundsPos().x < other.getMaxBoundsPos().x;
		if (getMaxBoundsPos().y != other.getMaxBoundsPos().y) return getMaxBoundsPos().y < other.getMaxBoundsPos().y;
		return getMaxBoundsPos().z < other.getMaxBoundsPos().z;
	}

	bool operator==(const AABBBoundingRegion& other) const {
		return this->getMinBoundsPos() == other.getMinBoundsPos() && this->getMaxBoundsPos() == other.getMaxBoundsPos();
	}

	bool operator!=(const AABBBoundingRegion& other) const {
		return !(*this == other);
	}

private:

	glm::vec3 bounds[2];
};
#endif
