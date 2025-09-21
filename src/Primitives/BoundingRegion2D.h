#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include "Ray2D.h"

class BoundingRegion2D 
{
public:
	template <typename It, typename PointExtractor>
	BoundingRegion2D(It begin, It end, PointExtractor extractor)
	{
		bounds[0] = glm::vec2(std::numeric_limits<float>::max());
		bounds[1] = glm::vec2(std::numeric_limits<float>::lowest());
		for (It it = begin; it != end; ++it) {
			glm::vec2 point = extractor(*it);
			
			bounds[0].x = glm::min(bounds[0].x, point.x);
			bounds[0].y = glm::min(bounds[0].y, point.y);
			
			bounds[1].x = glm::max(bounds[1].x, point.x);
			bounds[1].y = glm::max(bounds[1].y, point.y);
		}
	}

	BoundingRegion2D(const glm::vec2& min, const glm::vec2& max)
	{
		this->bounds[0].x = glm::min(min.x, max.x);
		this->bounds[0].y = glm::min(min.y, max.y);

		this->bounds[1].x = glm::max(min.x, max.x);
		this->bounds[1].y = glm::max(min.y, max.y);
	}

	bool intersectsBoundingRegion2D(const BoundingRegion2D& boundingRegion2D) const
	{
		return (this->bounds[1].x >= boundingRegion2D.getMinBoundsPos().x &&
			this->bounds[0].x <= boundingRegion2D.getMaxBoundsPos().x &&
			this->bounds[1].y >= boundingRegion2D.getMinBoundsPos().y &&
			this->bounds[0].y <= boundingRegion2D.getMaxBoundsPos().y);
	}

    bool intersectsRay(const Ray2D& ray) const {
        float tmin = (bounds[0].x - ray.origin.x) / ray.direction.x;
        float tmax = (bounds[1].x - ray.origin.x) / ray.direction.x;

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (bounds[0].y - ray.origin.y) / ray.direction.y;
        float tymax = (bounds[1].y - ray.origin.y) / ray.direction.y;

        if (tymin > tymax) std::swap(tymin, tymax);

        // intervals must overlap
        if ((tmin > tymax) || (tymin > tmax))
            return false;

        // update tmin/tmax to the overlapping interval
        tmin = std::max(tmin, tymin);
        tmax = std::min(tmax, tymax);

        return tmax >= 0; // true if intersection is in ray direction
    }

	const glm::vec2& getMinBoundsPos() const
	{
		return this->bounds[0];
	}
	const glm::vec2& getMaxBoundsPos() const
	{
		return this->bounds[1];
	}

	BoundingRegion2D(const BoundingRegion2D& other)
	{
		this->bounds[0] = other.bounds[0];
		this->bounds[1] = other.bounds[1];
	}

	BoundingRegion2D& operator=(const BoundingRegion2D& other)
	{
		if (this != &other) {
			this->bounds[0] = other.bounds[0];
			this->bounds[1] = other.bounds[1];
		}
		return *this;
	}

	bool operator<(const BoundingRegion2D& other) const {
		if (getMinBoundsPos().x != other.getMinBoundsPos().x) return getMinBoundsPos().x < other.getMinBoundsPos().x;
		if (getMinBoundsPos().y != other.getMinBoundsPos().y) return getMinBoundsPos().y < other.getMinBoundsPos().y;
		if (getMaxBoundsPos().x != other.getMaxBoundsPos().x) return getMaxBoundsPos().x < other.getMaxBoundsPos().x;
        return getMaxBoundsPos().y < other.getMaxBoundsPos().y;
	}

	bool operator==(const BoundingRegion2D& other) const {
		return this->getMinBoundsPos() == other.getMinBoundsPos() && this->getMaxBoundsPos() == other.getMaxBoundsPos();
	}

	bool operator!=(const BoundingRegion2D& other) const {
		return !(*this == other);
	}

private:

	glm::vec2 bounds[2];
};
