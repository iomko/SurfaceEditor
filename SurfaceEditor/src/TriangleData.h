#ifndef TRIANGLEDATA_H
#define TRIANGLEDATA_H

#include <functional>
#include <glm/vec3.hpp>


struct TriangleData {
public:
	glm::vec3 positions[3]{};
	glm::vec3 color;
	TriangleData()
	{
		this->color.x = 1.0f;
		this->color.y = 0.0f;
		this->color.z = 0.0f;
	}

	TriangleData(const glm::vec3* positionsArray, const glm::vec3& col)
		: color(col) {
		for (int i = 0; i < 3; ++i) {
			positions[i] = positionsArray[i];
		}
	}

	bool operator==(const TriangleData& other) const {
		for (int i = 0; i < 3; ++i) {
			if (positions[i] != other.positions[i]) {
				return false;
			}
		}
		return true;
	}
};

namespace std
{
	template <>
	struct hash<TriangleData>
	{
		size_t operator()(const TriangleData& key) const {
			size_t hashValue = 0;

			for (int i = 0; i < 3; ++i) {
				hashValue ^= std::hash<float>{}(key.positions[i].x);
				hashValue ^= std::hash<float>{}(key.positions[i].y);
				hashValue ^= std::hash<float>{}(key.positions[i].z);
			}

			return hashValue;
		}
	};

	template <>
	struct equal_to<TriangleData> {
		bool operator()(const TriangleData& lhs, const TriangleData& rhs) const {
			return lhs == rhs;
		}
	};
}

#endif