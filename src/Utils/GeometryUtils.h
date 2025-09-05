#pragma once
#include <stdexcept>
#include <vector>

#include <glm/glm.hpp>
#include "../Primitives/Plane.h"
#include "../DataStructures/ExtendedHalfEdge.h"

#include <algorithm>
#include <initializer_list>
#include <cmath> // fabs

namespace utils::geometry {

	enum class ProjectionAxis { ZY, XZ, XY };
    
    //before
    /*
    inline glm::vec2 createPerpendicular2DVector(const glm::vec2& firstPoint, const glm::vec2& secondPoint){
        glm::vec2 line{secondPoint.x - firstPoint.x, secondPoint.y - firstPoint.y};
        return glm::vec2{-line.y, line.x};
    }

    inline bool overlap(float minA, float maxA, float minB, float maxB) {
        float EPS = 1e-5f * std::max({
            fabsf(minA), fabsf(maxA), fabsf(minB), fabsf(maxB), 1.0f 
        });

        // Non-overlap if intervals are separated or just touching
        return (minA <= maxB - EPS) && (minB <= maxA - EPS);
    }

    inline bool polygon2DOverlap(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB){
        //najskor si musime ziskat vsetky axis, ktore budeme testovat
        std::vector<glm::vec2> axes;
        
        for (int i = 0; i < verticesA.size(); ++i) {
            glm::vec2 currentVertex = verticesA.at(i);
            glm::vec2 nextVertex = verticesA.at((i+1) % verticesA.size());


            glm::vec2 axis = createPerpendicular2DVector(currentVertex, nextVertex);
            if (glm::length(axis) > 1e-8f) {
                axes.emplace_back(glm::normalize(axis));
            }
        } 

        for (int i = 0; i < verticesB.size(); ++i) {
            glm::vec2 currentVertex = verticesB.at(i);
            glm::vec2 nextVertex = verticesB.at((i+1) % verticesB.size());

            //glm::vec2 axis = createPerpendicular2DVector(currentVertex, nextVertex);
            //axis = glm::normalize(axis);
            //axes.emplace_back(axis);
            
            glm::vec2 axis = createPerpendicular2DVector(currentVertex, nextVertex);
            if (glm::length(axis) > 1e-8f) {
                axes.emplace_back(glm::normalize(axis));
            }
        } 

        //mame vytvorene axis
        //teraz musime ist cez vsetky axes
        //create projection
        
        for (glm::vec2 axis : axes) {

            float minA = glm::dot(verticesA.at(0), axis);
            float maxA = glm::dot(verticesA.at(0), axis);

            for (int i = 1; i < verticesA.size(); ++i) {
                float projectedVertex = glm::dot(verticesA.at(i),axis);
                if(projectedVertex > maxA){
                    maxA = projectedVertex;
                }
                if(projectedVertex < minA){
                    minA = projectedVertex;
                }
            }

            float minB = glm::dot(verticesB.at(0), axis);
            float maxB = glm::dot(verticesB.at(0), axis);

            for (int i = 1; i < verticesB.size(); ++i) {
                float projectedVertex = glm::dot(verticesB.at(i),axis);
                if(projectedVertex > maxB){
                    maxB = projectedVertex;
                }
                if(projectedVertex < minB){
                    minB = projectedVertex;
                }
            }

            if (!overlap(minA, maxA, minB, maxB)) {
                return false;
            } else {

            }

        }
        return true;
    }
    */

    //after
    inline glm::vec2 createPerpendicular2DVector(const glm::vec2& a, const glm::vec2& b){
        // return unnormalized perpendicular (no normalization here)
        glm::vec2 line = b - a;
        return glm::vec2(-line.y, line.x);
    }

    // return true if polygons A and B overlap (touching = NOT overlap)
    inline bool polygon2DOverlap(const std::vector<glm::vec2>& verticesA,
                                 const std::vector<glm::vec2>& verticesB)
    {
        if (verticesA.empty() || verticesB.empty()) return false;

        // collect unique axes (store raw perpendiculars)
        std::vector<glm::vec2> axes;
        auto add_axis = [&](const glm::vec2 &axis_raw) {
            const float len2 = glm::dot(axis_raw, axis_raw);
            if (len2 < 1e-12f) return; // degenerate edge -> skip

            // compare direction (use normalized direction for comparison only)
            glm::vec2 na = axis_raw / std::sqrt(len2);

            const float DEDUPE_DOT_TOL = 1.0f - 1e-6f; // nearly parallel
            for (const glm::vec2 &existing : axes) {
                glm::vec2 ne = existing / glm::length(existing);
                if (std::fabs(glm::dot(na, ne)) > DEDUPE_DOT_TOL) {
                    return; // same axis already present
                }
            }
            axes.push_back(axis_raw); // store raw
        };

        auto collect_axes_from = [&](const std::vector<glm::vec2>& V) {
            const int n = (int)V.size();
            for (int i = 0; i < n; ++i) {
                const glm::vec2 &cur = V[i];
                const glm::vec2 &next = V[(i+1) % n];
                glm::vec2 axis = createPerpendicular2DVector(cur, next);
                add_axis(axis);
            }
        };

        collect_axes_from(verticesA);
        collect_axes_from(verticesB);

        // SAT: if for any axis the projections are separated (or just touching),
        // polygons do NOT overlap.
        for (const glm::vec2 &axis_raw : axes) {
            // project A
            float minA = glm::dot(verticesA[0], axis_raw);
            float maxA = minA;
            for (size_t i = 1; i < verticesA.size(); ++i) {
                float p = glm::dot(verticesA[i], axis_raw);
                if (p < minA) minA = p;
                if (p > maxA) maxA = p;
            }

            // project B
            float minB = glm::dot(verticesB[0], axis_raw);
            float maxB = minB;
            for (size_t i = 1; i < verticesB.size(); ++i) {
                float p = glm::dot(verticesB[i], axis_raw);
                if (p < minB) minB = p;
                if (p > maxB) maxB = p;
            }

            // EPS relative to projection magnitudes (axis is raw so projections may be large)
            float scaleRef = std::max({1.0f, fabsf(minA), fabsf(maxA), fabsf(minB), fabsf(maxB)});
            float EPS = 1e-6f * scaleRef;

            // If intervals are separated or just touching -> no overlap (touching treated as non-overlap)
            if (maxA <= minB + EPS || maxB <= minA + EPS) {
                return false;
            }
            // otherwise there's overlap on this axis -> check next axis
        }

        // no separating axis found -> polygons overlap (strict intersection)
        return true;
    }

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
		else if (projectionAxis == ProjectionAxis::XZ)
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
