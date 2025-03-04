#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

//CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/mark_domain_in_triangulation.h>
//
//CGAL
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Point Point;
//

#include "PolygonTraits.h"

class PolygonOperations
{
public:

	static std::vector<glm::vec2> triangulate2DPolygon(std::vector<int>& polygonIndices, const std::vector<glm::vec2>& vertices)
	{
		CDT cdt;
		std::vector<Vertex_handle> vertsInVertexHandle;

		for (const auto& vertex : vertices)
		{
			vertsInVertexHandle.push_back(cdt.insert(Point(vertex.x, vertex.y)));
		}
		
		for (int i = 0; i < polygonIndices.size(); ++i)
		{
			cdt.insert_constraint(
				vertsInVertexHandle.at(polygonIndices.at(i)),
				vertsInVertexHandle.at(polygonIndices.at((i + 1) % polygonIndices.size()))
			);
		}

		CGAL::mark_domain_in_triangulation(cdt);
		CGAL::refine_Delaunay_mesh_2(
			cdt,
			CGAL::parameters::domain_is_initialized(true)
		);

		
		std::vector<glm::vec2> accumulatedPointsData;
		for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
			fit != cdt.finite_faces_end(); ++fit)
		{
			if (fit->is_in_domain()) {
				accumulatedPointsData.push_back({ fit->vertex(0)->point().x(), fit->vertex(0)->point().y() });
				accumulatedPointsData.push_back({ fit->vertex(1)->point().x(), fit->vertex(1)->point().y() });
				accumulatedPointsData.push_back({ fit->vertex(2)->point().x(), fit->vertex(2)->point().y() });
			}
		}

		return accumulatedPointsData;
	}

	static glm::vec3 computeFaceNormal(HalfEdgeDS::Face* face) {
		auto vertexExtractor = [](HalfEdgeDS::Vertex& v) { return v.getPosition(); };
		return computeFaceNormal(face->faceVertexBegin(), face->faceVertexEnd(), vertexExtractor);
	}

	template <typename Iterator, typename Extractor>
	static glm::vec3 computeFaceNormal(Iterator begin, Iterator end, Extractor extractor) {
		int numOfVerticesInFace = std::distance(begin, end);

		if (numOfVerticesInFace < 3) {
			throw std::invalid_argument("A face must have at least three vertices to compute a normal.");
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
			// Compute face normal for a general polygon
			glm::vec3 normal(0.0f, 0.0f, 0.0f);
			//TODO: IMPLEMENT

			return glm::normalize(normal);
		}
	}

	/*
	static glm::vec3 calculatePolygonNormal(const std::vector<int>& polygonIndices,
		const std::vector<glm::vec3>& vertices, const std::map<int, AngleType>& angleTypesMap)
	{
		glm::vec3 normal(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < polygonIndices.size(); ++i)
		{
			const auto& polygonVertexPrev = polygonIndices.at((i + (polygonIndices.size() - 1)) % polygonIndices.size());
			const auto& polygonVertexCur = polygonIndices.at(i % polygonIndices.size());
			const auto& polygonVertexNext = polygonIndices.at((i + 1) % polygonIndices.size());

			glm::vec3 previousVertex = vertices.at(polygonVertexPrev);
			glm::vec3 currentVertex = vertices.at(polygonVertexCur);
			glm::vec3 nextVertex = vertices.at(polygonVertexNext);

			glm::vec3 vector_cur_prev = previousVertex - currentVertex;
			glm::vec3 vector_cur_next = nextVertex - currentVertex;

			const auto& angleTypesIt = angleTypesMap.find(polygonVertexCur);

			if (angleTypesIt != angleTypesMap.end())
			{
				if (angleTypesIt->second == AngleType::CONCAVE)
				{
					normal = glm::cross(vector_cur_prev, vector_cur_next);
				}
				else if (angleTypesIt->second == AngleType::CONVEX)
				{
					normal = glm::cross(vector_cur_next, vector_cur_prev);
				}
			}

			return glm::normalize(normal);
		}

		return normal;
	}

	
	static std::map<int, AngleType> calculateAngleTypesOfVertices(const std::vector<int>& polygonIndices,
		const std::vector<glm::vec3>& vertices, const glm::vec3& polygonNormal)
	{
		std::map<int, AngleType> angleTypes;

		for (int i = 0; i < polygonIndices.size(); ++i)
		{
			// Get the previous, current, and next vertices
			glm::vec3 previousVertex = vertices.at(polygonIndices.at((i + (polygonIndices.size() - 1)) % polygonIndices.size()));
			glm::vec3 currentVertex = vertices.at(polygonIndices.at(i % polygonIndices.size()));
			glm::vec3 nextVertex = vertices.at(polygonIndices.at((i + 1) % polygonIndices.size()));

			// Calculate the dot product
			glm::vec3 vector_prev_cur = currentVertex - previousVertex;
			glm::vec3 vector_prev_next = nextVertex - previousVertex;
			const auto& dotProduct = glm::dot(glm::cross(vector_prev_cur, vector_prev_next), polygonNormal);

			// Determine angle type
			if (dotProduct < 0.0)
			{
				angleTypes[polygonIndices.at(i)] = AngleType::CONCAVE;
			}
			else if (dotProduct > 0.0)
			{
				angleTypes[polygonIndices.at(i)] = AngleType::CONVEX;
			}
			else
			{
				angleTypes[polygonIndices.at(i)] = AngleType::FLAT;
			}
		}

		return angleTypes;
	}
	*/

	static float calculateSignedAreaOf2DPolygon(const std::vector<glm::vec2>& vertices)
	{
		float area = 0.0;
		size_t j = 1;
		for (size_t i = 0; i < vertices.size(); i++, j++) {
			j = j % vertices.size();
			area += (vertices.at(j).x - vertices.at(i).x) * (vertices.at(j).y + vertices.at(i).y);
		}
		return area / 2.0;
	}

};
