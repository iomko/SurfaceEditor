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
	AABBBoundingRegion(const glm::vec3* positionsArray, size_t arraySize)
	{
		glm::vec3 minVector = positionsArray[0];
		glm::vec3 maxVector = positionsArray[0];

		for (size_t i = 0; i < arraySize; ++i) {
			minVector.x = glm::min(minVector.x, positionsArray[i].x);
			minVector.y = glm::min(minVector.y, positionsArray[i].y);
			minVector.z = glm::min(minVector.z, positionsArray[i].z);

			maxVector.x = glm::max(maxVector.x, positionsArray[i].x);
			maxVector.y = glm::max(maxVector.y, positionsArray[i].y);
			maxVector.z = glm::max(maxVector.z, positionsArray[i].z);
		}

		bounds[0] = minVector;
		bounds[1] = maxVector;
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

	static bool rayTriangleIntersect(const glm::vec3 &orig, const glm::vec3 &dir, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, float &t)
	{
		// compute the plane's normal
		glm::vec3 v0v1 = v1 - v0;
		glm::vec3 v0v2 = v2 - v0;
		// no need to normalize
		glm::vec3 N = glm::cross(v0v1, v0v2); // N
		float area2 = N.length();
 
		// Step 1: finding P
    
		// check if the ray and plane are parallel.
		float NdotRayDirection = glm::dot(N, dir);
		if (fabs(NdotRayDirection) < 1e-6) // almost 0
			return false; // they are parallel, so they don't intersect! 

		// compute d parameter using equation 2
		float d = -glm::dot(N, v0);
		// compute t (equation 3)
		t = -(glm::dot(N, orig) + d) / NdotRayDirection;
    
		// check if the triangle is behind the ray
		if (t < 0) return false; // the triangle is behind
 
		// compute the intersection point using equation 1
		glm::vec3 P = orig + t * dir;
 
		// Step 2: inside-outside test
		glm::vec3 C; // vector perpendicular to triangle's plane
 
		// edge 0
		glm::vec3 edge0 = v1 - v0; 
		glm::vec3 vp0 = P - v0;
		C = glm::cross(edge0, vp0);
		if (glm::dot(N,C) < 0) return false; // P is on the right side
 
		// edge 1
		glm::vec3 edge1 = v2 - v1; 
		glm::vec3 vp1 = P - v1;
		C = glm::cross(edge1, vp1);
		if (glm::dot(N, C) < 0)  return false; // P is on the right side
 
		// edge 2
		glm::vec3 edge2 = v0 - v2; 
		glm::vec3 vp2 = P - v2;
		C = glm::cross(edge2, vp2);
		if (glm::dot(N, C) < 0) return false; // P is on the right side;

		return true; // this ray hits the triangle
	}
	
	bool operator==(const AABBBoundingRegion& other) const {
		return this->getMin() == other.getMin() && this->getMax() == other.getMax();
	}

	bool operator!=(const AABBBoundingRegion& other) const {
		return !(*this == other);
	}

private:
	glm::vec3 bounds[2];
};
#endif