#pragma once

#include "../Primitives/AABB.h"

#include <algorithm>
#include <vector>

template <typename Element>
class VectorSpatialIndex
{
public:
	explicit VectorSpatialIndex();

	void insert(const Element &element, const AABBBoundingRegion &bounds);
	void remove(const Element &element);
	void query(const Ray &ray, std::vector<Element> &results);
	void query(const AABBBoundingRegion &bounds, std::vector<Element> &results);
	void query(const Sphere &sphere, std::vector<Element> &results);
	std::vector<Element> getAllElements() const;

private:
	struct Entry
	{
		Element element;
		AABBBoundingRegion bounds;
	};

	std::vector<Entry> m_elements;
};

template <typename Element>
inline VectorSpatialIndex<Element>::VectorSpatialIndex() = default;

template <typename Element>
inline void VectorSpatialIndex<Element>::insert(
	const Element &element,
	const AABBBoundingRegion &bounds)
{
	m_elements.push_back({element, bounds});
}

template <typename Element>
inline void VectorSpatialIndex<Element>::remove(const Element &element)
{
	m_elements.erase(
		std::remove_if(
			m_elements.begin(),
			m_elements.end(),
			[&element](const Entry &entry)
			{
				return entry.element == element;
			}),
		m_elements.end());
}

template <typename Element>
inline void VectorSpatialIndex<Element>::query(
	const Ray &ray,
	std::vector<Element> &results)
{
	for (const Entry &entry : m_elements)
	{
		if (entry.bounds.intersectsRay(ray))
		{
			results.push_back(entry.element);
		}
	}
}

template <typename Element>
inline void VectorSpatialIndex<Element>::query(
	const AABBBoundingRegion &bounds,
	std::vector<Element> &results)
{
	for (const Entry &entry : m_elements)
	{
		if (entry.bounds.intersectsAABB(bounds))
		{
			results.push_back(entry.element);
		}
	}
}

template <typename Element>
inline void VectorSpatialIndex<Element>::query(
	const Sphere &sphere,
	std::vector<Element> &results)
{
	for (const Entry &entry : m_elements)
	{
		if (entry.bounds.intersectsSphere(sphere))
		{
			results.push_back(entry.element);
		}
	}
}

template <typename Element>
inline std::vector<Element> VectorSpatialIndex<Element>::getAllElements() const
{
	std::vector<Element> elements;
	elements.reserve(m_elements.size());
	for (const Entry &entry : m_elements)
	{
		elements.push_back(entry.element);
	}
	return elements;
}
