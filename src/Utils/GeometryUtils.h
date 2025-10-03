#pragma once
#include <stdexcept>
#include <vector>

#include <glm/glm.hpp>
#include "../Primitives/Plane.h"
#include "../Structures/ExtendedHalfEdge.h"

#include <algorithm>
#include <initializer_list>
#include <cmath> // fabs
#include <set>

namespace utils::geometry {

    struct Vec3LessEpsilon {
        float eps = 1e-6f; // tolerance
        bool operator()(const glm::vec3& a, const glm::vec3& b) const {
            if (std::abs(a.x - b.x) > eps) return a.x < b.x;
            if (std::abs(a.y - b.y) > eps) return a.y < b.y;
            if (std::abs(a.z - b.z) > eps) return a.z < b.z;
            return false; // considered equal
        }
    };

	enum class ProjectionAxis { ZY, XZ, XY };
    
    struct PlaneFaceIntersection {
        bool intersects;              // true if intersects in any way
        enum Type { None, Point, Edge, FullTriangle } type;
        glm::vec3 firstPoint;         // valid if Point or Edge
        glm::vec3 secondPoint;        // valid if Edge
    };

    inline PlaneFaceIntersection planeIntersectsFace(const Plane& plane, ExtendedFace* face) {
        std::set<glm::vec3, Vec3LessEpsilon> intersectionPoints;

        for(auto it = face->faceHalfEdgeBegin(); it != face->faceHalfEdgeEnd(); ++it) {
            ExtendedEdge* edge = (*it).m_edge;
            glm::vec3 v1 = edge->m_firstVertex->m_position;
            glm::vec3 v2 = edge->m_secondVertex->m_position;

            float d1 = glm::dot(plane.normal, v1 - plane.point);
            float d2 = glm::dot(plane.normal, v2 - plane.point);

            if((d1 < 0 && d2 > 0) || (d1 > 0 && d2 < 0)) {
                float t = -d1 / (d2 - d1);
                intersectionPoints.insert(v1 + t * (v2 - v1));
            } else if(d1 == 0) {
                intersectionPoints.insert(v1);
            } else if(d2 == 0) {
                intersectionPoints.insert(v2);
            }
        }

        PlaneFaceIntersection result{};
        switch(intersectionPoints.size()) {
            case 0:
                result.intersects = false;
                result.type = PlaneFaceIntersection::None;
                break;
            case 1:
                result.intersects = true;
                result.type = PlaneFaceIntersection::Point;
                result.firstPoint = *intersectionPoints.begin();
                break;
            case 2:
                result.intersects = true;
                result.type = PlaneFaceIntersection::Edge;
                {
                    auto it = intersectionPoints.begin();
                    result.firstPoint = *it++;
                    result.secondPoint = *it;
                }
                break;
            default:
                result.intersects = true;
                result.type = PlaneFaceIntersection::FullTriangle;
                break;
        }
        return result;
    }


    inline glm::vec2 createPerpendicular2DVector(const glm::vec2& a, const glm::vec2& b){
        glm::vec2 line = b - a;
        return glm::vec2(-line.y, line.x);
    }

    inline bool polygon2DOverlap(const std::vector<glm::vec2>& verticesA,
                                 const std::vector<glm::vec2>& verticesB)
    {
        if (verticesA.empty() || verticesB.empty()) return false;

        std::vector<glm::vec2> axes;
        auto add_axis = [&](const glm::vec2 &axis_raw) {
            const float len2 = glm::dot(axis_raw, axis_raw);
            if (len2 < 1e-12f) return; 

            glm::vec2 na = axis_raw / std::sqrt(len2);

            const float DEDUPE_DOT_TOL = 1.0f - 1e-6f;
            for (const glm::vec2 &existing : axes) {
                glm::vec2 ne = existing / glm::length(existing);
                if (std::fabs(glm::dot(na, ne)) > DEDUPE_DOT_TOL) {
                    return;
                }
            }
            axes.push_back(axis_raw);
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

    enum SideRelation {
        SameSide,
        OppositeSide,
        OnPlane
    };

    inline SideRelation checkSideRelation(
        const glm::vec3& planeNormal,
        const glm::vec3& p1,
        const glm::vec3& p2,
        float eps = 1e-6f)
    {
        float d1 = glm::dot(planeNormal, p1);
        float d2 = glm::dot(planeNormal, p2);

        bool on1 = std::fabs(d1) <= eps;
        bool on2 = std::fabs(d2) <= eps;

        if (on1 || on2) return SideRelation::OnPlane;

        if ((d1 > 0.f) == (d2 > 0.f))
            return SideRelation::SameSide;
        else
            return SideRelation::OppositeSide;
    }

    enum WindingOrder {
        None,
        CW,
        CCW
    };

    inline WindingOrder getWindingOrder(const std::vector<glm::vec2>& poly) {
        if (poly.size() < 3) return None; // not a polygon

        double area = 0.0;
        for (size_t i = 0; i < poly.size(); i++) {
            const glm::vec2& curr = poly[i];
            const glm::vec2& next = poly[(i + 1) % poly.size()];
            area += (next.x - curr.x) * (next.y + curr.y);
        }

        if (area > 0)  return CW;
        if (area < 0)  return CCW;
        return None;
    }

    inline glm::vec2 getInwardNormal(
        const glm::vec2& first,
        const glm::vec2& second,
        utils::geometry::WindingOrder winding)
    {
        glm::vec2 e = glm::normalize(second - first);

        glm::vec2 rightNormal{ e.y, -e.x };
        glm::vec2 leftNormal{ -e.y, e.x };

        if (winding == utils::geometry::WindingOrder::CCW)
            return leftNormal;
        else if (winding == utils::geometry::WindingOrder::CW)
            return rightNormal;
        else
            return glm::vec2(0.0f); // fallback
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
			return Plane{ glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
		}

		const glm::vec3& p0 = vertices[0];
		const glm::vec3& p1 = vertices[1];
		const glm::vec3& p2 = vertices[2];

		glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
		if (glm::length(normal) < epsilon) {
			return Plane{ p0, normal };
		}

		for (size_t i = 3; i < vertices.size(); ++i) {
			float dist = glm::dot(vertices[i] - p0, normal);
			if (std::abs(dist) > epsilon) {
				return Plane{ p0, normal };
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
			return glm::vec3(0.0f);
		}

		float t = glm::dot(plane.point - origin, plane.normal) / denom;

		if (t < 0.0f) {
			return glm::vec3(0.0f);
		}

		hit = true;
		return origin + t * direction;
	}

}
