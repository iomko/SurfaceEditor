#ifndef TRIANGLEDATA_H
#define TRIANGLEDATA_H

#include <functional>
#include <glm/vec3.hpp>

struct TriangleData {
public:
	glm::vec3 positions[3]{};
	TriangleData()
	{}

	TriangleData(const glm::vec3* positionsArray)
		{
		for (int i = 0; i < 3; ++i) {
			positions[i] = positionsArray[i];
		}
	}

	TriangleData(std::initializer_list<const glm::vec3*> positionsArray)
		{
		int i = 0;
		for (const auto& pos : positionsArray) {
			if (i < 3) {
				positions[i++] = *pos;
			}
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

	// Define operator< for TriangleData
	bool operator<(const TriangleData& other) const {
		for (int i = 0; i < 3; ++i) {
			if (positions[i].x < other.positions[i].x && positions[i].y < other.positions[i].y && positions[i].z < other.positions[i].z) return true;
			if (other.positions[i].x < positions[i].x && other.positions[i].y < positions[i].y && other.positions[i].z < positions[i].z) return false;
		}
		return false;  // lhs and rhs are considered equal
	}
};
/*
namespace std {
	template <>
	struct less<TriangleData> {
		bool operator()(const TriangleData& lhs, const TriangleData& rhs) const {
			for (int i = 0; i < 3; ++i) {
				if (lhs.positions[i].x < rhs.positions[i].x && lhs.positions[i].y < rhs.positions[i].y && lhs.positions[i].z < rhs.positions[i].z) return true;
				if (rhs.positions[i].x < lhs.positions[i].x && rhs.positions[i].y < lhs.positions[i].y && rhs.positions[i].z < lhs.positions[i].z) return false;
			}
			return false;  // lhs and rhs are considered equal
		}
	};
}

*/
#endif

/*
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

	template <typename T>
	struct hash<std::pair<TriangleData, T>> {
		size_t operator()(const std::pair<TriangleData, T>& p) const {
			size_t hashValue = 0;
			// Combine the hash values of the elements in the pair
			hashValue ^= std::hash<TriangleData>{}(p.first);
			hashValue ^= std::hash<T>{}(p.second);
			return hashValue;
		}
	};

	template <typename T>
	struct equal_to<std::pair<TriangleData, T>> {
		bool operator()(const std::pair<TriangleData, T>& lhs, const std::pair<TriangleData, T>& rhs) const {
			// Compare the elements in the pair for equality
			return lhs.first == rhs.first && lhs.second == rhs.second;
		}
	};
}

*/
