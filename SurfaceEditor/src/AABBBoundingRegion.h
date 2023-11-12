#ifndef AABBBOUNDINGREGION_H
#define AABBBOUNDINGREGION_H

#include "Ray.h"

class AABBBoundingRegion
{
public:
	AABBBoundingRegion(const std::vector<glm::vec3>& vertices)
	{
		/*
		minVector = maxVector = vertices[0];
		for (const auto& vertex : vertices) {
			minVector = glm::min(minVector, vertex);
			maxVector = glm::max(maxVector, vertex);
		}
		*/
	}
	AABBBoundingRegion(const glm::vec3& min, const glm::vec3& max){
		//construct min and max in case that min and max doesnt correspond to the actual min and max
		this->bounds[0].x = glm::min(min.x, max.x);
		this->bounds[0].y = glm::min(min.y, max.y);
		this->bounds[0].z = glm::min(min.z, max.z);
		
		this->bounds[1].x = glm::max(min.x, max.x);
		this->bounds[1].y = glm::max(min.y, max.y);
		this->bounds[1].z = glm::max(min.z, max.z);
	}


	bool intersectsAABB(const AABBBoundingRegion& aabb) const
	{
		/*
		bool firstTest = glm::max(this->maxVector.x,this->minVector.x) >= glm::min(aabb.minVector.x, aabb.maxVector.x);
		bool secondTest = glm::min(this->minVector.x,this->maxVector.x) <= glm::max(aabb.minVector.x, aabb.maxVector.x);
		bool thirdTest = glm::max(this->maxVector.y,this->minVector.y) >= glm::min(aabb.minVector.y, aabb.maxVector.y);
		bool fourthTest = glm::min(this->minVector.y,this->maxVector.y) <= glm::max(aabb.minVector.y, aabb.maxVector.y);
		bool fifthTest = glm::max(this->maxVector.z,this->minVector.z) >= glm::min(aabb.minVector.z, aabb.maxVector.z);
		bool seventhTest = glm::min(this->minVector.z,this->maxVector.z) <= glm::max(aabb.minVector.z, aabb.maxVector.z);
		
		return firstTest && secondTest && thirdTest && fourthTest && fifthTest && seventhTest;
		*/

		return (this->bounds[1].x >= aabb.getMin().x &&
			this->bounds[0].x <= aabb.getMax().x &&
			this->bounds[1].y >= aabb.getMin().y &&
			this->bounds[0].y <= aabb.getMax().y &&
			this->bounds[1].z >= aabb.getMin().z &&
			this->bounds[0].z <= aabb.getMax().z);
		
	}

	bool containsPoint(const glm::vec3& point) {
		return point.x >= this->getMin().x &&
			point.x <= this->getMax().x &&
			point.y >= this->getMin().y &&
			point.y <= this->getMax().y &&
			point.z >= this->getMin().z &&
			point.z <= this->getMax().z;
	}

	bool isWithIn(const AABBBoundingRegion& aabb) {
		return containsPoint(aabb.getMin()) &&
			containsPoint(aabb.getMax());
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
	/*
	bool operator==(const AABBBoundingRegion& lhs, const AABBBoundingRegion& rhs) {
		return lhs.getMin() == rhs.getMin() && lhs.getMax() == rhs.getMax();
	}

	bool operator!=(const AABBBoundingRegion& lhs, const AABBBoundingRegion& rhs) {
		return !(lhs == rhs);
	}
	*/

private:
	glm::vec3 bounds[2];
};
#endif