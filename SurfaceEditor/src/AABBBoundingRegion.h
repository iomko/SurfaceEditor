#ifndef AABBBOUNDINGREGION_H
#define AABBBOUNDINGREGION_H

#include "Ray.h"

#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AABBBoundingRegion
{
public:
	template <typename IteratorBegin, typename IteratorEnd, typename GetX, typename GetY, typename GetZ>
	AABBBoundingRegion(IteratorBegin begin, IteratorEnd end, GetX xPosition, GetY yPosition, GetZ zPosition)
	{
		initializeBounds();
		for (IteratorBegin it = begin; it != end; ++it) {
			updateBounds(glm::vec3(xPosition(*it), yPosition(*it), zPosition(*it)));
		}
	}

	AABBBoundingRegion(const glm::vec3* positionsArray, size_t arraySize)
	{
		initializeBounds();
		for (size_t i = 0; i < arraySize; ++i) {
			updateBounds(positionsArray[i]);
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

	bool intersectsAABB(const AABBBoundingRegion& aabb) const
	{
		return (this->bounds[1].x >= aabb.getMin().x &&
			this->bounds[0].x <= aabb.getMax().x &&
			this->bounds[1].y >= aabb.getMin().y &&
			this->bounds[0].y <= aabb.getMax().y &&
			this->bounds[1].z >= aabb.getMin().z &&
			this->bounds[0].z <= aabb.getMax().z);
	}

	bool containsPoint(const glm::vec3& point) const {
		return point.x >= this->getMin().x &&
			point.x <= this->getMax().x &&
			point.y >= this->getMin().y &&
			point.y <= this->getMax().y &&
			point.z >= this->getMin().z &&
			point.z <= this->getMax().z;
	}

	bool isWithIn(const AABBBoundingRegion& aabb) const {
		return containsPoint(aabb.getMin()) &&
			containsPoint(aabb.getMax());
	}

	const glm::vec3& getMin() const
	{
		return this->bounds[0];
	}
	const glm::vec3& getMax() const
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
		if (getMin().x != other.getMin().x) return getMin().x < other.getMin().x;
		if (getMin().y != other.getMin().y) return getMin().y < other.getMin().y;
		if (getMin().z != other.getMin().z) return getMin().z < other.getMin().z;
		if (getMax().x != other.getMax().x) return getMax().x < other.getMax().x;
		if (getMax().y != other.getMax().y) return getMax().y < other.getMax().y;
		return getMax().z < other.getMax().z;
	}
	
	bool operator==(const AABBBoundingRegion& other) const {
		return this->getMin() == other.getMin() && this->getMax() == other.getMax();
	}

	bool operator!=(const AABBBoundingRegion& other) const {
		return !(*this == other);
	}

private:

	void initializeBounds() {
		bounds[0] = glm::vec3(std::numeric_limits<float>::max());
		bounds[1] = glm::vec3(std::numeric_limits<float>::lowest());
	}

	void updateBounds(const glm::vec3& point) {
		bounds[0].x = glm::min(bounds[0].x, point.x);
		bounds[0].y = glm::min(bounds[0].y, point.y);
		bounds[0].z = glm::min(bounds[0].z, point.z);

		bounds[1].x = glm::max(bounds[1].x, point.x);
		bounds[1].y = glm::max(bounds[1].y, point.y);
		bounds[1].z = glm::max(bounds[1].z, point.z);
	}

	glm::vec3 bounds[2];
};
#endif