#pragma once
#include <algorithm>

namespace utils::interpolation
{
	inline float smoothstep(float edge0, float edge1, float x) {
		x = std::max(0.0f, std::min((x - edge0) / (edge1 - edge0), 1.0f));
		return x * x * (3 - 2 * x);
	}
}
