#ifndef AABBBOUNDINGREGION_H
#define AABBBOUNDINGREGION_H


class AABBBoundingRegion
{
public:
	AABBBoundingRegion(const std::vector<glm::vec3>& vertices)
	{
		minVector = maxVector = vertices[0];
		for (const auto& vertex : vertices) {
			minVector = glm::min(minVector, vertex);
			maxVector = glm::max(maxVector, vertex);
		}
	}
	AABBBoundingRegion(const glm::vec3& min, const glm::vec3& max) : minVector(min), maxVector(max){}


	bool intersectsAABB(const AABBBoundingRegion& aabb) const
	{
		bool firstTest = glm::max(this->maxVector.x,this->minVector.x) >= glm::min(aabb.minVector.x, aabb.maxVector.x);
		bool secondTest = glm::min(this->minVector.x,this->maxVector.x) <= glm::max(aabb.minVector.x, aabb.maxVector.x);
		bool thirdTest = glm::max(this->maxVector.y,this->minVector.y) >= glm::min(aabb.minVector.y, aabb.maxVector.y);
		bool fourthTest = glm::min(this->minVector.y,this->maxVector.y) <= glm::max(aabb.minVector.y, aabb.maxVector.y);
		bool fifthTest = glm::max(this->maxVector.z,this->minVector.z) >= glm::min(aabb.minVector.z, aabb.maxVector.z);
		bool seventhTest = glm::min(this->minVector.z,this->maxVector.z) <= glm::max(aabb.minVector.z, aabb.maxVector.z);

		return firstTest && secondTest && thirdTest && fourthTest && fifthTest && seventhTest;
	}

	const glm::vec3& getMin() const
	{
		return this->minVector;
	}
	const glm::vec3& getMax() const
	{
		return this->maxVector;
	}

	AABBBoundingRegion(const AABBBoundingRegion& other)
		: minVector(other.minVector), maxVector(other.maxVector)
	{
	}

	AABBBoundingRegion& operator=(const AABBBoundingRegion& other)
	{
		if (this != &other) {
			minVector = other.minVector;
			maxVector = other.maxVector;
		}
		return *this;
	}

private:
	glm::vec3 minVector{};
	glm::vec3 maxVector{};
};
#endif