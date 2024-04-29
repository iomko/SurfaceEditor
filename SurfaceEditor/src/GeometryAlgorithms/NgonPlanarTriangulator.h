#pragma once
#include <vector>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "PolygonTriangulator.h"
#include "../RayIntersectionAlg.h"


//CGAL defs and includes for least quares fitting
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/linear_least_squares_fitting_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3 Point_3;
typedef K::Plane_3 Plane_3;
typedef std::vector<Point_3> PointList;
//

//CGAL defs and includes for constrained delaunay triangulation
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/mark_domain_in_triangulation.h>

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Point Point;
//

class NgonPlanarTriangulator : public PolygonTriangulator
{

public:
	virtual bool calculateTriangulation(const std::vector<int>& ngonPlanarIndices, const std::vector<glm::vec3>& ngonPlanarVertices, std::vector<glm::vec3>& triangulatedNgonPlanarResult, glm::vec3& ngonNormalResult) override
	{
		//vertices in format that will be accepted by least squares fitting in CGAL
		PointList leastSquaresFormatVertices;

		//tieto projected vertices na 2D plane pouzivame pri vypocitavani signed area
		std::vector<glm::vec2> projectedVerticesOnZPlane2D;
		std::vector<glm::vec2> projectedVerticesOnYPlane2D;
		std::vector<glm::vec2> projectedVerticesOnXPlane2D;

		std::vector<glm::vec3> projectedVerticesOnZPlane3D;
		std::vector<glm::vec3> projectedVerticesOnYPlane3D;
		std::vector<glm::vec3> projectedVerticesOnXPlane3D;

		for (auto& vertex : ngonPlanarVertices)
		{
			projectedVerticesOnZPlane2D.push_back({ vertex.x, vertex.y });
			projectedVerticesOnYPlane2D.push_back({ vertex.x, vertex.z });
			projectedVerticesOnXPlane2D.push_back({ vertex.y, vertex.z });

			projectedVerticesOnZPlane3D.push_back({ vertex.x, vertex.y, 0.0 });
			projectedVerticesOnYPlane3D.push_back({ vertex.x, 0.0, vertex.z });
			projectedVerticesOnXPlane3D.push_back({ 0.0, vertex.y, vertex.z });

			leastSquaresFormatVertices.push_back(Point_3(vertex.x, vertex.y, vertex.z));
		}

		bool planarNgon = arePointsCoplanar(leastSquaresFormatVertices);

		if (planarNgon)
		{
			glm::vec3 firstNgonVertex = ngonPlanarVertices.at(0);
			glm::vec3 projectedNormal{0.0, 0.0, 0.0};
			glm::vec3 ngonNormal;
			glm::vec3 vertexComponentAfterTriangulation{ 0.0, 0.0, 0.0 };
			std::map<int, AngleType> ngonAngleTypesOfVertices;
			std::vector<glm::vec2> verticesOfPlaneProjectedNgon;
			glm::vec3 pointOfProjectedVertex{0.0, 0.0, 0.0};
			float areaOfNgonProjectedOnZPlane = calculateSignedAreaOf2DNgon(projectedVerticesOnZPlane2D);
			if (glm::abs(areaOfNgonProjectedOnZPlane) > 0.0001f)
			{
				calculateDirectionOfVectorComponentBasedOnArea(projectedNormal.z, areaOfNgonProjectedOnZPlane);
				calculateAngleTypesOfVertices(ngonPlanarIndices, projectedVerticesOnZPlane3D, projectedNormal, ngonAngleTypesOfVertices);
				triangulatePlaneProjectedNgon(ngonPlanarIndices, projectedVerticesOnZPlane2D, verticesOfPlaneProjectedNgon);

				glm::vec3 directionOfProjectedNormal = projectedNormal;
				glm::vec3 oppositeDirectionOfProjectedNormal = -projectedNormal;
				if (!calculateNormalOfNgonBasedOnAngleTypes(ngonPlanarIndices, ngonPlanarVertices, ngonAngleTypesOfVertices, ngonNormal))
				{
					return false;
				}

				for (const auto& planeProjectedVertex : verticesOfPlaneProjectedNgon)
				{
					glm::vec3 vertexOnProjectedPlane(planeProjectedVertex.x, planeProjectedVertex.y, 0.0);
					if(!calculateVertexComponentAfterTriangulation(vertexOnProjectedPlane, directionOfProjectedNormal, oppositeDirectionOfProjectedNormal, ngonNormal, firstNgonVertex, vertexComponentAfterTriangulation))
					{
						return false;
					}
					triangulatedNgonPlanarResult.push_back({ vertexOnProjectedPlane.x, vertexOnProjectedPlane.y, vertexComponentAfterTriangulation.z });
				}
				ngonNormalResult = ngonNormal;
				return true;

			} else
			{
				float areaOfNgonProjectedOnYPlane = calculateSignedAreaOf2DNgon(projectedVerticesOnYPlane2D);
				if(glm::abs(areaOfNgonProjectedOnYPlane) > 0.0001f)
				{
					calculateDirectionOfVectorComponentBasedOnArea(projectedNormal.y, areaOfNgonProjectedOnYPlane);
					calculateAngleTypesOfVertices(ngonPlanarIndices, projectedVerticesOnYPlane3D, projectedNormal, ngonAngleTypesOfVertices);
					triangulatePlaneProjectedNgon(ngonPlanarIndices, projectedVerticesOnYPlane2D, verticesOfPlaneProjectedNgon);

					glm::vec3 directionOfProjectedNormal = projectedNormal;
					glm::vec3 oppositeDirectionOfProjectedNormal = -projectedNormal;
					if (!calculateNormalOfNgonBasedOnAngleTypes(ngonPlanarIndices, ngonPlanarVertices, ngonAngleTypesOfVertices, ngonNormal))
					{
						return false;
					}

					for (const auto& planeProjectedVertex : verticesOfPlaneProjectedNgon)
					{
						glm::vec3 vertexOnProjectedPlane(planeProjectedVertex.x, 0.0, planeProjectedVertex.y);
						if(!calculateVertexComponentAfterTriangulation(vertexOnProjectedPlane, directionOfProjectedNormal, oppositeDirectionOfProjectedNormal, ngonNormal, firstNgonVertex, vertexComponentAfterTriangulation))
						{
							return false;
						}
						triangulatedNgonPlanarResult.push_back({ vertexOnProjectedPlane.x, vertexComponentAfterTriangulation.y, vertexOnProjectedPlane.y });
					}
					ngonNormalResult = ngonNormal;
					return true;

				} else
				{
					float areaOfNgonProjectedOnXPlane = calculateSignedAreaOf2DNgon(projectedVerticesOnZPlane2D);
					if(glm::abs(areaOfNgonProjectedOnXPlane) > 0.0001f)
					{
						calculateDirectionOfVectorComponentBasedOnArea(projectedNormal.x, areaOfNgonProjectedOnXPlane);
						calculateAngleTypesOfVertices(ngonPlanarIndices, projectedVerticesOnXPlane3D, projectedNormal, ngonAngleTypesOfVertices);
						triangulatePlaneProjectedNgon(ngonPlanarIndices, projectedVerticesOnXPlane2D, verticesOfPlaneProjectedNgon);


						glm::vec3 directionOfProjectedNormal = projectedNormal;
						glm::vec3 oppositeDirectionOfProjectedNormal = -projectedNormal;
						if (!calculateNormalOfNgonBasedOnAngleTypes(ngonPlanarIndices, ngonPlanarVertices, ngonAngleTypesOfVertices, ngonNormal))
						{
							return false;
						}

						for (const auto& planeProjectedVertex : verticesOfPlaneProjectedNgon)
						{
							glm::vec3 vertexOnProjectedPlane(0.0, planeProjectedVertex.x, planeProjectedVertex.y);
							if(!calculateVertexComponentAfterTriangulation(vertexOnProjectedPlane, directionOfProjectedNormal, oppositeDirectionOfProjectedNormal, ngonNormal, firstNgonVertex, vertexComponentAfterTriangulation))
							{
								return false;
							}
							triangulatedNgonPlanarResult.push_back({ vertexComponentAfterTriangulation.x, vertexOnProjectedPlane.x, vertexOnProjectedPlane.y });
						}
						ngonNormalResult = ngonNormal;
						return true;

					} else
					{
						return false;
					}
				}
			}

		}
		return false;
	}

private:

	enum AngleType
	{
		CONVEX = 0,
		CONCAVE = 1,
		FLAT = 2
	};

	bool calculateVertexComponentAfterTriangulation(glm::vec3& vertexOnProjectedPlane, glm::vec3& directionOfVertexOnProjectedPlane, glm::vec3& oppositeDirectionOfVertexOnProjectedPlane, 
		glm::vec3& planeNormal, glm::vec3& planeVertex, glm::vec3& vertexComponentAfterTriangulation)
	{
		bool intersectsPlane = true;
		float amountToBeMultiplied = 0.0;
		glm::vec3 componentAfterTriangulation{ 0.0, 0.0, 0.0 };
		if (!RayIntersectionAlg::intersectPlane(planeNormal, planeVertex, vertexOnProjectedPlane, directionOfVertexOnProjectedPlane, amountToBeMultiplied))
		{
			if (!RayIntersectionAlg::intersectPlane(planeNormal, planeVertex, vertexOnProjectedPlane, oppositeDirectionOfVertexOnProjectedPlane, amountToBeMultiplied))
			{
				intersectsPlane = false;
			} else
			{
				componentAfterTriangulation = oppositeDirectionOfVertexOnProjectedPlane * amountToBeMultiplied;
			}
		} else
		{
			componentAfterTriangulation = directionOfVertexOnProjectedPlane * amountToBeMultiplied;
		}

		if(glm::length(componentAfterTriangulation) < 0.000001)
		{
			componentAfterTriangulation = glm::vec3{ 0.0, 0.0, 0.0 };
		}

		vertexComponentAfterTriangulation = componentAfterTriangulation;
		return intersectsPlane;
	}

	bool calculateNormalOfNgonBasedOnAngleTypes(const std::vector<int>& ngonIndices, const std::vector<glm::vec3>& ngonVertices ,std::map<int, AngleType>& ngonAngleTypes, glm::vec3& returnedNormal) {
		bool calculatedNormal = false;
		for (int i = 0; i < ngonIndices.size(); ++i)
		{
			const auto& ngonIndexVertexPrev = ngonIndices.at((i + (ngonIndices.size() - 1)) % ngonIndices.size());
			const auto& ngonIndexVertexCur = ngonIndices.at(i % ngonIndices.size());
			const auto& ngonIndexVertexNext = ngonIndices.at((i + 1) % ngonIndices.size());

			glm::vec3 previousVertex = ngonVertices.at(ngonIndexVertexPrev);
			glm::vec3 currentVertex = ngonVertices.at(ngonIndexVertexCur);
			glm::vec3 nextVertex = ngonVertices.at(ngonIndexVertexNext);

			glm::vec3 vectorCurPrev = previousVertex - currentVertex;
			glm::vec3 vectorCurNext = nextVertex - currentVertex;

			const auto& angleTypesIt = ngonAngleTypes.find(ngonIndexVertexCur);

			if (angleTypesIt != ngonAngleTypes.end())
			{
				if (angleTypesIt->second == AngleType::CONCAVE)
				{
					returnedNormal = glm::cross(vectorCurPrev, vectorCurNext);
					calculatedNormal = true;
				}
				else if (angleTypesIt->second == AngleType::CONVEX)
				{
					returnedNormal = glm::cross(vectorCurNext, vectorCurPrev);
					calculatedNormal = true;
				}
			} else
			{
				calculatedNormal = false;
				return calculatedNormal;
			}

			if (calculatedNormal)
			{
				returnedNormal = glm::normalize(returnedNormal);
				return calculatedNormal;
			}
		}
		return calculatedNormal;
	}

	void calculateDirectionOfVectorComponentBasedOnArea(float& vectorComponent, float& areaOfNgon)
	{
		if (areaOfNgon < 0.0)
		{
			vectorComponent = 1.0;
		}
		else if (areaOfNgon > 0.0)
		{
			vectorComponent = -1.0;
		}
	}

	void calculateAngleTypesOfVertices(const std::vector<int>& ngonIndices, const std::vector<glm::vec3>& ngonVertices, const glm::vec3& ngonNormal, std::map<int, AngleType>& returnedAngleTypesOfNgon)
	{
		for (int i = 0; i < ngonIndices.size(); ++i)
		{
			glm::vec3 previousVertex = ngonVertices.at(ngonIndices.at((i + (ngonIndices.size() - 1)) % ngonIndices.size()));
			glm::vec3 currentVertex = ngonVertices.at(ngonIndices.at(i % ngonIndices.size()));
			glm::vec3 nextVertex = ngonVertices.at(ngonIndices.at((i + 1) % ngonIndices.size()));

			glm::vec3 vector_prev_cur = currentVertex - previousVertex;
			glm::vec3 vector_prev_next = nextVertex - previousVertex;

			const auto& dotProduct = glm::dot(glm::cross(vector_prev_cur, vector_prev_next), ngonNormal);

			if (dotProduct < 0.0f)
			{
				returnedAngleTypesOfNgon[ngonIndices.at(i)] = AngleType::CONCAVE;
			}
			else if (dotProduct > 0.0f)
			{
				returnedAngleTypesOfNgon[ngonIndices.at(i)] = AngleType::CONVEX;
			}
			else
			{
				returnedAngleTypesOfNgon[ngonIndices.at(i)] = AngleType::FLAT;
			}
		}
	}

	void triangulatePlaneProjectedNgon(const std::vector<int>& ngonIndices, const std::vector<glm::vec2>& ngonVertices, std::vector<glm::vec2>& triangulatedVertices)
	{
		CDT cdt;
		std::vector<Vertex_handle> vertsInVertexHandle;

		for (const auto& vertex : ngonVertices)
		{
			vertsInVertexHandle.push_back(cdt.insert(Point(vertex.x, vertex.y)));
		}

		//teraz ist cez edges
		for (int i = 0; i < ngonIndices.size(); ++i)
		{
			cdt.insert_constraint(vertsInVertexHandle.at(ngonIndices.at(i)), vertsInVertexHandle.at(ngonIndices.at((i + 1) % ngonIndices.size())));
		}

		CGAL::mark_domain_in_triangulation(cdt);
		CGAL::refine_Delaunay_mesh_2(cdt,
			CGAL::parameters::domain_is_initialized(true));
		for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
			fit != cdt.finite_faces_end(); ++fit)
		{
			if (fit->is_in_domain()) {
				triangulatedVertices.push_back({ fit->vertex(0)->point().x(), fit->vertex(0)->point().y() });
				triangulatedVertices.push_back({ fit->vertex(1)->point().x(), fit->vertex(1)->point().y() });
				triangulatedVertices.push_back({ fit->vertex(2)->point().x(), fit->vertex(2)->point().y() });
			}
		}
	}

	float calculateSignedAreaOf2DNgon(const std::vector<glm::vec2>& vertices) {
		float area = 0.0;
		size_t j = 1;
		for (size_t i = 0; i < vertices.size(); i++, j++) {
			j = j % vertices.size();
			area += (vertices.at(j).x - vertices.at(i).x) * (vertices.at(j).y + vertices.at(i).y);
		}
		return area / 2.0;
	}

	bool arePointsCoplanar(const PointList& points, double threshold = 1e-6) {
		// Compute the best-fitting plane
		Plane_3 plane;
		linear_least_squares_fitting_3(points.begin(), points.end(),
			plane, CGAL::Dimension_tag<0>());

		// Compute the normal vector of the fitted plane
		auto normal = plane.orthogonal_vector();

		// Get the residuals (errors) from the fitting process
		std::vector<double> residuals;
		residuals.reserve(points.size());
		for (const auto& point : points) {
			// Compute the signed distance of the point to the plane
			double distance = (point - plane.point()) * normal; // Dot product
			residuals.push_back(std::abs(distance));
		}

		// Compute the mean squared error
		double meanSquaredError = 0.0;
		for (const auto& residual : residuals) {
			meanSquaredError += residual * residual;
		}
		meanSquaredError /= residuals.size();

		// Check if the mean squared error is below the threshold
		return meanSquaredError <= threshold;
	}

};