#pragma once
#include <stdexcept>
#include <vector>

#include <glm/glm.hpp>
#include "../Primitives/Plane.h"
#include "../DataStructures/ExtendedHalfEdge.h"

namespace utils::geometry {

	enum class ProjectionAxis { ZY, ZX, XY };

	inline std::vector<glm::vec2> projectVertices(const std::vector<glm::vec3>& vertices, const ProjectionAxis& projectionAxis)
	{
		std::vector<glm::vec2> projectedVertices;
		if (projectionAxis == ProjectionAxis::ZY)
		{
			for (const glm::vec3& vertex : vertices)
			{
				projectedVertices.emplace_back(glm::vec2(vertex.z, vertex.y));
			}
		}
		else if (projectionAxis == ProjectionAxis::ZX)
		{
			for (const glm::vec3& vertex : vertices)
			{
				projectedVertices.emplace_back(glm::vec2(vertex.x, vertex.z));
			}
		}
		else if (projectionAxis == ProjectionAxis::XY)
		{
			for (const glm::vec3& vertex : vertices)
			{
				projectedVertices.emplace_back(glm::vec2(vertex.x, vertex.y));
			}
		}

		return projectedVertices;
	}

	std::vector<glm::vec3> triangulatePolygon(const std::vector<glm::vec3>& vertices);

	template <typename Iterator, typename PointExtractor>
	glm::vec3 computePolygonNormal(Iterator begin, Iterator end, PointExtractor extractor) {
		int numOfVerticesInFace = std::distance(begin, end);

		if (numOfVerticesInFace < 3) {
			throw std::invalid_argument("Face must have at least three vertices to compute a normal.");
		}

		if (numOfVerticesInFace == 3) {
			// Compute face normal for a triangle
			glm::vec3 vertices[3];
			int i = 0;
			for (Iterator it = begin; it != end && i < 3; ++it, ++i) {
				vertices[i] = extractor(*it);
			}

			glm::vec3 edge1 = vertices[1] - vertices[0];
			glm::vec3 edge2 = vertices[2] - vertices[0];
			glm::vec3 normal = glm::cross(edge1, edge2);
			return glm::normalize(normal);
		}
		else {
			// Compute face normal for a general polygon using Newell's method
			glm::vec3 normal(0.0f);
			Iterator it0 = begin;
			Iterator it1 = begin;
			++it1;

			for (; it1 != end; ++it0, ++it1) {
				glm::vec3 v0 = extractor(*it0);
				glm::vec3 v1 = extractor(*it1);

				normal.x += (v0.y - v1.y) * (v0.z + v1.z);
				normal.y += (v0.z - v1.z) * (v0.x + v1.x);
				normal.z += (v0.x - v1.x) * (v0.y + v1.y);
			}

			// wrap around: last to first
			glm::vec3 vLast = extractor(*it0);
			glm::vec3 vFirst = extractor(*begin);

			normal.x += (vLast.y - vFirst.y) * (vLast.z + vFirst.z);
			normal.y += (vLast.z - vFirst.z) * (vLast.x + vFirst.x);
			normal.z += (vLast.x - vFirst.x) * (vLast.y + vFirst.y);

			return glm::normalize(normal);
		}
	}

	inline glm::vec3 computePolygonNormal(ExtendedFace* face) {
		auto pointExtractor = [](ExtendedVertex& v) { return v.m_position; };
		return computePolygonNormal(face->faceVertexBegin(), face->faceVertexEnd(), pointExtractor);
	}

	inline float computePolygonArea(const std::vector<glm::vec2>& vertices) {
		if (vertices.size() < 3) return 0.0f;

		float area = 0.0f;
		size_t n = vertices.size();

		for (size_t i = 0; i < n; ++i) {
			const glm::vec2& current = vertices[i];
			const glm::vec2& next = vertices[(i + 1) % n];
			area += (current.x * next.y) - (next.x * current.y);
		}

		return std::abs(area) * 0.5f;
	}

	inline Plane fitPlaneToPoints(const std::vector<glm::vec3>& vertices, bool& allCoplanar)
	{
		constexpr float epsilon = 1e-5f;
		allCoplanar = false;

		if (vertices.size() < 3) {
			return Plane{ glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f) }; // fallback/default plane
		}

		const glm::vec3& p0 = vertices[0];
		const glm::vec3& p1 = vertices[1];
		const glm::vec3& p2 = vertices[2];

		glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
		if (glm::length(normal) < epsilon) {
			return Plane{ p0, normal }; // Degenerate normal, not a valid plane
		}

		for (size_t i = 3; i < vertices.size(); ++i) {
			float dist = glm::dot(vertices[i] - p0, normal);
			if (std::abs(dist) > epsilon) {
				return Plane{ p0, normal }; // Not coplanar
			}
		}

		allCoplanar = true;
		return Plane{ p0, normal };
	}

	inline glm::vec3 intersectRayPlane(const Plane& plane, const glm::vec3& origin, const glm::vec3& direction, bool& hit)
	{
		constexpr float epsilon = 1e-5f;
		hit = false;

		float denom = glm::dot(plane.normal, direction);
		if (std::abs(denom) < epsilon) {
			return glm::vec3(0.0f); // No intersection, direction is parallel to plane
		}

		float t = glm::dot(plane.point - origin, plane.normal) / denom;

		if (t < 0.0f) {
			return glm::vec3(0.0f); // Intersection is behind the origin
		}

		hit = true;
		return origin + t * direction;
	}

}
