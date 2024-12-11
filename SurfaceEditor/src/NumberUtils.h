#pragma once
#include <cmath>
#include <limits> 

class NumberUtils {
public:
	static bool isWholeNumber(float numerator, float denominator) {
		float result = numerator / denominator;
		return std::abs(result - std::round(result)) < std::numeric_limits<float>::epsilon();
	}

	static bool isPositive(float number) {
		return number >= 0.0f;
	}
};
