#pragma once
#include <vector>

namespace utils::containers
{

	template <typename T>
	void swapWithLast(std::vector<T>& vector, size_t index) {
		if (index < vector.size() - 1) {
			std::swap(vector[index], vector.back());
		}
	}

	template <typename T>
	void swapLastAndPop(std::vector<T>& vector, size_t index) {
		if (index < vector.size() - 1) {
			std::swap(vector[index], vector.back());
		}
		if (!vector.empty()) {
			vector.pop_back();
		}
	}

	template <typename T>
	void reverseSubrange(std::vector<T>& vec, size_t startingIndex, size_t endingIndex) {
		if (startingIndex >= vec.size() || endingIndex >= vec.size() || startingIndex > endingIndex)
			return;
		std::reverse(vec.begin() + startingIndex, vec.begin() + endingIndex + 1);
	}

}
