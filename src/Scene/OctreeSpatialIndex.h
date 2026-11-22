#pragma once

#include "../Primitives/AABB.h"
#include "../Structures/Octree.h"

#include <algorithm>
#include <map>
#include <stdexcept>
#include <vector>

template <typename Element>
class OctreeSpatialIndex
{
public:
	explicit OctreeSpatialIndex(const glm::vec3 &voxelSize);

	void insert(const Element &element, const AABBBoundingRegion &bounds);
	void remove(const Element &element);
	void query(const Ray &ray, std::vector<Element> &results);
	void query(const AABBBoundingRegion &bounds, std::vector<Element> &results);
	void query(const Sphere &sphere, std::vector<Element> &results);
	std::vector<Element> getAllElements() const;

private:
	template <typename QueryShape, typename Intersect>
	static void appendQueryResults(
		Octree<Element> &tree,
		const QueryShape &queryShape,
		Intersect intersects,
		std::vector<Element> &results);

	glm::vec3 getVoxelIndex(const glm::vec3 &position) const;

	glm::vec3 m_voxelSize;
	std::map<glm::vec3, Octree<Element>> m_trees;
	std::map<Element, std::vector<glm::vec3>> m_elementCells;
};

template <typename Element>
inline OctreeSpatialIndex<Element>::OctreeSpatialIndex(const glm::vec3 &voxelSize)
	: m_voxelSize(voxelSize)
{
	if (voxelSize.x <= 0.0f || voxelSize.y <= 0.0f || voxelSize.z <= 0.0f)
	{
		throw std::invalid_argument("Spatial-index voxel sizes must be positive.");
	}
}

template <typename Element>
inline void OctreeSpatialIndex<Element>::insert(
	const Element &element,
	const AABBBoundingRegion &bounds)
{
	// remove(element); expected to be called before insert if the element already exists

	const glm::vec3 minCell = getVoxelIndex(bounds.getMinBoundsPos());
	const glm::vec3 maxCell = getVoxelIndex(bounds.getMaxBoundsPos());
	auto &elementCells = m_elementCells[element];

	for (int x = static_cast<int>(minCell.x); x <= static_cast<int>(maxCell.x); ++x)
	{
		for (int y = static_cast<int>(minCell.y); y <= static_cast<int>(maxCell.y); ++y)
		{
			for (int z = static_cast<int>(minCell.z); z <= static_cast<int>(maxCell.z); ++z)
			{
				const glm::vec3 cell{
					static_cast<float>(x),
					static_cast<float>(y),
					static_cast<float>(z)};
				auto treeIt = m_trees.find(cell);
				if (treeIt == m_trees.end())
				{
					const glm::vec3 minBounds = cell * m_voxelSize;
					const glm::vec3 maxBounds = minBounds + m_voxelSize;
					treeIt = m_trees.emplace(
										cell,
										Octree<Element>(minBounds, maxBounds))
								 .first;
				}

				treeIt->second.addDataToOctree(element, bounds);
				elementCells.push_back(cell);
			}
		}
	}
}

template <typename Element>
inline void OctreeSpatialIndex<Element>::remove(const Element &element)
{
	const auto elementIt = m_elementCells.find(element);
	if (elementIt == m_elementCells.end())
	{
		return;
	}

	for (const glm::vec3 &cell : elementIt->second)
	{
		const auto treeIt = m_trees.find(cell);
		if (treeIt == m_trees.end())
		{
			continue;
		}

		treeIt->second.removeData(element);
		if (treeIt->second.isEmpty())
		{
			m_trees.erase(treeIt);
		}
	}

	m_elementCells.erase(elementIt);
}

template <typename Element>
inline void OctreeSpatialIndex<Element>::query(
	const Ray &ray,
	std::vector<Element> &results)
{
	for (auto &[cell, tree] : m_trees)
	{
		appendQueryResults(
			tree,
			ray,
			[](const AABBBoundingRegion &nodeBounds, const Ray &queryRay)
			{
				return nodeBounds.intersectsRay(queryRay);
			},
			results);
	}
}

template <typename Element>
inline void OctreeSpatialIndex<Element>::query(
	const AABBBoundingRegion &bounds,
	std::vector<Element> &results)
{
	for (auto &[cell, tree] : m_trees)
	{
		appendQueryResults(
			tree,
			bounds,
			[](const AABBBoundingRegion &nodeBounds, const AABBBoundingRegion &queryBounds)
			{
				return nodeBounds.intersectsAABB(queryBounds);
			},
			results);
	}
}

template <typename Element>
inline void OctreeSpatialIndex<Element>::query(
	const Sphere &sphere,
	std::vector<Element> &results)
{
	for (auto &[cell, tree] : m_trees)
	{
		appendQueryResults(
			tree,
			sphere,
			[](const AABBBoundingRegion &nodeBounds, const Sphere &querySphere)
			{
				return nodeBounds.intersectsSphere(querySphere);
			},
			results);
	}
}

template <typename Element>
inline std::vector<Element> OctreeSpatialIndex<Element>::getAllElements() const
{
	std::vector<Element> elements;
	elements.reserve(m_elementCells.size());
	for (const auto &[element, cells] : m_elementCells)
	{
		elements.push_back(element);
	}
	return elements;
}

template <typename Element>
template <typename QueryShape, typename Intersect>
inline void OctreeSpatialIndex<Element>::appendQueryResults(
	Octree<Element> &tree,
	const QueryShape &queryShape,
	Intersect intersects,
	std::vector<Element> &results)
{
	std::vector<OctreeNode<Element> *> nodes;
	tree.template findMaxDepthNodes<QueryShape>(queryShape, intersects, nodes);
	for (const OctreeNode<Element> *node : nodes)
	{
		for (const Element &element : node->nodeData)
		{
			if (std::find(results.begin(), results.end(), element) == results.end())
			{
				results.push_back(element);
			}
		}
	}
}

template <typename Element>
inline glm::vec3 OctreeSpatialIndex<Element>::getVoxelIndex(
	const glm::vec3 &position) const
{
	return glm::floor(position / m_voxelSize);
}
