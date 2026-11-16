#pragma once

#include "../Primitives/AABB.h"

#include <concepts>
#include <vector>

template <typename Index, typename Element>
concept SpatialIndex = requires(
	Index &index,
	const Element &element,
	const AABBBoundingRegion &bounds,
	const Ray &ray,
	const Sphere &sphere,
	std::vector<Element> &results) {
	{ index.insert(element, bounds) } -> std::same_as<void>;
	{ index.remove(element) } -> std::same_as<void>;
	{ index.query(ray, results) } -> std::same_as<void>;
	{ index.query(sphere, results) } -> std::same_as<void>;
	{ index.query(bounds, results) } -> std::same_as<void>;
	{ index.getAllElements() } -> std::same_as<std::vector<Element>>;
};
